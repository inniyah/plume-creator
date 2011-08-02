#include <QtGui>
#include <QApplication>
#include <QSettings>


#include "prjmanager.h"



#include "newprojectwizard.h"



PrjManager::PrjManager(QWidget *parent) :
    QDialog(parent)
{


    setWindowTitle(tr("Projects Manager"));
    setFixedSize(600, 500);

    QLabel *managerLabel = new QLabel;
    managerLabel->setText(tr("Please select in the list below the project you want to work with :"));

    QWidget *managerList = new QWidget;

    QFrame *projList = new QFrame;
    QVBoxLayout *scrollAeraLayout = new QVBoxLayout;

    QPushButton *closeButton = new QPushButton(tr("&Close"));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(hide()));


    QVBoxLayout *layout = new QVBoxLayout;
    QBoxLayout *boxLayout = new QBoxLayout(QBoxLayout::LeftToRight);

    boxLayout->addStretch(10);
    boxLayout->addWidget(closeButton);

    layout->addWidget(managerLabel);
    layout->addWidget(managerList);

    displayProjects();

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //projList->setStyleSheet("* {border-style: inset; border-radius: 4px; border: 1px solid black; background-color: white}");
    projList->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    projList->setFrameShadow(QFrame::Sunken);
    projList->setLineWidth(3);
    projList->setMidLineWidth(3);

    projList->setLayout(projListLayout);
    scrollArea->setWidget(projList);

    scrollAeraLayout->addWidget(scrollArea);
    managerList->setLayout(scrollAeraLayout);

    layout->addLayout(boxLayout);

    setLayout(layout);



}

//-----------------------------------------------------------------------------------


void PrjManager::displayProjects(){


    QMultiMap<int, QString> projName, projPath, projWorkPath, projLastModifiedTime, projCreationDate;

    projListLayout = new QVBoxLayout;


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


        QString projNameI = projName.value(i);
        QString projPathI = projPath.value(i);
        QString projWorkPathI = projWorkPath.value(i);
        QString projLastModifiedTimeI = projLastModifiedTime.value(i);
        QString projCreationDateI = projCreationDate.value(i);



        stackItem = new QFrame;
        stackItem->setFixedSize(520, 150);
        stackItem->setFrameStyle(QFrame::Panel | QFrame::Raised);
        stackItem->setFrameShadow(QFrame::Raised);
        stackItem->setLineWidth(3);
        stackItem->setMidLineWidth(3);

        itemLayout = new QGridLayout;

        QWidget *stretcher = new QWidget;
        stretcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QLabel *nameLabel = new QLabel(tr("<b>Name :</b>"));
        nameLabel->setAlignment(Qt::AlignRight);
        QLabel *pathLabel = new QLabel(tr("<b>Path :</b>"));
        pathLabel->setAlignment(Qt::AlignRight);
        QLabel *lastModLabel = new QLabel(tr("<b>Last modified :</b>"));
        lastModLabel->setAlignment(Qt::AlignRight);
        QLabel *creaDateLabel = new QLabel(tr("<b>Creation date :</b>"));
        creaDateLabel->setAlignment(Qt::AlignRight);


        QLabel *projNameLabel = new QLabel(projNameI);
        QLabel *projPath = new QLabel(QDir::toNativeSeparators(projPathI));
        QLabel *projLastModifiedTime = new QLabel(projLastModifiedTimeI);
        QLabel *projCreationDate = new QLabel(projCreationDateI);

        openButton = new OpenPrjButton;
        QString proj(projWorkPathI + "/" + projNameI + ".plume");
        QFile *file = new QFile(proj);
        openButton->openProjectFile(file);
        connect(openButton, SIGNAL(openProjectSignal(QFile*)), this, SIGNAL(openProjectSignal(QFile*)));



        deleteButton = new DeletePrjButton;
        deleteButton->deleteProjectNum(i);


        connect(deleteButton, SIGNAL(openProjectManagerSignal()), this, SIGNAL(openPrjManagerSignal()));
       //is used to update the manager display

        itemLayout->addWidget(nameLabel,0,0);
        itemLayout->addWidget(projNameLabel,0,1);
        itemLayout->addWidget(pathLabel,1,0);
        itemLayout->addWidget(projPath,1,1);
        itemLayout->addWidget(creaDateLabel,2,0);
        itemLayout->addWidget(projCreationDate,2,1);
        itemLayout->addWidget(lastModLabel,3,0);
        itemLayout->addWidget(projLastModifiedTime,3,1);
        itemLayout->addWidget(stretcher,0,2,3,2);
        itemLayout->addWidget(openButton,0,3,1,3);
        itemLayout->addWidget(deleteButton,2,3,3,3);

        stackItem->setLayout(itemLayout);

        projListLayout->addWidget(stackItem);




    }



    settings.endArray();

    QFrame *stackNewPrj = new QFrame;
    stackNewPrj->setFixedSize(520, 150);
    stackNewPrj->setFrameStyle(QFrame::Panel | QFrame::Raised);
    stackNewPrj->setLineWidth(3);
    stackNewPrj->setMidLineWidth(3);

    QVBoxLayout *newPrjStackLayout = new QVBoxLayout;

    QLabel *newPrjLabel = new QLabel(tr("<b>You can also create a new project :</b>"));
    newPrjLabel->setAlignment(Qt::AlignCenter);
    QPushButton *newPrjButton = new QPushButton(tr("Add project"));
    connect(newPrjButton,SIGNAL(clicked()), this, SIGNAL(newPrjSignal()));

    newPrjStackLayout->addWidget(newPrjLabel);
    newPrjStackLayout->addWidget(newPrjButton);

    stackNewPrj->setLayout(newPrjStackLayout);

    projListLayout->addWidget(stackNewPrj);






}




//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$





DeletePrjButton::DeletePrjButton(QWidget *parent) :
    QPushButton(parent)
{
    setText(tr("&Delete"));
    connect(this,SIGNAL(clicked()),this,SLOT(deleteProjectQuestion()));


}

//-----------------------------------------------------------------------------------


void DeletePrjButton::deleteProjectNum(int numPrj)
{
    del_numPrj = numPrj;
}

//-----------------------------------------------------------------------------------


void DeletePrjButton::deleteProjectQuestion()
{

    QMessageBox deleteQuestion;
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


void DeletePrjButton::delProject()
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

    QString del_projName = projWorkPath.value(del_numPrj);
    QString del_projWorkPath = projWorkPath.value(del_numPrj);


    projName.remove(del_numPrj);
    projPath.remove(del_numPrj);
    projWorkPath.remove(del_numPrj);
    projLastModifiedTime.remove(del_numPrj);
    projCreationDate.remove(del_numPrj);


//raise of 1 the next rows in the list

    for(int i = del_numPrj; i < size; ++i){

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



    removeDir(del_projWorkPath);






    QMessageBox deleteInfoBox;
    deleteInfoBox.setText("The project '" + del_projName + "' has been properly deleted !");
    deleteInfoBox.exec();

    emit openProjectManagerSignal();
}

//-----------------------------------------------------------------------------------

bool DeletePrjButton::removeDir(const QString &dirName)
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






//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$




OpenPrjButton::OpenPrjButton(QWidget *parent) :
    QPushButton(parent)
{
    setText(tr("&Open"));
    connect(this,SIGNAL(clicked()),this,SLOT(opProject()));
}

//-----------------------------------------------------------------------------------

void OpenPrjButton::openProjectFile(QFile *device)
{
prjFile = device;

}

//-----------------------------------------------------------------------------------

void OpenPrjButton::opProject()
{
emit openProjectSignal(prjFile);

}

