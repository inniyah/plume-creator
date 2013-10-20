#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QApplication>
#include <QSettings>
#include <QLocale>


#include "prjmanager.h"



#include "newprojectwizard.h"
//


PrjManager::PrjManager(QWidget *parent) :
    QDialog(parent),prjOpened(false)
{

    setAttribute(Qt::WA_DeleteOnClose);

    setWindowTitle(tr("Projects Manager"));
    //    setFixedSize(600, 500);

    QHBoxLayout *layout = new QHBoxLayout;

    QLabel *managerLabel = new QLabel;
    managerLabel->setText(tr("Please select in the list below the project you want to work with :"));

    //    QWidget *managerList = new QWidget;

    //    QFrame *projectDetails = new QFrame;

    QVBoxLayout *pictureLayout = new QVBoxLayout;

    QLabel *pictureLabel = new QLabel;
    pictureLabel->setPixmap(QPixmap(":/pics/plume-creator.png"));
    QLabel *versionLabel = new QLabel("V "+ QApplication::applicationVersion());

    pictureLayout->addWidget(pictureLabel);
    pictureLayout->addWidget(versionLabel,0,Qt::AlignCenter);


    QVBoxLayout *projectButtonsLayout = new QVBoxLayout;

    QPushButton *newButton = new QPushButton(QIcon(":/pics/tools-wizard.png"),tr("&New"));
    connect(newButton, SIGNAL(clicked()), this, SLOT(newPrj()));
    QPushButton *openButton = new QPushButton(QIcon(":/pics/applications-office.png"),tr("&Open"));
    connect(openButton, SIGNAL(clicked()), this, SLOT(openPrj()));
    QPushButton *editButton = new QPushButton(QIcon(":/pics/applications-system.png"),tr("&Edit"));
    QPushButton *deleteButton = new QPushButton(QIcon(":/pics/knotes_delete.png"),tr("&Delete"));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteProjectQuestion()));
    QPushButton *closeButton = new QPushButton(QIcon(":/pics/window-close.png"),tr("&Close"));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QMenu *editMenu = new QMenu();

    QAction *renameAct = new QAction(QIcon(":/pics/edit-rename.png"),tr("&Rename"), this);
    renameAct->setShortcut(Qt::Key_F2);
    renameAct->setToolTip(tr("Rename the project"));
    connect(renameAct, SIGNAL(triggered()), this, SLOT(renamePrj()));

    editMenu->addAction(renameAct);
    editButton->setMenu(editMenu);

    projectButtonsLayout->addWidget(newButton);
    projectButtonsLayout->addStretch(30);
    projectButtonsLayout->addWidget(openButton);
    projectButtonsLayout->addWidget(editButton);
    projectButtonsLayout->addStretch(30);
    projectButtonsLayout->addWidget(deleteButton);
    projectButtonsLayout->addStretch(30);
    projectButtonsLayout->addWidget(closeButton);

    QWidget *labelWidget = new QWidget;
    labelWidget->setFixedWidth(300);
    projListLayout = new QVBoxLayout;

    QFormLayout *prjLabelsLayout = new QFormLayout;
    nameLabel = new QLabel;
    creationDateLabel = new QLabel;
    modifiedDateLabel = new QLabel;
    pathLabel = new QLabel;
    pathLabel->setWordWrap(true);

    prjLabelsLayout->addRow(tr("&Name :"), nameLabel);
    prjLabelsLayout->addRow(tr("&Creation date :"), creationDateLabel);
    prjLabelsLayout->addRow(tr("&Modified :"), modifiedDateLabel);
    prjLabelsLayout->addRow(tr("&Path :"), pathLabel);
    labelWidget->setLayout(prjLabelsLayout);

    //    managerList->setLayout(projListLayout);



    layout->addLayout(pictureLayout);
    layout->addLayout(projListLayout);
    layout->addLayout(projectButtonsLayout);
    layout->addWidget(labelWidget);

    displayProjects();

    //    QVBoxLayout *layout = new QVBoxLayout;
    //    QBoxLayout *boxLayout = new QBoxLayout(QBoxLayout::LeftToRight);

    //    boxLayout->addStretch(10);
    //    boxLayout->addWidget(closeButton);

    //    layout->addWidget(managerLabel);
    //    layout->addWidget(managerList);


    //    QScrollArea *scrollArea = new QScrollArea;
    //    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //    //projList->setStyleSheet("* {border-style: inset; border-radius: 4px; border: 1px solid black; background-color: white}");
    //    projList->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    //    projList->setFrameShadow(QFrame::Sunken);
    //    projList->setLineWidth(3);
    //    projList->setMidLineWidth(3);

    //    projList->setLayout(projListLayout);
    //    scrollArea->setWidget(projList);

    //    scrollAeraLayout->addWidget(scrollArea);
    //    managerList->setLayout(scrollAeraLayout);

    //    layout->addLayout(boxLayout);

    setLayout(layout);


