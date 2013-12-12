#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>
#include <QApplication>
#include <QtCore>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkConfigurationManager>
#include <QDomDocument>
#include <QNetworkProxy>

class UpdateChecker : public QObject
{
    Q_OBJECT
public:
    explicit UpdateChecker(QObject *parent = 0);
    void setPackageType(int index);
    int packageType();
    void checkConnection();

protected:

signals:
    void replySignal(QString reply);

public slots:

private slots:
    void readSettings();



    void checkUpdate();
    void replyFinished(QNetworkReply *reply);
    void slotError(QNetworkReply::NetworkError netError);


    void netSocketIsConnected();
    void plumeSiteIsConnected();

    void socketError(QAbstractSocket::SocketError error);
    void timeOut();
private:
    bool proxyEnabled;
    bool proxySystemEnabled;
    QNetworkProxy::ProxyType proxyType;
QAbstractSocket *netSocket, *plumeSiteSocket;
QString proxyHostName;
    int proxyPort;
    QString proxyUserName;
    QString proxyPassword;

    QTimer *timer;
    int timerNumber;
    bool oneTime;
    bool connectionState;
        QNetworkAccessManager *manager;
        QString currentAppVersion;


        QString connectionText, webSiteAvailableText;

        QDomDocument domDocument;
        QDomElement root;
        QString text;

        int m_packageType;
};

#endif // UPDATECHECKER_H
