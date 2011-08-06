#include <QObject>
#include <QtGui>
#include <QDebug>

#include "wordcount.h"

WordCount::WordCount(QTextDocument *doc, QObject *parent) :
    QObject(parent)
{




    QString docText = doc->toPlainText();
    document = doc;

    //Character Count:


//    preCharCount = docText.length();

//    emit charCountSignal(preCharCount);

//    connect(doc,SIGNAL(contentsChanged()), this, SLOT(updateCharCount()));













    //Word Count:



    QStringList wordsList;

    wordsList = docText.split(" ", QString::SkipEmptyParts);

    finalWordCount = wordsList.size();

 //   emit wordCountSignal(finalWordCount);




    QStringList blockWordsList;
    QString blockText;

    QTextCursor textCursor(doc);
          textCursor.setPosition(0);
    blockNum = textCursor.blockNumber();

    blockText = document->findBlockByNumber(blockNum).text();
    blockWordsList = blockText.split(" ", QString::SkipEmptyParts);

preWordCount = finalWordCount - blockWordsList.size();

preBlockNum = blockNum;

//    QString debug;
//     qDebug() << "first finalWordCount : " << debug.setNum(finalWordCount,10);
//    qDebug() << "pre wordCount :" << debug.setNum(preWordCount, 10);
//    qDebug() << "textCursor position :" << debug.setNum(textCursor.position(), 10);
//    qDebug() << "constructor blockNumber : " << debug.setNum(blockNum,10);






    connect(doc, SIGNAL(contentsChanged()), this, SLOT(updateWordCount()));
    connect(doc, SIGNAL(cursorPositionChanged(QTextCursor)), this, SLOT(updateCursorPosition(QTextCursor)));










    //Paragraph Count:

preBlockCount = doc->blockCount();

 //   emit blockCountSignal(preBlockCount);

    connect(doc,SIGNAL(blockCountChanged(int)), this, SLOT(updateBlockCount(int)));

//    qDebug() << "pre blockCount :" << debug.setNum(doc->blockCount(), 10);



this->updateAll();


}

//--------------------------------------------------------------------------------------------------------------------------------


void WordCount::updateAll()
{

   //     emit charCountSignal(preCharCount);

emit wordCountSignal(finalWordCount);

emit blockCountSignal(preBlockCount);
}

//--------------------------------------------------------------------------------------------------------------------------------

//void WordCount::updateCharCount(/*int position,int charsRemoved,int charsAdded*/)
//{


//    QString docText = document->toPlainText();

//    finalCharCount = docText.length();

//    emit charCountSignal(finalCharCount);
//}


//-------------------------------------------------------------------------------------------------------------------------------

void WordCount::updateWordCount()
{

    QString blockText;




    if(blockNum == preBlockNum){

        blockText = document->findBlockByNumber(blockNum).text();
        wordsList = blockText.split(" ", QString::SkipEmptyParts);

        finalWordCount = preWordCount + wordsList.size();

        emit wordCountSignal(finalWordCount);

    }
    else{



        blockText = document->findBlockByNumber(blockNum).text();
        wordsList = blockText.split(" ", QString::SkipEmptyParts);

        preWordCount = finalWordCount - wordsList.size();;

finalWordCount = preWordCount + wordsList.size();

        emit wordCountSignal(finalWordCount);


        preBlockNum = blockNum;

}

}

void WordCount::updateCursorPosition(QTextCursor textCursor)
{

    blockNum = textCursor.blockNumber();


//    QString debug;
//     qDebug() << "new cursor position : " << debug.setNum(blockNum,10);



}

void WordCount::updateBlockCount(int blockCount)
{



    //new block

        QString docText = document->toPlainText();
        QStringList wordsList;

        wordsList = docText.split(" ", QString::SkipEmptyParts);

        finalWordCount = wordsList.size();


        emit wordCountSignal(finalWordCount);


        preBlockNum =  blockNum + 1;


        finalBlockCount = blockCount;
        emit blockCountSignal(blockCount);


}

