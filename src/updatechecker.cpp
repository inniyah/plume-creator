#include "updatechecker.h"

UpdateChecker::UpdateChecker(QObject *parent) :
    currentAppVersion(QApplication::applicationVersion()), m_packageType(0),
    connectionText(""), webSiteAvailableText("")

{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeOut()), Qt::UniqueConnection);

    readSettings();

    manager = new QNetworkAccessManager;
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)), Qt::UniqueConnection);




}

void UpdateChecker::setPackageType(int index)
{
    m_packageType = index;
}

int UpdateChecker::packageType()
{
    return m_packageType;
}


//---------------------------------------------------------------------------
void UpdateChecker::checkUpdate()
{

    manager->get(QNetworkRequest(QUrl("http://www.plume-creator.eu/version.latest")));


}

//---------------------------------------------------------------------------
bool UpdateChecker::checkConnection()
{
    emit replySignal(tr("Checking..."));

    connectionText = "";
            webSiteAvailableText = "";

timer->start(10000);
            // proxy setup :

    readSettings();
    QNetworkProxy proxy;
    if(proxyEnabled){
        if(proxySystemEnabled){
            proxy = QNetworkProxyFactory::systemProxyForQuery().first();
        }
        else{
            proxy.setType(proxyType);
            proxy.setHostName(proxyHostName);
            proxy.setPort(proxyPort);
            proxy.setUser(proxyUserName);
            proxy.setPassword(proxyPassword);
        }
    }
    else
        proxy.setType(QNetworkProxy::NoProxy);

    QNetworkProxy::setApplicationProxy(proxy);
    manager->setProxy(proxy);





    netSocket = new QAbstractSocket(QAbstractSocket::TcpSocket, this);
    connect(netSocket, SIGNAL(connected()), this, SLOT(netSocketIsConnected()), Qt::UniqueConnection);
    connect(netSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)), Qt::UniqueConnection);
    netSocket->connectToHost("sourceforge.net", 80);





}



//---------------------------------------------------------------------------

void UpdateChecker::replyFinished(QNetworkReply *reply)
{
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));


    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!domDocument.setContent(reply->readAll(), true, &errorStr, &errorLine,
                                &errorColumn)) {
        QString warnString =  "Plume Creator Version : Parse error at line %1, column %2:\n%3\n";
        qWarning() << warnString.arg(errorLine).arg(errorColumn).arg(errorStr);
        qWarning() << "QNetworkReply reply :  "<< domDocument.toString();


        return;
    }




    root = domDocument.documentElement();
    if (root.tagName() != "plume-latest-version") {
        qWarning() << tr("Plume Creator Version") << " : " << tr("The file is not a Plume Creator version file.");
        return;

    }

    QString milestone = root.attribute("milestone");
    QDomNode node;
#ifdef Q_OS_MAC
    node = root.elementsByTagName("OSX").at(0);
#endif


#ifdef Q_OS_LINUX
    if(m_packageType == 0) // source
        node = root.elementsByTagName("source").at(0);
    if(m_packageType == 1) // deb32
        node = root.elementsByTagName("deb32").at(0);
    if(m_packageType == 2) //deb64
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
        emit replySignal("<b><h3><center>Your current version " + currentAppVersion + " isn't understood !</h3><b>");
        emit replySignal(QString::number(currentNumbers.size()) + "  " +QString::number(currentNumberStrings.size()));
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

    //    if web version is behind the current version :
    if(numbers.at(0) < currentNumbers.at(0) || numbers.at(1) < currentNumbers.at(1)){
        niv1bool = false;
        niv2bool = false;
        niv3bool = false;
    }


    if(niv1bool || niv2bool || niv3bool){
        //        QString downloadLink = node.toElement().attribute("url1") + milestone + node.toElement().attribute("url2") + updateVersion + node.toElement().attribute("url3");
        QString downloadLink = "http://www.plume-creator.eu/site/index.php/en/download-en";
        emit replySignal(("<b><h3><center>An update is available ! Plume Creator version ")
                                 + updateVersion + tr("</h3><b><br>Download it directly here : <address><a href=")
                                 + downloadLink + ">" + downloadLink + tr("</a></address></center>"));
    }
    else{
        emit replySignal(tr("<b><h3><center>You are up to date !</h3><b>"));


        emit replySignal("close");
    }



}

//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
void UpdateChecker::slotError(QNetworkReply::NetworkError netError)
{
    qDebug() << "netError : " << netError;

}
void UpdateChecker::socketError(QAbstractSocket::SocketError error)
{
    qDebug() << "netError : " << error;




}

void UpdateChecker::timeOut()
{
    if(connectionText == "")
    connectionText = tr("You are not connected to the web");


    if(webSiteAvailableText == "")
    webSiteAvailableText = tr("Plume Creator website is unavailable");

    text = tr("<p>Connection status :"
              "<blockquote>- ") + connectionText
            + tr("</blockquote>"
                  "<blockquote>- ") + webSiteAvailableText + tr("</blockquote>"
                   "</p>");

    emit replySignal(text);
}


void UpdateChecker::netSocketIsConnected()
{


        connectionText = tr("You are connected to the web");
        connectionState = true;


   plumeSiteSocket = new QAbstractSocket(QAbstractSocket::TcpSocket, this);
    connect(plumeSiteSocket, SIGNAL(connected()), this, SLOT(plumeSiteIsConnected()), Qt::UniqueConnection);
    connect(plumeSiteSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)), Qt::UniqueConnection);
    plumeSiteSocket->connectToHost("www.plume-creator.eu", 80);
}

void UpdateChecker::plumeSiteIsConnected()
{

    bool webSiteState;



    webSiteAvailableText = tr("Plume Creator website is available");
        webSiteState = true;



checkUpdate();
timer->stop();

}


















//---------------------------------------------------------------------------
void UpdateChecker::readSettings()
{
    QSettings settings;
    settings.beginGroup( "Updater" );
    proxyEnabled = settings.value("Proxy/proxyEnabled", false).toBool();
    proxySystemEnabled = settings.value("Proxy/proxySystemEnabled", true).toBool();

    QString type = settings.value("Proxy/proxyType", "http").toString();
    if(type == "http")
        proxyType = QNetworkProxy::HttpProxy;
    else if(type == "socks5")
        proxyType =  QNetworkProxy::Socks5Proxy;
    else
        proxyType = QNetworkProxy::HttpProxy;
    proxyHostName = settings.value("Proxy/proxyHostName", "").toString();
    proxyPort = settings.value("Proxy/proxyPort", 1080).toInt();
    proxyUserName = settings.value("Proxy/proxyUserName", "").toString();
    proxyPassword = settings.value("Proxy/proxyPassword", "").toString();
    settings.endGroup();
}

