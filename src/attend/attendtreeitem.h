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
#ifndef ATTENDTREEITEM_H
#define ATTENDTREEITEM_H

#include <QString>
#include <QVariant>

#include "hub.h"


class AttendTreeItem
{
public:
    AttendTreeItem(const QList<QVariant> &data, AttendTreeItem *parent = 0);
    ~AttendTreeItem();

    void appendChild(AttendTreeItem *child);

    AttendTreeItem *child(int row);
    QList<AttendTreeItem*> childrenItems();
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    QList<QVariant>* dataList();
    int row() const;
    AttendTreeItem *parent();
    int idNumber() const;
    void setIdNumber(int number);
    bool isGroup() const;
    void setIsGroup(bool groupBool);
    bool isFolded() const;
    void setIsFolded(bool foldedBool);


    //details :

    QString name() const;
    void setName(QString name);
    QString aliases() const;
    void setAliases(QString aliases);
    QString quickDetails() const;
    void setQuickDetails(QString details);
    QString box_1Value() const;
    void setBox_1Value(QString value);
    QString box_2Value() const;
    void setBox_2Value(QString value);
    QString box_3Value() const;
    void setBox_3Value(QString value);
    QString spinBox_1_label() const;
    void setSpinbox_1_label(QString text);
    int spinBox_1Value() const;
    void setSpinbox_1Value(int value);

public slots:
    void setHub(Hub *varHub){hub = varHub;}

private:
    Hub *hub;
    QList<AttendTreeItem*> childItems;
    QList<QVariant> itemData;
    AttendTreeItem *parentItem;
    int itemId;
    bool m_isGroupBool;
    bool m_isFoldedBool;
    QString m_name, m_aliases,m_quickDetails,
    m_box_1Value, m_box_2Value, m_box_3Value, m_spinBox_1_label;
    int m_spinBox_1Value;


};

#endif // ATTENDTREEITEM_H
