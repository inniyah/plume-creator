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


#include <QObject>

#include "hub.h"
#include "mainTree/maintreeabstractmodel.h"
//
class MainTree : public QObject
{
    Q_OBJECT
public:
    explicit MainTree(QObject *parent = 0);

    bool startTree();
    MainTreeAbstractModel *mainTreeAbstractModel();
    void closeTree();

 protected:


signals:
    void textAndNoteSignal(int number, QString action);
    void nameChangedSignal(QString newName, int number);

public slots:
    void setHub(Hub *varHub){hub = varHub;}

private slots:



private:
    Hub *hub;
    MainTreeAbstractModel *mainTreeAbsModel;




};

#endif // MAINTREE_H
