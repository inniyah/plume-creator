#include <QtGui>

#include "fulltextzone.h"
#include "fullscreeneditor.h"
#include "digitalclock.h"
#include "timer.h"

FullscreenEditor::FullscreenEditor(QTextDocument *doc, int cursorPos, QWidget *parent) :
    QWidget(parent)
{

    FullTextZone *fullTextEdit = new FullTextZone(doc, this);
fullTextEdit->applyTextConfig();

    QHBoxLayout *baseLayout= new QHBoxLayout;
    baseLayout->setMargin(0);

    QWidget *mainWidget = new QWidget;
    mainWidget->setObjectName("mainBackground");


    QHBoxLayout *textLayout = new QHBoxLayout;

    textLayout->addStretch();
    textLayout->addWidget(fullTextEdit);
    textLayout->addStretch();




    DigitalClock *clock = new DigitalClock(this);
    //    Timer *timer = new Timer(this);
    timerLabel = new QLabel;
    wordCountLabel = new QLabel;

    QPushButton *showSynButton = new QPushButton(tr("Synopsys"), this);
    connect(showSynButton, SIGNAL(clicked()),this,SLOT(showSyn()));
showSynButton->setFixedSize(50,20);

QPushButton *showNoteButton = new QPushButton(tr("Note"), this);
connect(showNoteButton, SIGNAL(clicked()),this,SLOT(showNote()));
showNoteButton->setFixedSize(50,20);

    QPushButton *exitFullscreenButton = new QPushButton(tr("Exit"), this);
    connect(exitFullscreenButton, SIGNAL(clicked()),this,SLOT(close()));
exitFullscreenButton->setFixedSize(50,20);




    QHBoxLayout *statsLayout = new QHBoxLayout();
    statsLayout->addWidget(wordCountLabel);
    //      statsLayout->addWidget(timer);
    statsLayout->addWidget(timerLabel);
    statsLayout->addWidget(clock);
    statsLayout->addWidget(showSynButton);
                statsLayout->addWidget(showNoteButton);
                    statsLayout->addWidget(exitFullscreenButton);



    QVBoxLayout *mainLayout = new QVBoxLayout();

    mainLayout->addLayout(textLayout);
    mainLayout->addLayout(statsLayout);


    mainWidget->setLayout(mainLayout);


    baseLayout->addWidget(mainWidget);
    setLayout(baseLayout);

    connect(fullTextEdit, SIGNAL(callColorDialogSignal()), this, SLOT(callColorDialog()));

    applyConfig();

showFullScreen();

    // get screen width for fullscreen text width option :
    fullTextEdit->setXMax(QApplication::desktop()->screenGeometry().width() - 50);

    //set cursor position :
    for(int i = 0; i < cursorPos ; i++)
        fullTextEdit->moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);

    fullTextEdit->ensureCursorVisible();
}

//------------------------------------------------------------------------------------


void FullscreenEditor::setWordCount(int num)
{
    QString text;
    wordCountLabel->setText(text.setNum(num,10));
}

void FullscreenEditor::setTimer(QString text)
{
    timerLabel->setText(text);
}





void FullscreenEditor::closeEvent(QCloseEvent* event)
{
synWidget->close();
noteWidget->close();

    emit closeSignal();
     event->accept();


}



void FullscreenEditor::callColorDialog()
{
    QWidget *fullColorDialog = new QWidget(this, Qt::Dialog);
    QVBoxLayout *layout = new QVBoxLayout;

    QLabel *fullColorLabel = new QLabel(tr("FullScreen Colors :"));



    QFormLayout *colorsLayout = new QFormLayout;
    backColorButton = new QPushButton(fullColorDialog);
    textBackColorButton = new QPushButton(fullColorDialog);
    textColorButton = new QPushButton(fullColorDialog);
    connect(backColorButton,SIGNAL(clicked()), this, SLOT(setBackColorDialog()));
    connect(textBackColorButton,SIGNAL(clicked()), this, SLOT(setTextBackColorDialog()));
    connect(textColorButton,SIGNAL(clicked()), this, SLOT(setTextColorDialog()));


    colorsLayout->addRow(tr("&Background color:"), backColorButton);
    colorsLayout->addRow(tr("Text b&ackground color:"), textBackColorButton);
    colorsLayout->addRow(tr("&Text color:"), textColorButton);



    QPushButton *closeButton = new QPushButton(tr("Ok"),fullColorDialog);
    connect(closeButton,SIGNAL(clicked()), fullColorDialog, SLOT(close()));

    layout->addWidget(fullColorLabel);
    layout->addLayout(colorsLayout);
    layout->addWidget(closeButton);
    fullColorDialog->setLayout(layout);



    fullColorDialog->show();
}


void FullscreenEditor::setSyn(QTextDocument *doc, int cursorPos)
{
    synWidget = new QWidget(this, Qt::Tool | Qt::WindowStaysOnTopHint);

    FullTextZone *fullSynEdit = new FullTextZone(doc, synWidget);
    fullSynEdit->applySynConfig();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(fullSynEdit);

    synWidget->setLayout(layout);
    synWidget->setWindowTitle(tr("Synopsys"));

    //set cursor position :
    for(int i = 0; i < cursorPos ; i++)
        fullSynEdit->moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);

    fullSynEdit->ensureCursorVisible();

