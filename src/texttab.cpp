#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   

#include "texttab.h"
#include "ui_texttab.h"
//


TextTab::TextTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextTab)
{
    ui->setupUi(this);



    prevTextDocument = new MainTextDocument(this);
    textDocument = new MainTextDocument(this);
    nextTextDocument = new MainTextDocument(this);

    QScrollBar *baseScrollBar = ui->textZone->verticalScrollBar();

    connect(baseScrollBar, SIGNAL(rangeChanged(int,int)), this, SLOT(scrollBar_setRange(int, int)));
    connect(baseScrollBar, SIGNAL(valueChanged(int)), ui->verticalScrollBar, SLOT(setValue(int)));
    connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), baseScrollBar, SLOT(setValue(int)));
ui->textZone->verticalScrollBar()->hide();
//    ui->textZone->setVerticalScrollBar(vertScrollBar);





//    prevTextZone = new TextZone(this);
//    prevTextZone->setHub(hub);
//    prevTextZone->toHtml();

//    nextTextZone = new TextZone(this);
//    nextTextZone->setHub(hub);
//    nextTextZone->toHtml();


//    textZone = new TextZone(this);
//    textZone->setHub(hub);
//    textZone->toHtml();

//    QWidget *textZoneWidget = new QWidget;
//    textZoneVLayout = new QVBoxLayout;
//    textZoneVLayout->setSpacing(0);
//    textZoneVLayout->addWidget(textZone);
//    textZoneWidget->setLayout(textZoneVLayout);

//    QHBoxLayout *layout = new QHBoxLayout;
//    QSplitter *splitter = new QSplitter;
//    splitter->setOrientation(Qt::Vertical);
//    splitter->addWidget(prevTextZone);
//    splitter->addWidget(textZoneWidget);
//    splitter->addWidget(nextTextZone);



//    layout->addStretch();
//    //    layout->addLayout(vLayout);
//    layout->addWidget(splitter);
//    layout->addStretch();

//    setLayout(layout);


    setContextMenuPolicy(Qt::PreventContextMenu);




    ui->prevTextZone->hide();
    ui->nextTextZone->hide();


    QList<int> sizesList;
    sizesList << 0 << ui->textZoneWidget->height()  << 0;
    ui->splitter->setSizes(sizesList);






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

    ui->prevTextZone->setTextStyles(textStyles);
    ui->prevTextZone->createContent();
    ui->nextTextZone->setTextStyles(textStyles);
    ui->nextTextZone->createContent();
    ui->textZone->setTextStyles(textStyles);
    ui->textZone->createContent();






    //  stackName = name;

    textDocument = doc;
    QApplication::processEvents();
    ui->textZone->setDoc(textDocument);
    QApplication::processEvents();
    ui->textZone->document()->adjustSize();

// slim find replace :

    this->addSlimFindReplaceAction();
    ui->findReplace->setDocument(textDocument);
    ui->findReplace->setHub(hub);
    ui->findReplace->setTextEdit(ui->textZone);
ui->findReplace->hide();

    connect(ui->textZone, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChangedSlot()));
    connect(ui->textZone,SIGNAL(charFormatChangedSignal(QTextCharFormat)), this,SIGNAL(charFormatChangedSignal(QTextCharFormat)));
    connect(ui->textZone,SIGNAL(textChanged()), this,SIGNAL(textChangedSignal()));
    connect(ui->textZone,SIGNAL(styleSelectedSignal(int)), this,SLOT(changeTextStyleSlot(int)));
    connect(this, SIGNAL(setStyleSelectionSignal(int)), ui->textZone, SIGNAL(setStyleSelectionSignal(int)));
    connect(ui->textZone,SIGNAL(manageStylesSignal()), this,SIGNAL(manageStylesSignal()));

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

    ui->textZone->setFixedWidth(width);
    ui->textZone->document()->setTextWidth(width);
    ui->prevTextZone->setFixedWidth(width);
    ui->prevTextZone->document()->setTextWidth(width - scrollBarWidth);
    ui->nextTextZone->setFixedWidth(width);
    ui->nextTextZone->document()->setTextWidth(width - scrollBarWidth);
}
//-------------------------------------------------------------------------------
void TextTab::changeTextFontSlot(QFont font)
{
    ui->textZone->setTextFont(font);
}
//-------------------------------------------------------------------------------
void TextTab::changeTextHeightSlot(int height)
{
    ui->textZone->setTextHeight(height);
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


    QTextCursor tCursor = ui->textZone->textCursor();

    // select all of the blocks selected :

    QTextCursor tStartCursor = ui->textZone->textCursor();
    tStartCursor.setPosition(tCursor.selectionStart());
    tStartCursor.movePosition(QTextCursor::StartOfBlock);
    int startFirstBlock = tStartCursor.position();

    QTextCursor tEndCursor = ui->textZone->textCursor();
    tEndCursor.setPosition(tCursor.selectionEnd());
    tEndCursor.movePosition(QTextCursor::EndOfBlock);
    int endLastBlock = tEndCursor.position();

    tCursor.setPosition(startFirstBlock);
    tCursor.setPosition(endLastBlock, QTextCursor::KeepAnchor);


    // merge :
    tCursor.mergeBlockFormat(blockFormat);
    tCursor.mergeCharFormat(charFormat);
    ui->textZone->mergeCurrentCharFormat(charFormat);

}


