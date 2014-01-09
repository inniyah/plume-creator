#include "workbench.h"

Workbench::Workbench(QWidget *parent) :
    QWidget(parent), currentModeWidget(0)
{

    setMinimumSize(500, 300);
    setWindowTitle(tr("Workbench"));
    setFocusPolicy(Qt::WheelFocus);
    QApplication::setActiveWindow(this);
    raise();






    QHBoxLayout *mainLayout = new QHBoxLayout();


    QWidget *mainZone = new QWidget;
    zoneLayout = new QVBoxLayout;
    mainZone->setLayout(zoneLayout);



    commonToolBar = new QToolBar();
    commonToolBar->setOrientation(Qt::Vertical);


    this->resetCommonToolBar();

    mainLayout->addWidget(commonToolBar);
    mainLayout->addWidget(mainZone);

    mainLayout->setMargin(1);
    mainLayout->setSpacing(1);
    mainLayout->setContentsMargins(1,1,1,1);

    setLayout(mainLayout);


    show();



    applyConfig();

}

void Workbench::postConstructor()
{

}

Workbench::~Workbench()
{
    this->quitCurrentMode();
    saveConfig();
}
//------------------------------------------------------------------------------------

void Workbench::saveConfig()
{
    QSettings settings;
    settings.beginGroup( "Workbench");
    settings.setValue( "size",   size() );
    settings.setValue( "pos",   pos() );
    settings.setValue( "lastOpened", lastOpened);
//    QString lastOpened = settings.value( "lastOpened", "spreadsheet").toString();
//    if(lastOpened == "spreadsheet"){
//        outlinerBase->saveConfig();
//    }

    settings.endGroup();

}

//------------------------------------------------------------------------------------


void Workbench::applyConfig()
{
    QSettings settings;
    settings.beginGroup( "Outline" );
    resize(settings.value( "size", QSize( 1000, 750 ) ).toSize() );
    move(settings.value( "pos" ).toPoint() );
    QString lastOpened = settings.value( "lastOpened", "spreadsheet").toString();
    if(lastOpened == "spreadsheet")
        QTimer::singleShot(0, this, SLOT(shiftToSpreadsheet()));
    if(lastOpened == "infoSheet")
        QTimer::singleShot(0, this, SLOT(shiftToInfoSheet()));

    settings.endGroup();

}

void Workbench::resetCommonToolBar()
{
    commonToolBar->clear();

    shiftToSpreadsheetAct = new QAction(QIcon(":/pics/table.png"),"", this);
    shiftToSpreadsheetAct->setToolTip(tr("Shift to spreadsheet mode"));
    shiftToSpreadsheetAct->setCheckable(true);
    connect(shiftToSpreadsheetAct, SIGNAL(toggled(bool)), this, SLOT(shiftToSpreadsheetSingleShot()));

//    shiftToInfoSheetAct = new QAction(QIcon(":/pics/table.png"),"", this);
//    shiftToInfoSheetAct->setToolTip(tr("Shift to information mode"));
//    shiftToInfoSheetAct->setCheckable(true);
//    connect(shiftToInfoSheetAct, SIGNAL(toggled(bool)), this, SLOT(shiftToInfoSheetSingleShot()));

    QAction *quitAct = new QAction(QIcon(":/pics/window-close-32x32.png"),"", this);
    quitAct->setToolTip(tr("Close the Outliner"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));


    commonToolBar->addAction(shiftToSpreadsheetAct);
//    commonToolBar->addAction(shiftToInfoSheetAct);
    commonToolBar->addSeparator();
    commonToolBar->addAction(quitAct);
    commonToolBar->addSeparator();

}

//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------

void Workbench::quitCurrentMode()
{
    if(currentModeWidget)
        currentModeWidget->close();

    QApplication::processEvents();
}

//------------------------------------------------------------------------------------

void Workbench::shiftToSpreadsheet()
{
    this->quitCurrentMode();
    this->resetCommonToolBar();

    outlinerBase = new OutlinerBase(this);
    outlinerBase->setHub(hub);
    outlinerBase->setMainTreeAbstractModel(absTree);

    commonToolBar->addActions(outlinerBase->toolButtons());
    shiftToSpreadsheetAct->setDisabled(true);

    zoneLayout->addWidget(outlinerBase->spreadsheetWidget());

    connect(outlinerBase, SIGNAL(applySynNoteFontConfigSignal()), this, SIGNAL(applySynNoteFontConfigSignal()), Qt::UniqueConnection);
    connect(this, SIGNAL(updateOutlinerSignal()), outlinerBase, SLOT(updateOutliner()), Qt::UniqueConnection);

    outlinerBase->postConstructor();

    lastOpened = "spreadsheet";
    currentModeWidget = outlinerBase->spreadsheetWidget();
}
//------------------------------------------------------------------------------------
void Workbench::shiftToSpreadsheetSingleShot()
{
    QTimer::singleShot(0, this, SLOT(shiftToSpreadsheet()));
}
//------------------------------------------------------------------------------------
void Workbench::shiftToInfoSheet()
{
    this->quitCurrentMode();
    this->resetCommonToolBar();

    infoSheetBase = new InfoSheetBase(this);
    infoSheetBase->setHub(hub);

    commonToolBar->addActions(infoSheetBase->toolButtons());
    shiftToInfoSheetAct->setDisabled(true);

    zoneLayout->addWidget(infoSheetBase);

    infoSheetBase->postConstructor();

    lastOpened = "infoSheet";
    currentModeWidget = infoSheetBase;
}
//------------------------------------------------------------------------------------
void Workbench::shiftToInfoSheetSingleShot()
{
    QTimer::singleShot(0, this, SLOT(shiftToInfoSheet()));
}
//------------------------------------------------------------------------------------
