#include "menubar.h"
#include "prjmanager.h"
#include "newprojectwizard.h"
#include "settingsdialog.h"
#include "exporter.h"
#include "findreplace.h"
//
MenuBar::MenuBar(QWidget *parent) :
    QFrame(parent), currentVersion(QApplication::applicationVersion()), projectAlreadyOpened(false)
{

    giveStyle();




}
//---------------------------------------------------------------------------

void MenuBar::createContent()
{
    createActions(); // if buttons with menus
    createButtons();


    readSettings();
    setMenusEnabled(false);
}
//---------------------------------------------------------------------------

void MenuBar::newProject()
{
    NewProjectWizard projectWizard(this);
    projectWizard.exec();

    projectManager();

}

//---------------------------------------------------------------------------

//void MenuBar::open()
//{
//    QString fileName =
//            QFileDialog::getOpenFileName(this, tr("Open Project File"),
//                                         QDir::homePath(),
//                                         tr(".Plume Creator Files (*.plume)"));



//    if (fileName.isEmpty())
//        return;

//    closeProject();


//    //    tempDomFile = new QFile(fileName + ".temp");
//    //    if(tempDomFile->exists()){
//    //        if (!tempDomFile->open(QFile::ReadOnly | QFile::Text)) {
//    //            QMessageBox::warning(this, tr("Plume Creator File"),
//    //                                 tr("Cannot read file %1:\n%2.")
//    //                                 .arg(fileName)
//    //                                 .arg(tempDomFile->errorString()));
//    //            return;
//    //        }


//    //        QMessageBox::warning(this, tr("Plume Creator File"),
//    //                             tr("This project has been closed abruptly. Plume Creator will try to recuperate this project."));
//    //        connect(this, SIGNAL(openProjectSignal(QFile*)), treeDock, SLOT(openProjectSlot(QFile*)));
//    //        emit openProjectSignal(tempDomFile);

//    //        tempDomFile->close();

//    //    }
//    //    else{


//    file = new QFile(fileName);
//    if (!file->open(QFile::ReadOnly | QFile::Text)) {
//        QMessageBox::warning(this, tr("Plume Creator File"),
//                             tr("Cannot read file %1:\n%2.")
//                             .arg(fileName)
//                             .arg(file->errorString()));
//        return;
//    }
//    emit openProjectSignal(file);

//    file->close();

//    //    }
//}

//---------------------------------------------------------------------------

void MenuBar::projectManager()
{

    projManager = new PrjManager(this);
    projManager->setHub(hub);
    connect(projManager,SIGNAL(openPrjManagerSignal()), this, SLOT(openProjectManagerSlot()));
    connect(projManager,SIGNAL(newPrjSignal()), this, SLOT(openNewProjectSlot()));
    //    connect(projManager, SIGNAL(openProjectSignal(QFile*)), this, SLOT(openProjectSlot(QFile*)));
    connect(projManager, SIGNAL(openProjectNumberSignal(int)),this, SIGNAL(openProjectNumberSignal(int)));
    projManager->projectAlreadyOpened(projectAlreadyOpened);


    projManager->setModal(true);
    projManager->exec();


}

//---------------------------------------------------------------------------


void MenuBar::displayConfig(int tabIndex)
{
    SettingsDialog *settingsDialog = new SettingsDialog(this);
    settingsDialog->setHub(hub);
    settingsDialog->setTextStyles(textStyles);
    textStyles->saveBaseStyles();
    settingsDialog->createContent();

    connect(settingsDialog, SIGNAL(accepted()), this, SLOT(applyConfig()));
    connect(settingsDialog, SIGNAL(setDisplayModeSignal(QString)), this, SIGNAL(setDisplayModeSignal(QString)));
    connect(settingsDialog, SIGNAL(changeAllDocsTextStylesSignal()), this, SIGNAL(changeAllDocsTextStylesSignal()));
    connect(settingsDialog, SIGNAL(resetFullscreenTextWidthSignal()), this, SIGNAL(resetFullscreenTextWidthSignal()));


    settingsDialog->setCurrentTab(tabIndex);

    settingsDialog->exec();
    emit resetFullscreenTextWidthSignal();
    //    //    Config config;
    //    ConfigDialog dialog(/*config, */this);
    //    if (dialog.exec() == QDialog::Accepted) {
    //        //        loadPreferences(config);
    //    }
}


//--------------------------------------------------------------------------


