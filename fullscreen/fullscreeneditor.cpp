#include "fullscreen/fullscreeneditor.h"
#include "ui_fullscreeneditor.h"
#include <QtGui>

FullscreenEditor::FullscreenEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullscreenEditor)
{
    ui->setupUi(this);
    setWindowModality(Qt::ApplicationModal);


}

FullscreenEditor::~FullscreenEditor()
{


    settings.setValue( "FullTextArea/areaWidth", sliderCurrentValue);
    settings.setValue( "FullTextArea/zoom", textStyles->zoomModifier());

    if(textStyles->zoomModifier() != 0){
        textStyles->changeDocStyles(ui->fullTextEdit->document(), "removeZoom");
        originalDoc->setHtml(ui->fullTextEdit->document()->toHtml());
    }
    else if(textStyles->zoomModifier() == 0){
        originalDoc->setHtml(ui->fullTextEdit->document()->toHtml());
}

    delete ui;
}
//------------------------------------------------------------------------------------

void FullscreenEditor::createContent(QTextDocument *doc, int cursorPos)
{
    this->setAttribute(Qt::WA_DeleteOnClose);

    originalDoc = doc;
    QTextDocument *clonedDoc = doc->clone();

    textStyles->setZoomModifier(settings.value( "FullTextArea/zoom", 0).toInt());
    textStyles->changeDocStyles(clonedDoc, "applyZoom");

    ui->fullTextEdit->setDoc(clonedDoc);
    ui->fullTextEdit->setTextStyles(textStyles);
    ui->fullTextEdit->applyTextConfig();
    ui->fullTextEdit->setFrameShape(QFrame::NoFrame);


    createNotesMenu();

    createOptionMenu();


    //        QPushButton *showSynButton = new QPushButton(tr("Synopsis"), this);
    //        connect(showSynButton, SIGNAL(clicked()),this,SLOT(showSyn()));
    //        showSynButton->setFixedSize(50,20);

    //        QPushButton *showNoteButton = new QPushButton(tr("Note"), this);
    //        connect(showNoteButton, SIGNAL(clicked()),this,SLOT(showNote()));
    //        showNoteButton->setFixedSize(50,20);

    //        QPushButton *exitFullscreenButton = new QPushButton(tr("Exit"), this);
    //        connect(exitFullscreenButton, SIGNAL(clicked()),this,SLOT(close()));
    //        exitFullscreenButton->setFixedSize(50,20);




    connect(ui->fullTextEdit, SIGNAL(quitFullScreen()),this,SLOT(close()));
    connect(editWidget, SIGNAL(styleSelectedSignal(int)), this, SLOT(changeTextStyleSlot(int)));

    applyConfig();

    this->showFullScreen();

    // get screen width for fullscreen text width option :
    editWidget->setTextWidthMax( QApplication::desktop()->screenGeometry().width() * 0.75);
    this->loadTextWidthSliderValue();

    //set cursor position :
    for(int i = 0; i < cursorPos ; i++)
        ui->fullTextEdit->moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);

    //for wordcount :
    fullscreenWordCount = new WordCount(clonedDoc, this);
    //connect(tabWordCount, SIGNAL(charCountSignal(int)), this, SLOT(charCountUpdated(int)));
    connect(fullscreenWordCount, SIGNAL(wordCountSignal(int)), this, SLOT(setWordCount(int)));
//    connect(tabWordCount, SIGNAL(blockCountSignal(int)), this, SLOT(blockCountUpdated(int)));
    fullscreenWordCount->updateAll();




    this->setWindowState(this->windowState() | Qt::WindowActive);
    ui->fullTextEdit->ensureCursorVisible();

    connect(ui->fullTextEdit, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChangedSlot()));

    mouseTimer = new QTimer(this);
    connect(mouseTimer, SIGNAL(timeout()), this, SLOT(hideMouse()));
    mouseTimer->start(3000);

    ui->timerBuddyLabel->hide();
    ui->timerLabel->hide();

}

//------------------------------------------------------------------------------------
void FullscreenEditor::createNotesMenu()
{
    QMenu *notesMenu = new QMenu();


    QAction *synAct = new QAction(/*QIcon(":/pics/edit-find-replace.png"),*/tr("Synopsis"),this);
    synAct->setShortcut(Qt::Key_F9);
    synAct->setToolTip(tr("Show the synopsis"));
    connect(synAct, SIGNAL(triggered()),this,SLOT(showSyn()));

    QAction *noteAct = new QAction(/*QIcon(":/pics/edit-find-replace.png"),*/tr("Note"),this);
    noteAct->setShortcut(Qt::Key_F10);
    noteAct->setToolTip(tr("Show the note"));
    connect(noteAct, SIGNAL(triggered()),this,SLOT(showNote()));


    notesMenu->addAction(synAct);
    notesMenu->addAction(noteAct);
    ui->notesButton->setMenu(notesMenu);
}
//------------------------------------------------------------------------------------