//    QDesktopWidget *desktop = QApplication::desktop();
//    this->move(desktop->width()/2 - this->width()/2, desktop->height()/2 - this->height()/2);

}

//-----------------------------------------------------------------------------------


void PrjManager::displayProjects(){



    QMultiMap<int, QString> projName, projPath, projWorkPath, projLastModifiedTime, projCreationDate;


    table = new QTableWidget(this);
    table->setColumnCount(6);

    QSettings settings;


    settings.beginGroup("Manager/projects");

    QStringList groups = settings.childGroups();
    int size = groups.size();
    settings.setValue("size", size);

    if(size == 0){
        table->hide();
        return;
    }
    table->setRowCount(size);

    settings.endGroup();
//        QString string;
//        qDebug() << "size : " << string.setNum(size,10);
    // int size = settings.value("Manager/projects/size").toInt();





    settings.beginReadArray("Manager/projects");

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);

        projName.insert(i, settings.value("name").toString());
        projPath.insert(i, settings.value("path").toString());
        projWorkPath.insert(i, settings.value("workPath").toString());
        projLastModifiedTime.insert(i, settings.value("lastModified").toString());
        projCreationDate.insert(i, settings.value("creationDate").toString());


        QString projNameI = projName.value(i);
        QString projPathI = projPath.value(i);
        QString projWorkPathI = projWorkPath.value(i);
        QString projLastModifiedTimeI = projLastModifiedTime.value(i);
        QString projCreationDateI = projCreationDate.value(i);

        QDateTime *modTime = new QDateTime(QDateTime::fromString(projLastModifiedTimeI, Qt::ISODate));
        if(!modTime->isValid())
            modTime = new QDateTime(QDateTime::fromString(projLastModifiedTimeI, Qt::TextDate));
        //        if(!modTime->isValid())
        //            qDebug() << projLastModifiedTimeI;

        QTableWidgetItem *projNameItem = new QTableWidgetItem(projNameI);
        QTableWidgetItem *projPathItem = new QTableWidgetItem(projPathI);
        QTableWidgetItem *projWorkPathItem = new QTableWidgetItem(projWorkPathI);
        QTableWidgetItem *projLastModifiedTimeItem = new QTableWidgetItem(modTime->toString(Qt::ISODate));
        QTableWidgetItem *projCreationDateItem = new QTableWidgetItem(projCreationDateI);
        QTableWidgetItem *projNumberItem = new QTableWidgetItem(QString::number(i));


        table->setItem(i, 0, projNameItem);
        table->setItem(i, 1, projPathItem);
        table->setItem(i, 2, projWorkPathItem);
        table->setItem(i, 3, projLastModifiedTimeItem);
        table->setItem(i, 4, projCreationDateItem);
        table->setItem(i, 5, projNumberItem);




        //

        //        stackItem = new QFrame;
        //        stackItem->setFixedSize(520, 150);
        //        stackItem->setFrameStyle(QFrame::Panel | QFrame::Raised);
        //        stackItem->setFrameShadow(QFrame::Raised);
        //        stackItem->setLineWidth(3);
        //        stackItem->setMidLineWidth(3);

        //        itemLayout = new QGridLayout;

        //        QWidget *stretcher = new QWidget;
        //        stretcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        //        QLabel *nameLabel = new QLabel(tr("<b>Name :</b>"));
        //        nameLabel->setAlignment(Qt::AlignRight);
        //        QLabel *pathLabel = new QLabel(tr("<b>Path :</b>"));
        //        pathLabel->setAlignment(Qt::AlignRight);
        //        QLabel *lastModLabel = new QLabel(tr("<b>Last modified :</b>"));
        //        lastModLabel->setAlignment(Qt::AlignRight);
        //        QLabel *creaDateLabel = new QLabel(tr("<b>Creation date :</b>"));
        //        creaDateLabel->setAlignment(Qt::AlignRight);


        //        QLabel *projNameLabel = new QLabel(projNameI);
        //        QLabel *projPath = new QLabel(QDir::toNativeSeparators(projPathI));
        //        QLabel *projLastModifiedTime = new QLabel(projLastModifiedTimeI);
        //        QLabel *projCreationDate = new QLabel(projCreationDateI);

        //        openButton = new OpenPrjButton;
        //        QString proj(projWorkPathI + "/" + projNameI + ".plume");
        //        QFile *file = new QFile(proj);
        //        openButton->openProjectFile(file);
        //        openButton->openProjectSettingNumber(i);
        //        connect(openButton, SIGNAL(openProjectSignal(QFile*)), this, SIGNAL(openProjectSignal(QFile*)));
        //        connect(openButton, SIGNAL(openProjectNumberSignal(int)), this, SIGNAL(openProjectNumberSignal(int)));


        //        deleteButton = new DeletePrjButton;
        //        deleteButton->deleteProjectNum(i);


        //        connect(deleteButton, SIGNAL(openProjectManagerSignal()), this, SIGNAL(openPrjManagerSignal()));
        //        //is used to update the manager display

        //        itemLayout->addWidget(nameLabel,0,0);
        //        itemLayout->addWidget(projNameLabel,0,1);
        //        itemLayout->addWidget(pathLabel,1,0);
        //        itemLayout->addWidget(projPath,1,1);
        //        itemLayout->addWidget(creaDateLabel,2,0);
        //        itemLayout->addWidget(projCreationDate,2,1);
        //        itemLayout->addWidget(lastModLabel,3,0);
        //        itemLayout->addWidget(projLastModifiedTime,3,1);
        //        itemLayout->addWidget(stretcher,0,2,3,2);
        //        itemLayout->addWidget(openButton,0,3,1,3,Qt::AlignRight);
        //        itemLayout->addWidget(deleteButton,2,3,3,3,Qt::AlignRight);

        //        stackItem->setLayout(itemLayout);

        //        projListLayout->addWidget(stackItem);







    }

    settings.endArray();

    projListLayout->addWidget(table);

    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();

    table->hideColumn(1);
    table->hideColumn(2);
    table->hideColumn(3);
    table->hideColumn(4);
    table->hideColumn(5);

    table->resizeColumnToContents(0);

    table->sortByColumn(3);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setMinimumWidth(table->columnWidth(0) + 20) ;
    table->horizontalHeader()->setStretchLastSection(true);

    connect(table, SIGNAL(cellClicked(int,int)), this, SLOT(setPrjActivated(int,int)));
    connect(table, SIGNAL(itemSelectionChanged()), this, SLOT(itemsSelectionChangedSlot()));
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(openPrj()));
    connect(table, SIGNAL(cellActivated(int,int)), this, SLOT(openPrj()));
    table->setCurrentCell(0,0);
    this->setPrjActivated(0,0);

    //    QFrame *stackNewPrj = new QFrame;
    //    stackNewPrj->setFixedSize(520, 150);
    //    stackNewPrj->setFrameStyle(QFrame::Panel | QFrame::Raised);
    //    stackNewPrj->setLineWidth(3);
    //    stackNewPrj->setMidLineWidth(3);

    //    QVBoxLayout *newPrjStackLayout = new QVBoxLayout;

    //    QLabel *newPrjLabel = new QLabel(tr("<b>You can also create a new project :</b>"));
    //    newPrjLabel->setAlignment(Qt::AlignCenter);
    //    QPushButton *newPrjButton = new QPushButton(tr("Add project"));
    //    //  connect(newPrjButton,SIGNAL(clicked()), this, SIGNAL(newPrjSignal()));
    //    connect(newPrjButton,SIGNAL(clicked()), this, SLOT(newPrj()));

    //    newPrjStackLayout->addWidget(newPrjLabel);
    //    newPrjStackLayout->addWidget(newPrjButton);

    //    stackNewPrj->setLayout(newPrjStackLayout);

    //    projListLayout->addWidget(stackNewPrj);






}

