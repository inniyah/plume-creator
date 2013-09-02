/***************************************************************************
 *   Copyright (C) 2011 by Cyril Jacquet                                   *
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
#ifndef OUTLINERSPREADSHEET_H
#define OUTLINERSPREADSHEET_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QWidget>

#include "hub.h"
#include "mainTree/maintreecontextmenu.h"
#include "mainTree/maintreeabstractmodel.h"

class OutlinerSpreadsheet : public QTreeView
{
    Q_OBJECT
public:
    explicit OutlinerSpreadsheet(QWidget *parent = 0);
    
    void postConstructor();
protected :

    void focusInEvent ( QFocusEvent * event );
    void wheelEvent(QWheelEvent* event);
    void contextMenuEvent(QContextMenuEvent *event);
    void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);


signals:
    void columnOneResizedSignal(int newSize);
    void columnTwoResizedSignal(int newSize);
    void modifyFlagsForDropsSignal(QString type);

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void setMainTreeAbstractModel(MainTreeAbstractModel *tree){absTreeModel = tree;}
    void applySettingsFromData();
    void giveStyle();
    void finishStatusEdit();

private slots:
    void doNotWarnAgainSlot(bool dontWarnMe);
    void itemClicked(QModelIndex index);
    void itemEntered(QModelIndex index);


    void columnResizedSlot(int index,int oldSize,int newSize);


    void itemCollapsedSlot(QModelIndex index);
    void itemExpandedSlot(QModelIndex index);





private:
    QList<QModelIndex> allIndexesFromModel(QAbstractItemModel *model, const QModelIndex &parent);

    Hub *hub;
    MainTreeAbstractModel  *absTreeModel;

    bool wasClickedOnce;
    int itemIdClickedOnce;
    int enteredItemId;
QModelIndex enteredItemModelIndex;

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




    MainTreeContextMenu *contextMenu;
    QModelIndexList m_selectedIndexes;
    QModelIndex draggedIndex;

    // clicks :
    bool oneClickCheckpoint, twoClicksCheckpoint, threeClicksCheckpoint;
    QModelIndex oldIndex;






};



#endif // OUTLINERSPREADSHEET_H
