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
#ifndef OUTLINERABSTRACTMODEL_H
#define OUTLINERABSTRACTMODEL_H

#include <QAbstractTableModel>
#include <QDomDocument>
#include <QStringList>
#include <QObject>
#include <QTextDocument>

#include "hub.h"
#include "outliner/outlinertreeitem.h"

class OutlinerAbstractModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit OutlinerAbstractModel(QObject *parent = 0);
    ~OutlinerAbstractModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole);
    QModelIndex index(int, int, const QModelIndex&) const;
    QModelIndex parent(const QModelIndex&) const;
signals:

    void updateMainDomDocSignal(QDomDocument otoM_domDoc);
    void applySynNoteFontConfigSignal();

public slots:

    void setHub(Hub *varHub){hub = varHub;}

    // set the size hint :
    void shrinkRow(){itemHeight += 20; beginResetModel(); endResetModel();}
    void expandRow(){if(itemHeight > 40)itemHeight -= 20; beginResetModel(); endResetModel();}
    void setRowHeight(int height){itemHeight = height; beginResetModel(); endResetModel();}

    void resetAbsModel();


    void mtoO_setDomDoc(QDomDocument domDoc);
    void resetDomDoc();
    void parseFolderElement(const QDomElement &element);

    void mtoO_setNumForDoc(QHash<MainTextDocument *, int> numForDoc);
    void reset_mtoO_setNumForDoc();

    void columnOneResizedSlot(int newSize){textZoneColumnOneWidth = newSize;}
    void columnTwoResizedSlot(int newSize){textZoneColumnTwoWidth = newSize;}
    void resetAbsModelColumnOne();
    void resetAbsModelColumnTwo();

private slots:

    void updateMainDomDoc();
    void updateMainTextDoc(MainTextDocument *textDoc, int number);

private:
    QStringList givePovList(QString listOfPovNumbers);

    Hub *hub;
    OutlinerTreeItem *rootItem;
    int itemHeight;
    int textZoneColumnOneWidth;
    int textZoneColumnTwoWidth;

    QDomDocument mtoO_domDoc;
    QHash<int, QDomElement> domElementForNumber;
    QDomElement root;
    int numberOfDomElements, numberOfBooks, numberOfChapters, numberOfScenes;
    QStringList *titlesList;
    QStringList *leftHeadersList;
    QList<int> *numberList;
    QHash<MainTextDocument *, int> mtoO_numForDoc;
    QHash<MainTextDocument *, int> mtoO_numForClonedDoc;

    QList<OutlinerTreeItem *> *treeBookItemList;
    QList<OutlinerTreeItem *> *treeChapterItemList;

};

#endif // OUTLINERABSTRACTMODEL_H
