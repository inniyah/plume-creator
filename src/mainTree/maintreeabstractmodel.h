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
#ifndef MAINTREEABSTRACTMODEL_H
#define MAINTREEABSTRACTMODEL_H

#include <QAbstractTableModel>
#include <QDomDocument>
#include <QStringList>
#include <QObject>
#include <QTextDocument>

#include "hub.h"
#include "mainTree/maintreeitem.h"

class MainTreeAbstractModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit MainTreeAbstractModel(QObject *parent = 0);
    ~MainTreeAbstractModel();

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
    void setDomDocument(QDomDocument domDoc);


    static  QStringList giveStatusList();
    static QString giveStatus(int statusInt);
    static QPixmap giveDecoration(QModelIndex index, MainTreeItem::Tree tree);

signals:

    void updateMainDomDocSignal(QDomDocument otoM_domDoc);
    void applySynNoteFontConfigSignal();
    void applySettingsFromDataSignal();
    void textAndNoteSignal(int number,QString action);
    void displayBadgeSignal(bool value);
    void treeStructureChanged();

public slots:

    void setHub(Hub *varHub){hub = varHub;}


    void resetAbsModel();



    void resetDomDoc();
    void parseFolderElement(const QDomElement &element);

    void mtoO_setNumForDoc(QHash<MainTextDocument *, int> numForDoc);
    void reset_mtoO_setNumForDoc();

    void actionSlot(QString action, int idNumber, QVariant var);
    void actionSlot(QString action, int idNumber);


    // for "new" button in fullscreen :
    int addItemNext(int baseNumber);


    void modifyDataForOpenedSheetMarker(const int currentSheet);

private slots:

    void updateMainTextDoc(MainTextDocument *textDoc, int number);

private:
    void removeItem(QDomElement element);

    QDomElement addSheet(QDomElement targetElement, QString action, QString type);
    QStringList givePovList(QString listOfPovNumbers);
    QDomElement modifyAttributes(QDomElement newElement, QString typeOfNewElement);

    Hub *hub;
    MainTreeItem *rootItem;

    QList<int> freeNumList;
    QList<int> freeSeparatorsNumList;

    QDomDocument mtoO_domDoc;
    QHash<int, QDomElement> domElementForNumber;
    QDomElement root;
    int numberOfDomElements, numberOfBooks, numberOfActs, numberOfChapters, numberOfScenes;
    QStringList *titlesList;
    QStringList *leftHeadersList;
    QList<int> *numberList;
    QHash<MainTextDocument *, int> mtoO_numForDoc;
    QHash<MainTextDocument *, int> mtoO_numForClonedDoc;

    QList<MainTreeItem *> *treeBookItemList, *treeActItemList,
    *treeChapterItemList, *treeTrashItemList;

    int m_currentSheet;
};

#endif // MAINTREEABSTRACTMODEL_H
