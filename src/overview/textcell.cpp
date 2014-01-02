#include "textcell.h"
#include "ui_textcell.h"

TextCell::TextCell(QWidget *parent) :
    QWidget(parent), m_id(-1), m_title(""),
    ui(new Ui::TextCell)
{
    ui->setupUi(this);

ui->sideButtonTitle->setOrientation(Qt::Vertical);
ui->sideButtonTitle->setMirrored(true);
ui->sideButtonTitle->setFlat(true);
connect(ui->sideButtonTitle, SIGNAL(clicked()), this, SLOT(openSheet()));
}


TextCell::~TextCell()
{
    delete ui;
}

void TextCell::setTextDocument(MainTextDocument *doc)
{
    ui->textEdit->setDocument(doc);


    QSettings settings;

        int width = settings.value("Settings/TextArea/textWidth").toInt();
    ui->textEdit->setFixedWidth(width);

    doc->adjustSize();
   QTextCursor cursor = ui->textEdit->textCursor();
  cursor.movePosition(QTextCursor::End);
  QRect rect = ui->textEdit->cursorRect(cursor);
  ui->textEdit->setFixedHeight(rect.bottom()- ui->textEdit->contentsMargins().top()*2);

//ui->textEdit->hide();
//ui->textEdit->setAttribute(Qt::WA_DontShowOnScreen);
//  ui->textEdit->show();

//ui->textEdit->setFixedHeight(doc->size().height() - ui->textEdit->contentsMargins().top()*2);
//ui->textEdit->adjustSize();
//QApplication::processEvents();
//    QRect vRect = ui->textEdit->rect();

//    ui->sideButtonTitle->setText(QString::number(vRect.height()) + " - " + QString::number(rect.bottom()));
//    ui->sideButtonTitle->setText(QString::number(doc->firstBlock().lineCount()) + " - " + QString::number(doc->blockCount()));



}
QString TextCell::title() const
{
    return m_title;
}

void TextCell::setTitle(const QString &title)
{

    ui->sideButtonTitle->setText(title);

    m_title = title;
}
int TextCell::id() const
{
    return m_id;
}

void TextCell::setId(int id)
{
    m_id = id;
}

void TextCell::openSheet()
{
    if(m_id == -1)
        return;

    emit openSheet(this->id());
}
QString TextCell::type() const
{
    return m_type;
}

void TextCell::setType(const QString &type)
{
    m_type = type;
}



//void TextCell::setTextEditSize(MainTextDocument *doc)
//{

//}
