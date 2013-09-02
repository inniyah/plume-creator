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
#ifndef MENUBAR_H
#define MENUBAR_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QWidget>
#include <QFrame>

#include "hub.h"
#include "prjmanager.h"
#include "settingsdialog.h"
#include "editmenu.h"
#include "startcenter.h"
#include "prjmanager.h"
#include "newprojectwizard.h"
#include "settingsdialog.h"
#include "exporter/exporter.h"
#include "findreplace.h"
#include "mainTree/maintreeabstractmodel.h"

//
class MenuBar : public QFrame
{
    Q_OBJECT
public:
    explicit MenuBar(QWidget *parent = 0);
void createContent();

    void openStartCenter()
    {
        startCenter();
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

    void launchCheckUpdateSignal(QString mode);


    //repeater between Dialog Settings and MainWindow
    void setDisplayModeSignal(QString mode, bool isToolBarInStatusBar);
void changeAllDocsTextStylesSignal();
void resetFullscreenTextWidthSignal();

    // repeater to join editWidget to MainWindow :


void widthChangedSignal(int width);
void textFontChangedSignal(QFont font);
void textHeightChangedSignal(int textHeight);
void setStyleSelectionSignal(int);

void charFormatChangedSlotSignal(QTextCharFormat charFmt);
 void tabWidgetChangedSlotSignal(int newTabWidth);
void tabWitdhChangedSlotSignal(int);
void styleSelectedSignal(int styleIndex);


//docks actions :
void showTreeDockSignal(bool treeBool);
void showNotesDockSignal(bool notesBool);
void showAttendDockSignal(bool attendBool);
void showToolsDockSignal(bool toolsBool);
void launchOutlinerSignal();
void showFullscreenSignal();

public slots:


    void openNewProjectSlot();
QMenuBar *createMenuBar();


////repeater for editWidget :
//void tabChangedSlot(QTextCharFormat newTabFormat){editWidget->tabChangedSlot(newTabFormat);}


void applyConfig();
void setTextStyles(TextStyles *styles){textStyles = styles;}
void setHub(Hub *varHub){hub = varHub;}


//docks actions :
void setTreeDockAct(bool treeVisible){showTreeDockAct->setChecked(treeVisible);}
void setNoteDockAct(bool noteVisible){showNotesDockAct->setChecked(noteVisible);}
void setToolDockAct(bool toolVisible){showToolsDockAct->setChecked(toolVisible);}
void setAttendDockAct(bool attendVisible){showAttendDockAct->setChecked(attendVisible);}

void setMenusEnabled(bool enabledBool);

void setMainTreeAbstractModel(MainTreeAbstractModel *tree){absTreeModel = tree;}


private slots:

    void newProject();
    void startCenter();
    //   void open();
    void displayConfig(int tabIndex = 0);
    void exporter();
    void print();
    void closeProject();
    void findAndReplace();
    void manageStyles();
    void aboutQt();
    void about();
    void viewReleaseNotes();
    void checkUpdate(){ launchCheckUpdateDialog("none"); }
    void exit();

    void createEditWidget();


private:
    Hub *hub;
    MainTreeAbstractModel  *absTreeModel;

    QWidget *parentWidget;
QString currentVersion;
bool projectAlreadyOpened;

    QToolButton *newProjectButton,
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
    *startCenterAct,
    //   *openAct,
    *displayConfigAct,
    *closeProjectAct,
    *exitAct,
    *printAct,
    *exportAct,
    *findReplaceAct, *manageStylesAct,
    *aboutQtAct,
    *aboutAct,
    *viewReleaseNotesAct,
    *updaterAct,
   *showTreeDockAct, *showNotesDockAct, *showAttendDockAct, *showToolsDockAct, *launchOutlinerAct, *showFullscreenAct;

QMenu *projectGroup, *editGroup, *helpGroup, *viewGroup;

EditMenu *editWidget;
TextStyles *textStyles;

    void createActions();
    void createButtons();

    //settings
    void readSettings();
    void writeSettings();

    //style :
    void giveStyle();




};

#endif // MENUBAR_H
