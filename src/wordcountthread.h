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
#ifndef WORDCOUNTTHREAD_H
#define WORDCOUNTTHREAD_H

#include <QObject>
#include <QtCore>
#include <QDomDocument>
#include <QTextDocument>

#include "hub.h"

class WordCountThread : public QThread
{
    Q_OBJECT
public:
    explicit WordCountThread(QObject *parent = 0);

protected:
    void run();

signals:
    void chapterWordCountSignal(int count);
    void bookWordCountSignal(int count);
    void projectWordCountSignal(int count);

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void setDocs(QHash<MainTextDocument*,QFile*> fileForDocs);
    void setDom(QDomDocument domDoc);
    void setCurrentNumber(int number);

private slots:
    int countWords(QTextDocument *doc);

private:
    Hub *hub;
    QList<MainTextDocument *> textDocList;
    QDomDocument domDocument;
    int currNumber;
};

#endif // WORDCOUNTTHREAD_H
