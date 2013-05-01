#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   

#include "texttab.h"
//


TextTab::TextTab(QWidget *parent) :
    QWidget(parent)
{
    prevTextDocument = new MainTextDocument(this);
    textDocument = new MainTextDocument(this);
    nextTextDocument = new MainTextDocument(this);

    prevTextZone = new TextZone(this);
    prevTextZone->setHub(hub);
    prevTextZone->toHtml();

    nextTextZone = new TextZone(this);
    nextTextZone->setHub(hub);
    nextTextZone->toHtml();


    textZone = new TextZone(this);
    textZone->setHub(hub);
    textZone->toHtml();

    QWidget *textZoneWidget = new QWidget;
    textZoneVLayout = new QVBoxLayout;
    textZoneVLayout->setSpacing(0);
    textZoneVLayout->addWidget(textZone);
    textZoneWidget->setLayout(textZoneVLayout);

    QHBoxLayout *layout = new QHBoxLayout;
    QSplitter *splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(prevTextZone);
    splitter->addWidget(textZoneWidget);
    splitter->addWidget(nextTextZone);



    layout->addStretch();
    //    layout->addLayout(vLayout);
    layout->addWidget(splitter);
    layout->addStretch();

    setLayout(layout);


    setContextMenuPolicy(Qt::PreventContextMenu);




    prevTextZone->hide();
    nextTextZone->hide();


    QList<int> sizesList;
    sizesList << 0 << textZone->height()  << 0;
    splitter->setSizes(sizesList);





    //config







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


    giveStyle();

}

void TextTab::setTextStyles(TextStyles *styles)
{
    textStyles = styles;


}



//---------------------------------------------------------------------------------------

bool TextTab::openText(MainTextDocument *doc)
{

    prevTextZone->setTextStyles(textStyles);
    prevTextZone->createContent();
    nextTextZone->setTextStyles(textStyles);
    nextTextZone->createContent();
    textZone->setTextStyles(textStyles);
    textZone->createContent();






    //  stackName = name;

    textDocument = doc;
    textZone->setDoc(textDocument);
    QApplication::processEvents();
    textZone->document()->adjustSize();



    this->addSlimFindReplaceAction();

    connect(textZone, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChangedSlot()));
    connect(textZone,SIGNAL(charFormatChangedSignal(QTextCharFormat)), this,SIGNAL(charFormatChangedSignal(QTextCharFormat)));
    connect(textZone,SIGNAL(textChanged()), this,SIGNAL(textChangedSignal()));
    connect(textZone,SIGNAL(styleSelectedSignal(int)), this,SLOT(changeTextStyleSlot(int)));
    connect(this, SIGNAL(setStyleSelectionSignal(int)), textZone, SIGNAL(setStyleSelectionSignal(int)));
    connect(textZone,SIGNAL(manageStylesSignal()), this,SIGNAL(manageStylesSignal()));

    //    QString debug;
    //    qDebug() << "doc witdh : " << debug.setNum(textDocument->textWidth());
    //    qDebug() << "doc witdh : " << debug.setNum(doc->textWidth());



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


//-------------------------------------------------------------------------------
MainTextDocument* TextTab::document()
{
    return textDocument;
}

//-------------------------------------------------------------------------------

void TextTab::changeWidthSlot(int width)
{
    if(width == -1){
    QSettings settings;
    width = settings.value("Settings/TextArea/textWidth", this->width()/2 ).toInt();

    }


    int scrollBarWidth = 30;

    textZone->setFixedWidth(width);
    textZone->document()->setTextWidth(width - scrollBarWidth);
    prevTextZone->setFixedWidth(width);
    prevTextZone->document()->setTextWidth(width - scrollBarWidth);
    nextTextZone->setFixedWidth(width);
    nextTextZone->document()->setTextWidth(width - scrollBarWidth);
}
//-------------------------------------------------------------------------------
void TextTab::changeTextFontSlot(QFont font)
{
    textZone->setTextFont(font);
}
//-------------------------------------------------------------------------------
void TextTab::changeTextHeightSlot(int height)
{
    textZone->setTextHeight(height);
}