void FullscreenEditor::zoomIn()
{
    textStyles->setPreviousZoomModifier(textStyles->zoomModifier());
    textStyles->setZoomModifier(textStyles->zoomModifier() + 1);
    textStyles->changeDocStyles(ui->fullTextEdit->document(), "modifyZoom");
}

//------------------------------------------------------------------------------------

void FullscreenEditor::zoomOut()
{
    textStyles->setPreviousZoomModifier(textStyles->zoomModifier());
    textStyles->setZoomModifier(textStyles->zoomModifier() - 1);
    textStyles->changeDocStyles(ui->fullTextEdit->document(), "modifyZoom");
}
//------------------------------------------------------------------------------------

void FullscreenEditor::createOptionMenu()
{
    editWidget = new EditMenu();
    editWidget->setTextStyles(textStyles);
    editWidget->createContent();

    connect(editWidget, SIGNAL(zoomInSignal()), this, SLOT(zoomIn()));
    connect(editWidget, SIGNAL(zoomOutSignal()), this, SLOT(zoomOut()));


    QMenu *optionsMenu = new QMenu();

    QWidgetAction *editWidgetAct = new QWidgetAction(this);
    editWidgetAct->setDefaultWidget(editWidget);

    QAction *manageStylesAct = new QAction(/*QIcon(":/pics/edit-find-replace.png"),*/tr("Manage &Styles"),this);
    manageStylesAct->setShortcut(Qt::Key_F11);
    manageStylesAct->setToolTip(tr("Manage the styles"));
    connect(manageStylesAct, SIGNAL(triggered()), this, SIGNAL(manageStylesSignal()));

    QAction *setColorsAct = new QAction(/*QIcon(":/pics/edit-find-replace.png"),*/tr("Colors"),this);
    setColorsAct->setShortcut(Qt::Key_F12);
    setColorsAct->setToolTip(tr("Set the colors"));
    connect(setColorsAct, SIGNAL(triggered()), this, SLOT(callColorDialog()));

    optionsMenu->addAction(editWidgetAct);
    optionsMenu->addAction(manageStylesAct);
    optionsMenu->addAction(setColorsAct);
    ui->optionsButton->setMenu(optionsMenu);
}



//------------------------------------------------------------------------------------


void FullscreenEditor::setWordCount(int num)
{
    QString text;
    ui->wordCountLabel->setText(text.setNum(num,10));
}
//----------------------------------------------------------------------------------------
void FullscreenEditor::setTimer(QString text)
{
    ui->timerBuddyLabel->show();
    ui->timerLabel->show();
    if(text.left(1) == "-"){
        ui->timerBuddyLabel->hide();
        ui->timerLabel->hide();
    }
    ui->timerLabel->setText(text);
}



//----------------------------------------------------------------------------------------

void FullscreenEditor::closeEvent(QCloseEvent* event)
{
    synWidget->close();
    noteWidget->close();
    mouseTimer->stop();
    emit closeSignal();
    event->accept();


}

//----------------------------------------------------------------------------------------
void FullscreenEditor::mouseMoveEvent(QMouseEvent* event)
{


    this->unsetCursor();
    ui->fullTextEdit->viewport()->setCursor(QCursor(Qt::IBeamCursor));


    mouseTimer->stop();
    mouseTimer->start();
    event->accept();

}
//----------------------------------------------------------------------------------------
void FullscreenEditor::hideMouse()
{

    this->setCursor(QCursor(Qt::BlankCursor));
    ui->fullTextEdit->viewport()->setCursor(QCursor(Qt::BlankCursor));


}





//----------------------------------------------------------------------------------------


