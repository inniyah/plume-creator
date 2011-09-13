#include <QtGui>

#include "texttab.h"
#include "wordcount.h"



TextTab::TextTab(QWidget *parent) :
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


    connect(textZone,SIGNAL(charFormatChangedSignal(QTextCharFormat)), this,SIGNAL(charFormatChangedSignal(QTextCharFormat)));

    //config

    //temporary config


    //   textZone->setMinimumWidth(600);


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

bool TextTab::openText(QTextDocument *doc)
{

    //  stackName = name;

    textDocument = doc;
    textZone->setDocument(textDocument);
    textZone->document()->adjustSize();

    //for wordcount :
    tabWordCount = new WordCount(textDocument, this);
    //connect(tabWordCount, SIGNAL(charCountSignal(int)), this, SLOT(charCountUpdated(int)));
    connect(tabWordCount, SIGNAL(wordCountSignal(int)), this, SLOT(wordCountUpdated(int)));
    connect(tabWordCount, SIGNAL(blockCountSignal(int)), this, SLOT(blockCountUpdated(int)));
    tabWordCount->updateAll();

    QString debug;
    qDebug() << "doc witdh : " << debug.setNum(textDocument->textWidth());
    qDebug() << "doc witdh : " << debug.setNum(doc->textWidth());

    applyConfig();

    return true;

}

//---------------------------------------------------------------------------------------


//bool TextTab::saveText(QFile *textFile, QString name)
//{

//    if(name != stackName)
//        return false;



//    QTextDocumentWriter docWriter(textFile, "HTML");
//    bool written = docWriter.write(textDocument);




//    return written;


//}



QTextDocument* TextTab::document()
{
    return textDocument;
}

void TextTab::wordCountUpdated(int wordCount)
{
    //    QString debug;
    //    qDebug() << "wordCount : " << debug.setNum(wordCount,10);


    emit wordCountSignal(wordCount);


}

//void TextTab::charCountUpdated(int charCount)
//{
////    QString debug;
////    qDebug() << "charCount : " << debug.setNum(charCount,10);
//}

void TextTab::blockCountUpdated(int blockCount)
{
    //    QString debug;
    //    qDebug() << "blockCount : " << debug.setNum(blockCount,10);


    emit blockCountSignal(blockCount);

}

void TextTab::updateWordCounts()
{
    tabWordCount->updateAll();

}


void TextTab::changeWidthSlot(int width)
{
    textZone->setFixedWidth(width);
    textZone->document()->setTextWidth(textZone->width() - 20);
}

void TextTab::changeTextFontSlot(QFont font)
{
    textZone->setTextFont(font);
}

void TextTab::changeTextHeightSlot(int height)
{
    textZone->setTextHeight(height);
}


void TextTab::setTextFocus()
{
    textZone->setFocus();

}

void TextTab::setCursorPos(int pos)
{
    for(int i = 0; i < pos ; i++)
        textZone->moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);


    textZone->ensureCursorVisible();
}

int TextTab::saveCursorPos()
{
    QTextCursor cursor = textZone->textCursor();
    return cursor.position();

}

QTextCharFormat TextTab::tabChangedSlot()
{
    return textZone->textCursor().charFormat();
}

void TextTab::updateTextZone()
{

    textZone->document()->setTextWidth(textZone->width() - 20);

    qDebug() << "updateTextZone";
}


void TextTab::applyConfig()
{
    textZone->applyConfig();


    QSettings settings;
    settings.beginGroup( "Settings" );
    int bottMargin = settings.value("TextArea/bottomMargin", 10).toInt();
    int textIndent = settings.value("TextArea/textIndent", 20).toInt();
    int textHeight = settings.value("TextArea/textHeight", 12).toInt();
    QString fontFamily = settings.value("TextArea/textFontFamily", "Liberation Serif").toString();
    settings.endGroup();


    QTextBlockFormat blockFormat;
    blockFormat.setBottomMargin(bottMargin);
    blockFormat.setTextIndent(textIndent);
    QTextCursor *tCursor = new QTextCursor(document());
    tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
    tCursor->mergeBlockFormat(blockFormat);




    //apply default font in empty documents :

    if(document()->isEmpty()){

        QFont font;
        font.setFamily(fontFamily);
        font.setPointSize(textHeight);
        document()->setDefaultFont(font);

    }


}
