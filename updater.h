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
#ifndef UPDATER_H
#define UPDATER_H

#include <QtGui>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkConfigurationManager>
#include <QDomDocument>

class Updater : public QDialog
{
    Q_OBJECT
public:
    explicit Updater(QString mode = "none", QWidget *parent = 0);
    ~Updater();
    //    QNetworkAccessManager manager;
    //    QList<QNetworkReply *> currentDownloads;


    //    void doDownload(const QUrl &url);
    //    QString saveFileName(const QUrl &url);
    //    bool saveToDisk(const QString &filename, QIODevice *data);

signals:
    
public slots:
    void setCurrentVersion(QString currentVersion);

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

private :
    QNetworkAccessManager *manager;


    QLabel *currentVersionLabel, *connectionLabel, *verifyLabel;
    QVBoxLayout *verifyLayout;
    QString thisVersion;
    QPushButton *verifyButton;
    QCheckBox *checkUpdateAtStartupBox;

    QDomDocument domDocument;
    QDomElement root;
};

#endif // UPDATER_H
