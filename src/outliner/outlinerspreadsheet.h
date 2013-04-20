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
#ifndef OUTLINERSPREADSHEET_H
#define OUTLINERSPREADSHEET_H

#include <QtGui>
#include <QWidget>

#include "hub.h"


class OutlinerSpreadsheet : public QTreeView
{
    Q_OBJECT
public:
    explicit OutlinerSpreadsheet(QWidget *parent = 0);
    
protected :
    //    void	updateGeometries ();
    //    void	paintEvent ( QPaintEvent * event );
    void focusInEvent ( QFocusEvent * event );
    //        void mouseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void contextMenuEvent(QContextMenuEvent *event);


signals:
    void otoM_actionSignal(QString action, int idNumber);
    void columnOneResizedSignal(int newSize);
    void columnTwoResizedSignal(int newSize);

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void temp_moveUp(){moveUp();}
    void temp_moveDown(){moveDown();}
    void applySettingsFromData();
    void giveStyle();

private slots:
    void doNotWarnAgainSlot(bool dontWarnMe);
    void itemClicked(QModelIndex index);
    void prepareContextMenu();
    void itemEntered(QModelIndex index);

    void rename(){emit otoM_actionSignal("rename", enteredItemId);}
    void addItemNext(){emit otoM_actionSignal("addItemNext", enteredItemId);}
    void addChild(){emit otoM_actionSignal("addChild", enteredItemId);}
    void addSeparator(){emit otoM_actionSignal("addSeparator", enteredItemId);}
    void moveUp(){emit otoM_actionSignal("moveUp", enteredItemId);}
    void moveDown(){emit otoM_actionSignal("moveDown", enteredItemId);}
    void removeItem(){emit otoM_actionSignal("removeItem", enteredItemId);}
    void autoRenameChilds(){emit otoM_actionSignal("autoRenameChilds", enteredItemId);}
    void addMulti(){emit otoM_actionSignal("addMulti", enteredItemId);}

    void columnResizedSlot(int index,int oldSize,int newSize);


    void itemCollapsedSlot(QModelIndex index);
    void itemExpandedSlot(QModelIndex index);





private:
    QList<QModelIndex> allIndexesFromModel(QAbstractItemModel *model, const QModelIndex &parent);

    Hub *hub;
    bool wasClickedOnce;
    int itemIdClickedOnce;
    int enteredItemId;

    QAction *renameAct,
    *addItemNextAct,
    *addChildAct,
    *addSeparatorAct,
    *autoRenameChildsAct,
    *delYesAct,
    *moveUpAct,
    *moveDownAct,
    *addMultiAct;
    QMenu *delItemMenu,
    *advancedMenu;
};

#endif // OUTLINERSPREADSHEET_H
