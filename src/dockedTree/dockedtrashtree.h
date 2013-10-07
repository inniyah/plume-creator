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
#ifndef DOCKEDTRASHTREE_H
#define DOCKEDTRASHTREE_H

#include <QTreeView>
#include "hub.h"
#include "mainTree/maintreecontextmenu.h"
#include "mainTree/maintreeabstractmodel.h"


class DockedTrashTree : public QTreeView
{
    Q_OBJECT
public:
    explicit DockedTrashTree(QWidget *parent = 0);
    void postConstructor();

protected:
    void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
 void dragEnterEvent(QDragEnterEvent *event);
 void dragLeaveEvent(QDragLeaveEvent *event);
 void dropEvent(QDropEvent *event);
 void contextMenuEvent(QContextMenuEvent *event);
 void leaveEvent(QEvent *event);
 void enterEvent(QEvent *event);

signals:
 void modifyFlagsForDropsSignal(QString type);
 void textAndNoteSignal(int number, QString action);
 void currentOpenedSheetSignal(int sheetIdNumber);

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void setMainTreeAbstractModel(MainTreeAbstractModel *tree){absTreeModel = tree;}
    void applySettingsFromData();

private slots:
    void itemCollapsedSlot(QModelIndex index);
    void itemExpandedSlot(QModelIndex index);
    void itemClicked(QModelIndex index);

    void extendTrashTree();
    void shrinkTrashTree();

private:
    Hub *hub;
   MainTreeAbstractModel  *absTreeModel;

    MainTreeContextMenu *contextMenu;
    QList<QModelIndex> allIndexesFromModel(QAbstractItemModel *model, const QModelIndex &parent);
QModelIndexList m_selectedIndexes;
QModelIndex draggedIndex;

// clicks :
bool oneClickCheckpoint, twoClicksCheckpoint, threeClicksCheckpoint;
QModelIndex oldIndex;

bool isLeaveEventStopped;
};

#endif // DOCKEDTRASHTREE_H
