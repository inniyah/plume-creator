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
#ifndef CHANGESTESTS_H
#define CHANGESTESTS_H

#include <QObject>
#include <QDomDocument>
#include <QHash>
#include <QFile>
#include <QTextDocument>
#include "maintextdocument.h"

class ChangesTests : public QObject
{
    Q_OBJECT
public:
    static bool test(int oldInt, int newInt);
    static bool test(QString oldString, QString newString);
    static bool test(QHash<QTextDocument *, QFile *> oldHash, QHash<QTextDocument *, QFile *> newHash);
    static bool test(QHash<QTextDocument *, int> oldHash, QHash<QTextDocument *, int> newHash);
    static bool test(QHash<MainTextDocument *, QFile *> oldHash, QHash<MainTextDocument *, QFile *> newHash);
    static bool test(QHash<MainTextDocument *, int> oldHash, QHash<MainTextDocument *, int> newHash);
    static bool test(QHash<int, QDomElement> oldHash, QHash<int, QDomElement> newHash);
signals:
    
public slots:
    
};

#endif // CHANGESTESTS_H
