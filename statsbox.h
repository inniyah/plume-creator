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
#ifndef STATSBOX_H
#define STATSBOX_H

#include <QWidget>
#include <QtGui>
#include <QDomDocument>
//
class StatsBox : public QFrame
{
    Q_OBJECT
public:
    explicit StatsBox(QWidget *parent = 0);

signals:
    void timerSignal(QString);
    void fetchDomAndDocsSignal();
    void fetchCurrentNumber();
    void docsForProjectWordCountSignal(QHash<QTextDocument*,QFile*>);
    void domForProjectWordCountSignal(QDomDocument);
    void setCurrentNumberSignal(int);

public slots:
    void setWordCount(int num);

private slots:
    void displayProjectWordCount();
    void setChapterWordCount(int count);
    void setBookWordCount(int count);
    void setProjectWordCount(int count);

private:
    QPushButton *projectWordCountButton;
    QLabel *wordCountLabel;
    QLabel *currentProjectCountLabel, *currentBookCountLabel, *currentChapterCountLabel;

};

#endif // STATSBOX_H
