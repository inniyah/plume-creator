#include <QtGui>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkSession>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QtGlobal>

#include "updater.h"

Updater::Updater(QString mode, QWidget *parent) :
    QDialog(parent)
{

    this->setAttribute(Qt::WA_DeleteOnClose);

    setWindowTitle(tr("Update Checker"));

    QLabel *checkerLabel = new QLabel;
    checkerLabel->setText(tr("Please press this button to verify if an update is available :"));

    currentVersionLabel = new QLabel;
    currentVersionLabel->setText(tr("Error : version not readable"));
    connectionLabel = new QLabel;
    connectionLabel->setText(tr("No connection"));
    QLabel *linkLabel = new QLabel;
    linkLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    linkLabel->setOpenExternalLinks(true);
    linkLabel->setText(tr("<center>Website : <br><address><a href=\http://sourceforge.net/projects/plume-creator\>http://sourceforge.net/projects/plume-creator</a></address></center>"));
    verifyLabel = new QLabel;
    verifyLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    verifyLabel->setOpenExternalLinks(true);
    checkUpdateAtStartupBox = new QCheckBox(tr("Always check at Plume startup ?"));


    verifyButton = new QPushButton(tr("&Check"));
    connect(verifyButton, SIGNAL(clicked()), this, SLOT(checkUpdate()));

    QPushButton *closeButton = new QPushButton(tr("&Close"));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));

    verifyLayout = new QVBoxLayout;
    verifyLayout->addWidget(verifyLabel);
    verifyLayout->addWidget(checkerLabel);
    verifyLayout->addWidget(verifyButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(currentVersionLabel);
    layout->addSpacing(20);
    layout->addWidget(connectionLabel);
    layout->addSpacing(20);
    layout->addWidget(linkLabel);
    layout->addSpacing(20);
    layout->addLayout(verifyLayout);
    layout->addWidget(closeButton);
    layout->addWidget(checkUpdateAtStartupBox);
    this->setLayout(layout);

    readSettings();


    manager = new QNetworkAccessManager;
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    //    connect(&manager, SIGNAL(finished(QNetworkReply*)),
    //            SLOT(downloadFinished(QNetworkReply*)));




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
Updater::~Updater()
{

    writeSettings();
}
//---------------------------------------------------------------------------

void Updater::setCurrentVersion(QString currentVersion)
{
    thisVersion = currentVersion;

    currentVersionLabel->setText(tr("<center>Current version : ") + currentVersion + tr("</center>"));
}





//---------------------------------------------------------------------------
void Updater::checkUpdate()
{

    manager->get(QNetworkRequest(QUrl("http://www.plume-creator.eu/version.latest")));


    //http://188.165.39.41/~btvuhmyz/version.latest




}

//---------------------------------------------------------------------------
bool Updater::checkConnection()
{

    qDebug() << "a";

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

    connectionLabel->setText(tr("<p>Connection status :"
                                "<blockquote>- ") + connectionText + tr("</blockquote>"
                                "<blockquote>- ") + webSiteAvailableText + tr("</blockquote>"
                                "</p>"));

    if(connectionState && webSiteState){
        verifyButton->setEnabled(true);
        return true;
    }
    else{
        verifyButton->setEnabled(false);
        return false;
    }




}



//---------------------------------------------------------------------------

void Updater::replyFinished(QNetworkReply *reply)
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
    node = root.elementsByTagName("deb32").at(0);
#endif

#ifdef Q_OS_WIN32
    node = root.elementsByTagName("Win").at(0);
#endif


    QString updateVersion = node.toElement().attribute("latest");

    if(thisVersion.toFloat() < updateVersion.toFloat()){
        QString downloadLink = node.toElement().attribute("url1") + milestone + node.toElement().attribute("url2") + updateVersion + node.toElement().attribute("url3");
        verifyLabel->setText(("<center>An update is available ! Plume Creator version ")
                             + updateVersion + tr("<br>Download it directly here : <br><address><a href=\ ")
                             + downloadLink + "\>" + downloadLink + tr("</a></address></center>"));
    }
    else if(thisVersion.toFloat() == updateVersion.toFloat()){
        verifyLabel->setText(tr("You are up to date !"));

    }



}
//---------------------------------------------------------------------------
void Updater::slotError(QNetworkReply::NetworkError netError)
{
    qDebug() << "netError : " << netError;

}