//----------------------------------------------------------------------------------------

void PrjManager::newPrj()
{
    close();
    emit newPrjSignal();

}
//----------------------------------------------------------------------------------------

void PrjManager::setPrjActivated(int row,int column)
{
    if(table->isHidden())
        return;

    currentRow = row;
    prjActivated = table->item(row, 5)->data(0).toInt();


    // change info labels :

    nameLabel->setText(table->item(row, 0)->data(0).toString());
    QString time;
    QString projCreateTime = table->item(row, 4)->data(0).toString();
    QDateTime *createTime = new QDateTime(QDateTime::fromString(projCreateTime, Qt::ISODate));
    if(!createTime->isValid())
        createTime = new QDateTime(QDateTime::fromString(projCreateTime, Qt::TextDate));
    if(!createTime->isValid())
        time = projCreateTime;
    //        createTime = new QDateTime(QDateTime::fromString(projCreateTime,"ddd MMM d HH:mm:ss yyyy"));
    //    qDebug() << projCreateTime;
    if(createTime->isValid())
        time = createTime->toString(Qt::TextDate);
    creationDateLabel->setText(time);



    QString projLastModifiedTime = table->item(row, 3)->data(0).toString();
    QDateTime *modTime = new QDateTime(QDateTime::fromString(projLastModifiedTime, Qt::ISODate));
    if(!modTime->isValid())
        modTime = new QDateTime(QDateTime::fromString(projLastModifiedTime, Qt::TextDate));
    if(!createTime->isValid())
        time = projLastModifiedTime;
    if(createTime->isValid())
        time = modTime->toString(Qt::TextDate);
    modifiedDateLabel->setText(modTime->toString(Qt::TextDate));
    pathLabel->setText(table->item(row, 1)->data(0).toString());

}

