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

#ifndef ATTENDBOX_H
#define ATTENDBOX_H

#include <QtGui>
#include <QWidget>
#include <QDomDocument>

#include "notezone.h"
//
class AttendBox : public QFrame
{
    Q_OBJECT
public:
    explicit AttendBox(QWidget *parent = 0);
    bool readProjectAttendance(QFile *device);

signals:
    void projectAttendanceList(QHash<QListWidgetItem *, QDomElement> domElementForItem_, QHash<int, QDomElement> domElementForItemNumber_);
    void removeAttendNumberSignal(int itemNumber);
    void addAttendNumberToSheetSignal(QList<int> list, int sheetNumber);
    void removeAttendNumberFromSheetSignal(QList<int> list, int sheetNumber);

public slots:
    void openSheetAttendList(int number ,QString attendString);
    bool saveAll();
    bool saveThisDoc();
    bool saveDomDocument();
    bool closeAll();
    void setCurrentList(int number);
    void setCurrentListName(QString currentTabName);
    void setManagerSheetList(int number);
    void updateAllAttendances(QHash<int,QString> allAttendancesForNumber);
    void accept();

private slots:
    void buildList();
    QListWidgetItem *createItem(const QDomElement &element,
                                QListWidgetItem *item);

    QList<QListWidgetItem *> *sortItems(QList<int> *attend, QString sorting = "nothing");
    bool showSheetAttendanceList(QList<QListWidgetItem *> *itemList);


    QListWidgetItem *separator(QString separatorName);
    void addAttendManagerButton();
    void itemActivatedSlot(QListWidgetItem* itemActivated);
    void projectItemActivated(QListWidgetItem* itemActivated);
    void launchAttendManager();
    void setProjectList();

    void showDetailAnimation();
    void hideDetailAnimation();
    void showDetails();
    void hideDetails();
    void openDetail(QListWidgetItem* item);

    void readSettings();
    void writeSettings();

    void centerWindow(){    if(attendManager->x() < 0)
            attendManager->move(0, attendManager->y());}

    void attendManagerDestroyed();

    void saveAndUpdate();
    void firstnameChanged();
    void lastnameChanged();
    void nameChanged();
    void levelChanged();
    void roleChanged();

    void newCharSlot();
    void newItemSlot();
    void newPlaceSlot();
    void newAttendElementSlot(QString tagName);
    void deleteItems();

    void toSheetSlot();
    void toAllSlot();
    void managerSheetListSelectionChanged();
    void projectListSelectionChanged();

private:
    QListWidget *attendList, *abstractList;
    QDomDocument domDocument;
    QDomElement root;
    QListWidgetItem *managerLauncher;
    QFile *attFile;
    QString devicePath;

    QHash<QTextDocument *, QFile *> fileForDoc;
    QHash<QTextDocument *, QFile *>::iterator u;

    int currentSheetNumber;
    QListWidget *projectList, *dockProjectList;
    QGroupBox *managerSheetListBox;
    QListWidget *managerSheetList;
    QGridLayout *detailLayout;
    QPushButton *showDetailButton, *hideDetailButton, *newCharButton, *newItemButton, *newPlaceButton, *deleteButton
    ,*toSheetButton, *toAllButton;
    QComboBox *levelComboBox, *roleComboBox;

    bool detailsHiddenBool;
    bool firstDetailOpening;
    QString currentTabName;

    NoteZone *editZone;

    QLineEdit *firstnameEdit, *lastnameEdit, *nameEdit;

    QToolButton *fontsMenuButton;
    QFontComboBox *detailFontCombo;
    QSpinBox *detailTextHeightSpin, *detailTextIndentSpin, *detailTextMarginSpin;

    QDomElement currentElement;
    QHash<QListWidgetItem *, QDomElement> domElementForItem;
    QHash<int, QDomElement> domElementForItemNumber;
    QHash<int, QString> attendStringForNumber;

    QWidget* attendManager;
    bool attendManagerLaunched;
    bool deletingItemBool;
bool newAttendElementBool;

    int currentManagerSheetList;

    QString newAttendName;

    QList<QListWidgetItem *> projectItemList;
    QList<QListWidgetItem *> managerSheetItemList;
};

#endif // ATTENDBOX_H
