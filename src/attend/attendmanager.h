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
#ifndef ATTENDMANAGER_H
#define ATTENDMANAGER_H

#include <QDialog>
#include "attendabstractmodel.h"
#include "attendmanagertreeproxymodel.h"
#include "hub.h"
#include "attendsettings.h"

namespace Ui {
class AttendManager;
}

class AttendManager : public QDialog
{
    Q_OBJECT
    
public:
    explicit AttendManager(QWidget *parent, AttendAbstractModel *absModel);
    ~AttendManager();
    void postConstructor();
    void openDetailsOf(QModelIndex object);

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void applySettings();

private slots:
    void setItemActivated(QModelIndex index);
    void launchAttendSettings();
    void attendSettingsAccepted();
    void saveToElement();
    void saveText();
    void textAlreadyChangedSlot(bool textChanged){textAlreadyChanged = textChanged;}
    void editItemTitle(QModelIndex index);
    void setNameSlot(QString value);
    void resetDomElementForNumber();

    void on_addObjectAction_triggered();

    void on_addGroupAction_triggered();

    void on_removeAction_triggered();

    void expandAll();
    void nameEditingFinished();

private:
    void connectAll();
    void disconnectAll();
    void saveToAttendTreeItem();

    Ui::AttendManager *ui;
    Hub *hub;
    AttendManagerTreeProxyModel *managerProxyModel;
    AttendAbstractModel *abstractModel;
    QHash<int, QDomElement> domElementForNumber;
    QDomElement openedElement;
    bool textAlreadyChanged;
    bool oneClickCheckpoint, twoClicksCheckpoint, threeClicksCheckpoint;
    QModelIndex oldIndex;

    AttendTreeItem *attendTreeItemActivated;
};

#endif // ATTENDMANAGER_H