//    synDoc = doc;
//    synCursorPos = cursorPos;

}


void FullscreenEditor::setNote(QTextDocument *doc, int cursorPos)
{


    noteWidget = new QWidget(this, Qt::Tool | Qt::WindowStaysOnTopHint);

    FullTextZone *fullNoteEdit = new FullTextZone(doc, noteWidget);
    fullNoteEdit->applyNoteConfig();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(fullNoteEdit);

    noteWidget->setLayout(layout);
    noteWidget->setWindowTitle(tr("Notes"));

    //set cursor position :
    for(int i = 0; i < cursorPos ; i++)
        fullNoteEdit->moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);

    fullNoteEdit->ensureCursorVisible();
//    noteDoc = doc;
//    noteCursorPos = cursorPos;
}

void FullscreenEditor::showSyn()
{
synWidget->show();
}

void FullscreenEditor::showNote()
{
    noteWidget->show();

}










//---------------------------------------------------------------------------
//----------Style sheets---------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------


void FullscreenEditor::setBaseStyleSheet()
{
    //baseStyrlSheet = "FullTextZone {
}


void FullscreenEditor::setBackColorDialog()
{


    QColor backgroundColor = QColorDialog::getColor(Qt::white, this, "Background Color" /*, QColorDialog::ShowAlphaChannel*/);


    QString string;
    QString r = string.setNum(backgroundColor.red(),10);
    QString g = string.setNum(backgroundColor.green(), 10);
    QString b = string.setNum(backgroundColor.blue(),10);

    backColorButton->setStyleSheet("background: rgb(" + r + ", " + g + ", " + b + ");");

    settings.setValue("FullTextArea/backgroundColor", backgroundColor);
    setBackColor();
    applyStyleSheet();
}

void FullscreenEditor::setBackColor()
{
    QColor backgroundColor = settings.value("FullTextArea/backgroundColor", Qt::black).toString();

    QString string;
    QString r = string.setNum(backgroundColor.red(),10);
    QString g = string.setNum(backgroundColor.green(), 10);
    QString b = string.setNum(backgroundColor.blue(),10);

    backColorString = "QWidget#mainBackground {background-color: rgb(" + r + ", " + g + ", " + b + ");}";



}


void FullscreenEditor::setTextBackColorDialog()
{
    QColor textBackColor = QColorDialog::getColor(Qt::white, this, "Background Color"/* , QColorDialog::ShowAlphaChannel*/);


    QString string;
    QString r = string.setNum(textBackColor.red(),10);
    QString g = string.setNum(textBackColor.green(), 10);
    QString b = string.setNum(textBackColor.blue(),10);

    textBackColorButton->setStyleSheet("background: rgb(" + r + ", " + g + ", " + b + ");");


    settings.setValue("FullTextArea/textBackgroundColor", textBackColor);

    setTextBackColor();
    applyStyleSheet();
}

void FullscreenEditor::setTextBackColor()
{
    QColor textBackgroundColor = settings.value("FullTextArea/textBackgroundColor", Qt::black).toString();

    QString string;
    QString r = string.setNum(textBackgroundColor.red(),10);
    QString g = string.setNum(textBackgroundColor.green(), 10);
    QString b = string.setNum(textBackgroundColor.blue(),10);

    textBackColorString = "FullTextZone {background-color: rgb(" + r + ", " + g + ", " + b + ");"
            "border: 0px none black; border-radius: 0px;"
            "}";


}

void FullscreenEditor::setTextColorDialog()
{
    QColor textColor = QColorDialog::getColor(Qt::white, this, "Background Color"/* , QColorDialog::ShowAlphaChannel*/);


    QString string;
    QString r = string.setNum(textColor.red(),10);
    QString g = string.setNum(textColor.green(), 10);
    QString b = string.setNum(textColor.blue(),10);

    textColorButton->setStyleSheet("background: rgb(" + r + ", " + g + ", " + b + ");");


    settings.setValue("FullTextArea/textColor", textColor);

    setTextColor();
    applyStyleSheet();
}

void FullscreenEditor::setTextColor()
{
    QColor textColor = settings.value("FullTextArea/textColor", Qt::lightGray).toString();

    QString string;
    QString r = string.setNum(textColor.red(),10);
    QString g = string.setNum(textColor.green(), 10);
    QString b = string.setNum(textColor.blue(),10);

    textColorString = "FullTextZone {color: rgb(" + r + ", " + g + ", " + b + ");}";
}



//void FullscreenEditor::setZoomIn()
//{

//// font-size: 20pt ;
//}



void FullscreenEditor::applyStyleSheet()
{
    setStyleSheet(backColorString + textBackColorString + textColorString);
}



//---------------------------------------------------------------------------
//----------Apply Config---------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------


void FullscreenEditor::applyConfig()
{





    //style sheets :
    setBaseStyleSheet();
    setBackColor();
    setTextBackColor();
    setTextColor();

    applyStyleSheet();




}
