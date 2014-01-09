#include "fullscreen/fullscreeneditor.h"
#include "ui_fullscreeneditor.h"

FullscreenEditor::FullscreenEditor(QWidget *parent) :
    QWidget(parent), firstLaunch(true), currentCursorPos(0),
    ui(new Ui::FullscreenEditor)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowModality(Qt::ApplicationModal);



}

FullscreenEditor::~FullscreenEditor()
{

    this->restoreDoc();
    emit openSheetOnMainWindow(numberOfCurrentFullscreenSheet, currentCursorPos);

    this->showNormal();

    delete ui;
}
//------------------------------------------------------------------------------------
void FullscreenEditor::postConstructor()
{
    originalDoc = new MainTextDocument(this, hub->spellChecker());
    clonedDoc = new MainTextDocument(this, hub->spellChecker());
    clonedNoteDoc = new MainTextDocument(this, hub->spellChecker());
    clonedSynDoc = new MainTextDocument(this, hub->spellChecker());



    synWidget = new QWidget(this, Qt::Tool | Qt::WindowStaysOnTopHint);
    fullSynEdit = new FullTextZone(synWidget);
    fullSynEdit->setHub(hub);

    noteWidget = new QWidget(this, Qt::Tool | Qt::WindowStaysOnTopHint);
    fullNoteEdit = new FullTextZone(noteWidget);
    fullNoteEdit->setHub(hub);

    ui->progressBar->setHub(hub);
    ui->progressBar->postConstructor();
    ui->progressBar->init();

//    ui->fullTextEdit = new FullTextZone(this);
    ui->fullTextEdit->setHub(hub);
    connect(hub, SIGNAL(savingSignal()), this, SLOT(restoreDoc()));

    this->setAttribute(Qt::WA_DeleteOnClose);

    createNotesMenu();

    createOptionMenu();

    createTreeView();

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
    connect(ui->treeView, SIGNAL(currentOpenedSheetSignal(int)),absTreeModel, SLOT(  modifyDataForOpenedSheetMarker(int)), Qt::UniqueConnection);
    connect(this, SIGNAL(currentOpenedSheetSignal(int)),absTreeModel, SLOT(  modifyDataForOpenedSheetMarker(int)), Qt::UniqueConnection);

    applyConfig();


    mouseTimer = new QTimer(this);
    connect(mouseTimer, SIGNAL(timeout()), this, SLOT(hideMouse()));
    mouseTimer->start(6000);

    ui->timerBuddyLabel->hide();
    ui->timerLabel->hide();

    this->showFullScreen();

}

//------------------------------------------------------------------------------------
void FullscreenEditor::setText(MainTextDocument *doc)
{


    originalDoc = doc;
    // for wordcont goal :
    baseWordCount = originalDoc->wordCount();
    //    QTextDocument *clonedDoc = doc->clone();

    clonedDoc = new MainTextDocument(this, hub->spellChecker());

    // for wordcount :
    connect(clonedDoc, SIGNAL(wordCountChanged(QString,int,int)), this, SLOT(setWordCount(QString,int,int)), Qt::UniqueConnection);

    clonedDoc->setHtml(doc->toHtml());

    textStyles->setZoomModifier(settings.value( "FullTextArea/zoom", 0).toInt());
    textStyles->changeDocStyles(clonedDoc, "applyZoom");

    ui->fullTextEdit->setDoc(clonedDoc);
    ui->fullTextEdit->setTextStyles(textStyles);
    ui->fullTextEdit->applyTextConfig();
    ui->fullTextEdit->setFrameShape(QFrame::NoFrame);



    // get screen width for fullscreen text width option :
    editWidget->setTextWidthMax( QApplication::desktop()->screenGeometry().width() * 0.75);
    this->loadTextWidthSliderValue();


    //for autosave :

    connect(clonedDoc, SIGNAL(contentsChanged()), hub, SLOT(addToSaveQueue()), Qt::UniqueConnection);


    //    fullscreenWordCount = new WordCount(clonedDoc, this);

    //connect(tabWordCount, SIGNAL(charCountSignal(int)), this, SLOT(charCountUpdated(int)));
    //    connect(fullscreenWordCount, SIGNAL(countDeltaSignal(int)), ui->progressBar, SLOT(changeProgressBy(int)));
    //    connect(tabWordCount, SIGNAL(blockCountSignal(int)), this, SLOT(blockCountUpdated(int)));
    //    fullscreenWordCount->updateAll();




    this->setWindowState(this->windowState() | Qt::WindowActive);

    connect(ui->fullTextEdit, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChangedSlot()), Qt::UniqueConnection);


    //for wordcount goal :
    clonedDoc->connectWordCount();

    connect(clonedDoc, SIGNAL(wordCountChanged(QString,int,int)), this, SLOT(wordCountChangedSlot(QString,int,int)), Qt::UniqueConnection);

}