void MenuBar::closeProject()
{
    //    if(projectAlreadyOpened == false)
    //        return;

    //    if(file == 0)
    //        return;

    QMessageBox msgBox(this);
    msgBox.setText(tr("Do you want to close the current project ?"));
    msgBox.setInformativeText(tr("Your changes are already saved."));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Ok:
        hub->closeCurrentProject();
        projectAlreadyOpened = false;
        break;

    case QMessageBox::Cancel:
        return;
        break;
    default:
        // should never be reached
        break;
    }





}

//--------------------------------------------------------------------------


void MenuBar::exporter()
{

    emit saveProjectSignal();

    Exporter *exporterDialog = new Exporter(QString("export"), this);
    exporterDialog->setHub(hub);
    exporterDialog->postConstructor();
    exporterDialog->exec();

}

//--------------------------------------------------------------------------


void MenuBar::print()
{
    if(!hub->isProjectOpened())
        return;

    emit saveProjectSignal();

    Exporter *exporterDialog = new Exporter(QString("print"), this);
    exporterDialog->setHub(hub);
    exporterDialog->exec();


}
//--------------------------------------------------------------------------



void MenuBar::findAndReplace()
{
    if(!hub->isProjectOpened())
        return;

    emit saveProjectSignal();

    FindReplace *findReplaceDialog = new FindReplace(this);
    findReplaceDialog->setHub(hub);
    findReplaceDialog->postConstructor();
    findReplaceDialog->exec();

}

//--------------------------------------------------------------------------

void MenuBar::manageStyles()
{
    this->displayConfig(2);


}

//--------------------------------------------------------------------------

void MenuBar::aboutQt()
{
    QMessageBox::aboutQt(this, tr("About Qt"));



}

//--------------------------------------------------------------------------

void MenuBar::viewReleaseNotes()
{


    QDialog *relNotesDialog = new QDialog(this);
    relNotesDialog->setWindowTitle(tr("Release Notes"));
    relNotesDialog->setMinimumSize(600,400);


    QVBoxLayout *layout = new QVBoxLayout;

    QLabel *relNotesText = new QLabel();



    QFile *readmeFile = new QFile(":/README");
    readmeFile->open(QFile::ReadOnly | QFile::Text);
    QTextStream textFileStream( readmeFile );

    QPlainTextEdit *textViewer = new QPlainTextEdit(textFileStream.readAll());
    readmeFile->close();
    textViewer->setReadOnly(true);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);

    connect(buttonBox, SIGNAL(accepted()), relNotesDialog, SLOT(close()));


    layout->addWidget(relNotesText);
    layout->addWidget(textViewer);
    layout->addWidget(buttonBox);

    relNotesDialog->setLayout(layout);

    relNotesDialog->exec();


}

//----------------------------------------------------------------------------

void MenuBar::about()
{
    QMessageBox::about(this, tr("About Plume Creator"),
                       "<p><center><b>Plume Creator</b></p>"
                       "<p><b>A Project Manager and Rich Text Editor for Writers.</b></p>"

                       "<p>Version " + currentVersion + "</p>"


                       "<p>Copyright (C) 2011 by Cyril Jacquet</p>"
                       "<p>terreville@gmail.com </p></center>"
                       "<br>"
                       "<p>Plume Creator is free software: you can redistribute it and/or modify "
                       "it under the terms of the GNU General Public License as published by "
                       "the Free Software Foundation, either version 3 of the License, or "
                       "(at your option) any later version.</p> "
                       "<br>"
                       "<p>Plume Creator is distributed in the hope that it will be useful, "
                       "but WITHOUT ANY WARRANTY; without even the implied warranty of "
                       "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
                       "GNU General Public License for more details.</p>"
                       "<br>"
                       "<p>You should have received a copy of the GNU General Public License "
                       "along with Plume Creator.  If not, see <address>http://www.gnu.org/licenses</address>.</p>"
                       );


}


//--------------------------------------------------------------------------


void MenuBar::launchCheckUpdateDialog(QString mode)
{
    emit launchCheckUpdateSignal(mode);

}
//--------------------------------------------------------------------------

void MenuBar::exit()
{

    writeSettings();
    emit exitSignal();


}





//---------------------------------------------------------------------------


QMenuBar *MenuBar::createMenuBar()
{

    QMenuBar *menubar = new QMenuBar();

    createActions();

    menubar->addMenu(projectGroup);
    menubar->addMenu(editGroup);
    menubar->addMenu(viewGroup);
    menubar->addMenu(helpGroup);

    return menubar;
}



//---------------------------------------------------------------------------