//-------------------------------------------------------------------------------
void TextTab::changeTextStyleSlot(int styleIndex)
{


    QTextBlockFormat blockFormat;
    blockFormat.setBottomMargin(textStyles->blockBottomMarginAt(styleIndex));
    blockFormat.setTextIndent(textStyles->blockFirstLineIndentAt(styleIndex));
    blockFormat.setLeftMargin(textStyles->blockLeftMarginAt(styleIndex));
    blockFormat.setAlignment(textStyles->blockAlignmentTrueNameAt(styleIndex));
    blockFormat.setTopMargin(0);
    blockFormat.setRightMargin(0);
    QTextCharFormat charFormat;
    charFormat.setFontPointSize(textStyles->fontSizeAt(styleIndex));
    charFormat.setFontFamily(textStyles->fontFamilyAt(styleIndex));
    //    charFormat.setFontItalic(textStyles->fontItalicAt(styleIndex));
    //    if (textStyles->fontBoldAt(styleIndex) == true)
    //        charFormat.setFontWeight(75);
    //    else
    //        charFormat.setFontWeight(50);
    //    charFormat.setFontUnderline(textStyles->fontUnderlineAt(styleIndex));
    //    charFormat.setFontStrikeOut(textStyles->fontStrikeOutAt(styleIndex));

    //    charFormat.clearForeground();


    QTextCursor tCursor = textZone->textCursor();

    // select all of the blocks selected :

    QTextCursor tStartCursor = textZone->textCursor();
    tStartCursor.setPosition(tCursor.selectionStart());
    tStartCursor.movePosition(QTextCursor::StartOfBlock);
    int startFirstBlock = tStartCursor.position();

    QTextCursor tEndCursor = textZone->textCursor();
    tEndCursor.setPosition(tCursor.selectionEnd());
    tEndCursor.movePosition(QTextCursor::EndOfBlock);
    int endLastBlock = tEndCursor.position();

    tCursor.setPosition(startFirstBlock);
    tCursor.setPosition(endLastBlock, QTextCursor::KeepAnchor);


    // merge :
    tCursor.mergeBlockFormat(blockFormat);
    tCursor.mergeCharFormat(charFormat);
    textZone->mergeCurrentCharFormat(charFormat);

}


//-------------------------------------------------------------------------------
void TextTab::setTextFocus()
{
    if(textZone->isVisible())
        textZone->setFocus();

}
//-------------------------------------------------------------------------------
void TextTab::setCursorPos(int pos)
{
    for(int i = 0; i < pos ; i++)
        textZone->moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);


    textZone->ensureCursorVisible();
}
//-------------------------------------------------------------------------------
int TextTab::cursorPos()
{
    QTextCursor cursor = textZone->textCursor();
    return cursor.position();

}
//-------------------------------------------------------------------------------
QTextCharFormat TextTab::tabFontChangedSlot()
{
    return textZone->textCursor().charFormat();
}

//-------------------------------------------------------------------------------
void TextTab::cursorPositionChangedSlot()
{
    QTextCursor tCursor = textZone->textCursor();

    if((tCursor.atStart() == true
        || tCursor.position() == 1
        || tCursor.position() == 0) && tCursor.hasSelection() == false){
        this->changeTextStyleSlot(textStyles->defaultStyleIndex());
    }

    int currentStyleIndex = textStyles->compareStylesWithText(tCursor.charFormat(), tCursor.blockFormat());

    emit setStyleSelectionSignal(currentStyleIndex);

}

//-------------------------------------------------------------------------------


void TextTab::updateTextZone()
{

    textZone->document()->setTextWidth(textZone->width() - 20);
    prevTextZone->document()->setTextWidth(prevTextZone->width() - 20);

    //    qDebug() << "updateTextZone";
}


//-------------------------------------------------------------------------------

