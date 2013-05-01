/***************************************************************************
 *   Copyright (C) 2012 by Cyril Jacquet                                   *
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
#ifndef FILEUPDATER_H
#define FILEUPDATER_H

#include <QObject>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QDomDocument>


class FileUpdater : public QWidget
{
    Q_OBJECT
public:
    explicit FileUpdater(QWidget *parent = 0);
    bool checkAttendanceFile(QString projectFileName);
    bool checkTreeFile(QString projectFileName);
    bool checkInfoFile(QString projectFileName);
    static bool isZip(QString projectFileName);

    QString getAttendVersion(){return attendVersion;}
    QString getTreeVersion(){return treeVersion;}
    QString getInfoVersion(){return infoVersion;}

signals:
    
public slots:

private slots:
    void updateAttendanceFile();
    void updateTreeFile();
    void updateInfoFile();

private:
    QFile *attFile;
    QFile *treeFile;
    QFile *infoFile;
    QDomDocument attendDomDocument;
    QDomElement attendRoot;
    QDomDocument treeDomDocument;
    QDomElement treeRoot;
    QDomDocument infoDomDocument;
    QDomElement infoRoot;
    QString devicePath;
    QString attendVersion;
    QString treeVersion;
    QString infoVersion;


};

#endif // FILEUPDATER_H
