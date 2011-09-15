#include <QtGui>

#include "outline.h"
#include "outlineitem.h"

Outline::Outline(QWidget *parent) :
    QWidget(parent)
{

    QVBoxLayout *layout = new QVBoxLayout();

    setMinimumSize(600, 600);
    setWindowTitle(tr("Outliner"));
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);


    area = new QScrollArea;
    areaWidget = new QWidget(area);
    areaLayout = new QVBoxLayout;

    areaWidget->setMinimumSize(600,600);
    areaWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding );


    areaLayout->setSpacing(30);

    //   addItems();


    areaWidget->setLayout(areaLayout);
    area->setWidget(areaWidget);









    //setWindowState(Qt::WindowMaximized);


    showListsAct = new QAction(tr("Show all lists"), this);
    showListsAct->setCheckable(true);
    //   zoomInAct->setShortcuts(QKeySequence::Undo);
    //   zoomInAct->setStatusTip(tr("Undo the last operation"));
    connect(showListsAct, SIGNAL(toggled(bool)), this, SIGNAL(showListsSignal(bool)));

    showNotesAct = new QAction(tr("Show all notes"), this);
    showNotesAct->setCheckable(true);
    connect(showNotesAct, SIGNAL(toggled(bool)), this, SIGNAL(showNotesSignal(bool)));

    expandAllTextsAct = new QAction(tr("Expand all texts"), this);
    expandAllTextsAct->setCheckable(true);
    connect(expandAllTextsAct, SIGNAL(toggled(bool)), this, SIGNAL(expandAllTextsSignal(bool)));


    // sizes menu :

    QWidgetAction *listSliderAct = new QWidgetAction(this);
    QGroupBox *listSliderBox = new QGroupBox(tr("Attendances width"), this);
    QVBoxLayout *listSliderLayout = new QVBoxLayout(listSliderBox);
    listSlider = new QSlider(Qt::Horizontal, this);
    listSlider->setRange(200, 600);
    listSliderLayout->addWidget(listSlider);
    listSliderBox->setLayout(listSliderLayout);
    listSliderAct->setDefaultWidget(listSliderBox);

    QWidgetAction *synSliderAct = new QWidgetAction(this);
    QGroupBox *synSliderBox = new QGroupBox(tr("Synopsies width"), this);
    QVBoxLayout *synSliderLayout = new QVBoxLayout(synSliderBox);
    synSlider = new QSlider(Qt::Horizontal, this);
    synSlider->setRange(200, 600);
    synSliderLayout->addWidget(synSlider);
    listSliderBox->setLayout(synSliderLayout);
    synSliderAct->setDefaultWidget(synSliderBox);

    QWidgetAction *noteSliderAct = new QWidgetAction(this);
    QGroupBox *noteSliderBox = new QGroupBox(tr("Notes width"), this);
    QVBoxLayout *noteSliderLayout = new QVBoxLayout(noteSliderBox);
    noteSlider = new QSlider(Qt::Horizontal, this);
    noteSlider->setRange(200, 600);
    noteSliderLayout->addWidget(noteSlider);
    noteSliderBox->setLayout(noteSliderLayout);
    noteSliderAct->setDefaultWidget(noteSliderBox);

    QMenu *sizeMenu = new QMenu(this);
    sizeMenu->addAction(listSliderAct);
    sizeMenu->addAction(synSliderAct);
    sizeMenu->addAction(noteSliderAct);


    QToolButton *sizeMenuButton = new QToolButton(this);
    sizeMenuButton->setText(tr("Size"));
    sizeMenuButton->setPopupMode(QToolButton::InstantPopup);
    sizeMenuButton->setMenu(sizeMenu);



    // fonts menu :

    QWidgetAction *synFontAct = new QWidgetAction(this);
    QGroupBox *synFontBox = new QGroupBox(tr("Synopsies font"), this);
    QGridLayout *synFontLayout = new QGridLayout(synFontBox);
    synFontCombo = new QFontComboBox(this);
    synTextHeightSpin = new QSpinBox;
    synTextHeightSpin->setRange(6,30);
    synTextHeightSpin->setValue(12);
    synFontLayout->addWidget(synFontCombo, 0,0);
    synFontLayout->addWidget(synTextHeightSpin, 0,1);
    synFontBox->setLayout(synFontLayout);
    synFontAct->setDefaultWidget(synFontBox);

    QWidgetAction *noteFontAct = new QWidgetAction(this);
    QGroupBox *noteFontBox = new QGroupBox(tr("Notes font"), this);
    QGridLayout *noteFontLayout = new QGridLayout(noteFontBox);
    noteFontCombo = new QFontComboBox(this);
    noteTextHeightSpin = new QSpinBox;
    noteTextHeightSpin->setRange(6,30);
    noteTextHeightSpin->setValue(12);
    noteFontLayout->addWidget(noteFontCombo, 0,0);
    noteFontLayout->addWidget(noteTextHeightSpin, 0,1);
    noteFontBox->setLayout(noteFontLayout);
    noteFontAct->setDefaultWidget(noteFontBox);

    QMenu *fontMenu = new QMenu(this);
    fontMenu->addAction(synFontAct);
    fontMenu->addAction(noteFontAct);


    QToolButton *fontsMenuButton = new QToolButton(this);
    fontsMenuButton->setText(tr("Fonts"));
    fontsMenuButton->setPopupMode(QToolButton::InstantPopup);
    fontsMenuButton->setMenu(fontMenu);


    // toolbar :

    QToolBar *toolBar = new QToolBar("Outliner Tools",this);
    toolBar->addAction(showListsAct);
    toolBar->addAction(showNotesAct);
    toolBar->addAction(expandAllTextsAct);

    toolBar->addWidget(sizeMenuButton);
    toolBar->addWidget(fontsMenuButton);

    layout->addWidget(toolBar);
    layout->addWidget(area);
    setLayout(layout);

    show();


    qDebug() << "outline launched";
}



