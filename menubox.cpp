#include "menubox.h"
#include "prjmanager.h"
#include "newprojectwizard.h"
#include "settingsdialog.h"
#include "exporter.h"

MenuBox::MenuBox(QWidget *parent) :
    QFrame(parent)
{

    file = 0;
    giveStyle();
    createActions(); // if buttons with menus
    createButtons();


    readSettings();

}

//---------------------------------------------------------------------------

void MenuBox::newProject()
{
    NewProjectWizard projectWizard(this);
    projectWizard.exec();

    projectManager();

}

//---------------------------------------------------------------------------

//void MenuBox::open()
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

void MenuBox::projectManager()
{
    projManager = new PrjManager(this);
    connect(projManager,SIGNAL(openPrjManagerSignal()), this, SLOT(openProjectManagerSlot()));
    connect(projManager,SIGNAL(newPrjSignal()), this, SLOT(openNewProjectSlot()));
    connect(projManager, SIGNAL(openProjectSignal(QFile*)), this, SLOT(openProjectSlot(QFile*)));
    connect(projManager, SIGNAL(openProjectNumberSignal(int)),this, SIGNAL(openProjectNumberSignal(int)));
    projManager->exec();
}

//---------------------------------------------------------------------------


void MenuBox::displayConfig()
{

    SettingsDialog *settingsDialog = new SettingsDialog(this);
    connect(settingsDialog, SIGNAL(accepted()), this, SLOT(applyConfig()));
    settingsDialog->exec();
    //    //    Config config;
    //    ConfigDialog dialog(/*config, */this);
    //    if (dialog.exec() == QDialog::Accepted) {
    //        //        loadPreferences(config);
    //    }
}


//--------------------------------------------------------------------------


