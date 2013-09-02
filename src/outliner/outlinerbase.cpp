#include "outliner/outlinerbase.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   


OutlinerBase::OutlinerBase(QWidget *parent) :
    QWidget(parent), spreadsheetMode(false), isResetOn(false)
{

    setWindowTitle(tr("Workbench - Outliner"));
    setFocusPolicy(Qt::WheelFocus);



    spreadsheet = new OutlinerSpreadsheet();













    //    QMessageBox msgBox;
    //    msgBox.setIcon(QMessageBox::Warning);
    //    msgBox.setText(tr("The Ouliner is in heavy maintenance !<br>For now, it's only an empty GUI. Please wait for a future release !"));
    //    msgBox.exec();


}
//------------------------------------------------------------------------------------
void OutlinerBase::postConstructor()
{

    shiftToSpreadsheet();

    applySpreadsheetConfig();

}

//------------------------------------------------------------------------------------

void OutlinerBase::saveConfig()
{
    QSettings settings;
    settings.beginGroup( "Outline");

        //       settings.setValue( "spreadsheetState", spreadsheet->horizontalHeader()->saveState());
        settings.setValue( "spreadsheetState", spreadsheet->header()->saveState());
        settings.setValue( "lastOpened", "spreadsheet" );
        //       settings.setValue( "spreadsheetVertSize", spreadsheet->verticalHeader()->sectionSize(0));


    settings.endGroup();

}

//------------------------------------------------------------------------------------


QList<QAction *> OutlinerBase::toolButtons()
{
    expandSpreadsheetAct = new QAction(QIcon(":/pics/zoom-out.png"),"", this);
    expandSpreadsheetAct->setToolTip(tr("Expand spreadsheet"));

    shrinkSpreadsheetAct = new QAction(QIcon(":/pics/zoom-in.png"),"", this);
    shrinkSpreadsheetAct->setToolTip(tr("Shrink spreadsheet"));

    moveUpAct = new QAction(QIcon(":/pics/go-up-32x32.png"),"", this);
    moveUpAct->setToolTip(tr("Move up"));

    moveDownAct = new QAction(QIcon(":/pics/go-down-32x32.png"),"", this);
    moveDownAct->setToolTip(tr("Move down"));

    resetAct = new QAction(QIcon(":/pics/view-refresh-32x32.png"),"", this);
    resetAct->setToolTip(tr("Reset the view"));
    connect(resetAct, SIGNAL(triggered()), this, SLOT(resetSpreadsheetState()));

    QAction *separator = new QAction("sep", this);
    separator->setSeparator(true);

    QList<QAction *> actionList;
    actionList << separator
                                << expandSpreadsheetAct
                                << shrinkSpreadsheetAct;
//                                << moveUpAct
//                                << moveDownAct
//                                << resetAct;

    return actionList;


}

//------------------------------------------------------------------------------------
void OutlinerBase::applySpreadsheetConfig()
{
    QSettings settings;
    settings.beginGroup( "Outline" );
    //    spreadsheet->horizontalHeader()->restoreState(settings.value( "spreadsheetState",0 ).toByteArray());
    spreadsheet->header()->restoreState(settings.value( "spreadsheetState",0 ).toByteArray());
    //    spreadsheet->verticalHeader()->setDefaultSectionSize(settings.value( "spreadsheetVertSize",40 ).toInt());



    settings.endGroup();



}


//------------------------------------------------------------------------------------


QWidget *OutlinerBase::spreadsheetWidget()
{
    return spreadsheet;
}


//------------------------------------------------------------------------------------



