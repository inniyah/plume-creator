#include "slimfindreplace.h"
#include "ui_slimfindreplace.h"

SlimFindReplace::SlimFindReplace(QWidget *parent) :
    QWidget(parent),allowRestartFromBeginning(false),allowRestartFromEnd(false),
    ui(new Ui::SlimFindReplace)
{
    ui->setupUi(this);

}

SlimFindReplace::~SlimFindReplace()
{
    delete ui;
}
void SlimFindReplace::setDocument(MainTextDocument *doc)
{
    document = doc;
    this->applyConfig();
}
void SlimFindReplace::setTextEdit(QTextEdit *txtEdit)
{
    textEdit = txtEdit;
    connect(textEdit, SIGNAL(destroyed()), this,SLOT(closeSlot()));
}



void SlimFindReplace::on_findLineEdit_textChanged(const QString &arg1)
{
    findText = arg1;

    document->textHighlighter()->setTextToHighlight(arg1);

    positionsList = findTextPos(arg1);

    if(!positionsList.isEmpty()){
        QTextCursor cursor = textEdit->textCursor();
        cursor.setPosition(positionsList.first());
        textEdit->setTextCursor(cursor);
        textEdit->ensureCursorVisible();
    }
    else {
        qDebug() << "no match";
    }

}

void SlimFindReplace::on_replaceLineEdit_textChanged(const QString &arg1)
{
    replaceText = arg1;
}

void SlimFindReplace::on_closeButton_clicked()
{


    int animDuration = 500;

    QTimer::singleShot(animDuration, this, SLOT(closeSlot()));

    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setDuration(animDuration);

    int xManager = this->geometry().x();
    int yManager = this->geometry().y();
    int wManager = this->width();
    int hManager = this->height();


    animation->setStartValue(QRect(xManager,yManager,wManager,hManager));
    animation->setEndValue(QRect(xManager,yManager + height(),wManager,0));
    animation->start();

}

void SlimFindReplace::closeSlot()
{
    this->saveConfig();
    document->textHighlighter()->setTextToHighlight("");

    this->close();
}



void SlimFindReplace::on_previousButton_clicked()
{
    QTextCursor cursor = textEdit->textCursor();
    oldPosition = cursor.position();

    if(allowRestartFromEnd){
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        allowRestartFromEnd = false;
    }

    positionsList = findTextPos(findText);

    if(!positionsList.isEmpty()){

        int maxPrevPos = -1;
        foreach(const int &textPos, positionsList){
            if(cursor.position() < textPos)
                break;
            maxPrevPos = textPos;
        }
        if(maxPrevPos != -1){
            cursor.setPosition(maxPrevPos);
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, findText.size());

            textEdit->setFocus();
            textEdit->setTextCursor(cursor);
            textEdit->ensureCursorVisible();
        }
        else
            allowRestartFromEnd = true;
    }
    else {
        cursor.setPosition(oldPosition);
        textEdit->setTextCursor(cursor);
        textEdit->ensureCursorVisible();
        qDebug() << "no match";
    }

}

void SlimFindReplace::on_nextButton_clicked()
{
    QTextCursor cursor = textEdit->textCursor();
    oldPosition = cursor.position();

    if(allowRestartFromBeginning){
        cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
        allowRestartFromBeginning = false;
    }

    positionsList = findTextPos(findText);

    if(!positionsList.isEmpty()){

        int maxNextPos = -1;
        foreach(const int &textPos, positionsList)
            if(cursor.position() + findText.size() < textPos){
                maxNextPos = textPos;
                break;
            }

        if(maxNextPos != -1){
            cursor.setPosition(maxNextPos);
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, findText.size());

            textEdit->setFocus();
            textEdit->setTextCursor(cursor);
            textEdit->ensureCursorVisible();
        }
        else
            allowRestartFromBeginning = true; // must click once again to do restart from  beginning
    }
    else {
        cursor.setPosition(oldPosition);
        textEdit->setTextCursor(cursor);
        textEdit->ensureCursorVisible();
        qDebug() << "no match";

    }
}

void SlimFindReplace::on_replaceAndNextButton_clicked()
{
    QTextCursor cursor = textEdit->textCursor();



    bool isTextGood;
    QTextCursor checkCursor(document);
    checkCursor.setPosition(document->find(findText, cursor.position(), findFlags).position());
    qDebug() << "checkCursor.position() : "<< QString::number(checkCursor.position());

    checkCursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, findText.size());
    qDebug() << "checkCursor.selectedText() : "<< checkCursor.selectedText();
    qDebug() << "cursor.selectedText() : "<< cursor.selectedText();

    if(cursor.selectedText() == checkCursor.selectedText() && !cursor.selectedText().isEmpty())
        isTextGood = true;
    else
        isTextGood = false;


    if(replaceText.isEmpty())
        return;

    if(isTextGood){
        cursor.insertText(replaceText);
        cursor.setPosition(cursor.position() - 1);
textEdit->setTextCursor(cursor);
    }
    on_nextButton_clicked();
}

QList<int> SlimFindReplace::findTextPos(QString text)
{
    QList<int> list;
    QTextCursor cursor;
    cursor = document->find(text, 0, findFlags);
    if(!cursor.isNull())
        list.append(cursor.position());
    while(!cursor.isNull()){
        cursor = document->find(text, cursor.position(), findFlags);
        list.append(cursor.position());
    }
    return list;
}



void SlimFindReplace::applyConfig()
{
    QSettings settings;

    settings.beginGroup( "SlimFindReplace" );
    ui->caseSensitiveCheckBox->setChecked(settings.value("caseSensitive", false).toBool());
    settings.endGroup();

    settings.beginGroup( "FindReplace" );
    settings.endGroup();

}
void SlimFindReplace::saveConfig()
{
    QSettings settings;

    settings.beginGroup( "SlimFindReplace" );
    settings.setValue("caseSensitive", ui->caseSensitiveCheckBox->isChecked());
    settings.endGroup();

    settings.beginGroup( "FindReplace" );
    settings.endGroup();
}

void SlimFindReplace::on_caseSensitiveCheckBox_toggled(bool checked)
{
    if(checked){

        findFlags |= QTextDocument::FindCaseSensitively;
        document->textHighlighter()->setCaseSensitivity(true);
        caseSensitive = true;
    }
    else{
        findFlags &= ~QTextDocument::FindCaseSensitively;
        caseSensitive = false;
        document->textHighlighter()->setCaseSensitivity(false);
    }
    on_findLineEdit_textChanged(ui->findLineEdit->text());
}
