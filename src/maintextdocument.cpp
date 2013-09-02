#include "maintextdocument.h"

MainTextDocument::MainTextDocument(QObject *parent) :
    QTextDocument(parent), m_cursorPos(0), isWordCountEnabled(false)
{

    wordCountEngine = new WordCountEngine(this, this);

    highlighter = new TextHighlighter(this);
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

void MainTextDocument::connectWordCount()
{
    connect(wordCountEngine,SIGNAL(wordCountChanged(int)), this, SLOT(wordCountChangedSlot(int)), Qt::UniqueConnection);
isWordCountEnabled = true;
}
void MainTextDocument::disconnectWordCount()
{
    disconnect(wordCountEngine,SIGNAL(wordCountChanged(int)), this, SLOT(wordCountChangedSlot(int)));
    isWordCountEnabled = false;

}
//-------------------------------------------------------------

void MainTextDocument::wordCountChangedSlot(int count)
{
    if(isWordCountEnabled)
    emit wordCountChanged(this->docType(), this->idNumber(), count);
}

TextHighlighter* MainTextDocument::textHighlighter()
{
    return highlighter;
}
