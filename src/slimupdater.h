/***************************************************************************
 *   Copyright (C) 2011 by Cyril Jacquet                                   *
 *   terreville@gmail.com                                                 *
 *                                                                         *
 *  This file is part of Plume Creator.                                    *
 *                                                                         *
 *  Plume Creator is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  Plume Creator is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with Plume Creator.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef SLIMUPDATER_H
#define SLIMUPDATER_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkConfigurationManager>
#include <QDomDocument>
#include <QNetworkProxy>

#include "hub.h"

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
    void setHub(Hub *varHub){hub = varHub;}
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

    void on_verifyButton_clicked();

private :
    Hub *hub;
    Ui::SlimUpdater *ui;

    bool proxyEnabled;
    bool proxySystemEnabled;
    QNetworkProxy::ProxyType proxyType;
    QString proxyHostName;
    int proxyPort;
    QString proxyUserName;
    QString proxyPassword;

    QString text;

QTimer *timer;
int timerNumber;
bool oneTime;

    QNetworkAccessManager *manager;
    QString currentAppVersion;

    QString thisVersion;

    QDomDocument domDocument;
    QDomElement root;
};

#endif // SLIMUPDATER_H
