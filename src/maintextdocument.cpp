#include "maintextdocument.h"

MainTextDocument::MainTextDocument(QObject *parent, SpellChecker *spellCheck) :
    QTextDocument(parent), m_cursorPos(0), isWordCountEnabled(false)
{



    wordCountEngine = new WordCountEngine(this, this);




    m_spellChecker = spellCheck;
//    connect(this, SIGNAL(attendTree_namesListChanged(QStringList)), this, SLOT(attendTree_namesListChangedSlot(QStringList)));

    highlighter = new TextHighlighter(this);
highlighter->setSpellChecker(spellCheck);
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





//-------------------------------------------------------------
//-------------------------------------------------------------
//------------------Spell checker---------------------------
//-------------------------------------------------------------

bool MainTextDocument::activateSpellChecker()
{
    if(!m_spellChecker->activate()){
        qWarning() << "activateSpellChecker() without dictionary";
        return false;
    }
//    m_spellChecker->setDict(m_dictionaryPath, m_userDictionary, m_attendTree_names);



    highlighter->rehighlight();

    return true;
}

//-------------------------------------------------------------

void MainTextDocument::deactivateSpellChecker()
{
    m_spellChecker->deactivate();
    highlighter->rehighlight();


}
//-------------------------------------------------------------

void MainTextDocument::setDicts()
{


    if(m_spellChecker->isActive()){
        highlighter->rehighlight();
    }
}

//-------------------------------------------------------------
SpellChecker *MainTextDocument::spellChecker() const
{
    return m_spellChecker;
}
//-------------------------------------------------------------

void MainTextDocument::setSpellChecker(SpellChecker *spellChecker)
{
    m_spellChecker = spellChecker;
highlighter->setSpellChecker(spellChecker);
}

//-------------------------------------------------------------