//-------------------------------------------------------------------------------
void TextTab::setTextFocus()
{
    if(ui->textZone->isVisible())
        ui->textZone->setFocus();

}
//-------------------------------------------------------------------------------
void TextTab::setCursorPos(int pos)
{
    ui->textZone->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
    for(int i = 0; i < pos ; i++)
        ui->textZone->moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);


    ui->textZone->ensureCursorVisible();
}
//-------------------------------------------------------------------------------
int TextTab::cursorPos()
{
    QTextCursor cursor = ui->textZone->textCursor();
    return cursor.position();

}
//-------------------------------------------------------------------------------
QTextCharFormat TextTab::tabFontChangedSlot()
{
    return ui->textZone->textCursor().charFormat();
}

//-------------------------------------------------------------------------------
void TextTab::cursorPositionChangedSlot()
{
    QTextCursor tCursor = ui->textZone->textCursor();

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

    ui->textZone->document()->setTextWidth(ui->textZone->width() - 20);
    ui->prevTextZone->document()->setTextWidth(ui->prevTextZone->width() - 20);

    //    qDebug() << "updateTextZone";
}


//-------------------------------------------------------------------------------

void TextTab::showPrevText(bool showPrevTextBool)
{
    ui->prevTextZone->setHidden(!showPrevTextBool);
    ui->prevTextZone->setMaximumHeight(ui->textZone->height()/3);
    ui->textZone->setFocus();
    ui->textZone->ensureCursorVisible();

}

//-------------------------------------------------------------------------------
bool TextTab::setShowPrevTextButton()
{
    return !ui->prevTextZone->isHidden();
}

//-------------------------------------------------------------------------------
void  TextTab::setPrevText(MainTextDocument *prevDoc)
{
    if(prevDoc == 0){
        ui->prevTextZone->setHidden(true);
        ui->prevTextToolButton->setHidden(true);
        return;
    }
    ui->prevTextZone->setHidden(false);
    ui->prevTextToolButton->setHidden(false);

    prevTextDocument = prevDoc;
    ui->prevTextZone->setDoc(prevTextDocument);
    ui->prevTextZone->document()->adjustSize();


    QTextCursor curs =  ui->prevTextZone->textCursor();
    curs.movePosition(QTextCursor::End);
    ui->prevTextZone->setTextCursor(curs);
}

//-------------------------------------------------------------------------------
void  TextTab::setPrevButtonState(bool state)
{
    if(state)
        ui->prevTextToolButton->setArrowType(Qt::UpArrow);
else
        ui->prevTextToolButton->setArrowType(Qt::DownArrow);
}

//-------------------------------------------------------------------------------

void TextTab::on_prevTextToolButton_clicked()
{
    if(ui->prevTextToolButton->arrowType() == Qt::DownArrow){


        QList<int> sizesList;
        sizesList << ui->textZone->height()/3 << ui->textZone->height()  << ui->nextTextZone->height();
        ui->splitter->setSizes(sizesList);

        ui->textZone->setFocus();
        ui->textZone->ensureCursorVisible();

        this->setPrevButtonState(true);
    }
    else{
        QList<int> sizesList;
        sizesList << 0 << ui->textZone->height()  << ui->nextTextZone->height() ;
        ui->splitter->setSizes(sizesList);


        ui->textZone->setFocus();
        ui->textZone->ensureCursorVisible();
        this->setPrevButtonState(false);

}
}



//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------

void TextTab::showNextText(bool showNextTextBool)
{
    ui->nextTextZone->setHidden(!showNextTextBool);
    ui->nextTextZone->setMaximumHeight(ui->textZone->height()/3);
    ui->textZone->setFocus();
    ui->textZone->ensureCursorVisible();
}

