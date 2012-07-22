#include "menubox.h"
#include "prjmanager.h"
#include "newprojectwizard.h"
#include "settingsdialog.h"
#include "exporter.h"
#include "updater.h"
//
MenuBox::MenuBox(QWidget *parent) :
    QFrame(parent), extFile(0), currentVersion("0.49")
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




    projManager->setModal(true);
    projManager->show();

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

    Exporter *exporterDialog = new Exporter(QString("export"), file, this);
    exporterDialog->exec();

}

//--------------------------------------------------------------------------


void MenuBox::print()
{

    if(file == 0)
        return;

    emit saveProjectSignal();

    Exporter *exporterDialog = new Exporter(QString("print"), file, this);
    exporterDialog->exec();


}
//--------------------------------------------------------------------------

void MenuBox::aboutQt()
{
    QMessageBox::aboutQt(this, tr("About Qt"));



}

//----------------------------------------------------------------------------

void MenuBox::about()
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


void MenuBox::launchCheckUpdateDialog(QString mode)
{

    Updater *checkUpdateDialog = new Updater(mode);
    checkUpdateDialog->setCurrentVersion(currentVersion);

    checkUpdateDialog->exec();

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
    connect(newProjectButton, SIGNAL(clicked()), this, SLOT(newProject()));

    projectManagerButton = new QToolButton(this);
    projectManagerButton->setMaximumSize(buttonSize);
    projectManagerButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    projectManagerButton->setText(tr("Project &Manager"));
    // projectManagerAct->setShortcut(QKeySequence::New);
    projectManagerButton->setToolTip(tr("Create and manage your projects"));
    connect(projectManagerButton, SIGNAL(clicked()), this, SLOT(projectManager()));

    //    openButton = new QToolButton(this);
    //    openButton->setMaximumSize(buttonSize);
    //    openButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //    openButton->setText(tr("&Open..."));
    //    openButton->setShortcut(QKeySequence::Open);
    //    openButton->setToolTip(tr("Open an existing file"));
    //    connect(openButton, SIGNAL(clicked()), this, SLOT(open()));

    displayConfigButton = new QToolButton(this);
    displayConfigButton->setMaximumSize(buttonSize);
    displayConfigButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    displayConfigButton->setText(tr("&Configure"));
    //    displayConfigButton->setShortcut(QKeySequence::Print);
    displayConfigButton->setToolTip(tr("Display the configuration"));
    connect(displayConfigButton, SIGNAL(clicked()), this, SLOT(displayConfig()));

    exportButton = new QToolButton(this);
    exportButton->setMaximumSize(buttonSize);
    exportButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    exportButton->setText(tr("&Export"));
    //   exportButton->setShortcut(QKeySequence::Print);
    exportButton->setToolTip(tr("Export the project"));
    connect(exportButton, SIGNAL(clicked()), this, SLOT(exporter()));

    printButton = new QToolButton(this);
    printButton->setMaximumSize(buttonSize);
    printButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    printButton->setText(tr("&Print"));
    printButton->setShortcut(QKeySequence::Print);
    printButton->setToolTip(tr("Print part of the project"));
    connect(printButton, SIGNAL(clicked()), this, SLOT(print()));

    closeProjectButton = new QToolButton(this);
    closeProjectButton->setMaximumSize(buttonSize);
    closeProjectButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    closeProjectButton->setText(tr("&Close project"));
    closeProjectButton->setShortcut(QKeySequence::Close);
    closeProjectButton->setToolTip(tr("Print the document"));
    connect(closeProjectButton, SIGNAL(clicked()), this, SLOT(closeProject()));;

    exitButton = new QToolButton(this);
    exitButton->setMaximumSize(buttonSize);
    exitButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    exitButton->setText(tr("E&xit"));
    exitButton->setShortcut(QKeySequence::Quit);
    exitButton->setToolTip(tr("Exit the application"));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(exit()));


    aboutButton = new QToolButton(this);
    aboutButton->setMaximumSize(buttonSize);
    aboutButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    aboutButton->setText(tr("About Plume Creator"));
    // aboutButton->setShortcut(QKeySequence::Quit);
    aboutButton->setToolTip(tr("about the application"));
    connect(aboutButton, SIGNAL(clicked()), this, SLOT(about()));

    aboutQtButton = new QToolButton(this);
    aboutQtButton->setMaximumSize(buttonSize);
    aboutQtButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    aboutQtButton->setText(tr("About Qt"));
    // aboutQtButton->setShortcut(QKeySequence::Quit);
    aboutQtButton->setToolTip(tr("about Qt"));
    connect(aboutQtButton, SIGNAL(clicked()), this, SLOT(aboutQt()));

    updaterButton = new QToolButton(this);
    updaterButton->setMaximumSize(buttonSize);
    updaterButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    updaterButton->setText(tr("Check Update"));
    // aboutQtButton->setShortcut(QKeySequence::Quit);
    updaterButton->setToolTip(tr("check for an update"));
    connect(updaterButton, SIGNAL(clicked()), this, SLOT(checkUpdate()));

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
    baseGridLayout->addWidget(updaterButton);
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


    if(extFile !=0)
        openExternalProject(extFile);

    extFile = 0;


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


