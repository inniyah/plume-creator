/***************************************************************************
 *   Copyright (C) 2011 by Cyril Jacquet                                   *
 *   terreville@google.com                                                 *
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
#include <QtGui>
#include <QDomDocument>

class MainTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit MainTree(QWidget *parent = 0);

    bool read(QFile *device);
    bool write(QFile *device);
    void closeTree();

protected:
    void contextMenuEvent(QContextMenuEvent *event);
//    void keyPressEvent(QKeyEvent *event);

    //drag drop :
void dropEvent(QDropEvent* event);
void mousePressEvent(QMouseEvent* event);
void dragEnterEvent(QDragEnterEvent *event);

signals:
    void textAndNoteSignal(QFile *textFile,QFile *noteFile,QFile *synFile, QString name, int number, QString action);
    void textAndNoteSignal(int number, QString action);
    void nameChangedSignal(QString newName, int number);

public slots:


private slots:
    void updateDomElement(QTreeWidgetItem *item, int column);

    bool openTextFile(QTreeWidgetItem *treeItem,int column);
    void itemEnteredSlot(QTreeWidgetItem *treeItemPressed,int column);
    void itemActivatedSlot(QTreeWidgetItem *treeItemPressed,int column);

void itemCollapsedSlot(QTreeWidgetItem* item);
void itemExpandedSlot(QTreeWidgetItem* item);

    void prepareContextMenu();
    void rename(QTreeWidgetItem *item);
    QTreeWidgetItem * addItemNext(QTreeWidgetItem * item = 0);
    QTreeWidgetItem * addChild(QTreeWidgetItem * item = 0);
    void moveUp();
    void moveDown();
    void delYesItem();
    void autoRenameChilds();
    void split();
    void addMulti();

    void buildTree();

private:
    void parseFolderElement(const QDomElement &element,
                            QTreeWidgetItem *parentItem = 0);
    QTreeWidgetItem *createItem(const QDomElement &element,
                                QTreeWidgetItem *parentItem = 0);
    QDomElement modifyAttributes(QDomElement originalElement,QDomElement newElement, QString level);

    QDomDocument domDocument;
    QDomElement root;
    QHash<QTreeWidgetItem *, QDomElement> domElementForItem;
    QHash<QTreeWidgetItem *, QDomElement>::iterator h;
    QIcon folderIcon;
    QIcon sceneIcon;

QList<int> freeNumList;

    QTreeWidgetItem *m_itemEntered,
    *m_preItemEntered;

    QAction *renameAct,
    *addItemNextAct,
    *addChildAct,
    *autoRenameChildsAct,
    *delYesAct,
    *moveUpAct,
    *moveDownAct,
    *splitAct,
    *addMultiAct;
    QMenu *delItemMenu,
    *advancedMenu;

    QString devicePath;
    QFile *deviceFile;
    QFile *textFile;
    QFile *noteFile;
    QFile *synFile;
QString name;

    bool prjIsJustOpened;
    bool treeOpened;

    QDomElement lastSiblingElement;
QString level_;
QDomElement siblingElement_;
QDomElement preElement;


QTreeWidgetItem *itemOfWork;

int number;


//drag drop :
int getQTreeWidgetItemDepth(QTreeWidgetItem*item);
int mParent;
QTreeWidgetItem *draggedItem, *targetItem;
};

#endif // MAINTREE_H
