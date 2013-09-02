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
#ifndef FINDREPLACE_H
#define FINDREPLACE_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QDialog>
#include <QDomDocument>

#include "hub.h"


namespace Ui {
class FindReplace;
}

class FindReplace : public QDialog
{
    Q_OBJECT
    
public:
    explicit FindReplace(QWidget *parent = 0);
    void postConstructor();
    ~FindReplace();
    
signals:

public slots:
    void setHub(Hub *varHub){hub = varHub;}

private slots:
    //tree :
    void createTree();




private:
    Hub *hub;
    Ui::FindReplace *ui;


    QDomDocument domDocument;
    QDomElement root;

};

#endif // FINDREPLACE_H