//---------------------------------------------------------------------------

void MenuBox::setExternalProject(QFile *externalFile)
{
    QFileInfo *externalFileInfo = new QFileInfo(externalFile->fileName());
    QString extFileName = externalFileInfo->fileName();
    QString extFilePath = externalFileInfo->path();


    // verify the .plume extension :

    QString ext = extFileName.mid(extFileName.indexOf(".") + 1);
    if(ext != "plume"){
        QMessageBox::warning(this, tr("Plume Creator Warning"),
                             tr("The file you are trying to open\n"
                                " isn't a Plume project.\n"
                                "\n\n"
                                "Please open a .plume file."));
        extFile = 0;
        return;
    }


    //verify if the project isn't already in the manager :



    QSettings settings;

    int size = settings.value("Manager/projects/size", 0).toInt();
    settings.beginReadArray("Manager/projects");

    QStringList valueList;

    for(int i=0; i < size; ++i){
        settings.setArrayIndex(i);
        valueList.append(QDir::fromNativeSeparators(settings.value("workPath").toString()));
    }

    settings.endArray();





    //    QFile *logFile = new QFile;
    //    logFile->setFileName(QDir::toNativeSeparators(extFilePath + "/plume.log"));
    //    logFile->waitForBytesWritten(500);
    //    logFile->close();
    //    logFile->open(QFile::ReadWrite | QFile::Text | QFile::Append);
    //    if(logFile->isWritable())
    //    {
    //        QTextStream logStream(logFile);

    //            logStream << QDateTime::currentDateTime().toString() << " :  extFilePath : " << extFilePath;
    //            logStream << "\n";
    //            logStream << QDateTime::currentDateTime().toString() << " :  valueList.first() : " << valueList.first();
    //            logStream << "\n";

    //    }
    //    logFile->close();





    if(valueList.contains(QDir::fromNativeSeparators(extFilePath))){
        QMessageBox::warning(this, tr("Plume Creator Warning"),
                             tr("The file you are trying to open\n"
                                " is already in the Plume project manager.\n"));
        extFile = 0;
        return;
    }



    //find the .prjinfo, else create it :

    QStringList filters;
    QDir dir(extFilePath);
    filters.clear();
    filters << "*.prjinfo";



    if(dir.entryList(filters).isEmpty()){
        filters.clear();
        filters << "*.plume";

        QFile plumeFile(dir.entryList(filters).first());
        QString projectName = plumeFile.fileName().split("/").last().remove(".plume");



        // create an empty file :

        QFile file(extFilePath + "/" + projectName + ".prjinfo");
        file.close();
        file.open(QIODevice::ReadWrite | QIODevice::Text);


        QDomDocument domDoc("plume-information");

        QDomElement root = domDoc.createElement("root");
        root.setTagName("plume-information");
        root.setAttribute("projectName", projectName);
        root.setAttribute("version","0.2");
        domDoc.appendChild(root);

        QDomElement prjInfoElem = domDoc.createElement("prj");
        prjInfoElem.setAttribute("name", projectName);
        prjInfoElem.setAttribute("path", extFilePath.left(extFilePath.size() - projectName.size() - 1 ));
        prjInfoElem.setAttribute("workPath", extFilePath);
        prjInfoElem.setAttribute("lastModified", QDateTime::currentDateTime().toString());
        prjInfoElem.setAttribute("creationDate", QDateTime::currentDateTime().toString());
        root.appendChild(prjInfoElem);


        file.flush();

        const int IndentSize = 4;

        QTextStream out(&file);
        out.flush();
        domDoc.save(out, IndentSize);

        file.close();
    }

    //modify the path attributes in case the project was moved:

    filters.clear();
    filters << "*.prjinfo";

    QFile plumeFile(extFilePath + "/" + dir.entryList(filters).first());
    QString projectName = plumeFile.fileName().split("/").last().remove(".prjinfo");

    QString string;
    qDebug() << "dirCount : "  << string.setNum(dir.entryList(filters).count()) << " = " << dir.entryList(filters).first();
    qDebug() << "plumeFile : " << plumeFile.fileName();
    QFileInfo *dirInfo = new QFileInfo(plumeFile);
    QString devicePath = dirInfo->absolutePath();
    qDebug() << "File path:" << devicePath;

    plumeFile.open(QIODevice::ReadOnly | QIODevice::Text);

//    QString f(plumeFile.readAll());
//    qDebug() << "f : "<< f;


    QDomDocument domDocument;
    QString errorStr;
    int errorLine;
    int errorColumn;


    if (!domDocument.setContent(&plumeFile, true, &errorStr, &errorLine,
                                &errorColumn)) {
        QMessageBox::information(this, tr("Plume Creator Prj XML"),
                                 tr("Parse error at line %1, column %2:\n%3\n")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));


        return;
    }




    QDomElement root = domDocument.documentElement();
    if (root.tagName() != "plume-information") {
        QMessageBox::information(this, tr("Plume Creator Prj Tree"),
                                 tr("The file is not a Plume Creator information file."));
        return;
    } else if (root.hasAttribute("version")
               && root.attribute("version") != "0.2") {
        QMessageBox::information(this, tr("Plume Creator Prj Tree"),
                                 tr("The file is not an Plume Creator information file version 0.2 "
                                    "file."));
        return;
    }


    QDomElement prjInfoElem = root.firstChildElement("prj");