//---------------------------------------------------------------------------
void Updater::readSettings()
{
    QSettings settings;
    settings.beginGroup( "Updater" );
    checkUpdateAtStartupBox->setChecked(settings.value( "checkAtStartup", true).toBool());
    settings.endGroup();
}



//---------------------------------------------------------------------------
void Updater::writeSettings()
{
    QSettings settings;
    settings.beginGroup( "Updater" );
    settings.setValue( "checkAtStartup", checkUpdateAtStartupBox->checkState());
    settings.endGroup();
}






//void Updater::doDownload(const QUrl &url)
//{
//    QNetworkRequest request(url);
//    QNetworkReply *reply = manager.get(request);
//    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors(QList<QSslError>)));

//    currentDownloads.append(reply);
//}

//QString Updater::saveFileName(const QUrl &url)
//{
//    QString path = url.path();
//    QString basename = QFileInfo(path).fileName();

//    if (basename.isEmpty())
//        basename = "download";

//    if (QFile::exists(basename)) {
//        // already exists, don't overwrite
//        int i = 0;
//        basename += '.';
//        while (QFile::exists(basename + QString::number(i)))
//            ++i;

//        basename += QString::number(i);
//    }

//    return basename;
//}

//bool Updater::saveToDisk(const QString &filename, QIODevice *data)
//{
//    QFile file(filename);
//    if (!file.open(QIODevice::WriteOnly)) {
//        fprintf(stderr, "Could not open %s for writing: %s\n",
//                qPrintable(filename),
//                qPrintable(file.errorString()));
//        return false;
//    }

//    file.write(data->readAll());
//    file.close();

//    return true;
//}

//void Updater::execute()
//{
//    QStringList args = QCoreApplication::instance()->arguments();
//    args.takeFirst();           // skip the first argument, which is the program's name
//    if (args.isEmpty()) {
//        printf("Qt Download example - downloads all URLs in parallel\n"
//               "Usage: download url1 [url2... urlN]\n"
//               "\n"
//               "Downloads the URLs passed in the command-line to the local directory\n"
//               "If the target file already exists, a .0, .1, .2, etc. is appended to\n"
//               "differentiate.\n");
//        QCoreApplication::instance()->quit();
//        return;
//    }

//    foreach (QString arg, args) {
//        QUrl url = QUrl::fromEncoded(arg.toLocal8Bit());
//        doDownload(url);
//    }
//}

//void Updater::sslErrors(const QList<QSslError> &sslErrors)
//{
//#ifndef QT_NO_OPENSSL
//    foreach (const QSslError &error, sslErrors)
//        fprintf(stderr, "SSL error: %s\n", qPrintable(error.errorString()));
//#endif
//}

//void Updater::downloadFinished(QNetworkReply *reply)
//{
//    QUrl url = reply->url();
//    if (reply->error()) {
//        fprintf(stderr, "Download of %s failed: %s\n",
//                url.toEncoded().constData(),
//                qPrintable(reply->errorString()));
//    } else {
//        QString filename = saveFileName(url);
//        if (saveToDisk(filename, reply))
//            printf("Download of %s succeeded (saved to %s)\n",
//                   url.toEncoded().constData(), qPrintable(filename));
//    }

//    currentDownloads.removeAll(reply);
//    reply->deleteLater();

//    if (currentDownloads.isEmpty())
//        // all downloads finished
//        QCoreApplication::instance()->quit();
//}

////int main(int argc, char **argv)
////{
////    QCoreApplication app(argc, argv);

////    DownloadManager manager;
////    QTimer::singleShot(0, &manager, SLOT(execute()));

////    app.exec();
////}
