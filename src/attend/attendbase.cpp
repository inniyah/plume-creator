#include "attendbase.h"
#include "ui_attendbase.h"

AttendBase::AttendBase(QWidget *parent) :
    QFrame(parent), isManagerLaunched(false),
    ui(new Ui::AttendBase)
{
    ui->setupUi(this);
    ui->splitter->setCollapsible(0, false);



}

AttendBase::~AttendBase()
{
    delete ui;
}

void AttendBase::startAttendance()
{
    absModel = new AttendAbstractModel();
    absModel->setHub(hub);

    absModel->resetAbsModel();

    AttendGlobalTreeProxyModel *globalProxyModel = new AttendGlobalTreeProxyModel;
    globalProxyModel->setHub(hub);
    globalProxyModel->setSourceModel(absModel);
    ui->globalTreeView->setModel(globalProxyModel );

    AttendSheetTreeProxyModel *sheetProxyModel = new AttendSheetTreeProxyModel;
sheetProxyModel->setHub(hub);
    sheetProxyModel->setSourceModel(absModel);
    ui->sheetTreeView->setModel(sheetProxyModel );
ui->sheetTreeView->expandAll();

    connect(hub, SIGNAL(currentSheetNumberChanged(int)), sheetProxyModel, SLOT(currentSheetModified(int)), Qt::UniqueConnection);
    connect(globalProxyModel, SIGNAL(attendSheetDataNumbersDropped(QList<int>)),sheetProxyModel, SLOT(removeSheetObjects(QList<int>)), Qt::UniqueConnection);


    connect(ui->manageButton, SIGNAL(clicked()), this, SLOT(launchAttendManager()), Qt::UniqueConnection);


}

void AttendBase::launchAttendManager()
{
    if(isManagerLaunched)
        return;

    AttendManager *manager = new AttendManager(this, absModel);
    manager->setHub(hub);
    manager->postConstructor();
    manager->setModal(false);
    manager->show();
    connect(manager, SIGNAL(finished(int)), this, SLOT(setManagerLaunched()));
isManagerLaunched = true;
}