void MenuBox::closeProject()
{
    if(file == 0)
        return;

    QMessageBox msgBox(this);
    msgBox.setText(tr("Do you want to close the current project ?"));
    msgBox.setInformativeText(tr("Your changes are already saved."));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Ok:
        emit closeProjectSignal();

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


void MenuBox::exporter()
{
    if(file == 0)
        return;

    emit saveProjectSignal();

    Exporter *exporterDialog = new Exporter(file, this);
    exporterDialog->exec();

}

//--------------------------------------------------------------------------


void MenuBox::print()
{




}
//--------------------------------------------------------------------------

void MenuBox::aboutQt()
{
    QMessageBox::aboutQt(this, tr("About Qt"));



}
//--------------------------------------------------------------------------

void MenuBox::about()
{
    QMessageBox::about(this, tr("About Plume Creator"),
                       "<p><center><b>Plume Creator</b></p>"
                       "<p><b>A Project Manager and Rich Text Editor for Writers.</b></p>"

                       "<p>Version 0.34 Beta</p>"


                       "<p>Copyright (C) 2011 by Cyril Jacquet</p>"
                       "<p>terreville@google.com </p></center>"
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

void MenuBox::exit()
{

    writeSettings();
    emit exitSignal();


}







//---------------------------------------------------------------------------


void MenuBox::createActions()
{

}

//---------------------------------------------------------------------------


void MenuBox::createButtons()
{
    QSize buttonSize(120,60);


    QVBoxLayout *baseGridLayout = new QVBoxLayout;

    newProjectButton = new QToolButton(this);
    newProjectButton->setMaximumSize(buttonSize);
    newProjectButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    newProjectButton->setText(tr("&New Project"));
    newProjectButton->setShortcut(QKeySequence::New);
    newProjectButton->setToolTip(tr("Create a new project"));
    connect(newProjectButton, SIGNAL(pressed()), this, SLOT(newProject()));

    projectManagerButton = new QToolButton(this);
    projectManagerButton->setMaximumSize(buttonSize);
    projectManagerButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    projectManagerButton->setText(tr("Project &Manager"));
    // projectManagerAct->setShortcut(QKeySequence::New);
    projectManagerButton->setToolTip(tr("Create a new project"));
    connect(projectManagerButton, SIGNAL(pressed()), this, SLOT(projectManager()));

    //    openButton = new QToolButton(this);
    //    openButton->setMaximumSize(buttonSize);
    //    openButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //    openButton->setText(tr("&Open..."));
    //    openButton->setShortcut(QKeySequence::Open);
    //    openButton->setToolTip(tr("Open an existing file"));
    //    connect(openButton, SIGNAL(pressed()), this, SLOT(open()));

    displayConfigButton = new QToolButton(this);
    displayConfigButton->setMaximumSize(buttonSize);
    displayConfigButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    displayConfigButton->setText(tr("&Configure"));
    //   displayConfigButton->setShortcut(QKeySequence::Print);
    displayConfigButton->setToolTip(tr("Display the configuration"));
    connect(displayConfigButton, SIGNAL(pressed()), this, SLOT(displayConfig()));

    exportButton = new QToolButton(this);
    exportButton->setMaximumSize(buttonSize);
    exportButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    exportButton->setText(tr("&Export"));
    //   exportButton->setShortcut(QKeySequence::Print);
    exportButton->setToolTip(tr("Export the project"));
    connect(exportButton, SIGNAL(pressed()), this, SLOT(exporter()));

    printButton = new QToolButton(this);
    printButton->setMaximumSize(buttonSize);
    printButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    printButton->setText(tr("&Print"));
    printButton->setShortcut(QKeySequence::Print);
    printButton->setToolTip(tr("Print part of the project"));
    connect(printButton, SIGNAL(pressed()), this, SLOT(print()));

    closeProjectButton = new QToolButton(this);
    closeProjectButton->setMaximumSize(buttonSize);
    closeProjectButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    closeProjectButton->setText(tr("&Close project"));
    closeProjectButton->setShortcut(QKeySequence::Close);
    closeProjectButton->setToolTip(tr("Print the dprintocument"));
    connect(closeProjectButton, SIGNAL(pressed()), this, SLOT(closeProject()));;

    exitButton = new QToolButton(this);
    exitButton->setMaximumSize(buttonSize);
    exitButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    exitButton->setText(tr("E&xit"));
    exitButton->setShortcut(QKeySequence::Quit);
    exitButton->setToolTip(tr("Exit the application"));
    connect(exitButton, SIGNAL(pressed()), this, SLOT(exit()));


    aboutButton = new QToolButton(this);
    aboutButton->setMaximumSize(buttonSize);
    aboutButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    aboutButton->setText(tr("About Plume Creator"));
    // aboutButton->setShortcut(QKeySequence::Quit);
    aboutButton->setToolTip(tr("about the application"));
    connect(aboutButton, SIGNAL(pressed()), this, SLOT(about()));

    aboutQtButton = new QToolButton(this);
    aboutQtButton->setMaximumSize(buttonSize);
    aboutQtButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    aboutQtButton->setText(tr("About Qt"));
    // aboutQtButton->setShortcut(QKeySequence::Quit);
    aboutQtButton->setToolTip(tr("aboutQt the application"));
    connect(aboutQtButton, SIGNAL(pressed()), this, SLOT(aboutQt()));

    //    QSize size(80,30);
    //    newProjectButton->setFixedSize(size);
    //    projectManagerButton->setFixedSize(size);
    // //   openButton->setFixedSize(size);
    //    displayConfigButton->setFixedSize(size);
    //    closeProjectButton->setFixedSize(size);
    //    exitButton->setFixedSize(size);
    //    printButton->setFixedSize(size);
    //    aboutQtButton->setFixedSize(size);
    //    aboutButton->setFixedSize(size);




    baseGridLayout->addWidget(newProjectButton);
    baseGridLayout->addWidget(projectManagerButton);
    //   baseGridLayout->addWidget(openButton);
    baseGridLayout->addSpacing(5);

    baseGridLayout->addWidget(displayConfigButton);
    baseGridLayout->addWidget(exportButton);
    baseGridLayout->addWidget(printButton);
    baseGridLayout->addSpacing(5);


    baseGridLayout->addWidget(aboutButton);
    baseGridLayout->addWidget(aboutQtButton);
    baseGridLayout->addSpacing(5);

    baseGridLayout->addWidget(closeProjectButton);
    baseGridLayout->addWidget(exitButton);
    baseGridLayout->addStretch(0);

    baseGridLayout->setSpacing(0);
    setLayout(baseGridLayout);
}






//---------------------------------------------------------------------------

void MenuBox::readSettings()
{


}

//---------------------------------------------------------------------------

void MenuBox::writeSettings()
{



}



//---------------------------------------------------------------------------

void MenuBox::openProjectManagerSlot()
{

    if(projManager->isVisible()){
        projManager->close();
    }

    projectManager();
}
//---------------------------------------------------------------------------

void MenuBox::openNewProjectSlot()
{

    newProject();

    if(projManager->isVisible()){
        projManager->close();
        projectManager();

    }
}
//---------------------------------------------------------------------------

void MenuBox::openProjectSlot(QFile *device)
{

    closeProject();

    file = new QFile;
    file = device;

    emit openProjectSignal(device);

    if(projManager->isVisible()){
        projManager->close();
    }


}






void MenuBox::giveStyle()
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


void MenuBox::applyConfig()
{


    emit applyConfigSignal();
}
