#include <QtGui>

#include "texttab.h"
#include "wordcount.h"



TextTab::TextTab(QFile *textFile, QString name, QWidget *parent) :
    QWidget(parent)
{
    textDocument = new QTextDocument(this);


    textZone = new TextZone(textDocument, this);
    textZone->toHtml();


    QHBoxLayout *layout = new QHBoxLayout;

    layout->addStretch();
    layout->addWidget(textZone);
    layout->addStretch();

    setLayout(layout);


    setContextMenuPolicy(Qt::PreventContextMenu);





    //config


    //temporary config


    textZone->setMinimumWidth(600);


//    QFont synLiberationFont("Liberation Serif", 13);
//    textZone->setFont(synLiberationFont);
//    textZone->setFontPointSize(13);

//    QTextBlockFormat synTextBlockFormat;
//    synTextBlockFormat.setBottomMargin(10);
//    synTextBlockFormat.setTextIndent(50);
//    QTextCharFormat synTextCharFormat;
//    synTextCharFormat.setFontFamily("Liberation Serif");
//    synTextCharFormat.setFontPointSize(13);

//    QTextCursor synTextCursor;
//    synTextCursor = textZone->textCursor();
//    synTextCursor.setBlockFormat(synTextBlockFormat);
//    synTextCursor.setCharFormat(synTextCharFormat);



}




//---------------------------------------------------------------------------------------

bool TextTab::openText(QFile *textFile, QString name)
{

    stackName = name;







    textFile->open(QFile::ReadOnly | QFile::Text);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream textFileStream( textFile );
    textDocument->setHtml(textFileStream.readAll());
 //   textZone->setText( textFileStream.readAll() );
    QApplication::restoreOverrideCursor();


    textFile->close();

textZone->setDocument(textDocument);




//for wordcount :
tabWordCount = new WordCount(textDocument, this);
//connect(tabWordCount, SIGNAL(charCountSignal(int)), this, SLOT(charCountUpdated(int)));
connect(tabWordCount, SIGNAL(wordCountSignal(int)), this, SLOT(wordCountUpdated(int)));
connect(tabWordCount, SIGNAL(blockCountSignal(int)), this, SLOT(blockCountUpdated(int)));
tabWordCount->updateAll();

    return true;

}

//---------------------------------------------------------------------------------------


bool TextTab::saveText(QFile *textFile, QString name)
{

    if(name != stackName)
        return false;



//    textFile->setFileName(textFile->fileName());
//    textFile->open(QFile::WriteOnly | QFile::Text);

//    if(textFile->isWritable())
//    {

//        QTextStream stream(textFile);
//        stream << textZone->toHtml();
//        stream.flush();
//        textFile->close();


//        return true;
//    }
//    else{
//        qDebug() << textFile->fileName() << " isn't Writtable.";

//        return false;
//    }

    QTextDocumentWriter docWriter(textFile, "HTML");
    bool written = docWriter.write(textDocument);




    return written;


}



QTextDocument* TextTab::document()
{
    return textDocument;
}

void TextTab::wordCountUpdated(int wordCount)
{
    QString debug;
    qDebug() << "wordCount : " << debug.setNum(wordCount,10);


    emit wordCountSignal(wordCount);


}

//void TextTab::charCountUpdated(int charCount)
//{
////    QString debug;
////    qDebug() << "charCount : " << debug.setNum(charCount,10);
//}

void TextTab::blockCountUpdated(int blockCount)
{
    QString debug;
    qDebug() << "blockCount : " << debug.setNum(blockCount,10);


    emit blockCountSignal(blockCount);

}

void TextTab::updateWordCounts()
{
tabWordCount->updateAll();

}