//------------------------------------------------------------------------------------
void FullscreenEditor::createNotesMenu()
{
    QMenu *notesMenu = new QMenu();


    //    QAction *synAct = new QAction(/*QIcon(":/pics/edit-find-replace.png"),*/tr("Synopsis"),this);
    //    synAct->setShortcut(Qt::Key_F2);
    //    synAct->setToolTip(tr("Show the synopsis"));
    //    connect(synAct, SIGNAL(triggered()),this,SLOT(showSyn()));

    QAction *notesAct = new QAction(/*QIcon(":/pics/edit-find-replace.png"),*/tr("Note"),this);
    notesAct->setShortcut(Qt::Key_F10);
    notesAct->setToolTip(tr("Show the notes"));
    connect(notesAct, SIGNAL(triggered()),this,SLOT(showNote()));
    connect(notesAct, SIGNAL(triggered()),this,SLOT(showSyn()));


    //    notesMenu->addAction(synAct);
    notesMenu->addAction(notesAct);
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

void FullscreenEditor::setShowFullScrollbar(bool value)
{

    saveConfig();
}
//------------------------------------------------------------------------------------

void FullscreenEditor::createOptionMenu()
{
    editWidget = new EditMenu();
    editWidget->setHub(hub);
    editWidget->setTextStyles(textStyles);
    editWidget->createContent();

    QStringList widgetToHideList;
    widgetToHideList << "fontComboBox" << "fontSizeSpinBox";
    editWidget->hideWidgetsByName(widgetToHideList);

    connect(editWidget, SIGNAL(zoomInSignal()), this, SLOT(zoomIn()));
    connect(editWidget, SIGNAL(zoomOutSignal()), this, SLOT(zoomOut()));


    QMenu *optionsMenu = new QMenu();

    QWidgetAction *editWidgetAct = new QWidgetAction(this);
    editWidgetAct->setDefaultWidget(editWidget);

    QAction *manageStylesAct = new QAction(/*QIcon(":/pics/edit-find-replace.png"),*/tr("Manage &Styles"),this);
    manageStylesAct->setShortcut(Qt::Key_F7);
    manageStylesAct->setToolTip(tr("Manage the styles"));
    connect(manageStylesAct, SIGNAL(triggered()), this, SIGNAL(manageStylesSignal()));

    QAction *setColorsAct = new QAction(/*QIcon(":/pics/edit-find-replace.png"),*/tr("Colors"),this);
    //    setColorsAct->setShortcut(Qt::Key_F12);
    setColorsAct->setToolTip(tr("Set the colors"));
    connect(setColorsAct, SIGNAL(triggered()), this, SLOT(callColorDialog()));

    showScrollbarAct = new QAction(/*QIcon(":/pics/edit-find-replace.png"),*/tr("Show Scroll Bar"),this);
    showScrollbarAct->setCheckable(true);
    showScrollbarAct->setToolTip(tr("Show the scroll bar"));
    connect(showScrollbarAct, SIGNAL(triggered(bool)), this, SLOT(setShowFullScrollbar(bool)));

    optionsMenu->addAction(editWidgetAct);
    optionsMenu->addAction(manageStylesAct);
    optionsMenu->addAction(setColorsAct);
    optionsMenu->addAction(showScrollbarAct);
    ui->optionsButton->setMenu(optionsMenu);
}



//------------------------------------------------------------------------------------


void FullscreenEditor::setWordCount(QString type, int id, int count)
{
    ui->wordCountLabel->setText(QString::number(count));
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
    saveConfig();

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

void FullscreenEditor::setSyn(MainTextDocument *doc)
{
    originalSynDoc = doc;

    fullSynEdit = new FullTextZone(synWidget);

    clonedSynDoc = new MainTextDocument();
    clonedSynDoc->setHtml(doc->toHtml());
    fullSynEdit->setDoc(clonedSynDoc);
    fullSynEdit->applySynConfig();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(fullSynEdit);

    synWidget->setLayout(layout);
    synWidget->setWindowTitle(tr("Synopsis"));

    //for autosave :

    connect(clonedSynDoc, SIGNAL(contentsChanged()), hub, SLOT(addToSaveQueue()), Qt::UniqueConnection);
    connect(fullSynEdit, SIGNAL(cursorPositionChangedSignal(int)), this, SLOT(synCursorPositionChangedSlot(int)), Qt::UniqueConnection);

    //    synDoc = doc;
    //    synCursorPos = cursorPos;

}
//----------------------------------------------------------------------------------------

void FullscreenEditor::setNote(MainTextDocument *doc)
{
    originalNoteDoc = doc;

    fullNoteEdit = new FullTextZone(noteWidget);

    clonedNoteDoc = new MainTextDocument();
    clonedNoteDoc->setHtml(doc->toHtml());
    fullNoteEdit->setDoc(clonedNoteDoc);
    fullNoteEdit->applyNoteConfig();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(fullNoteEdit);

    noteWidget->setLayout(layout);
    noteWidget->setWindowTitle(tr("Notes"));

    connect(clonedNoteDoc, SIGNAL(contentsChanged()), hub, SLOT(addToSaveQueue()), Qt::UniqueConnection);
    connect(fullNoteEdit, SIGNAL(cursorPositionChangedSignal(int)), this, SLOT(noteCursorPositionChangedSlot(int)), Qt::UniqueConnection);

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

    currentCursorPos = tCursor.position();

}


//-------------------------------------------------------------------------------
void FullscreenEditor::changeTextStyleSlot(int styleIndex)
{
    //    qDebug() << "changeTextStyleSlot";

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

void FullscreenEditor::noteCursorPositionChangedSlot(int position)
{
    currentNoteCursoPos = position;
}

void FullscreenEditor::synCursorPositionChangedSlot(int position)
{
    currentSynCursoPos = position;

}


//-------------------------------------------------------------------

void FullscreenEditor::restoreDoc()
{

    settings.setValue( "FullTextArea/areaWidth", sliderCurrentValue);
    settings.setValue( "FullTextArea/zoom", textStyles->zoomModifier());

    QTextDocument *doc = ui->fullTextEdit->document()->clone(this);
    if(textStyles->zoomModifier() != 0){
        textStyles->changeDocStyles(doc, "removeZoom");
    }
    originalDoc->setHtml(doc->toHtml());

    originalSynDoc->setHtml(clonedSynDoc->toHtml());
    originalNoteDoc->setHtml(clonedNoteDoc->toHtml());



}

//-------------------------------------------------------------------

void FullscreenEditor::wordCountChangedSlot(QString type, int id, int count)
{
    int delta = count - baseWordCount;

    ui->progressBar->changeProgressBy(delta);

    baseWordCount = count;

}


//---------------------------------------------------------------------------
//----------Style sheets---------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------


void FullscreenEditor::setBaseStyleSheet()
{
    //    baseStyleSheet = "FullTextZone {
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
    QColor backgroundColor = settings.value("FullTextArea/backgroundColor", QVariant(QColor(Qt::black))).toString();

    QString string;
    QString r = string.setNum(backgroundColor.red(),10);
    QString g = string.setNum(backgroundColor.green(), 10);
    QString b = string.setNum(backgroundColor.blue(),10);

    backColorString = "QWidget#mainBackground {background-color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QToolButton#treeButton {background-color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QPushButton#backColorButton {background-color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QTreeView#treeView {background-color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QToolButton#prevButton {background-color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QToolButton#nextButton {background-color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QToolButton#newButton {background-color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QLabel#timerLabel {background-color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QLabel#timerBuddyLabel {background-color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QLabel#clockLabel {background-color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QLabel#clockBuddyLabel {background-color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QLabel#wordCountLabel {background-color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QLabel#wordCountBuddyLabel {background-color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QLabel#wordGoalLabel {background-color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QWidget#controlsPanelWidget {background-color: rgb(" + r + ", " + g + ", " + b + ");}";



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
    QColor textBackgroundColor = settings.value("FullTextArea/textBackgroundColor", QVariant(QColor(Qt::black))).toString();

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
    QColor textColor = settings.value("FullTextArea/textColor", QVariant(QColor(Qt::lightGray))).toString();

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
    QColor addOnColor = settings.value("FullTextArea/addOnColor", QVariant(QColor(Qt::lightGray))).toString();

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
            "QLabel#wordGoalLabel {color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QToolButton#treeButton {color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QPushButton#addOnColorButton {background-color: rgb(" + r + ", " + g + ", " + b + ");}"
            "QTreeView::item#treeView {color: rgb(" + r + ", " + g + ", " + b + ");}";

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




    setTreeViewVisible(settings.value("FullTextArea/treeOpened", "false").toBool());

    showScrollbarAct->setChecked(settings.value("FullTextArea/showScrollbar", false).toBool());

    if(showScrollbarAct->isChecked())
        ui->fullTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    else
        ui->fullTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}


void FullscreenEditor::saveConfig()
{
    settings.setValue("FullTextArea/treeOpened",ui->treeView->isVisible());
    settings.setValue("FullTextArea/showScrollbar", showScrollbarAct->isChecked());


    if(showScrollbarAct->isChecked())
        ui->fullTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    else
        ui->fullTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}




//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------






void FullscreenEditor::openBySheetNumber(int number)
{
    if(number == 0)
        return;

    clonedDoc->disconnectWordCount();
    disconnect(clonedDoc, SIGNAL(wordCountChanged(QString,int,int)), this, SLOT(setWordCount(QString,int,int)));
    disconnect(clonedDoc, SIGNAL(contentsChanged()), hub, SLOT(addToSaveQueue()));
    disconnect(ui->fullTextEdit, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChangedSlot()));
    disconnect(fullNoteEdit, SIGNAL(cursorPositionChangedSignal(int)), this, SLOT(noteCursorPositionChangedSlot(int)));
    disconnect(fullSynEdit, SIGNAL(cursorPositionChangedSignal(int)), this, SLOT(synCursorPositionChangedSlot(int)));
    disconnect(clonedDoc, SIGNAL(wordCountChanged(QString,int,int)), this, SLOT(wordCountChangedSlot(QString,int,int)));

    disconnect(clonedNoteDoc, SIGNAL(contentsChanged()), hub, SLOT(addToSaveQueue()));

    disconnect(clonedSynDoc, SIGNAL(contentsChanged()), hub, SLOT(addToSaveQueue()));

    if(firstLaunch == false)
        restoreDoc();
    firstLaunch = false;





    QList<MainTextDocument *> sheetsList = hub->project()->mainTree_numForDocHash().keys(number);

    if(!sheetsList.isEmpty())
        if(number != sheetsList.first()->idNumber())
            return;
    if(sheetsList.isEmpty())
        return;
    while(!sheetsList.isEmpty()){
        MainTextDocument *doc = sheetsList.takeFirst();
        if(doc->docType() == "text"){
            this->setText(doc);
            this->setTextCursorPos(doc->cursorPos());
            //            qDebug() << "doc CursorPos : "<< QString::number(doc->cursorPos());
        }
        else if(doc->docType() == "synopsis"){
            this->setSyn(doc);
            this->setSynCursorPos(doc->cursorPos());
        }
        else if(doc->docType() == "note"){
            this->setNote(doc);
            this->setNoteCursorPos(doc->cursorPos());
        }
        else
            return;
    }


    numberOfCurrentFullscreenSheet = number;
    ui->treeButton->setText(hub->project()->mainTree_domElementForNumberHash().value(number).attribute("name", tr("Project")));
    emit currentOpenedSheetSignal(number);


    ui->fullTextEdit->setFocus();

}



void FullscreenEditor::setTextCursorPos(int pos)
{
    ui->fullTextEdit->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
    //set cursor position :
    for(int i = 0; i < pos ; i++)
        ui->fullTextEdit->moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);

    ui->fullTextEdit->ensureCursorVisible();
    //    qDebug() << "setTextCursorPos : "<< QString::number(pos);
}

void FullscreenEditor::setSynCursorPos(int pos)
{
    fullSynEdit->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
    //set cursor position :
    for(int i = 0; i < pos ; i++)
        fullSynEdit->moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);

    fullSynEdit->ensureCursorVisible();

}

