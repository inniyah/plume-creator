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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   

#include "hub.h"
#include "menubar.h"
#include "maintree.h"
#include "statsbox.h"
#include "itembox.h"
#include "notezone.h"
#include "texttab.h"
#include "fullscreen/fullscreeneditor.h"
#include "attend/attendbase.h"
#include "orientationbutton.h"
#include "slimupdater.h"
#include "wordgoalprogressbar.h"
#include "workbench.h"
#include "dockedTree/dockedtree.h"
#include "dockedTree/dockedtreeproxy.h"
#include "dockedTree/dockedtrashtree.h"
#include "dockedTree/dockedtrashtreeproxy.h"
//
namespace Ui {
class MainWindow;
class DockedTreeBase;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void postConstructor();

protected:
    void closeEvent(QCloseEvent* event);
    void resizeEvent(QResizeEvent* event);
    bool eventFilter(QObject *obj, QEvent *event);
    void paintEvent(QPaintEvent *);

signals:
    void closeSignal();
    void closeAllChildrenWindowsSignal();

    void openTabTextSignal(QFile *textFile);
    void properSaveTabTextSignal(QFile *textFile);
    void closeTabTextSignal(QFile *textFile);
    void tabWidgetWidth(int value);
    void applyConfigSignal();
    void changeAllDocsTextStylesSignal();
    void applyStyleSheetSignal();

    void currentNumber(int);


    // outliner :
    void applySynNoteFontConfigSignal();
    void updateOutlinerSignal();


    void currentOpenedSheetSignal(int sheetIdNumber);

public slots:


    void openProjectSlot();
    void openExternalProject(QString externalFile);
    void closeProjectSlot();

    void textSlot(int number = 0, QString action = "save");

private slots:
    void tabChangeSlot(int tabNum);
    void tabCloseRequest(int tabNum);
    void closeAllDocsSlot();
    void saveAllDocsSlot();
    void tabRenamingSlot(QString newName, int tabNum);
    void setProjectNumberSlot(int prjNumber);
    void editFullscreen();
    void launchWorkbench();

    void textChangedSlot();
    void textAlreadyChangedSlot(bool textChanged){textAlreadyChanged = textChanged;}

    void showPrevAndNextTexts(bool showTextsBool = true);
    void openSheet(int sheetNumber = 0, int textCursorPos = 0, int noteCursorPos = 0, int synCursorPos = 0);


    //config :
    void readDocksSettings();
    void writeDocksSettings();
    void applyConfig();
    void configTimer();
    void setDisplayMode(QString mode, bool isToolBarInStatusBar);
    void changeOrientationOfNoteDock(Qt::DockWidgetArea noteDockArea);

    void reconnectAFterTabClose();
    void showPrevText(bool showPrevTextBool);
    void showNextText(bool showNextTextBool);

    void checkHiddenDocks();
    // for netbook mode :
    void attendDockHidesOthers(bool attendIsVisible){if(attendIsVisible) noteDock->hide();}
    void noteDockHidesOthers(bool noteIsVisible){if(noteIsVisible) attendDock->hide();}


    //attendance :
    void setCurrentAttendList(int tabNum);

    int setCurrentNumber();

    //wordcount :
    //    void updateSceneWC(int count){QString sceneWCNum; sceneWCLabel->setText(tr("Words : ") + sceneWCNum.setNum(count));}

    void updateProjectWCLabel(int count);
    void updateBookWCLabel(int count);
    void updateActWCLabel(int count);
    void updateChapterWCLabel(int count);
    void updateCurrentSheetWCLabel(int count);



    //updater :
    void launchSlimUpdater(QString mode = "auto");
    void closeSlimUpdater();

    void giveStyle();


    void killWorkbench();


    // spell check :
    void activateSpellCheck(bool isActivated);

private:
    Ui::DockedTreeBase *ui_dockedTreeBase;
    Ui::MainWindow *ui;

    TextStyles *textStyles;
    Hub *hub;

    void createMenuBar();
    void createMenuDock();
    void createTreeDock();
    void createNoteDock();
    void createToolDock();
    void createAttendDock();
    void createDocksToolBar();
    void createStatusBar();
    void setDockSizes();

    MenuBar *menu;
    QToolBox *toolBox;
    //    EditMenuBox *editMenu;
    MainTree *mainTree;
    DockedTree *dockedTree;
    DockedTrashTree *dockedTrashTree;
    QDockWidget *treeDock;
    QDockWidget *noteDock;
    QDockWidget *toolDock;
    QDockWidget *attendDock;
    QStackedLayout *synLayout;
    QStackedLayout *noteLayout;
    StatsBox *stats;
    ItemBox *items;
    AttendBase *attendBase;
    QBoxLayout *noteDockLayout;
    QSplitter *noteSplitter;


    QToolBar *docksToolBar;
    OrientationButton *menuDockButton, *toolDockButton, *treeDockButton, *noteDockButton, *attendDockButton, *editDockButton;
    QAction *menuDockAct, *toolDockAct, *treeDockAct, *noteDockAct, *attendDockAct, *editDockAct,
    *fullscreenAct, *workbenchAct;
    QList<QAction *> toolBarActionsList;
    QToolBar *statusDockToolBar;

    QLabel *sceneWCLabel;


    bool m_firstStart;
    bool m_firstStart_checkDisplay;
    bool checkUpdateAtStartupBool;
    bool checkScreenResAtStartupBool;

    void setConnections();
    QList<MainTextDocument *> *textDocList;
    QList<MainTextDocument *> *noteDocList;
    QList<MainTextDocument *> *synDocList;
    QStringList *nameList;
    QList<TextTab *> *textWidgetList;
    QList<NoteZone *> *noteWidgetList;
    QList<NoteZone *> *synWidgetList;


    QList<int> *tabNumList;
    QList<int> *numList;

    QToolButton *showPrevSceneButton, *showNextSceneButton;
    OrientationButton *showPrvScnButton, *showNxtScnButton;
    //settings
    void readSettings();
    void writeSettings();
    int settingNumber;
    bool menuBarOnTop;
    void autosaveTimer();
    int autosaveTime;
    QTimer *timer;
    QString displayMode;
    bool oneTabOnly;
    bool noTabBool;

    void setEditMenuConnections();

    FullscreenEditor *fullEditor;


QString noTabCss;

    bool textAlreadyChanged;
    bool isExternalProjectOpeningBool;

    WordGoalProgressBar *wordGoalBar;
    QSystemTrayIcon *systemTray;

    bool isProjectOpened;

    //wordcount :
    QLabel *projectWCLabel,*bookWCLabel,*actWCLabel,
    *chapterWCLabel, *currentWCLabel;


    Workbench *workbench;
    bool workbenchLaunched;

    QString css;

    QString numberSymbol;
};

#endif // MAINWINDOW_H
