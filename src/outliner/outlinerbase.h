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
#ifndef OUTLINERBASE_H
#define OUTLINERBASE_H

#include <QtGui>
#include <QWidget>
#include <QDomDocument>

#include "outliner/outlinerabstractmodel.h"
#include "outliner/outlinerspreadsheet.h"

class OutlinerBase : public QWidget

{
    Q_OBJECT
public:
    explicit OutlinerBase(QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void toSpreadsheetSignal(bool spreadsheetSignal);
    void updateMainDomDocSignal(QDomDocument otoM_domDoc);
    void applySynNoteFontConfigSignal();

    void otoM_actionSignal(QString action, int idNumber);

public slots:
    void saveConfig();
    void applyConfig();
void showOutliner();
void updateOutliner();

// for domDocument :
void mtoO_setDomDoc(QDomDocument domDoc);

// for textDocument :
void mtoO_setNumForDoc(QHash<QTextDocument *, int> numForDoc);

//for attendances :
void mtoO_updateAttendances(QHash<int, QString> attendStringForNumber);
void mtoO_setProjectAttendanceList(QHash<QListWidgetItem *, QDomElement> domElementForItem_, QHash<int, QDomElement> domElementForItemNumber_);



private slots:
    void shiftToSpreadsheet();
    void applySpreadsheetConfig();

void moveViewTo(int hBarValue, int vBarValue);

    //for attendance :
    QList<QListWidgetItem *> *sortAttendItems(QList<int> *attend, QString sorting = "nothing");
    QListWidgetItem *attendSeparator(QString separatorName);
    QList<QListWidgetItem *> *openSheetAttendList(int number ,QString attendString);



private:
    QVBoxLayout *zoneLayout;
    bool spreadsheetMode;
QAction *shiftToSpreadsheetAct, *expandSpreadsheetAct, *shrinkSpreadsheetAct, *moveUpAct, *moveDownAct;

OutlinerAbstractModel *absModel;
OutlinerSpreadsheet *spreadsheet;

QString lastOpened;

QDomDocument mtoO_domDoc;

QHash<QTextDocument *, int> mtoO_numForDoc;

    QHash<QListWidgetItem *, QDomElement> attend_domElementForItem;
    QHash<int, QDomElement> attend_domElementForItemNumber;

    QString newAttendName;

};

#endif // OUTLINERBASE_H
