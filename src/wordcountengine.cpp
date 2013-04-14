#include "wordcountengine.h"

WordCountEngine::WordCountEngine(QObject *parent, QTextDocument *doc) :
    QObject(parent), m_wordCount(0), preBlockNum(1), blockNum(-1), wordCountWithoutBlock(0)
{
    m_doc = doc;


    connect(doc, SIGNAL(contentsChanged()), this, SLOT(update()));
    connect(doc, SIGNAL(cursorPositionChanged(QTextCursor)), this, SLOT(updateCursorPosition(QTextCursor)));
    update();







}

void WordCountEngine::update()
{
    //    Word Count:
    QStringList wordsList;

    if(preBlockNum != blockNum){

        // calculate all !


        QString docText = m_doc->toPlainText();

        docText.replace("\n", " ");

        wordsList = docText.split(" ", QString::SkipEmptyParts);

        m_wordCount = wordsList.size();




        if(blockNum != -1){
        QString blockText = m_doc->findBlockByNumber(blockNum).text();
        blockText.replace("\n", " ");
        wordsList = blockText.split(" ", QString::SkipEmptyParts);
        wordCountWithoutBlock = m_wordCount - wordsList.size();

        preBlockNum = blockNum;
        }

    }
    else{
        // intelligent calculating :

        QString blockText = m_doc->findBlockByNumber(blockNum).text();
        blockText.replace("\n", " ");
        wordsList = blockText.split(" ", QString::SkipEmptyParts);

        m_wordCount = wordCountWithoutBlock + wordsList.size();

        qDebug() <<  "intelligent wordcount";


    }





    emit wordCountChanged(m_wordCount);
    //Paragraph Count:

    preBlockCount = m_doc->blockCount();

    //   emit blockCountSignal(preBlockCount);


}



void WordCountEngine::updateCursorPosition(QTextCursor textCursor)
{
    blockNum = textCursor.blockNumber();


}
