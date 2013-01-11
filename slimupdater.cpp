#include "slimupdater.h"
#include "ui_slimupdater.h"

SlimUpdater::SlimUpdater(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SlimUpdater)
{
    ui->setupUi(this);


    ui->packageComboBox->hide();
#ifdef Q_OS_LINUX
    ui->packageComboBox->show();
#endif

    connect(ui->closeButton, SIGNAL(clicked()), this, SIGNAL(closeUpdaterSignal()));


    ui->updateLabel->setText("test");
}

SlimUpdater::~SlimUpdater()
{
    delete ui;
}

