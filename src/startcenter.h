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
#ifndef STARTCENTER_H
#define STARTCENTER_H

#include <QDialog>
#include "hub.h"

namespace Ui {
class StartCenter;
}

class StartCenter : public QDialog
{
    Q_OBJECT
    
public:
    explicit StartCenter(QWidget *parent = 0);
    ~StartCenter();
    void postConstructor();

signals:
    void newPrjSignal();

public slots:
    void setHub(Hub *varHub){hub = varHub;}

private slots:
    void on_newButton_clicked();

    void on_openButton_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::StartCenter *ui;
    Hub *hub;

    QString defaultOpenPath;

};

#endif // STARTCENTER_H