//------------------------------------------------------------------------------


//------------------------------------------------------------------------------


void Outline::buildItem(QTextDocument *synDocument, QTextDocument *noteDocument, QString title, int number, QString tagName)
{

    OutlineItem *item = new OutlineItem(areaWidget);
    QString string;
    item->setObjectName("outlineItem_" + string.setNum(number));
    item->setDocuments(synDocument, noteDocument);
    item->setTitle(title);

    connect(this,SIGNAL(showListsSignal(bool)), item, SLOT(showListSlot(bool)));
    connect(this,SIGNAL(showNotesSignal(bool)), item, SLOT(showNoteSlot(bool)));
    connect(this,SIGNAL(expandAllTextsSignal(bool)), item, SLOT(expandTexts(bool)));
    connect(item,SIGNAL(resizingSignal()), this, SLOT(resizingSlot()));
    connect(this,SIGNAL(updateSizeSignal()), item, SLOT(updateSizeSlot()));
    connect(listSlider,SIGNAL(valueChanged(int)), item, SLOT(changeListWidth(int)));
    connect(synSlider,SIGNAL(valueChanged(int)), item, SLOT(changeSynWidth(int)));
    connect(noteSlider,SIGNAL(valueChanged(int)), item, SLOT(changeNoteWidth(int)));
    connect(synFontCombo, SIGNAL(currentFontChanged(QFont)), item, SLOT(changeSynFont(QFont)));
    connect(synTextHeightSpin, SIGNAL(valueChanged(int)), item, SLOT(changeSynTextHeight(int)));
    connect(noteFontCombo, SIGNAL(currentFontChanged(QFont)), item, SLOT(changeNoteFont(QFont)));
    connect(noteTextHeightSpin, SIGNAL(valueChanged(int)), item, SLOT(changeNoteTextHeight(int)));
    connect(this, SIGNAL(connectUpdateTextsSignal()), item, SLOT(connectUpdateTextsSlot()));
    connect(this, SIGNAL(disconnectUpdateTextsSignal()), item, SLOT(disconnectUpdateTextsSlot()));

    connect(item,SIGNAL(newOutlineTitleSignal(QString,int)), this, SIGNAL(newOutlineTitleSignal(QString,int)));
    connect(item, SIGNAL(writeThisSignal(int)), this, SIGNAL(writeThisSignal(int)));

    if(tagName == "scene")
        areaLayout->addWidget(item,0,Qt::AlignRight);

    if(tagName == "chapter")
        areaLayout->addWidget(item,0,Qt::AlignCenter);

    if(tagName == "book")
        areaLayout->addWidget(item,0,Qt::AlignLeft);


    //   areaLayout->addStretch();

    //emit updateSizeSignal();


}