void FullscreenEditor::callColorDialog()
{
    QWidget *fullColorDialog = new QWidget(this, Qt::Dialog);
    QVBoxLayout *layout = new QVBoxLayout;

    QLabel *fullColorLabel = new QLabel(tr("FullScreen Colors :"));



    QFormLayout *colorsLayout = new QFormLayout;
    backColorButton = new QPushButton(fullColorDialog);
    backColorButton->setObjectName("backColorButton");
    textBackColorButton = new QPushButton(fullColorDialog);
    textBackColorButton->setObjectName("textBackColorButton");
    textColorButton = new QPushButton(fullColorDialog);
    textColorButton->setObjectName("textColorButton");
    addOnColorButton = new QPushButton(fullColorDialog);
    addOnColorButton->setObjectName("addOnColorButton");

    connect(backColorButton,SIGNAL(clicked()), this, SLOT(setBackColorDialog()));
    connect(textBackColorButton,SIGNAL(clicked()), this, SLOT(setTextBackColorDialog()));
    connect(textColorButton,SIGNAL(clicked()), this, SLOT(setTextColorDialog()));
    connect(addOnColorButton,SIGNAL(clicked()), this, SLOT(setAddOnColorDialog()));


    colorsLayout->addRow(tr("&Background color:"), backColorButton);
    colorsLayout->addRow(tr("Text b&ackground color:"), textBackColorButton);
    colorsLayout->addRow(tr("&Text color:"), textColorButton);
    colorsLayout->addRow(tr("&Timers color:"), addOnColorButton);



    QPushButton *closeButton = new QPushButton(tr("Ok"),fullColorDialog);
    connect(closeButton,SIGNAL(clicked()), fullColorDialog, SLOT(close()));

    layout->addWidget(fullColorLabel);
    layout->addLayout(colorsLayout);
    layout->addWidget(closeButton);
    fullColorDialog->setLayout(layout);



    fullColorDialog->show();
}
//----------------------------------------------------------------------------------------

void FullscreenEditor::setSyn(QTextDocument *doc, int cursorPos)
{
    synWidget = new QWidget(this, Qt::Tool | Qt::WindowStaysOnTopHint);

    FullTextZone *fullSynEdit = new FullTextZone(synWidget);
    fullSynEdit->setDoc(doc);
    fullSynEdit->applySynConfig();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(fullSynEdit);

    synWidget->setLayout(layout);
    synWidget->setWindowTitle(tr("Synopsis"));

    //set cursor position :
    for(int i = 0; i < cursorPos ; i++)
        fullSynEdit->moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);

    fullSynEdit->ensureCursorVisible();

    //    synDoc = doc;
    //    synCursorPos = cursorPos;

}
//----------------------------------------------------------------------------------------

void FullscreenEditor::setNote(QTextDocument *doc, int cursorPos)
{


    noteWidget = new QWidget(this, Qt::Tool | Qt::WindowStaysOnTopHint);

    FullTextZone *fullNoteEdit = new FullTextZone(noteWidget);
    fullNoteEdit->setDoc(doc);
    fullNoteEdit->applyNoteConfig();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(fullNoteEdit);

    noteWidget->setLayout(layout);
    noteWidget->setWindowTitle(tr("Notes"));

    //set cursor position :
    for(int i = 0; i < cursorPos ; i++)
        fullNoteEdit->moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);

    fullNoteEdit->ensureCursorVisible();
    //    noteDoc = doc;
    //    noteCursorPos = cursorPos;
}
//----------------------------------------------------------------------------------------
void FullscreenEditor::showSyn()
{
    synWidget->show();
}
//----------------------------------------------------------------------------------------
void FullscreenEditor::showNote()
{
    noteWidget->show();

}

//----------------------------------------------------------------------------------------

void FullscreenEditor::loadTextWidthSliderValue()
{
    QSettings settings;
    int sliderValue = settings.value("FullTextArea/areaWidth", 600).toInt();
    editWidget->setTextWidthSliderValue(sliderValue);


    sliderTextWidthValueChanged(sliderValue);
    connect(editWidget, SIGNAL(textWidthSliderValueChanged(int)), this, SLOT(sliderTextWidthValueChanged(int)), Qt::UniqueConnection);

}

//----------------------------------------------------------------------------------------

void FullscreenEditor::sliderTextWidthValueChanged(int value)
{
    sliderCurrentValue = value;
    ui->fullTextEdit->setFixedWidth(value);


}

void FullscreenEditor::resetFullscreenTextWidthSlot()
{




    ui->fullTextEdit->setFixedWidth(sliderCurrentValue + 1);
    ui->fullTextEdit->setFixedWidth(sliderCurrentValue - 1);


}
//--------------------------------------------------------------------------------



