#include "infosheetbase.h"
#include "ui_infosheetbase.h"

InfoSheetBase::InfoSheetBase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoSheetBase)
{
    ui->setupUi(this);

    setWindowTitle(tr("Workbench - Informations"));
    setFocusPolicy(Qt::WheelFocus);

    this->setAttribute(Qt::WA_DeleteOnClose);
}

//-----------------------------------------------------------------------------------

void InfoSheetBase::postConstructor()
{

    this->applyInfoSheetConfig();

}

//-----------------------------------------------------------------------------------


InfoSheetBase::~InfoSheetBase()
{
    delete ui;
}


//-----------------------------------------------------------------------------------

void InfoSheetBase::applyInfoSheetConfig()
{
    QSettings settings;
    settings.beginGroup( "InfoSheet" );

    settings.endGroup();

}

//-----------------------------------------------------------------------------------

QList<QAction *> InfoSheetBase::toolButtons()
{
    applyChangesAct = new QAction(QIcon(":/pics/dialog-ok-apply-32x32.png"),"", this);
    applyChangesAct->setToolTip(tr("Apply changes"));

    cancelChangesAct = new QAction(QIcon(":/pics/edit-undo-32x32.png"),"", this);
    cancelChangesAct->setToolTip(tr("Apply changes"));

    QAction *separator = new QAction("sep", this);
    separator->setSeparator(true);

    QList<QAction *> actionList;
    actionList << separator
                                << applyChangesAct
                                << cancelChangesAct
;
    return actionList;


}
//------------------------------------------------------------------------------------

void InfoSheetBase::closeEvent(QCloseEvent *event)
{
//    this->saveConfig();

    event->accept();
}