void OutlinerBase::shiftToSpreadsheet()
{

    spreadsheetMode = true;
    lastOpened = "spreadsheet";

    spreadsheet->setHub(hub);
    spreadsheet->setMainTreeAbstractModel(absTreeModel);
spreadsheet->postConstructor();






    proxy = new OutlinerSpreadheetProxy;
    proxy->setHub(hub);
    proxy->setSourceModel(absTreeModel);
    connect(absTreeModel, SIGNAL(applySettingsFromDataSignal()), spreadsheet, SLOT(applySettingsFromData()), Qt::UniqueConnection);
    connect(spreadsheet, SIGNAL(modifyFlagsForDropsSignal(QString)), proxy, SLOT(modifyFlagsForDrops(QString)), Qt::UniqueConnection);
    connect(proxy, SIGNAL(resetAbsModelSignal()), absTreeModel, SLOT(resetAbsModel()), Qt::UniqueConnection);


    spreadsheet->setModel(proxy );

    spreadsheet->setWordWrap(true);

    OutlinerItemNoteDelegate *noteDelegate = new OutlinerItemNoteDelegate;
    noteDelegate->setHub(hub);
    //spreadsheet->setItemDelegateForColumn(0, delegate); //tree
    spreadsheet->setItemDelegateForColumn(1, noteDelegate); //syn
    spreadsheet->setItemDelegateForColumn(2, noteDelegate); //notes

    OutlinerItemPoVDelegate *povDelegate = new OutlinerItemPoVDelegate;
    povDelegate->setHub(hub);
    spreadsheet->setItemDelegateForColumn(3, povDelegate); // Point of View

    OutlinerItemStatusDelegate *statusDelegate = new OutlinerItemStatusDelegate;
    spreadsheet->setItemDelegateForColumn(4, statusDelegate); //status
    connect(statusDelegate, SIGNAL(finishStatusEdit()), spreadsheet, SLOT(finishStatusEdit()), Qt::UniqueConnection);








//    connect(spreadsheet, SIGNAL(columnOneResizedSignal(int)), proxy, SLOT(columnOneResizedSlot(int)), Qt::UniqueConnection);
//    connect(spreadsheet, SIGNAL(columnTwoResizedSignal(int)), proxy, SLOT(columnTwoResizedSlot(int)), Qt::UniqueConnection);
//    //    connect(spreadsheet, SIGNAL(columnOneResizedSignal(int)), absModel, SLOT(resetAbsModelColumnOne()));
    //    connect(spreadsheet, SIGNAL(columnTwoResizedSignal(int)), absModel, SLOT(resetAbsModelColumnTwo()));


    QTimer::singleShot(0, this, SLOT(applySpreadsheetConfig()));
    spreadsheet->reset();


    connect(absTreeModel, SIGNAL(applySettingsFromDataSignal()), spreadsheet, SLOT(applySettingsFromData()), Qt::UniqueConnection);
    this->updateOutliner();


    connect(hub, SIGNAL(resetSpreadsheetOutlinerSignal()), this, SLOT(updateOutliner()), Qt::UniqueConnection);
    connect(shrinkSpreadsheetAct, SIGNAL(triggered()), proxy, SLOT(shrinkRow()), Qt::UniqueConnection);
    connect(shrinkSpreadsheetAct, SIGNAL(triggered()), spreadsheet, SLOT(applySettingsFromData()), Qt::UniqueConnection);
    connect(expandSpreadsheetAct, SIGNAL(triggered()), proxy, SLOT(expandRow()), Qt::UniqueConnection);
    connect(expandSpreadsheetAct, SIGNAL(triggered()), spreadsheet, SLOT(applySettingsFromData()), Qt::UniqueConnection);
    connect(absTreeModel, SIGNAL(applySynNoteFontConfigSignal()), this, SIGNAL(applySynNoteFontConfigSignal()), Qt::UniqueConnection);


    QSettings settings;
    settings.beginGroup( "Outline" );

    if(settings.value( "forceReset_2", true ).toBool() == true){
        settings.setValue("forceReset_2", false);
        resetSpreadsheetState();
    }
settings.endGroup();
}

//------------------------------------------------------------------------------------

void OutlinerBase::updateOutliner()
{


    absTreeModel->mtoO_setNumForDoc(hub->mainTree_numForDocHash());

                absTreeModel->resetAbsModel();



}

//------------------------------------------------------------------------------------

void OutlinerBase::resetSpreadsheetState()
{
    QSettings settings;
settings.beginGroup( "Outline" );
settings.setValue( "spreadsheetState", 0);
settings.endGroup();
spreadsheet->parentWidget()->layout()->removeWidget(spreadsheet);
QHBoxLayout *box = new QHBoxLayout;
box->addWidget(spreadsheet);
delete box;

this->shiftToSpreadsheet();

}

//------------------------------------------------------------------------------------


void OutlinerBase::moveViewTo(int hBarValue, int vBarValue)
{
    spreadsheet->verticalScrollBar()->setValue(vBarValue);
    spreadsheet->horizontalScrollBar()->setValue(hBarValue);
}











//------------------------------------------------------------------------------------
//------- note / synopsis textDocuments :-----------------------------------------------------------------
//------------------------------------------------------------------------------------
// mtoO means Main to Outliner

void OutlinerBase::mtoO_setNumForDoc(QHash<MainTextDocument *, int> numForDoc)
{
    mtoO_numForDoc = numForDoc;
}






























