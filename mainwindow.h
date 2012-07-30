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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include "menubar.h"
#include "maintree.h"
#include "statsbox.h"
#include "itembox.h"
#include "notezone.h"
#include "texttab.h"
#include "fullscreeneditor.h"
#include "outline.h"
#include "attendbox.h"
#include "orientationbutton.h"

//
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event);
    void resizeEvent(QResizeEvent* event);
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void closeSignal();

    void openTabTextSignal(QFile *textFile);
    void properSaveTabTextSignal(QFile *textFile);
    void closeTabTextSignal(QFile *textFile);
    void tabWidgetWidth(int value);
    void applyConfigSignal();

    void currentNumber(int);

public slots:
    void openProjectSlot(QFile *projectFile);
    void openExternalProject(QFile *externalFile);
    void closeProjectSlot();

    void textSlot(QTextDocument *textDoc = 0, QTextDocument *noteDoc = 0, QTextDocument *synDoc = 0, int cursorPosition = 0, int synCursorPosition = 0, int noteCursorPosition = 0, QString name = "nothing", int number = 0, QString action = "save");
    void secondTextSlot(int number = 0, QString action = "nothing");

private slots:
    void tabChangeSlot(int tabNum);
    void tabCloseRequest(int tabNum);
    void closeAllDocsSlot();
    void saveAllDocsSlot();
    void tabRenamingSlot(QString newName, int tabNum);
    void setProjectNumberSlot(int prjNumber);
    void editFullscreen();
    void launchOutliner();


    void setShowPreviousTextButton(bool showPrevTextBool);

    //config :
    void applyConfig();
    void configTimer();
    void setDisplayMode(QString mode);
    void changeOrientationOfNoteDock(Qt::DockWidgetArea noteDockArea);

    void reconnectAFterTabClose();
    void showPrevText(bool showPrevTextBool);

    void checkHiddenDocks();
// for netbook mode :
    void attendDockHidesOthers(bool attendIsVisible){if(attendIsVisible) noteDock->hide();}
    void noteDockHidesOthers(bool noteIsVisible){if(noteIsVisible) attendDock->hide();}


    //attendance :
    void setCurrentAttendList(int tabNum);

    int setCurrentNumber();

    //wordcount :
    void updateSceneWC(int count){QString sceneWCNum; sceneWCLabel->setText(tr("S : ") + sceneWCNum.setNum(count));}

private:

    void createMenuBar();
    void createMenuDock();
    void createTreeDock();
    void createNoteDock();
    void createToolDock();
    void createAttendDock();
    void createDocksToolBar();
    void createStatusBar();

    QWidget *widgetToHideWith;
    MenuBar *menu;
    QToolBox *toolBox;
    //    EditMenuBox *editMenu;
    MainTree *mainTree;
    QDockWidget *treeDock;
    QDockWidget *noteDock;
    QDockWidget *toolDock;
    QDockWidget *attendDock;
    QStackedLayout *synLayout;
    QStackedLayout *noteLayout;
    StatsBox *stats;
    ItemBox *items;
    AttendBox *attendList;
    QBoxLayout *noteDockLayout;

    QStatusBar *bar;
    QToolBar *docksToolBar;
    OrientationButton *menuDockButton;
    OrientationButton *toolDockButton;
    OrientationButton *treeDockButton;
    OrientationButton *noteDockButton;
    OrientationButton *attendDockButton;
    OrientationButton *editDockButton;

    QLabel *sceneWCLabel;

    QFile *file;

    bool m_firstStart;
    bool m_firstStart_checkDisplay;
    bool checkUpdateAtStartupBool;
    bool checkScreenResAtStartupBool;

    QTabWidget *tabWidget;
    void setConnections();
    QList<QTextDocument *> *textDocList;
    QList<QTextDocument *> *noteDocList;
    QList<QTextDocument *> *synDocList;
    QStringList *nameList;
    QList<TextTab *> *textWidgetList;
    QList<NoteZone *> *noteWidgetList;
    QList<NoteZone *> *synWidgetList;
    int objectNum;

    QList<int> *tabNumList;
    QList<int> *numList;

QToolButton *showPrevSceneButton;

    //settings
    void readSettings();
    void writeSettings();
    int settingNumber;
    bool menuBarOnTop;
    void autosaveTimer();
    bool NoProjectOpened;
    int autosaveTime;
    QTimer *timer;
    QString displayMode;

    void setEditMenuConnections();

    FullscreenEditor *fullEditor;
};

#endif // MAINWINDOW_H