//----------------------------------------------------------------------------------------

void PrjManager::itemsSelectionChangedSlot()
{
    QList<QTableWidgetItem *> itemsList = table->selectedItems();

    if(itemsList.isEmpty())
        return;

    for (int i = 1; i < itemsList.size(); ++i) {
        itemsList.at(i)->setSelected(false);
    }

    QTableWidgetItem *item = itemsList.takeFirst();
    setPrjActivated(item->row(), item->column());

}

//----------------------------------------------------------------------------------------

void PrjManager::openPrj()
{
    if(prjOpened)
        return;
    if(table->isHidden())
        return;

QString proj;
    if(table->item(currentRow, 2)->data(0).toString() == "OBSOLETE")
        proj =  table->item(currentRow, 1)->data(0).toString() + "/" + table->item(currentRow, 0)->data(0).toString() + ".plume";
    else// compatibility for old system :
        proj = table->item(currentRow, 2)->data(0).toString() + "/" + table->item(currentRow, 0)->data(0).toString() + ".plume";

proj = QDir::fromNativeSeparators(proj);

    hub->startProject(proj);
    emit openProjectNumberSignal(prjActivated);

    prjOpened = true;

    this->close();
}
//-----------------------------------------------------------------------------------


void PrjManager::deleteProjectQuestion()
{
    if(table->isHidden())
        return;

    QMessageBox deleteQuestion(this);
    deleteQuestion.setIcon(QMessageBox::Warning);
    deleteQuestion.setText("This project will be deleted.");
    deleteQuestion.setInformativeText("Are you really sure to delete this project ?");
    deleteQuestion.setStandardButtons(QMessageBox::Cancel);
    QPushButton *delButton = deleteQuestion.addButton(tr("Delete project"), QMessageBox::ActionRole);
    deleteQuestion.setDefaultButton(QMessageBox::Cancel);
    deleteQuestion.exec();

    if (deleteQuestion.clickedButton() == delButton) {

        delProject();



    }
}


//-----------------------------------------------------------------------------------


