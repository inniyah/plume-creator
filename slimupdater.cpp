#include "slimupdater.h"
#include "ui_slimupdater.h"

slimUpdater::slimUpdater(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::slimUpdater)
{
    ui->setupUi(this);


    ui->packageComboBox->hide();
#ifdef Q_OS_LINUX
    ui->packageComboBox->show();
#endif



}

slimUpdater::~slimUpdater()
{
    delete ui;
}