//-------------------------------------------------------------------------------
bool TextTab::setShowNextTextButton()
{
    return !ui->nextTextZone->isHidden();
}

//-------------------------------------------------------------------------------
void  TextTab::setNextText(MainTextDocument *nextDoc)
{
    if(nextDoc == 0){
        ui->nextTextZone->setHidden(true);
        ui->nextTextToolButton->setHidden(true);
        return;
    }
    ui->nextTextZone->setHidden(false);
    ui->nextTextToolButton->setHidden(false);

    nextTextDocument = nextDoc;
    ui->nextTextZone->setDoc(nextTextDocument);
    ui->nextTextZone->document()->adjustSize();


    QTextCursor curs =  ui->nextTextZone->textCursor();
    curs.movePosition(QTextCursor::Start);
    ui->nextTextZone->setTextCursor(curs);
}

//-------------------------------------------------------------------------------
void  TextTab::setNextButtonState(bool state)
{
    if(state)
        ui->nextTextToolButton->setArrowType(Qt::DownArrow);
else
        ui->nextTextToolButton->setArrowType(Qt::UpArrow);
}

//-------------------------------------------------------------------------------

void TextTab::on_nextTextToolButton_clicked()
{
    if(ui->nextTextToolButton->arrowType() == Qt::UpArrow){


        QList<int> sizesList;
        sizesList << ui->prevTextZone->height() << ui->textZone->height()  << ui->textZone->height()/3;
        ui->splitter->setSizes(sizesList);

        ui->textZone->setFocus();
        ui->textZone->ensureCursorVisible();

        this->setNextButtonState(true);
    }
    else{
        QList<int> sizesList;
        sizesList << ui->prevTextZone->height() << ui->textZone->height()  << 0;
        ui->splitter->setSizes(sizesList);


        ui->textZone->setFocus();
        ui->textZone->ensureCursorVisible();
        this->setNextButtonState(false);

}

}

//-------------------------------------------------------------------------------
void TextTab::on_splitter_splitterMoved(int pos, int index)
{
    qDebug() << "pos : "<< QString::number(pos) << "index : "<<QString::number(index) ;
    if(index == 1 && pos != 0)
        this->setPrevButtonState(true);
    if(index == 1 && pos == 0)
        this->setPrevButtonState(false);
    if(index == 2 && pos != ui->splitter->maximumHeight())
        this->setNextButtonState(true);
    if(index == 2 && pos == ui->splitter->maximumHeight())
        this->setNextButtonState(false);
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

    ui->textZone->addActions(actions);

}
//-------------------------------------------------------------------------------

void  TextTab::launchSlimFindReplace()
{
    ui->findReplace->show();

}

//-------------------------------------------------------------------------------
void TextTab::applyConfig()
{
    ui->textZone->applyConfig();


    QSettings settings;
    settings.beginGroup( "Settings" );
    //    int bottMargin = settings.value("TextArea/bottomMargin", 10).toInt();
    //    int textIndent = settings.value("TextArea/textIndent", 20).toInt();
    //    int textHeight = settings.value("TextArea/textHeight", 12).toInt();
    //    QString fontFamily = settings.value("TextArea/textFontFamily", "Liberation Serif").toString();
    changeWidthSlot(settings.value("TextArea/textWidth", this->width()/2).toInt());
    settings.endGroup();



}



//-------------------------------------------------------------------
void TextTab::giveStyle()
{

}
//-------------------------------------------------------------------


void TextTab::paintEvent(QPaintEvent *)
 {
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
 }
//-------------------------------------------------------------------

void TextTab::resizeEvent(QResizeEvent *event)
{
//    int percent = ui->textZone->width()/this->width();

//    int w = event->size().width();

//    ui->textZone->setFixedWidth(w*percent);
//    ui->prevTextZone->setFixedWidth(w*percent);
//    ui->nextTextZone->setFixedWidth(w*percent);

//    ui->textZone->document()->setTextWidth(width - scrollBarWidth);
//    ui->prevTextZone->document()->setTextWidth(width - scrollBarWidth);
//    ui->nextTextZone->document()->setTextWidth(width - scrollBarWidth);
}

//-------------------------------------------------------------------
void TextTab::scrollBar_setRange(int min, int max)
{
    ui->verticalScrollBar->setMinimum(min);
    ui->verticalScrollBar->setMaximum(max);

    if(min == 0 && max == 0)
        ui->verticalScrollBar->hide();
    else
        ui->verticalScrollBar->show();

}