void PrjManager::delProject()
{


    QMultiMap<int, QString> projName, projPath, projWorkPath, projLastModifiedTime, projCreationDate;


    QSettings settings;

    int size = settings.value("Manager/projects/size").toInt();

    settings.beginReadArray("Manager/projects");

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);

        projName.insert(i, settings.value("name").toString());
        projPath.insert(i, settings.value("path").toString());
        projWorkPath.insert(i, settings.value("workPath").toString());
        projLastModifiedTime.insert(i, settings.value("lastModified").toString());
        projCreationDate.insert(i, settings.value("creationDate").toString());
    }
    settings.endArray();

    QString del_projName = projName.value(prjActivated);
    QString del_projPath = projPath.value(prjActivated);
    QString del_projWorkPath = projWorkPath.value(prjActivated);


    projName.remove(prjActivated);
    projPath.remove(prjActivated);
    projWorkPath.remove(prjActivated);
    projLastModifiedTime.remove(prjActivated);
    projCreationDate.remove(prjActivated);


    //raise of 1 the next rows in the list

    for(int i = prjActivated; i < size; ++i){

        projName.replace(i, projName.value(i + 1));
        projPath.replace(i, projPath.value(i + 1));
        projWorkPath.replace(i, projWorkPath.value(i + 1));
        projLastModifiedTime.replace(i, projLastModifiedTime.value(i + 1));
        projCreationDate.replace(i, projCreationDate.value(i + 1));
    }

    projName.remove(size);
    projPath.remove(size);
    projWorkPath.remove(size);
    projLastModifiedTime.remove(size);
    projCreationDate.remove(size);



    settings.remove("Manager/projects");


    int finalSize = size - 1;


    settings.beginWriteArray("Manager/projects");
    for (int i = 0; i < finalSize; ++i) {

        settings.setArrayIndex(i);
        settings.setValue("name", projName.value(i));
        settings.setValue("path", projPath.value(i));
        settings.setValue("workPath", projWorkPath.value(i));
        settings.setValue("lastModified", projLastModifiedTime.value(i));
        settings.setValue("creationDate", projCreationDate.value(i));
    }
    settings.endArray();


    if(del_projWorkPath =="OBSOLETE"){
        QFile file(del_projPath + "/" + del_projName +".plume");
        file.remove();
        QFile bckFile(del_projPath + "/" + del_projName +".plume_backup");
        bckFile.remove();
    }
        else  // compatibility for old system :
    removeDir(del_projWorkPath);






    QMessageBox deleteInfoBox(this);
    deleteInfoBox.setText("The project '" + del_projName + "' has been properly deleted !");
    deleteInfoBox.exec();

    emit openPrjManagerSignal();
}

//-----------------------------------------------------------------------------------

bool PrjManager::removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }

    return result;
}

//-----------------------------------------------------------------------------------

void PrjManager::renamePrj()
{

    if(table->isHidden())
        return;

    if(noProjectAlreadyOpened == false){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("You can't rename while a project is opened !<br> Please close the current project and try again."));
        msgBox.exec();

        return;
    }


    QString oldName = table->item(currentRow, 0)->data(0).toString();

    // Question :


    renameDialog = new QDialog(this);
    renameDialog->setWindowTitle(tr("Rename Project"));
    QVBoxLayout *layout = new QVBoxLayout;

    QLabel *renameText = new QLabel(tr("Type a new name for the selected project :"));
    renameEdit = new QLineEdit(oldName);
    renameEdit->setValidator(new QRegExpValidator(QRegExp("[^\x002F\\\\:\x002A\?\x0022<>|]+"), renameEdit));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                       | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptRenaming()));
    connect(buttonBox, SIGNAL(rejected()), renameDialog, SLOT(close()));


    layout->addWidget(renameText);
    layout->addWidget(renameEdit);
    layout->addWidget(buttonBox);

    renameDialog->setLayout(layout);

    renameDialog->exec();

}

//-----------------------------------------------------------------------------------

