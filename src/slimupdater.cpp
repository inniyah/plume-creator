#ifdef INCLUDE_SLIMUPDATER_PLUGIN

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkSession>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QtGlobal>

#include "slimupdater.h"
#include "ui_slimupdater.h"

SlimUpdater::SlimUpdater(QString mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SlimUpdater)
  , currentAppVersion(QApplication::applicationVersion()), timerNumber(0), oneTime(true)
{
    ui->setupUi(this);


    ui->packageComboBox->hide();
#ifdef Q_OS_LINUX
    ui->packageComboBox->show();
#endif

    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(closeUpdater()));









    //    connect(&manager, SIGNAL(finished(QNetworkReply*)),
    //            SLOT(downloadFinished(QNetworkReply*)));


    updateChecker = new UpdateChecker();
    connect(updateChecker, SIGNAL(replySignal(QString)), this, SLOT(replySlot(QString)));

    connect(ui->packageComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(packageComboBox_currentIndexChanged(int)));


    readSettings();


updateChecker->checkConnection();



}

SlimUpdater::~SlimUpdater()
{






    writeSettings();
   delete updateChecker;
   delete ui;

}

//---------------------------------------------------------------------------

void SlimUpdater::setCurrentVersion(QString currentVersion)
{


    ui->updateLabel->setText(tr("<center>Current version : ") + currentAppVersion + tr("</center>"));
}




//---------------------------------------------------------------------------


void SlimUpdater::replySlot(QString reply)
{


    if(reply == "close"){
    QTimer::singleShot(5000, this, SLOT(closeUpdater()));
return;
}

    ui->updateLabel->setText(reply);
}

//---------------------------------------------------------------------------

void SlimUpdater::closeUpdater()
{
    int animDuration = 500;

    QTimer::singleShot(animDuration, this, SIGNAL(closeUpdaterSignal()));

    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setDuration(animDuration);

    int xManager = this->geometry().x();
    int yManager = this->geometry().y();
    int wManager = this->width();
    int hManager = this->height();


    animation->setStartValue(QRect(xManager,yManager,wManager,hManager));
    animation->setEndValue(QRect(xManager,yManager + height(),wManager,0));
    animation->start();



}



//---------------------------------------------------------------------------
void SlimUpdater::readSettings()
{
    QSettings settings;
    settings.beginGroup( "Updater" );
    ui->checkUpdateAtStartupBox->setChecked(settings.value( "checkAtStartup_2", true).toBool());
    ui->packageComboBox->setCurrentIndex(settings.value("linuxDistrib", 0).toInt());
updateChecker->setPackageType(settings.value("linuxDistrib", 0).toInt());

    settings.endGroup();
}



//---------------------------------------------------------------------------
void SlimUpdater::writeSettings()
{
    QSettings settings;
    settings.beginGroup( "Updater" );
    settings.setValue( "checkAtStartup_2", ui->checkUpdateAtStartupBox->checkState());
    settings.setValue( "linuxDistrib", ui->packageComboBox->currentIndex());
    settings.endGroup();
}


void SlimUpdater::on_verifyButton_clicked()
{

updateChecker->checkConnection();

}

void SlimUpdater::packageComboBox_currentIndexChanged(int index)
{
    updateChecker->setPackageType(index);
}

#endif
