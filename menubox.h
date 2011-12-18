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
#ifndef MENUBOX_H
#define MENUBOX_H

#include <QtGui>
#include <QWidget>
#include <QFrame>

#include "prjmanager.h"
#include "settingsdialog.h"
//
class MenuBox : public QFrame
{
    Q_OBJECT
public:
    explicit MenuBox(QWidget *parent = 0);

    void openManager()
    {
        projectManager();
    }

    void firstLaunch()
    {
        readSettings();
    }

protected:


signals:

    void exitSignal();

    void openProjectSignal(QFile *device);
    void closeProjectSignal();
    void saveProjectSignal();

    void openProjectNumberSignal(int prjNumber);

    void applyConfigSignal();

public slots:


    void openProjectManagerSlot();
    void openNewProjectSlot();
    void openProjectSlot(QFile *device);

private slots:

    void newProject();
    void projectManager();
    //   void open();
    void displayConfig();
    void exporter();
    void print();
    void closeProject();
    void aboutQt();
    void about();
    void exit();

    void applyConfig();

private:



    QToolButton *newProjectButton,
    *projectManagerButton,
    //   *openButton,
    *displayConfigButton,
    *closeProjectButton,
    *exitButton,
    *printButton,
    *exportButton,
    *aboutQtButton,
    *aboutButton;






    void createActions();
    void createButtons();

    //settings
    void readSettings();
    void writeSettings();

    //style :
    void giveStyle();



    PrjManager *projManager;
    QFile *file;
};

#endif // MENUBOX_H
