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
#ifndef MENUBAR_H
#define MENUBAR_H

#include <QtGui>
#include <QWidget>
#include <QFrame>

#include "prjmanager.h"
#include "settingsdialog.h"
#include "editmenu.h"

//
class MenuBar : public QFrame
{
    Q_OBJECT
public:
    explicit MenuBar(QWidget *parent = 0);

    void openManager()
    {
        projectManager();
    }

    void firstLaunch()
    {
        readSettings();
    }
    void launchCheckUpdateDialog(QString mode = "none");

protected:


signals:

    void exitSignal();

    void openProjectSignal(QFile *device);
    void closeProjectSignal();
    void saveProjectSignal();

    void openProjectNumberSignal(int prjNumber);

    void applyConfigSignal();

    //repeater between Dialog Settings and MainWindow
    void setDisplayModeSignal(QString mode);


    // repeater to join editWidget to MainWindow :


void widthChangedSignal(int width);
void textFontChangedSignal(QFont font);
void textHeightChangedSignal(int textHeight);

void charFormatChangedSlotSignal(QTextCharFormat charFmt);
 void tabWidgetChangedSlotSignal(int newTabWidth);
  void showPrevTextSignal(bool prevText);
void tabWitdhChangedSlotSignal(int);

public slots:


    void openProjectManagerSlot();
    void openNewProjectSlot();
    void openProjectSlot(QFile *device);
void setExternalProject(QFile *externalFile);
bool openExternalProject(QFile *externalPrjFile);
QMenuBar *createMenuBar();


//repeater for editWidget :
void tabChangedSlot(QTextCharFormat newTabFormat){editWidget->tabChangedSlot(newTabFormat);}
void setShowPreviousTextButton(bool showPrevTextBool){editWidget->setShowPreviousTextButton(showPrevTextBool);}


void applyConfig();

private slots:

    void newProject();
    void projectManager();
    //   void open();
    void displayConfig();
    void exporter();
    void print();
    void closeProject();
    void findAndReplace();
    void aboutQt();
    void about();
    void checkUpdate(){ launchCheckUpdateDialog("none"); }
    void exit();

    void createEditWidget();


private:

QString currentVersion;

    QToolButton *newProjectButton,
    *projectManagerButton,
    //   *openButton,
    *displayConfigButton,
    *closeProjectButton,
    *exitButton,
    *printButton,
    *exportButton,
    *findReplaceButton,
    *aboutQtButton,
    *aboutButton,
    *updaterButton;

    QAction *newProjectAct,
    *projectManagerAct,
    //   *openAct,
    *displayConfigAct,
    *closeProjectAct,
    *exitAct,
    *printAct,
    *exportAct,
    *findReplaceAct,
    *aboutQtAct,
    *aboutAct,
    *updaterAct;
QMenu *projectGroup;
QMenu *editGroup;
QMenu *helpGroup;

EditMenu *editWidget;


    void createActions();
    void createButtons();

    //settings
    void readSettings();
    void writeSettings();

    //style :
    void giveStyle();



    PrjManager *projManager;
    QFile *file;

    QFile *extFile;
};

#endif // MENUBAR_H