void TextTab::showPrevText(bool showPrevTextBool)
{
    prevTextZone->setHidden(!showPrevTextBool);
    prevTextZone->setMaximumHeight(textZone->height()/3);
    textZone->setFocus();
    textZone->ensureCursorVisible();

}

//-------------------------------------------------------------------------------
bool TextTab::setShowPrevTextButton()
{
    return !prevTextZone->isHidden();
}

//-------------------------------------------------------------------------------
void  TextTab::setPrevText(MainTextDocument *prevDoc)
{
    if(prevDoc == 0){
        prevTextZone->setHidden(true);
        return;
    }

    prevTextDocument = prevDoc;
    prevTextZone->setDoc(prevTextDocument);
    prevTextZone->document()->adjustSize();


    QTextCursor curs =  prevTextZone->textCursor();
    curs.movePosition(QTextCursor::End);
    prevTextZone->setTextCursor(curs);
}

//-------------------------------------------------------------------------------

void TextTab::showNextText(bool showNextTextBool)
{
    nextTextZone->setHidden(!showNextTextBool);
    nextTextZone->setMaximumHeight(textZone->height()/3);
    textZone->setFocus();
    textZone->ensureCursorVisible();
}

//-------------------------------------------------------------------------------
bool TextTab::setShowNextTextButton()
{
    return !nextTextZone->isHidden();
}

//-------------------------------------------------------------------------------
void  TextTab::setNextText(MainTextDocument *nextDoc)
{
    if(nextDoc == 0){
        nextTextZone->setHidden(true);
        return;
    }

    nextTextDocument = nextDoc;
    nextTextZone->setDoc(nextTextDocument);
    nextTextZone->document()->adjustSize();


    QTextCursor curs =  nextTextZone->textCursor();
    curs.movePosition(QTextCursor::Start);
    nextTextZone->setTextCursor(curs);
}



//-------------------------------------------------------------------------------
void  TextTab::addSlimFindReplaceAction()
{
    QList<QAction*> actions;



    QAction *findAct = new QAction(QIcon(":/pics/edit-find-replace.png"),tr("Find && replace"), this);
    findAct->setShortcuts(QKeySequence::Find);
    findAct->setStatusTip(tr("Find text"));
    connect(findAct, SIGNAL(triggered()), this, SLOT(launchSlimFindReplace()));

    actions.append(findAct);

    textZone->addActions(actions);

}
//-------------------------------------------------------------------------------

void  TextTab::launchSlimFindReplace()
{
    SlimFindReplace *findReplace = new SlimFindReplace(this);
    findReplace->setHub(hub);
    findReplace->setDocument(textDocument);
    findReplace->setTextEdit(textZone);

    textZoneVLayout->insertWidget(-1, findReplace);

}

//-------------------------------------------------------------------------------
void TextTab::applyConfig()
{
    textZone->applyConfig();


    QSettings settings;
    settings.beginGroup( "Settings" );
    //    int bottMargin = settings.value("TextArea/bottomMargin", 10).toInt();
    //    int textIndent = settings.value("TextArea/textIndent", 20).toInt();
    //    int textHeight = settings.value("TextArea/textHeight", 12).toInt();
    //    QString fontFamily = settings.value("TextArea/textFontFamily", "Liberation Serif").toString();
    changeWidthSlot(settings.value("TextArea/textWidth", this->width()/2).toInt());
    settings.endGroup();



}


void TextTab::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

//-------------------------------------------------------------------
void TextTab::giveStyle()
{
    QString css = ""
            "TextZone {    border-width: 0px;"
            "border-style: outset;"
            "border-radius: 0px;"
//            "margin: 4px"
            "}"
            "QSplitter {"
            "border: 0px none transparent;"
            "spacing: 0px;"
            "padding: 0px;"
            "margin: 0px;"
            "}"
            "QSplitter::handle {"
            "background-color: gray;"
            "height: 1px"
            "}"
            ;




    setStyleSheet(css);
}