void MenuBar::createActions()
{
    newProjectAct = new QAction(QIcon(":/pics/tools-wizard.png"),tr("&New Project"), this);
    newProjectAct->setShortcut(QKeySequence::New);
    newProjectAct->setToolTip(tr("Create a new project"));
    connect(newProjectAct, SIGNAL(triggered()), this, SLOT(newProject()));

    projectManagerAct = new QAction(QIcon(":/pics/go-home.png"),tr("Project &Manager"), this);
    // projectManagerAct->setShortcut(QKeySequence::New);
    projectManagerAct->setToolTip(tr("Create and manage your projects"));
    connect(projectManagerAct, SIGNAL(triggered()), this, SLOT(projectManager()));

    displayConfigAct = new QAction(QIcon(":/pics/configure.png"),tr("&Configure"), this);
    //    displayConfigAct->setShortcut(QKeySequence::Print);
    displayConfigAct->setToolTip(tr("Display the configuration"));
    connect(displayConfigAct, SIGNAL(triggered()), this, SLOT(displayConfig()));

    exportAct = new QAction(QIcon(":/pics/document-export.png"),tr("&Export"), this);
    //   exportAct->setShortcut(QKeySequence::Print);
    exportAct->setToolTip(tr("Export the project"));
    connect(exportAct, SIGNAL(triggered()), this, SLOT(exporter()));

    printAct = new QAction(QIcon(":/pics/document-print.png"),tr("&Print"), this);
    printAct->setShortcut(QKeySequence::Print);
    printAct->setToolTip(tr("Print part of the project"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    closeProjectAct = new QAction(QIcon(":/pics/window-close.png"),tr("&Close project"), this);
    closeProjectAct->setShortcut(QKeySequence::Close);
    closeProjectAct->setToolTip(tr("Print the document"));
    connect(closeProjectAct, SIGNAL(triggered()), this, SLOT(closeProject()));;

    exitAct = new QAction(QIcon(":/pics/application-exit.png"),tr("E&xit"), this);
    exitAct->setShortcut(QKeySequence::Quit);
    exitAct->setToolTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(exit()));


    projectGroup = new QMenu(tr("&Project"),this);
    projectGroup->addAction(projectManagerAct);
    projectGroup->addSeparator();
    projectGroup->addAction(newProjectAct);
    projectGroup->addSeparator();
    projectGroup->addAction(displayConfigAct);
    projectGroup->addSeparator();
    projectGroup->addAction(exportAct);
    projectGroup->addAction(printAct);
    projectGroup->addSeparator();
    projectGroup->addAction(closeProjectAct);
    projectGroup->addAction(exitAct);

    findReplaceAct = new QAction(QIcon(":/pics/edit-find-replace.png"),tr("&Find && Replace"),this);
    // aboutAct->setShortcut(QKeySequence::Quit);
    findReplaceAct->setToolTip(tr("Find && Replace Dialog"));
    connect(findReplaceAct, SIGNAL(triggered()), this, SLOT(findAndReplace()));

    //    createEditWidget();
    //    QWidgetAction *editWidgetAct = new QWidgetAction(this);
    //    editWidgetAct->setDefaultWidget(editWidget);

    manageStylesAct = new QAction(/*QIcon(":/pics/edit-find-replace.png"),*/tr("Manage &Styles"),this);
    manageStylesAct->setShortcut(Qt::Key_F7);
    manageStylesAct->setToolTip(tr("Manage the styles"));
    connect(manageStylesAct, SIGNAL(triggered()), this, SLOT(manageStyles()));

    editGroup = new QMenu(tr("&Edit"),this);
    editGroup->addAction(findReplaceAct);
    editGroup->addSeparator();
    //    editGroup->addAction(editWidgetAct);
    editGroup->addAction(manageStylesAct);

    showTreeDockAct = new QAction(QIcon(":/pics/view-list-tree.png"),tr("&Project"),this);
    showTreeDockAct->setCheckable(true);
    showTreeDockAct->setShortcut(Qt::Key_F5);
    showTreeDockAct->setToolTip(tr("Show the project dock"));
    connect(showTreeDockAct, SIGNAL(toggled(bool)), this, SIGNAL(showTreeDockSignal(bool)));

    showNotesDockAct = new QAction(QIcon(":/pics/im-status-message-edit.png"),tr("&Notes"),this);
    showNotesDockAct->setCheckable(true);
    showNotesDockAct->setShortcut(Qt::Key_F2);
    showNotesDockAct->setToolTip(tr("Show the note dock"));
    connect(showNotesDockAct, SIGNAL(toggled(bool)), this, SIGNAL(showNotesDockSignal(bool)));

    showAttendDockAct = new QAction(QIcon(":/pics/meeting-organizer.png"),tr("&Attendance"),this);
    showAttendDockAct->setCheckable(true);
    showAttendDockAct->setShortcut(Qt::Key_F5);
    showAttendDockAct->setToolTip(tr("Show the attendance dock"));
    connect(showAttendDockAct, SIGNAL(toggled(bool)), this, SIGNAL(showAttendDockSignal(bool)));

    showToolsDockAct = new QAction(QIcon(":/pics/preferences-system.png"),tr("&Tools"),this);
    showToolsDockAct->setCheckable(true);
    showToolsDockAct->setShortcut(Qt::Key_F4);
    showToolsDockAct->setToolTip(tr("Show the tool dock"));
    connect(showToolsDockAct, SIGNAL(toggled(bool)), this, SIGNAL(showToolsDockSignal(bool)));

    launchOutlinerAct = new QAction(QIcon(":/pics/view-time-schedule.png"),tr("&Outliner"),this);
    launchOutlinerAct->setShortcut(Qt::Key_F6);
    launchOutlinerAct->setToolTip(tr("Show the outliner"));
    connect(launchOutlinerAct, SIGNAL(triggered()), this, SIGNAL(launchOutlinerSignal()));

    showFullscreenAct = new QAction(QIcon(":/pics/view-fullscreen.png"),tr("&Fullscreen"), this);
    showFullscreenAct->setShortcut(Qt::Key_F11);
    showFullscreenAct->setToolTip(tr("Edit fullscreen"));
    connect(showFullscreenAct, SIGNAL(triggered()), this, SIGNAL(showFullscreenSignal()));

    viewGroup = new QMenu(tr("&View"),this);
    //viewGroup->addAction(showTreeDockAct);
    viewGroup->addAction(showNotesDockAct);
    viewGroup->addAction(showAttendDockAct);
    viewGroup->addAction(showToolsDockAct);
    viewGroup->addSeparator();
    viewGroup->addAction(launchOutlinerAct);
    viewGroup->addAction(showFullscreenAct);



    aboutAct = new QAction(QIcon(":/pics/help-about.png"),tr("About"),this);
    // aboutAct->setShortcut(QKeySequence::Quit);
    aboutAct->setToolTip(tr("about the application"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About Qt"),this);
    // aboutQtAct->setShortcut(QKeySequence::Quit);
    aboutQtAct->setToolTip(tr("about Qt"));
    connect(aboutQtAct, SIGNAL(triggered()), this, SLOT(aboutQt()));

    viewReleaseNotesAct = new QAction(tr("Release notes"),this);
    // aboutQtAct->setShortcut(QKeySequence::Quit);
    viewReleaseNotesAct->setToolTip(tr("Open the Readme with the release notes"));
    connect(viewReleaseNotesAct, SIGNAL(triggered()), this, SLOT(viewReleaseNotes()));

    updaterAct = new QAction(QIcon(":/pics/download.png"),tr("Check Update"),this);
    // aboutQtAct->setShortcut(QKeySequence::Quit);
    updaterAct->setToolTip(tr("check for an update"));
    connect(updaterAct, SIGNAL(triggered()), this, SLOT(checkUpdate()));

    helpGroup = new QMenu(tr("&Help"),this);
    helpGroup->addAction(aboutAct);
    helpGroup->addAction(aboutQtAct);
    helpGroup->addSeparator();
    helpGroup->addAction(viewReleaseNotesAct);
    helpGroup->addAction(updaterAct);
}

//---------------------------------------------------------------------------



void MenuBar::createEditWidget()
{

    editWidget = new EditMenu;
    editWidget->setHub(hub);
    editWidget->setTextStyles(textStyles);
    editWidget->createContent();

    QStringList widgetToHideList;
    widgetToHideList << "zoomBox" << "textWidthBox";
    editWidget->hideWidgetsByName(widgetToHideList);

    //editWidget->setFrameStyle(QFrame::Panel);
    //editWidget->setLineWidth(2);
    //editWidget->setMidLineWidth(3);

    // repeater to join editWidget to MainWindow :

    connect(editWidget, SIGNAL(widthChangedSignal(int)), this, SIGNAL(widthChangedSignal(int)));

    connect(editWidget,SIGNAL(textFontChangedSignal(QFont)),this,SIGNAL(textFontChangedSignal(QFont)));
    connect(editWidget,SIGNAL(textHeightChangedSignal(int)),this,SIGNAL(textHeightChangedSignal(int)));

    connect(this,SIGNAL(charFormatChangedSlotSignal(QTextCharFormat)),editWidget,SLOT(charFormatChangedSlot(QTextCharFormat)));

    connect(editWidget,SIGNAL(styleSelectedSignal(int)), this, SIGNAL(styleSelectedSignal(int)));
    connect(this,SIGNAL(setStyleSelectionSignal(int)), editWidget, SLOT(setStyleSelectionSlot(int)));
}



//---------------------------------------------------------------------------

void MenuBar::setMenusEnabled(bool enabledBool)
{
    editGroup->setEnabled(enabledBool);
    displayConfigAct->setEnabled(enabledBool);
    printAct->setEnabled(enabledBool);
    exportAct->setEnabled(enabledBool);
}


//---------------------------------------------------------------------------
void MenuBar::createButtons()
{
    //    QSize buttonSize(120,60);


    //    QVBoxLayout *baseGridLayout = new QVBoxLayout;

    //    newProjectButton = new QToolButton(this);
    //    newProjectButton->setMaximumSize(buttonSize);
    //    newProjectButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //    newProjectButton->setText(tr("&New Project"));
    //    newProjectButton->setShortcut(QKeySequence::New);
    //    newProjectButton->setToolTip(tr("Create a new project"));
    //    connect(newProjectButton, SIGNAL(clicked()), this, SLOT(newProject()));

    //    projectManagerButton = new QToolButton(this);
    //    projectManagerButton->setMaximumSize(buttonSize);
    //    projectManagerButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //    projectManagerButton->setText(tr("Project &Manager"));
    //    // projectManagerAct->setShortcut(QKeySequence::New);
    //    projectManagerButton->setToolTip(tr("Create and manage your projects"));
    //    connect(projectManagerButton, SIGNAL(clicked()), this, SLOT(projectManager()));

    //    //    openButton = new QToolButton(this);
    //    //    openButton->setMaximumSize(buttonSize);
    //    //    openButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //    //    openButton->setText(tr("&Open..."));
    //    //    openButton->setShortcut(QKeySequence::Open);
    //    //    openButton->setToolTip(tr("Open an existing file"));
    //    //    connect(openButton, SIGNAL(clicked()), this, SLOT(open()));

    //    displayConfigButton = new QToolButton(this);
    //    displayConfigButton->setMaximumSize(buttonSize);
    //    displayConfigButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //    displayConfigButton->setText(tr("&Configure"));
    //    //    displayConfigButton->setShortcut(QKeySequence::Print);
    //    displayConfigButton->setToolTip(tr("Display the configuration"));
    //    connect(displayConfigButton, SIGNAL(clicked()), this, SLOT(displayConfig()));

    //    findReplaceButton = new QToolButton(this);
    //    findReplaceButton->setText(tr("&Find && Replace"));
    //    // aboutAct->setShortcut(QKeySequence::Quit);
    //    findReplaceButton->setToolTip(tr("Find & Replace Dialog"));
    //    connect(findReplaceButton, SIGNAL(clicked()), this, SLOT(findAndReplace()));


    //    exportButton = new QToolButton(this);
    //    exportButton->setMaximumSize(buttonSize);
    //    exportButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //    exportButton->setText(tr("&Export"));
    //    //   exportButton->setShortcut(QKeySequence::Print);
    //    exportButton->setToolTip(tr("Export the project"));
    //    connect(exportButton, SIGNAL(clicked()), this, SLOT(exporter()));

    //    printButton = new QToolButton(this);
    //    printButton->setMaximumSize(buttonSize);
    //    printButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //    printButton->setText(tr("&Print"));
    //    printButton->setShortcut(QKeySequence::Print);
    //    printButton->setToolTip(tr("Print part of the project"));
    //    connect(printButton, SIGNAL(clicked()), this, SLOT(print()));

    //    closeProjectButton = new QToolButton(this);
    //    closeProjectButton->setMaximumSize(buttonSize);
    //    closeProjectButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //    closeProjectButton->setText(tr("&Close project"));
    //    closeProjectButton->setShortcut(QKeySequence::Close);
    //    closeProjectButton->setToolTip(tr("Print the document"));
    //    connect(closeProjectButton, SIGNAL(clicked()), this, SLOT(closeProject()));;

    //    exitButton = new QToolButton(this);
    //    exitButton->setMaximumSize(buttonSize);
    //    exitButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //    exitButton->setText(tr("E&xit"));
    //    exitButton->setShortcut(QKeySequence::Quit);
    //    exitButton->setToolTip(tr("Exit the application"));
    //    connect(exitButton, SIGNAL(clicked()), this, SLOT(exit()));


    //    aboutButton = new QToolButton(this);
    //    aboutButton->setMaximumSize(buttonSize);
    //    aboutButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //    aboutButton->setText(tr("About"));
    //    // aboutButton->setShortcut(QKeySequence::Quit);
    //    aboutButton->setToolTip(tr("about the application"));
    //    connect(aboutButton, SIGNAL(clicked()), this, SLOT(about()));

    //    aboutQtButton = new QToolButton(this);
    //    aboutQtButton->setMaximumSize(buttonSize);
    //    aboutQtButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //    aboutQtButton->setText(tr("About Qt"));
    //    // aboutQtButton->setShortcut(QKeySequence::Quit);
    //    aboutQtButton->setToolTip(tr("about Qt"));
    //    connect(aboutQtButton, SIGNAL(clicked()), this, SLOT(aboutQt()));

    //    updaterButton = new QToolButton(this);
    //    updaterButton->setMaximumSize(buttonSize);
    //    updaterButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //    updaterButton->setText(tr("Check Update"));
    //    // aboutQtButton->setShortcut(QKeySequence::Quit);
    //    updaterButton->setToolTip(tr("check for an update"));
    //    connect(updaterButton, SIGNAL(clicked()), this, SLOT(checkUpdate()));

    //    //    QSize size(80,30);
    //    //    newProjectButton->setFixedSize(size);
    //    //    projectManagerButton->setFixedSize(size);
    //    // //   openButton->setFixedSize(size);
    //    //    displayConfigButton->setFixedSize(size);
    //    //    closeProjectButton->setFixedSize(size);
    //    //    exitButton->setFixedSize(size);
    //    //    printButton->setFixedSize(size);
    //    //    aboutQtButton->setFixedSize(size);
    //    //    aboutButton->setFixedSize(size);




    //    baseGridLayout->addWidget(newProjectButton);
    //    baseGridLayout->addWidget(projectManagerButton);
    //    //   baseGridLayout->addWidget(openButton);
    //    baseGridLayout->addSpacing(5);

    //    baseGridLayout->addWidget(findReplaceButton);
    //    baseGridLayout->addSpacing(5);

    //    baseGridLayout->addWidget(displayConfigButton);
    //    baseGridLayout->addWidget(exportButton);
    //    baseGridLayout->addWidget(printButton);
    //    baseGridLayout->addSpacing(5);


    //    baseGridLayout->addWidget(aboutButton);
    //    baseGridLayout->addWidget(aboutQtButton);
    //    baseGridLayout->addWidget(updaterButton);
    //    baseGridLayout->addSpacing(5);

    //    baseGridLayout->addWidget(closeProjectButton);
    //    baseGridLayout->addWidget(exitButton);
    //    baseGridLayout->addStretch(0);

    //    baseGridLayout->setSpacing(0);
    //    setLayout(baseGridLayout);
}






//---------------------------------------------------------------------------

void MenuBar::readSettings()
{


}

//---------------------------------------------------------------------------

void MenuBar::writeSettings()
{



}



//---------------------------------------------------------------------------

void MenuBar::openProjectManagerSlot()
{

    if(projManager->isVisible()){
        projManager->close();
    }

    projectManager();

    // for the drag drop open in OSX :



}
//---------------------------------------------------------------------------

void MenuBar::openNewProjectSlot()
{

    newProject();


    projectManager();



}



//---------------------------------------------------------------------------
void MenuBar::giveStyle()
{


    //    setStyleSheet(" QToolButton {"
    //                  "background-color: grey;"
    //                  "border-style: outset;"
    //                  "border-width: 1px;"
    //                  "border-radius: 0px;"
    //                  "border-color: black;"
    //                  "font: bold 12px;"
    //                  "min-width: 10em;"
    //                  "padding: 6px;"
    //                  "}"
    //                  "QToolButton:pressed {"
    //                  "background-color: rgb(150, 150, 150);"
    //                  "border-style: inset;"
    //                  "}"
    //);


}



//---------------------------------------------------------------------------
//----------Apply Config---------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------


void MenuBar::applyConfig()
{
    //    editWidget->applyConfig();

    emit applyConfigSignal();
}