//------------------------------------------------------------------------------
void Outline::buildSeparator()
{

    QFrame *separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Raised);
    separator->setLineWidth(4);
    separator->setFixedWidth(400);
    areaLayout->addWidget(separator,0,Qt::AlignRight);

    //            emit updateSizeSignal();

}
void Outline::buildStretcher()
{
    QFrame *stretcher = new QFrame();
    stretcher->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    areaLayout->addWidget(stretcher);



    emit updateSizeSignal();
    resizingSlot();
    area->update();

    applyConfig();


}
//------------------------------------------------------------------------------


void Outline::setItemTitle(QString newTitle, int number)
{
    QString string;
    OutlineItem *item = areaWidget->findChild<OutlineItem *>("outlineItem_" + string.setNum(number));

    //    qDebug() << "setItemTitle on : " << "outlineItem_" << string.setNum(number);

    item->setTitle(newTitle);
}

//------------------------------------------------------------------------------


void Outline::cleanArea()
{
    QWidget *erasingWidget = new QWidget;
    QVBoxLayout *erasingLayout = new QVBoxLayout;

    QList<OutlineItem *> allOutlineItems = areaWidget->findChildren<OutlineItem *>();



    while(!allOutlineItems.isEmpty()){
        qDebug() << "cleanArea ";

        OutlineItem *item = allOutlineItems.takeFirst();
        item->setObjectName("");
        //        allOutlineItems.first()->setParent(erasingLayout);
        erasingLayout->addWidget(item);
        item->setParent(erasingWidget);
        item->deleteLater();
    }



    QList<QFrame *> allOutlineSeparators = areaWidget->findChildren<QFrame *>();


    while(!allOutlineSeparators.isEmpty()){
        //       qDebug() << "cleanArea : " << areaLayout->children().first()->objectName();

        QFrame *item = allOutlineSeparators.takeFirst();
        item->setObjectName("");
        //        allOutlineItems.first()->setParent(erasingLayout);
        erasingLayout->addWidget(item);
        item->setParent(erasingWidget);
        item->deleteLater();
    }



    erasingWidget->setLayout(erasingLayout);
    erasingWidget->deleteLater();




    //    while(!areaLayout->children().isEmpty()){
    //        qDebug() << "cleanArea : " << areaLayout->children().first()->objectName();

    //        areaLayout->children().first()->setObjectName("");
    //        areaLayout->children().first()->setParent(erasingLayout);
    ////        if(areaLayout->children().first()->isWidgetType()){
    ////            QWidget *widget = areaLayout->children().first();
    ////            erasingLayout->addWidget(widget);
    ////        }
    //    }



    //    areaLayout = new QVBoxLayout;
    //    areaWidget->setLayout(areaLayout);
}

//------------------------------------------------------------------------------
void Outline::resizingSlot()
{
    //    QWidget *widget = areaWidget->findChild<QWidget *>(objectName);
    areaWidget->setMinimumSize(area->viewport()->width(), area->viewport()->height());
    areaWidget->adjustSize();

}


//------------------------------------------------------------------------------

