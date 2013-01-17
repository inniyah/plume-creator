

#ifndef SLIMUPDATER_H
#define SLIMUPDATER_H

#include <QtGui>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkConfigurationManager>
#include <QDomDocument>

namespace Ui {
class SlimUpdater;
}

class SlimUpdater : public QWidget
{
    Q_OBJECT
    
public:
    explicit SlimUpdater(QString mode = "none", QWidget *parent = 0);
    ~SlimUpdater();
    
signals:
    void closeUpdaterSignal();

public slots:
    void setCurrentVersion(QString currentVersion);
    void setMode(QString mode);

    //    void execute();
    //    void downloadFinished(QNetworkReply *reply);
    //    void sslErrors(const QList<QSslError> &errors);


private slots:
    void checkUpdate();
    void replyFinished(QNetworkReply *reply);
    void slotError(QNetworkReply::NetworkError);
    bool checkConnection();

    void readSettings();
    void writeSettings();

    void closeUpdater();

private :
    Ui::SlimUpdater *ui;

    bool proxyEnabled;
    bool proxySystemEnabled;
    QString proxyHostName;
    int proxyPort;
    QString proxyUserName;
    QString proxyPassword;



    QNetworkAccessManager *manager;
    QString currentAppVersion;

    QString thisVersion;

    QDomDocument domDocument;
    QDomElement root;
};

#endif // SLIMUPDATER_H
