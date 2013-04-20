#include "maintextdocument.h"

MainTextDocument::MainTextDocument(QObject *parent) :
    QTextDocument(parent), m_cursorPos(0)
{

    wordCountEngine = new WordCountEngine(this, this);
    connect(wordCountEngine,SIGNAL(wordCountChanged(int)), this, SLOT(wordCountChangedSlot(int)));

}

//-------------------------------------------------------------

int MainTextDocument::idNumber() const
{
    return itemId;
}

void MainTextDocument::setIdNumber(int number)
{
    itemId = number;
}

//-------------------------------------------------------------

QString MainTextDocument::docType() const
{
    return m_docType;
}

void MainTextDocument::setDocType(QString type)
{
    m_docType = type;
}

//-------------------------------------------------------------

int MainTextDocument::cursorPos() const
{
    return m_cursorPos;
}

void MainTextDocument::setCursorPos(int pos)
{
    m_cursorPos = pos;
}

//-------------------------------------------------------------

void MainTextDocument::wordCountChangedSlot(int count)
{
//    qDebug() << "wordCountChanged : " << this->docType();
//    qDebug() << "wordCountChanged : " << QString::number(count);
    emit wordCountChanged(this->docType(), this->idNumber(), count);
}
