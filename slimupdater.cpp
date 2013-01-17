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
  , currentAppVersion(QApplication::applicationVersion())
{
    ui->setupUi(this);


    ui->packageComboBox->hide();
#ifdef Q_OS_LINUX
    ui->packageComboBox->show();
#endif

    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(closeUpdater()));
    connect(ui->verifyButton, SIGNAL(clicked()), this, SLOT(checkConnection()));


    ui->updateLabel->setText("test");



    readSettings();


    // proxy setup :

    QNetworkProxy proxy;
    if(proxyEnabled){
        if(proxySystemEnabled){
            proxy = QNetworkProxyFactory::systemProxyForQuery().first();
        }
        else{
            proxy.setType(QNetworkProxy::Socks5Proxy);
            proxy.setHostName(proxyHostName);
            proxy.setPort(proxyPort);
            proxy.setUser(proxyUserName);
            proxy.setPassword(proxyPassword);
        }
    }
    else
        proxy.setType(QNetworkProxy::NoProxy);

    manager = new QNetworkAccessManager;
    manager->setProxy(proxy);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    //    connect(&manager, SIGNAL(finished(QNetworkReply*)),
    //            SLOT(downloadFinished(QNetworkReply*)));





    this->setMode(mode);





}

SlimUpdater::~SlimUpdater()
{






    writeSettings();
    delete ui;

}
//---------------------------------------------------------------------------
void SlimUpdater::setMode(QString mode)
{

    if(mode == "auto"){
        bool connectionState = checkConnection();
        if(connectionState == true)
            checkUpdate();

    }
    else{


        checkConnection();
        //        QTimer *timer = new QTimer(this);
        //        connect(timer, SIGNAL(timeout()), this, SLOT(checkConnection()));
        //        timer->start(10000);
    }
}

//---------------------------------------------------------------------------

void SlimUpdater::setCurrentVersion(QString currentVersion)
{
    thisVersion = currentVersion;

    ui->updateLabel->setText(tr("<center>Current version : ") + currentVersion + tr("</center>"));
}





//---------------------------------------------------------------------------
void SlimUpdater::checkUpdate()
{

    manager->get(QNetworkRequest(QUrl("http://www.plume-creator.eu/version.latest")));





}

//---------------------------------------------------------------------------
bool SlimUpdater::checkConnection()
{


    QAbstractSocket *netSocket = new QAbstractSocket(QAbstractSocket::TcpSocket, this);
    netSocket->connectToHost("sourceforge.net", 80);

    QString connectionText;
    bool connectionState;
    if (netSocket->waitForConnected(1000)){
        connectionText = tr("You are connected to the web");
        connectionState = true;
    }
    else{
        connectionText = tr("You are not connected to the web");
        connectionState = false;
    }

    QString webSiteAvailableText;
    bool webSiteState;
    QAbstractSocket *plumeSiteSocket = new QAbstractSocket(QAbstractSocket::TcpSocket, this);
    plumeSiteSocket->connectToHost("www.plume-creator.eu", 80);
    if (plumeSiteSocket->waitForConnected(1000)){
        webSiteAvailableText = tr("Plume Creator website is available");
        webSiteState = true;
    }
    else{
        webSiteAvailableText = tr("Plume Creator website is unavailable");
        webSiteState = false;
    }

    ui->updateLabel->setText(tr("<p>Connection status :"
                                "<blockquote>- ") + connectionText + tr("</blockquote>"
                                                                        "<blockquote>- Verify the proxy settings"
                                                                        "<blockquote>- ") + webSiteAvailableText + tr("</blockquote>"
                                                                                                                      "<blockquote>- Click again on the refresh button --></blockquote>"
                                                                                                                      "</p>"));

    if(connectionState && webSiteState){
        checkUpdate();
        return true;
    }
    else{
        return false;
    }


}



//---------------------------------------------------------------------------

void SlimUpdater::replyFinished(QNetworkReply *reply)
{
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));


    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!domDocument.setContent(reply->readAll(), true, &errorStr, &errorLine,
                                &errorColumn)) {
        QMessageBox::information(this, tr("Plume Creator Version"),
                                 tr("Parse error at line %1, column %2:\n%3\n")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));

        //        qDebug() << "File path:" << device->fileName();
        //        qDebug() << "File readable:" << device->isReadable();
        //        qDebug() << "File open:" << device->isOpen();


        return;
    }




    root = domDocument.documentElement();
    if (root.tagName() != "plume-latest-version") {
        QMessageBox::information(this, tr("Plume Creator Version"),
                                 tr("The file is not a Plume Creator version file."));
        return;

    }

    QString milestone = root.attribute("milestone");
    QDomNode node;
