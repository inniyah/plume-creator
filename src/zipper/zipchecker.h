/***************************************************************************
 *   Copyright (C) 2013 by Cyril Jacquet                                   *
 *   cyril.jacquet@plume-creator.eu                                        *
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
#ifndef ZIPCHECKER_H
#define ZIPCHECKER_H

#include <QObject>
#include <QStringList>
#include <QDir>
#include <QHash>

#include "utils.h"

class ZipChecker : public QObject
{
    Q_OBJECT
public:
    explicit ZipChecker(QObject *parent = 0);
    QStringList list();
    void addFile(QString type, int idNumber);
    void removeFile(QString type, int idNumber);
    void clearList();
    static bool compareAndClean(QString tempDirectory, QStringList fileList);
    static bool isZip(QString fileName);

signals:
    
public slots:

private:
    QStringList baseList;
    QStringList textsList;
};

#endif // ZIPCHECKER_H