void PrjManager::acceptRenaming()
{
    QString newName = renameEdit->text();
    QString oldName = table->item(currentRow, 0)->data(0).toString();
    QString prjPath = table->item(currentRow, 1)->data(0).toString();
    QString oldWorkPath = table->item(currentRow, 2)->data(0).toString();

    if (newName.size() == 0 || newName == table->item(currentRow, 0)->data(0).toString()){
        renameDialog->close();
        return;
    }





    // to change : .plume, .prjinfo (+ xml), .attend,  directory name, settings


    QFile *plumeFile = new QFile(oldWorkPath + "/" + oldName + ".plume");
    renameXML(plumeFile, oldName, newName);
    plumeFile->rename(oldWorkPath + "/" + newName + ".plume");
    plumeFile->close();

    QFile *attendFile = new QFile(oldWorkPath + "/" + oldName + ".attend");
    if(attendFile->exists()){
        attendFile->rename(oldWorkPath + "/" + newName + ".attend");
        attendFile->close();
    }

    QFile *prjinfoFile = new QFile(oldWorkPath + "/" + oldName + ".prjinfo");
    if(prjinfoFile->exists()){
        renameXML(prjinfoFile, oldName, newName);
        prjinfoFile->rename(oldWorkPath + "/" + newName + ".prjinfo");
        prjinfoFile->close();
    }

    QDir *dir = new QDir(prjPath + "/" + oldName);
    QString newPrjWorkPath = prjPath + "/" + newName;
    bool renameSuccess = dir->rename(prjPath + "/" + oldName, newPrjWorkPath);

//    qDebug() << dir->absolutePath();
//    qDebug() << "renameSuccess : " <<renameSuccess;

    QSettings settings;

    settings.beginWriteArray("Manager/projects");

    settings.setArrayIndex(prjActivated);
    settings.setValue("name", newName);
    settings.setValue("workPath", newPrjWorkPath);

    settings.endArray();





    table->item(currentRow, 0)->setText(newName);
    table->item(currentRow, 2)->setText(newPrjWorkPath);

    renameDialog->close();
}




void PrjManager::renameXML(QFile *xmlFile, QString oldName, QString newName)
{

    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument domDocument;

    if (!domDocument.setContent(xmlFile, true, &errorStr, &errorLine,
                                &errorColumn)) {
        QMessageBox::information(this, tr("Plume Creator Tree"),
                                 tr("Parse error at line %1, column %2:\n%3\n")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));

        //        qDebug() << "File path:" << device->fileName();
        //        qDebug() << "File readable:" << device->isReadable();
        //        qDebug() << "File open:" << device->isOpen();


        return;
    }

    //QFileInfo *dirInfo = new QFileInfo(*device);
    //devicePath = dirInfo->path();
    ////    qDebug() << "File path:" << devicePath;


    QDomElement root = domDocument.documentElement();


    if(root.attribute("projectName") == oldName)
        root.setAttribute("projectName", newName);

    //write :
    xmlFile->waitForBytesWritten(500);
    xmlFile->close();
    xmlFile->open(QFile::ReadWrite | QFile::Text | QFile::Truncate);
    if(xmlFile->isWritable())
    {
        xmlFile->flush();


        const int IndentSize = 4;

        QTextStream out(xmlFile);
        out.flush();
        domDocument.save(out, IndentSize);
        xmlFile->close();
    }

}
////$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
////$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$





//DeletePrjButton::DeletePrjButton(QWidget *parent) :
//    QPushButton(parent)
//{
//    setFixedSize(120, 30);
//    setText(tr("&Delete"));
//    connect(this,SIGNAL(clicked()),this,SLOT(deleteProjectQuestion()));


//}

////-----------------------------------------------------------------------------------


//void DeletePrjButton::deleteProjectNum(int numPrj)
//{
//    del_numPrj = numPrj;
//}

////-----------------------------------------------------------------------------------


//void DeletePrjButton::deleteProjectQuestion()
//{

//    QMessageBox deleteQuestion(this);
//    deleteQuestion.setIcon(QMessageBox::Warning);
//    deleteQuestion.setText("This project will be deleted.");
//    deleteQuestion.setInformativeText("Are you really sure to delete this project ?");
//    deleteQuestion.setStandardButtons(QMessageBox::Cancel);
//    QPushButton *delButton = deleteQuestion.addButton(tr("Delete project"), QMessageBox::ActionRole);
//    deleteQuestion.setDefaultButton(QMessageBox::Cancel);
//    deleteQuestion.exec();