//    if(extFilePath != prjInfoElem.attribute("workPath")){




        prjInfoElem.setAttribute("path", extFilePath.left(extFilePath.size() - projectName.size() - 1 ));
        prjInfoElem.setAttribute("workPath", extFilePath);

plumeFile.close();
        plumeFile.open(QIODevice::ReadWrite | QIODevice::Text |QIODevice::Truncate	);

        plumeFile.flush();

        const int IndentSize = 4;


        QTextStream out(&plumeFile);
        out.flush();
        domDocument.save(out, IndentSize);



        qDebug() << "eee";
//    }



        plumeFile.close();


    filters.clear();
    filters << "*.prjinfo";


    QFile *prjinfoFile = new QFile(extFilePath + "/" + dir.entryList(filters).first());

    extFile = prjinfoFile;



    openProjectManagerSlot();


}

//---------------------------------------------------------------------------
bool MenuBox::openExternalProject(QFile *externalPrjFile)
{



    int ret = QMessageBox::question(this, tr("Open Project"),
                                    tr("<p>You are opening a Plume project.</p>\n"
                                       "<br>"
                                       "<p>Do you want to add this project to the manager list ?</p>"),
                                    QMessageBox::Yes | QMessageBox::Cancel,
                                    QMessageBox::Yes);

    switch (ret) {
    case QMessageBox::Yes:
    {


        QFile file(externalPrjFile->fileName());

        file.open(QIODevice::ReadWrite | QIODevice::Text);


        QDomDocument domDocument;
        QString errorStr;
        int errorLine;
        int errorColumn;


        if (!domDocument.setContent(&file, true, &errorStr, &errorLine,
                                    &errorColumn)) {
            QMessageBox::information(this, tr("Plume Creator XML"),
                                     tr("Parse error at line %1, column %2:\n%3\n")
                                     .arg(errorLine)
                                     .arg(errorColumn)
                                     .arg(errorStr));


            return false;
        }

        //        QFileInfo *dirInfo = new QFileInfo(file);
        //        QString devicePath = dirInfo->path();
        //        qDebug() << "File path:" << devicePath;


        QDomElement root = domDocument.documentElement();
        if (root.tagName() != "plume-information") {
            QMessageBox::information(this, tr("Plume Creator Tree"),
                                     tr("The file is not a Plume Creator information file."));
            return false;
        } else if (root.hasAttribute("version")
                   && root.attribute("version") != "0.2") {
            QMessageBox::information(this, tr("Plume Creator Tree"),
                                     tr("The file is not an Plume Creator information file version 0.2 "
                                        "file."));
            return false;
        }


        QDomElement prjInfoElem = root.firstChildElement("prj");

        QString projectName = prjInfoElem.attribute("name", tr("error"));
        QString projectDirectory = prjInfoElem.attribute("path", tr("error"));
        QString workingPath = prjInfoElem.attribute("workPath", tr("error"));
        QString lastModified = prjInfoElem.attribute("lastModified", QDateTime::currentDateTime().toString());
        QString creationDate = prjInfoElem.attribute("creationDate", QDateTime::currentDateTime().toString());

        root.appendChild(prjInfoElem);



        file.close();



        //Saving to .conf the new project :

        QSettings settings;

        int size = settings.value("Manager/projects/size", 0).toInt();


        settings.beginWriteArray("Manager/projects");

        settings.setArrayIndex(size);
        settings.setValue("name", projectName);
        settings.setValue("path", projectDirectory);
        settings.setValue("workPath", workingPath);
        settings.setValue("lastModified", lastModified);
        settings.setValue("creationDate", creationDate);

        settings.endArray();




        extFile = 0;


        //open or update the project manager :
        openProjectManagerSlot();

        return true;
    }
        break;
    case QMessageBox::Cancel:
        return false;

        break;
    default:
        // should never be reached
        break;


    }



}


//---------------------------------------------------------------------------
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