void FullscreenEditor::setNoteCursorPos(int pos)
{
    fullNoteEdit->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
    //set cursor position :
    for(int i = 0; i < pos ; i++)
        fullNoteEdit->moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);

    fullNoteEdit->ensureCursorVisible();
}












void FullscreenEditor::on_newButton_clicked()
{
    emit newSheetSignal(numberOfCurrentFullscreenSheet);
    QApplication::processEvents();
    on_nextButton_clicked();
}


void FullscreenEditor::on_treeButton_toggled(bool checked)
{
    this->setTreeViewVisible(checked);
}

void FullscreenEditor::setTreeViewVisible(bool isVisible)
{

    ui->treeView->setVisible(isVisible);
    ui->treeButton->setChecked(isVisible);

}

void FullscreenEditor::createTreeView()
{
    ui->treeView->setHub(hub);
    ui->treeView->setMainTreeAbstractModel(absTreeModel);
    ui->treeView->postConstructor();


    DockedTreeProxy *proxy = new DockedTreeProxy;
    proxy->setHub(hub);
    proxy->setSourceModel(absTreeModel);
    proxy->postConstructor();
    proxy->setSourceModel(absTreeModel);
    ui->treeView->setModel(proxy);

    ui->treeView->setModel(proxy);
    connect(absTreeModel, SIGNAL(applySettingsFromDataSignal()), ui->treeView, SLOT(applySettingsFromData()), Qt::UniqueConnection);
    connect(ui->treeView, SIGNAL(modifyFlagsForDropsSignal(QString)), proxy, SLOT(modifyFlagsForDrops(QString)), Qt::UniqueConnection);
    connect(proxy, SIGNAL(resetAbsModelSignal()), absTreeModel, SLOT(resetAbsModel()), Qt::UniqueConnection);
    ui->treeView->applySettingsFromData();


    connect(ui->treeView, SIGNAL(textAndNoteSignal(int,QString)), this, SLOT(openBySheetNumber(int)), Qt::UniqueConnection);


}
void FullscreenEditor::on_prevButton_clicked()
{
    //find directly before :
    int prevNum = hub->project()->mainTree_domElementForNumberHash().key(hub->project()->mainTree_domElementForNumberHash().value(numberOfCurrentFullscreenSheet).previousSiblingElement(hub->project()->mainTree_domElementForNumberHash().value(numberOfCurrentFullscreenSheet).tagName()));


    //find before the father :
    if(prevNum == 0){
        QDomElement father = hub->project()->mainTree_domElementForNumberHash().value(numberOfCurrentFullscreenSheet).parentNode().toElement();
        QDomElement prevFather = father.previousSiblingElement(father.tagName());
        if(prevFather.isNull())
            return;
        QDomElement lastChild = prevFather.lastChild().toElement();
        //cancel :
        if(lastChild.isNull())
            return;
        prevNum = hub->project()->mainTree_domElementForNumberHash().key(lastChild);

    }

    this->openBySheetNumber(prevNum);
}

void FullscreenEditor::on_nextButton_clicked()
{
    //find directly after :

    int nextNum = hub->project()->mainTree_domElementForNumberHash().key(hub->project()->mainTree_domElementForNumberHash().value(numberOfCurrentFullscreenSheet).nextSiblingElement(hub->project()->mainTree_domElementForNumberHash().value(numberOfCurrentFullscreenSheet).tagName()));


    //find after the father :
    if(nextNum == 0){
        QDomElement father = hub->project()->mainTree_domElementForNumberHash().value(numberOfCurrentFullscreenSheet).parentNode().toElement();
        QDomElement nextFather = father.nextSiblingElement(father.tagName());
        if(nextFather.isNull())
            return;
        QDomElement firstChild = nextFather.firstChild().toElement();
        //cancel :
        if(firstChild.isNull())
            return;
        nextNum = hub->project()->mainTree_domElementForNumberHash().key(firstChild);
    }

    this->openBySheetNumber(nextNum);

}


