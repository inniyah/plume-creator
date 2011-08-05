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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include "menubox.h"
#include "editmenubox.h"
#include "maintree.h"
#include "statsbox.h"
#include "itembox.h"
#include "notezone.h"
#include "texttab.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event);
    void resizeEvent(QResizeEvent* event);
signals:
    void closeSignal();

    void openTabTextSignal(QFile *textFile);
    void properSaveTabTextSignal(QFile *textFile);
    void closeTabTextSignal(QFile *textFile);
void tabWidgetWidth(int value);

public slots:
    void goFullscreenSlot(bool m_fullscreen);
    void openProjectSlot(QFile *projectFile);
    void closeProjectSlot();

    void textSlot(QFile *textFile = 0, QFile *noteFile = 0, QFile *synFile = 0, QString name = "nothing", int number = 0, QString action = "nothing");
    void secondTextSlot(int number = 0, QString action = "nothing");

private slots:
    void tabChangeSlot(int tabNum);
    void tabCloseRequest(int tabNum);
    void closeAllDocsSlot();
void tabRenamingSlot(QString newName, int tabNum);
void setProjectNumberSlot(int prjNumber);

private:

    void createMenuDock();
    void createTreeDock();
    void createNoteDock();
    void createToolDock();

    MenuBox *menu;
    EditMenuBox *editMenu;
    MainTree *mainTree;
    QStackedLayout *synLayout;
    QStackedLayout *noteLayout;
    StatsBox *stats;
    ItemBox *items;

    QFile *file;

    bool m_firstStart;

    QTabWidget *tabWidget;
    void setTextTabConnections();
    QList<QFile *> *textFileList;
    QList<QFile *> *noteFileList;
    QList<QFile *> *synFileList;
    QStringList *nameList;
    QList<TextTab *> *textWidgetList;
    QList<NoteZone *> *noteWidgetList;
    QList<NoteZone *> *synWidgetList;
int objectNum;

    QList<int> *tabNumList;
    QList<int> *numList;

    //settings
    void readSettings();
    void writeSettings();
int settingNumber;


};

#endif // MAINWINDOW_H