//    if (deleteQuestion.clickedButton() == delButton) {

//        delProject();



//    }
//}


////-----------------------------------------------------------------------------------


//void DeletePrjButton::delProject()
//{


//    QMultiMap<int, QString> projName, projPath, projWorkPath, projLastModifiedTime, projCreationDate;


//    QSettings settings;

//    int size = settings.value("Manager/projects/size").toInt();

//    settings.beginReadArray("Manager/projects");

//    for (int i = 0; i < size; ++i) {
//        settings.setArrayIndex(i);

//        projName.insert(i, settings.value("name").toString());
//        projPath.insert(i, settings.value("path").toString());
//        projWorkPath.insert(i, settings.value("workPath").toString());
//        projLastModifiedTime.insert(i, settings.value("lastModified").toString());
//        projCreationDate.insert(i, settings.value("creationDate").toString());
//    }
//    settings.endArray();

//    QString del_projName = projWorkPath.value(del_numPrj);
//    QString del_projWorkPath = projWorkPath.value(del_numPrj);


//    projName.remove(del_numPrj);
//    projPath.remove(del_numPrj);
//    projWorkPath.remove(del_numPrj);
//    projLastModifiedTime.remove(del_numPrj);
//    projCreationDate.remove(del_numPrj);


//    //raise of 1 the next rows in the list

//    for(int i = del_numPrj; i < size; ++i){

//        projName.replace(i, projName.value(i + 1));
//        projPath.replace(i, projPath.value(i + 1));
//        projWorkPath.replace(i, projWorkPath.value(i + 1));
//        projLastModifiedTime.replace(i, projLastModifiedTime.value(i + 1));
//        projCreationDate.replace(i, projCreationDate.value(i + 1));
//    }

//    projName.remove(size);
//    projPath.remove(size);
//    projWorkPath.remove(size);
//    projLastModifiedTime.remove(size);
//    projCreationDate.remove(size);



//    settings.remove("Manager/projects");


//    int finalSize = size - 1;


//    settings.beginWriteArray("Manager/projects");
//    for (int i = 0; i < finalSize; ++i) {

//        settings.setArrayIndex(i);
//        settings.setValue("name", projName.value(i));
//        settings.setValue("path", projPath.value(i));
//        settings.setValue("workPath", projWorkPath.value(i));
//        settings.setValue("lastModified", projLastModifiedTime.value(i));
//        settings.setValue("creationDate", projCreationDate.value(i));
//    }
//    settings.endArray();



//    removeDir(del_projWorkPath);






//    QMessageBox deleteInfoBox(this);
//    deleteInfoBox.setText("The project '" + del_projName + "' has been properly deleted !");
//    deleteInfoBox.exec();

//    emit openProjectManagerSignal();
//}

////-----------------------------------------------------------------------------------

//bool DeletePrjButton::removeDir(const QString &dirName)
//{
//    bool result = true;
//    QDir dir(dirName);

//    if (dir.exists(dirName)) {
//        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
//            if (info.isDir()) {
//                result = removeDir(info.absoluteFilePath());
//            }
//            else {
//                result = QFile::remove(info.absoluteFilePath());
//            }

//            if (!result) {
//                return result;
//            }
//        }
//        result = dir.rmdir(dirName);
//    }

//    return result;
//}






////$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
////$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$




//OpenPrjButton::OpenPrjButton(QWidget *parent) :
//    QPushButton(parent)
//{
//    setFixedSize(120, 30);
//    setText(tr("&Open"));
//    connect(this,SIGNAL(clicked()),this,SLOT(opProject()));
//}

////-----------------------------------------------------------------------------------

//void OpenPrjButton::openProjectFile(QFile *device)
//{
//    prjFile = device;

//}

////-----------------------------------------------------------------------------------

//void OpenPrjButton::openProjectSettingNumber(int prjNumber)
//{
//    projectNumber = prjNumber;
//}

////-----------------------------------------------------------------------------------

//void OpenPrjButton::opProject()
//{
//    emit openProjectSignal(prjFile);
//    emit openProjectNumberSignal(projectNumber);
//}