void Outline::resizeEvent(QResizeEvent *event)
{
    //   if(area->viewport()->width() > 600 && area->viewport()->height() > 600)
    //        areaWidget->setMinimumSize(area->viewport()->width(), area->viewport()->height());
    //areaWidget->setMinimumSize(area->viewport()->width(), area->viewport()->height());
    //    QString debug;
    //    qDebug() << "w : " << debug.setNum(areaWidget->size().width());
    //    qDebug() << "h : " << debug.setNum(areaWidget->size().height());
}







//------------------------------------------------------------------------------
//------------Apply Config------------------------------------------------------
//------------------------------------------------------------------------------

void Outline::applyConfig()
{
    QSettings settings;
    settings.beginGroup( "Outline" );
    resize(settings.value( "size", QSize( 1000, 750 ) ).toSize() );
    move(settings.value( "pos" ).toPoint() );
    bool showListsBool = settings.value("showLists", false).toBool();
    bool showNotesBool = settings.value("showNotes", false).toBool();
    bool expandAllTextsBool = settings.value("expandAllTexts", false).toBool();
    int listsWidth = settings.value("listsWidth", 250).toInt();
    int synsWidth = settings.value("synsWidth", 250).toInt();
    int notesWidth = settings.value("notesWidth", 250).toInt();
    int synTextHeight = settings.value("synTextHeight", 12).toInt();
    QFont synFont;
    synFont.setFamily(settings.value("synFontFamily", "Liberation Serif").toString());
    int noteTextHeight = settings.value("noteTextHeight", 12).toInt();
    QFont noteFont;
    noteFont.setFamily(settings.value("noteFontFamily", "Liberation Serif").toString());
    settings.endGroup();

    synTextHeightSpin->setValue(synTextHeight);
    synFontCombo->setCurrentFont(synFont);
    noteTextHeightSpin->setValue(noteTextHeight);
    noteFontCombo->setCurrentFont(noteFont);
    listSlider->setValue(listsWidth);
    synSlider->setValue(synsWidth);
    noteSlider->setValue(notesWidth);
    showListsAct->setChecked(showListsBool);
    showNotesAct->setChecked(showNotesBool);
    expandAllTextsAct->setChecked(expandAllTextsBool);

    resizingSlot();

}

//------------------------------------------------------------------------------------

void Outline::saveConfig()
{
    QSettings settings;
    settings.beginGroup( "Outline");
    settings.setValue( "size", size() );
    settings.setValue( "pos", pos() );
    settings.setValue("showLists", showListsAct->isChecked());
    settings.setValue("showNotes", showNotesAct->isChecked());
    settings.setValue("expandAllTexts", expandAllTextsAct->isChecked());
    settings.setValue("listsWidth", listSlider->value());
    settings.setValue("synsWidth", synSlider->value());
    settings.setValue("notesWidth", noteSlider->value());
    settings.setValue("synTextHeight", synTextHeightSpin->value());
    settings.setValue("synFontFamily", synFontCombo->currentFont());
    settings.setValue("noteTextHeight", noteTextHeightSpin->value());
    settings.setValue("noteFontFamily", noteFontCombo->currentFont());
    settings.endGroup();



}

//------------------------------------------------------------------------------------
void Outline::setOutlinerViewportPos(int number)
{
    QString string;
    QWidget *widget = areaWidget->findChild<QWidget *>("outlineItem_" + string.setNum(number));
    area->ensureWidgetVisible(widget);

    qDebug() << "setOutlinerViewportPos : " << widget->objectName();
    QString debug;
    qDebug() << "setOutlinerViewportPos x : " << debug.setNum(widget->mapToParent(QPoint(0,0)).x());
    qDebug() << "setOutlinerViewportPos y : " << debug.setNum(widget->mapToParent(QPoint(0,0)).y());


}
//------------------------------------------------------------------------------------
void Outline::closeEvent(QCloseEvent* event)
{

    saveConfig();

    //    qDebug() << "Outline closeEvent";



    event->accept();
}