#ifdef Q_OS_MAC
    node = root.elementsByTagName("OSX").at(0);
#endif


#ifdef Q_OS_LINUX
    if(ui->packageComboBox->currentIndex() == 0) // source
        node = root.elementsByTagName("source").at(0);
    if(ui->packageComboBox->currentIndex() == 1) // deb32
        node = root.elementsByTagName("deb32").at(0);
    if(ui->packageComboBox->currentIndex() == 2) //deb64
        node = root.elementsByTagName("deb64").at(0);

#endif

#ifdef Q_OS_WIN32
    node = root.elementsByTagName("Win").at(0);
#endif


    QString updateVersion = node.toElement().attribute("latest");

    QStringList numberStrings;
    numberStrings = updateVersion.split(".");
    QStringList currentNumberStrings;
    currentNumberStrings = currentAppVersion.split(".");

    QList<int> numbers;
    while(!numberStrings.isEmpty())
        numbers.append(numberStrings.takeFirst().toInt());
    QList<int> currentNumbers;
    while(!currentNumberStrings.isEmpty())
        currentNumbers.append(currentNumberStrings.takeFirst().toInt());

    if(currentNumbers.size() > 3 || currentNumbers.size() == 0){
        ui->updateLabel->setText("<b><h3><center>Your current version " + currentAppVersion + " isn't understood !</h3><b>");
        ui->updateLabel->setText(QString::number(currentNumbers.size()) + "  " +QString::number(currentNumberStrings.size()));
        return;
    }
    int sizeMax = qMax(numbers.size(), currentNumbers.size());

    for(int i = 0; i < sizeMax; ++i){
        if(i >= numbers.size())
            numbers.append(0);
        if(i >= currentNumbers.size())
            currentNumbers.append(0);
    }

    bool niv1bool = false;
    bool niv2bool = false;
    bool niv3bool = false;

    if(numbers.size() == 0 )
        return;
    if(sizeMax >= 1 && numbers.at(0) > currentNumbers.at(0)){
        niv1bool = true;
    }

    if(sizeMax >= 2 && numbers.at(1) > currentNumbers.at(1)){ //subversion :
        niv2bool = true;

    }

    if(sizeMax >= 3 && numbers.at(2) > currentNumbers.at(2)){ // beta versions :
        niv3bool = true;

    }




    if(niv1bool || niv2bool || niv3bool){
        QString downloadLink = node.toElement().attribute("url1") + milestone + node.toElement().attribute("url2") + updateVersion + node.toElement().attribute("url3");
        ui->updateLabel->setText(("<b><h3><center>An update is available ! Plume Creator version ")
                                 + updateVersion + tr("</h3><b><br>Download it directly here : <address><a href=")
                                 + downloadLink + ">" + downloadLink + tr("</a></address></center>"));
    }
    else{
        ui->updateLabel->setText(tr("<b><h3><center>You are up to date !</h3><b>"));


        QTimer::singleShot(5000, this, SLOT(closeUpdater()));
    }



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
void SlimUpdater::slotError(QNetworkReply::NetworkError netError)
{
    qDebug() << "netError : " << netError;

}



//---------------------------------------------------------------------------
void SlimUpdater::readSettings()
{
    QSettings settings;
    settings.beginGroup( "Updater" );
    ui->checkUpdateAtStartupBox->setChecked(settings.value( "checkAtStartup_1", true).toBool());
    ui->packageComboBox->setCurrentIndex(settings.value("linuxDistrib", 1).toInt());
    proxyEnabled = settings.value("Proxy/proxyEnabled", false).toBool();
    proxySystemEnabled = settings.value("Proxy/proxySystemEnabled", true).toBool();
    proxyHostName = settings.value("Proxy/proxyHostName", "").toString();
    proxyPort = settings.value("Proxy/proxyUserName", 1080).toInt();
    proxyUserName = settings.value("Proxy/proxyUserName", "").toString();
    proxyPassword = settings.value("Proxy/proxyEnabled", "").toString();
    settings.endGroup();
}



//---------------------------------------------------------------------------
void SlimUpdater::writeSettings()
{
    QSettings settings;
    settings.beginGroup( "Updater" );
    settings.setValue( "checkAtStartup_1", ui->checkUpdateAtStartupBox->checkState());
    settings.setValue( "linuxDistrib", ui->packageComboBox->currentIndex());
    settings.endGroup();
}

