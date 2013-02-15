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
#ifndef FINDREPLACE_H
#define FINDREPLACE_H

#include <QDialog>
#include <QtGui>
#include <QDomDocument>

namespace Ui {
class FindReplace;
}

class FindReplace : public QDialog
{
    Q_OBJECT
    
public:
    explicit FindReplace(QFile *device = 0, QWidget *parent = 0);
    ~FindReplace();
    
signals:

public slots:

private slots:
    //tree :
    void createTree(QFile *device);
    bool read(QFile *file);
    void closeTree();
    void buildTree();
    void itemClickedSlot(QTreeWidgetItem* item, int column);




private:
    Ui::FindReplace *ui;



    QFile *targetDevice;

    //tree :

    void parseFolderElement(const QDomElement &element,
                            QTreeWidgetItem *parentItem = 0);
    QTreeWidgetItem *createItem(const QDomElement &element,
                                QTreeWidgetItem *parentItem = 0);

    QDomDocument domDocument;
    QDomElement root;
    QHash<QTreeWidgetItem *, QDomElement> domElementForItem;
    QHash<QTreeWidgetItem *, QDomElement>::iterator h;
    QHash<int, QDomElement> domElementForNumber;
    QHash<int, QDomElement>::iterator t;
    QIcon folderIcon;
    QIcon sceneIcon;


    // accept :
    QTextDocument * buildFinalDoc();
//    QTextDocument* prepareTextDoc(QFile *textFile);
//    QTextDocument* prepareSynDoc(QFile *synFile);
//    QTextDocument* prepareNoteDoc(QFile *noteFile);

};

#endif // FINDREPLACE_H
