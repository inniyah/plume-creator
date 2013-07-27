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
#ifndef ATTENDBASE_H
#define ATTENDBASE_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QWidget>
#include <QFrame>

#include "hub.h"
#include "attend/attendabstractmodel.h"
#include "attend/attendsheettreeproxymodel.h"
#include "attend/attendglobaltreeproxymodel.h"
#include "attend/attendmanager.h"
//#include "attend/globaltreeview.h"
//#include "attend/sheettreeview.h"

namespace Ui {
class AttendBase;
}

class AttendBase : public QFrame
{
    Q_OBJECT
    
public:
    explicit AttendBase(QWidget *parent = 0);
    ~AttendBase();
    void startAttendance();

signals:


public slots:
    void setHub(Hub *varHub){hub = varHub;}

private slots:
    AttendManager* launchAttendManager();
    void openDetailsOf_fromGlobal(QModelIndex index = QModelIndex());
    void openDetailsOf_fromSheet(QModelIndex index = QModelIndex());

    void setManagerLaunched(){isManagerLaunched = false;}
    void on_collapseButton_clicked();

    void on_splitter_splitterMoved(int pos, int index);

    void expandAll();


private:
   Ui::AttendBase *ui;
    Hub *hub;

    AttendAbstractModel *absModel;
    AttendGlobalTreeProxyModel *globalProxyModel;
    AttendSheetTreeProxyModel *sheetProxyModel;
    bool isManagerLaunched;
};

#endif // ATTENDBASE_H
