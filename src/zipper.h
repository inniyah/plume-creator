/***************************************************************************
 *   Copyright (C) 2012 by Cyril Jacquet                                   *
 *   cyril.jacquet@plume-creator.eu                                                 *
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
#ifndef ZIPPER_H
#define ZIPPER_H

#include <QObject>
#include <QtCore>


class Zipper : public QThread
{
    Q_OBJECT
public:
    explicit Zipper(QObject *parent = 0);
    void setJob(QString job, QString fileName, QString workPath);


protected:
    void run();

signals:

    void zipStarted(QString mode);
    void zipProgressed();
    void zipFinished();
    void zipError(QString error);


private:
    void extract(QString zipFileName);
    void compress(QString zipFileName);
    bool checkZip(QString zipFileName);
    QString currentJob;
    QString m_fileName, m_workPath;
    bool oneTime;

};

#endif // ZIPPER_H
