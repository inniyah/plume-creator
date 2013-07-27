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
#ifndef MAINTREE_H
#define MAINTREE_H

#include <QWidget>
#include <QtXml>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QDomDocument>

#include <outliner/outlinerbase.h>
#include "textstyles.h"
#include "hub.h"
//
class MainTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit MainTree(QWidget *parent = 0);

    bool startTree();
    void closeTree();

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    //    void keyPressEvent(QKeyEvent *event);

    //drag drop :
    void dropEvent(QDropEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void dragEnterEvent(QDragEnterEvent *event);


signals:
    void textAndNoteSignal(int number, QString action);
    void nameChangedSignal(QString newName, int number);

    // for Outliner :
    void disconnectUpdateTextsSignal();
    void connectUpdateTextsSignal();
    void mainFocusOutSignal();

    void showOutlinerSignal();
    void saveOutliner();
    void resetDomDocSignal();

    void setNumForDocSignal(QHash<MainTextDocument *, int>);
    void applySynNoteFontConfigSignal();

    // for attendance :

    void attendStringSignal(int number, QString attendString);
    void allAttendancesSignal(QHash<int, QString> attendStringForNumber);
    void projectAttendanceList(QHash<QListWidgetItem *, QDomElement> domElementForItem_, QHash<int, QDomElement> domElementForItemNumber_);

    // for global wordcount :
    void docsForProjectWordCountSignal(QHash<MainTextDocument *, QFile *> fileForDocs);
    void domForProjectWordCountSignal(QDomDocument domDoc);


public slots:
    void setHub(Hub *varHub){hub = varHub;}

    void saveCursorPos(int cursorPosition, int synCursorPosition, int noteCursorPosition, int number);
//    bool saveDoc(MainTextDocument *doc);
    MainTextDocument * prevText(int num);
    MainTextDocument * nextText(int num);

    // Outline :
    void launchOutliner();
    void updateOutliner();

    // for attendance :
    void readAllAttendances();
    void setOutlinerProjectAttendList(QHash<QListWidgetItem *, QDomElement> domElementForItem_, QHash<int, QDomElement> domElementForItemNumber_);
    void removeAttendNumberSlot(int itemNumber);
    void addAttendNumberToSheetSlot(QList<int> list, int sheetNumber);
    void removeAttendNumberFromSheetSlot(QList<int> list, int sheetNumber);

    // for global wordcount :
    void giveDocsAndDomForProjectWordCount();

    // for text styles :
    void changeAllDocsTextStyles();
    void setTextStyles(TextStyles *styles){textStyles = styles;}

   // for "new" button in fullscreen :
    int addItemNext(int baseNumber);

private slots:
    void updateDomElement(QTreeWidgetItem *item, int column);

    bool openTextFile(QTreeWidgetItem *treeItem,int column);
    void itemEnteredSlot(QTreeWidgetItem *treeItemPressed,int column);
    void itemActivatedSlot(QTreeWidgetItem *treeItemPressed,int column);

    void itemCollapsedSlot(QTreeWidgetItem* item);
    void itemExpandedSlot(QTreeWidgetItem* item);

    void prepareContextMenu();
    void rename();
    void rename(QTreeWidgetItem *item);
    QTreeWidgetItem * addItemNext(QTreeWidgetItem * item = 0);
    QTreeWidgetItem * addChild(QTreeWidgetItem * item = 0);
    QTreeWidgetItem * addSeparator(QTreeWidgetItem * item = 0);
    void moveUp();
    void moveDown();
    void delYesItem();
    void removeItem(QDomElement element);
    void autoRenameChilds();
    void split();
    void splitChoiceChanged(int choice);
    void splitYes();
    void addMulti();

    void buildTree();

    //outliner :

    void otoM_actionSlot(QString action,int idNumber);
    void  updateMainDomDocFromOutliner();

    void buildOutliner();
    void  killOutliner();
    void newOutlineTitleSlot(QString newTitle,int number);
    void writeThisSlot(int number);
    void deletedSlot();
    void setOutlineViewPos();
    void saveOutlineSettings();
    void insertOutlinerItem(int newNumber, int numberOfRef);
    void outlinerClosed(){outlinerLaunched = false;}

    QHash<MainTextDocument *, int> setNumForDoc();


private:
    Hub *hub;
    void parseFolderElement(const QDomElement &element,
                            QTreeWidgetItem *parentItem = 0);
    QTreeWidgetItem *createItem(const QDomElement &element,
                                QTreeWidgetItem *parentItem = 0);
    QDomElement modifyAttributes(QDomElement originalElement,QDomElement newElement, QString level);

    QDomDocument domDocument;
    QHash<QTreeWidgetItem *, QDomElement> domElementForItem;
    QHash<QTreeWidgetItem *, QDomElement>::iterator h;
    QHash<int, QDomElement> domElementForNumber;
    QHash<int, QDomElement>::iterator t;
    QIcon folderIcon;
    QIcon sceneIcon;

    QList<int> freeNumList;

    QTreeWidgetItem *m_itemEntered,
    *m_preItemEntered;

    QAction *renameAct,
    *addItemNextAct,
    *addChildAct,
    *addSeparatorAct,
    *autoRenameChildsAct,
    *delYesAct,
    *moveUpAct,
    *moveDownAct,
    *splitAct,
    *addMultiAct;
    QMenu *delItemMenu,
    *advancedMenu;

    QString splitChoice;

    QString devicePath;
//    QFile *deviceFile;

    bool prjIsJustOpened;
    bool treeOpened;

    QDomElement lastSiblingElement;
    QString level_;
    QDomElement siblingElement_;
    QDomElement preElement;


    QTreeWidgetItem *itemOfWork;

    //    int number;


    //drag drop :
    int getQTreeWidgetItemDepth(QTreeWidgetItem*item);
    int mParent;
    QTreeWidgetItem *draggedItem, *targetItem;






//    QHash<MainTextDocument *, QFile *> fileForDoc;
//    QHash<MainTextDocument *, QFile *>::iterator u;


    //Outline :

//    Outline *outliner;
    int widgetTargetedNumber;
    QTreeWidgetItem *itemTargetedForOutliner;

    //NEW Outliner :

    OutlinerBase *outlinerBase;
    bool outlinerLaunched;




    //attendance :
    QHash<QListWidgetItem *, QDomElement> attend_domElementForItem;
    QHash<int, QDomElement> attend_domElementForItemNumber;


    //text styles :

TextStyles *textStyles;
};

#endif // MAINTREE_H