void FullscreenEditor::setWidth()
{
    //    QWidget *widthDialog = new QWidget(this, Qt::Dialog);
    //    QVBoxLayout *layout = new QVBoxLayout;

    //    QLabel *textZoneWidthLabel = new QLabel(tr("Text Area Width :"));
    //    widthSlider = new QSlider(Qt::Horizontal);
    //    widthSlider->setRange(500, xMax);

    //    QPushButton *closeButton = new QPushButton(tr("Ok"),widthDialog);
    //    connect(closeButton,SIGNAL(clicked()), widthDialog, SLOT(close()));

    //    layout->addWidget(textZoneWidthLabel);
    //    layout->addWidget(widthSlider);
    //    layout->addWidget(closeButton);
    //    widthDialog->setLayout(layout);


    //    loadSliderValue();

    //    widthDialog->show();

}

//----------------------------------------------------------------------------------------
void FullscreenEditor::setZoom()
{


}


//-------------------------------------------------------------------------------
void FullscreenEditor::cursorPositionChangedSlot()
{
    QTextCursor tCursor = ui->fullTextEdit->textCursor();

    if((tCursor.atStart() == true
        || tCursor.position() == 1
        || tCursor.position() == 0) && tCursor.hasSelection() == false){
        this->changeTextStyleSlot(textStyles->defaultStyleIndex());
    }

    int currentStyleIndex = textStyles->compareStylesWithText(tCursor.charFormat(), tCursor.blockFormat(), "zoom");
//    qDebug() << "currentStyleIndex : " << QString::number(currentStyleIndex);
    editWidget->setStyleSelectionSlot(currentStyleIndex);

}


//-------------------------------------------------------------------------------
void FullscreenEditor::changeTextStyleSlot(int styleIndex)
{
    qDebug() << "changeTextStyleSlot";

    QTextBlockFormat blockFormat;
    blockFormat.setBottomMargin(textStyles->blockBottomMarginAt(styleIndex));
    blockFormat.setTextIndent(textStyles->blockFirstLineIndentAt(styleIndex));
    blockFormat.setLeftMargin(textStyles->blockLeftMarginAt(styleIndex));
    blockFormat.setAlignment(textStyles->blockAlignmentTrueNameAt(styleIndex));
    blockFormat.setTopMargin(0);
    blockFormat.setRightMargin(0);
    QTextCharFormat charFormat;
    charFormat.setFontPointSize(textStyles->fontSizeAt(styleIndex) + textStyles->zoomModifier());
    charFormat.setFontFamily(textStyles->fontFamilyAt(styleIndex));
    //    charFormat.setFontItalic(textStyles->fontItalicAt(styleIndex));
    //    if (textStyles->fontBoldAt(styleIndex) == true)
    //        charFormat.setFontWeight(75);
    //    else
    //        charFormat.setFontWeight(50);
    //    charFormat.setFontUnderline(textStyles->fontUnderlineAt(styleIndex));
    //    charFormat.setFontStrikeOut(textStyles->fontStrikeOutAt(styleIndex));

    //    charFormat.clearForeground();


    QTextCursor tCursor = ui->fullTextEdit->textCursor();

    // select all of the blocks selected :

    QTextCursor tStartCursor = ui->fullTextEdit->textCursor();
    tStartCursor.setPosition(tCursor.selectionStart());
    tStartCursor.movePosition(QTextCursor::StartOfBlock);
    int startFirstBlock = tStartCursor.position();

    QTextCursor tEndCursor = ui->fullTextEdit->textCursor();
    tEndCursor.setPosition(tCursor.selectionEnd());
    tEndCursor.movePosition(QTextCursor::EndOfBlock);
    int endLastBlock = tEndCursor.position();

    tCursor.setPosition(startFirstBlock);
    tCursor.setPosition(endLastBlock, QTextCursor::KeepAnchor);


    // merge :
    tCursor.mergeBlockFormat(blockFormat);
    tCursor.mergeCharFormat(charFormat);
    ui->fullTextEdit->mergeCurrentCharFormat(charFormat);

}


//---------------------------------------------------------------------------
//----------Style sheets---------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------


void FullscreenEditor::setBaseStyleSheet()
{
    //baseStyrlSheet = "FullTextZone {
}


void FullscreenEditor::setBackColorDialog()
{


    QColor backgroundColor = QColorDialog::getColor(Qt::white, this, "Background Color" /*, QColorDialog::ShowAlphaChannel*/);


    QString string;
    QString r = string.setNum(backgroundColor.red(),10);
    QString g = string.setNum(backgroundColor.green(), 10);
    QString b = string.setNum(backgroundColor.blue(),10);


    settings.setValue("FullTextArea/backgroundColor", backgroundColor);
    setBackColor();
    applyStyleSheet();
}

