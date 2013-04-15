#include "maintextdocument.h"

MainTextDocument::MainTextDocument(QObject *parent) :
    QTextDocument(parent)
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

void MainTextDocument::wordCountChangedSlot(int count)
{
//    qDebug() << "wordCountChanged : " << this->docType();
//    qDebug() << "wordCountChanged : " << QString::number(count);
    emit wordCountChanged(this->docType(), this->idNumber(), count);
}
