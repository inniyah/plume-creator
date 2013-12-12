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
#ifndef OUTLINERBASE_H
#define OUTLINERBASE_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QWidget>
#include <QDomDocument>

#include "hub.h"
#include "outliner/outlinerspreadsheet.h"
#include "outliner/outlinerspreadsheetproxy.h"
#include "mainTree/maintreeabstractmodel.h"
#include "outliner/outlineritemnotedelegate.h"
#include "outliner/outlineritempovdelegate.h"
#include "outliner/outlineritemstatusdelegate.h"

class OutlinerBase : public QWidget

{
    Q_OBJECT
public:
    explicit OutlinerBase(QWidget *parent = 0);
    void postConstructor();
   ~OutlinerBase();
    QList<QAction *> toolButtons();
    QWidget *spreadsheetWidget();

protected:
//    void closeEvent(QCloseEvent *event);

signals:
    void toSpreadsheetSignal(bool spreadsheetSignal);
    void updateMainDomDocSignal();
    void applySynNoteFontConfigSignal();

    void otoM_actionSignal(QString action, int idNumber);

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void setMainTreeAbstractModel(MainTreeAbstractModel *tree){absTreeModel = tree;}

    void saveConfig();
//void showOutliner();
void updateOutliner();


// for textDocument :
void mtoO_setNumForDoc(QHash<MainTextDocument *, int> numForDoc);


private slots:
    void shiftToSpreadsheet();
    void applySpreadsheetConfig();
    void resetSpreadsheetState();

void moveViewTo(int hBarValue, int vBarValue);


private:
    Hub *hub;
    MainTreeAbstractModel  *absTreeModel;
OutlinerSpreadheetProxy *proxy;

    bool spreadsheetMode;
QAction *expandSpreadsheetAct, *shrinkSpreadsheetAct, *moveUpAct, *moveDownAct, *resetAct;


OutlinerSpreadsheet *spreadsheet;

QString lastOpened;

QDomDocument mtoO_domDoc;

QHash<MainTextDocument *, int> mtoO_numForDoc;

    QHash<QListWidgetItem *, QDomElement> attend_domElementForItem;
    QHash<int, QDomElement> attend_domElementForItemNumber;

    QString newAttendName;
bool isResetOn;
};

#endif // OUTLINERBASE_H