void FullscreenEditor::setBackColor()
{
    QColor backgroundColor = settings.value("FullTextArea/backgroundColor", Qt::black).toString();

    QString string;
    QString r = string.setNum(backgroundColor.red(),10);
    QString g = string.setNum(backgroundColor.green(), 10);
    QString b = string.setNum(backgroundColor.blue(),10);

    backColorString = "QWidget#mainBackground {background-color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QPushButton#backColorButton {background-color: rgb(" + r + ", " + g + ", " + b + ");}";



}


void FullscreenEditor::setTextBackColorDialog()
{
    QColor textBackColor = QColorDialog::getColor(Qt::white, this, "Background Color"/* , QColorDialog::ShowAlphaChannel*/);


    QString string;
    QString r = string.setNum(textBackColor.red(),10);
    QString g = string.setNum(textBackColor.green(), 10);
    QString b = string.setNum(textBackColor.blue(),10);



    settings.setValue("FullTextArea/textBackgroundColor", textBackColor);

    setTextBackColor();
    applyStyleSheet();
}

void FullscreenEditor::setTextBackColor()
{
    QColor textBackgroundColor = settings.value("FullTextArea/textBackgroundColor", Qt::black).toString();

    QString string;
    QString r = string.setNum(textBackgroundColor.red(),10);
    QString g = string.setNum(textBackgroundColor.green(), 10);
    QString b = string.setNum(textBackgroundColor.blue(),10);

    textBackColorString = "FullTextZone {background-color: rgb(" + r + ", " + g + ", " + b + ");"
            "border: 0px none black; border-radius: 0px;"
            "}"
            "QPushButton#textBackColorButton {background-color: rgb(" + r + ", " + g + ", " + b + ");}";


}

void FullscreenEditor::setTextColorDialog()
{
    QColor textColor = QColorDialog::getColor(Qt::white, this, "Background Color"/* , QColorDialog::ShowAlphaChannel*/);


    QString string;
    QString r = string.setNum(textColor.red(),10);
    QString g = string.setNum(textColor.green(), 10);
    QString b = string.setNum(textColor.blue(),10);



    settings.setValue("FullTextArea/textColor", textColor);

    setTextColor();
    applyStyleSheet();
}

void FullscreenEditor::setTextColor()
{
    QColor textColor = settings.value("FullTextArea/textColor", Qt::lightGray).toString();

    QString string;
    QString r = string.setNum(textColor.red(),10);
    QString g = string.setNum(textColor.green(), 10);
    QString b = string.setNum(textColor.blue(),10);

    textColorString = "FullTextZone {color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QPushButton#textColorButton {background-color: rgb(" + r + ", " + g + ", " + b + ");}";
}

void FullscreenEditor::setAddOnColorDialog()
{
    QColor addOnColor = QColorDialog::getColor(Qt::white, this, "Timers Color"/* , QColorDialog::ShowAlphaChannel*/);


    QString string;
    QString r = string.setNum(addOnColor.red(),10);
    QString g = string.setNum(addOnColor.green(), 10);
    QString b = string.setNum(addOnColor.blue(),10);



    settings.setValue("FullTextArea/addOnColor", addOnColor);

    setAddOnColor();
    applyStyleSheet();
}

void FullscreenEditor::setAddOnColor()
{
    QColor addOnColor = settings.value("FullTextArea/addOnColor", Qt::lightGray).toString();

    QString string;
    QString r = string.setNum(addOnColor.red(),10);
    QString g = string.setNum(addOnColor.green(), 10);
    QString b = string.setNum(addOnColor.blue(),10);

    addOnColorString =
            "QLabel#timerLabel {color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QLabel#timerBuddyLabel {color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QLabel#clockLabel {color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QLabel#clockBuddyLabel {color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QLabel#wordCountLabel {color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QLabel#wordCountBuddyLabel {color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QPushButton#addOnColorButton {background-color: rgb(" + r + ", " + g + ", " + b + ");}";

    //    "QToolButton {background-color: red; border: none;}"
    ;
}




//void FullscreenEditor::setZoomIn()
//{

//// font-size: 20pt ;
//}



void FullscreenEditor::applyStyleSheet()
{
    setStyleSheet(backColorString + textBackColorString + textColorString + addOnColorString);
}



//---------------------------------------------------------------------------
//----------Apply Config---------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------


void FullscreenEditor::applyConfig()
{





    //style sheets :
    setBaseStyleSheet();
    setBackColor();
    setTextBackColor();
    setTextColor();
    setAddOnColor();

    applyStyleSheet();




}
