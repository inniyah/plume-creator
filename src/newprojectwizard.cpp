#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QDir>


#include "newprojectwizard.h"
//
NewProjectWizard::NewProjectWizard(QWidget *parent)
    : QWizard(parent)
{
    addPage(new IntroPage);
    addPage(new SelectPage);

    setWindowTitle(tr("Project Creator"));

}

QHash<QString, int> NewProjectWizard::structureToCreate()
{
    return m_structureToCreate;
}

QString NewProjectWizard::newProjectFileName()
{
    return m_newProjectFileName;

}
void NewProjectWizard::accept()

{

    QString projectNameFinish = field("projectNameField").toString();
    QString projectDirectoryFinish = field("projectDirectoryField").toString().toUtf8();
    QString projectFileNameFinish = projectDirectoryFinish + "/" + field("projectNameField").toString() + ".plume";
    QString workingPath = QDir::tempPath() + "/newProject";

// cleaning :
    Utils::removeDir(workingPath);


    // Creation of the doc using DOM


    QDir path;
    path.mkpath(workingPath);
    path.setCurrent(workingPath);

//    path.mkdir("objects");
    path.mkdir("text");
    path.mkdir("attend");
    path.mkdir("dicts");


// create tree :

    QDomDocument domDoc("plume-tree");

    QFile file("tree");
    file.open(QIODevice::ReadWrite | QIODevice::Text);

    QDomElement root = domDoc.createElement("root");
    root.setTagName("plume-tree");
    root.setAttribute("projectName", projectNameFinish);
    root.setAttribute("version","0.5");
    domDoc.appendChild(root);


    file.write(domDoc.toByteArray(3));
    file.close();




    // create attend :


    QDomDocument attendDomDoc("plume-attendance");

    QFile attendFile("attendance");
    attendFile.open(QIODevice::ReadWrite | QIODevice::Text);

    QDomElement attendRoot = attendDomDoc.createElement("attendance");
    attendRoot.setTagName("plume-attendance");
    attendRoot.setAttribute("projectName", projectNameFinish);
    QString defaultLevelsNames = tr("Main") + "--" + tr("Secondary") + "--" + tr("None");
    attendRoot.setAttribute("box_1", defaultLevelsNames);
    QString defaultRolesNames = tr("None") + "--" + tr("Protagonist") + "--" + tr("Supporting") + "--" + tr("Neutral") + "--" + tr("Antagonist");
    attendRoot.setAttribute("box_2", defaultRolesNames);
    attendRoot.setAttribute("version","0.5");
    attendDomDoc.appendChild(attendRoot);

    QDomElement charGroup = attendDomDoc.createElement("group");
    charGroup.setAttribute("name", tr("Characters"));
    attendRoot.appendChild(charGroup);
    QDomElement itemGroup = attendDomDoc.createElement("group");
    itemGroup.setAttribute("name", tr("Items"));
    attendRoot.appendChild(itemGroup);
    QDomElement placeGroup = attendDomDoc.createElement("group");
    placeGroup.setAttribute("name", tr("Places"));
    attendRoot.appendChild(placeGroup);

    int groupNumber = 1;

    QDomNodeList groupList = attendDomDoc.elementsByTagName("group");
    for(int i = 0; i < groupList.count(); ++i){
        QDomElement element = groupList.at(i).toElement();
        element.setAttribute("number",  groupNumber );

        // create docs :

        element.setAttribute("attendPath", "/attend/A" + QString::number(groupNumber) + ".html");

        QFile *attendDocFile = new QFile(workingPath + element.attribute("attendPath"));


        QTextDocument *attendDocument = new QTextDocument();
        attendDocument->toHtml();
        QTextDocumentWriter docWriter(attendDocFile, "HTML");
        docWriter.write(attendDocument);


        delete attendDocument;

        groupNumber += 1;



    }


    attendFile.write(attendDomDoc.toByteArray(3));
    attendFile.close();




    // create info



    QFile infoFile( "info");
    infoFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);


    QDomDocument infoDomDoc("plume-information");

    QDomElement infoRoot = infoDomDoc.createElement("root");
    infoRoot.setTagName("plume-information");
    infoRoot.setAttribute("projectName", projectNameFinish);
    infoRoot.setAttribute("version","0.2");
    infoRoot.setAttribute("lastModified", QDateTime::currentDateTime().toString(Qt::ISODate));
    infoRoot.setAttribute("creationDate", QDateTime::currentDateTime().toString(Qt::ISODate));
    infoDomDoc.appendChild(infoRoot);


    infoFile.write(infoDomDoc.toByteArray(3));
    infoFile.close();




    // compress :

    QReadWriteLock lock;
    bool isLockingSuccessfull = lock.tryLockForWrite();

    if(isLockingSuccessfull == true){


        JlCompress::compressDir(projectFileNameFinish, workingPath);
}


    lock.unlock();







    //Saving to .conf the new project :
    QFile zipFile(projectFileNameFinish);
    Utils::addProjectToSettingsArray(zipFile.fileName());

    m_newProjectFileName = projectFileNameFinish;

//    QSettings settings;

//    int size = settings.value("Manager/projects/size", 0).toInt();


//    settings.beginWriteArray("Manager/projects");

//    settings.setArrayIndex(size);
//    settings.setValue("name", projectNameFinish);
//    settings.setValue("path", projectDirectoryFinish);
//    settings.setValue("workPath", workingPath);
//    settings.setValue("lastModified", QDateTime::currentDateTime().toString(Qt::ISODate));
//    settings.setValue("creationDate", creationDate);

//    settings.endArray();

    QDialog::accept();


}



IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("New Project"));
    setSubTitle(tr("Welcome to Plume Creator !"));

    label = new QLabel(tr("This assistant will help you create a new project ."));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

void IntroPage::initializePage()
{

}


SelectPage::SelectPage(QWidget *parent)
    : QWizardPage(parent)
{


    setTitle(tr("New Project"));
    setSubTitle(tr("Specify the path of your new project. "
                   "Please note you must have writing rights in it. "
                   "Plume Creator will create a basic skeleton."));


    //setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo1.png"));

    label = new QLabel(tr("Please complete the fields below: "));
    label->setWordWrap(true);

    projectNameLabel = new QLabel(tr("Project name :"));
    projectNameLabelLineEdit = new QLineEdit;
    projectNameLabelLineEdit->setValidator(new QRegExpValidator(QRegExp("[^\x002F\\\\:\x002A\?\x0022<>|]+"), projectNameLabelLineEdit));

    //    directoryLabel = new QLabel(tr("Path : "));
    directoryLabelLineEdit = new QLineEdit;
    QPushButton *directoryButton = new QPushButton(tr("Select path"));
    directoryButton->setMaximumWidth(100);

    connect(directoryButton, SIGNAL(clicked()), this, SLOT(setExistingDirectory()));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(label, 0, 1);
    //    layout->addWidget(directoryLabel, 1, 0);
    layout->addWidget(projectNameLabel, 1, 0);
    layout->addWidget(projectNameLabelLineEdit, 1, 1);
    layout->addWidget(directoryButton, 2, 0);
    layout->addWidget(directoryLabelLineEdit, 2, 1);
    setLayout(layout);




    registerField("projectNameField*", projectNameLabelLineEdit);
    registerField("projectDirectoryField", directoryLabelLineEdit);
}
void SelectPage::initializePage()
{
    directoryLabelLineEdit->setText(QDir::toNativeSeparators(QDir::homePath()));
}


void SelectPage::setExistingDirectory()
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    QString directory = QFileDialog::getExistingDirectory(this, tr("QFileDialog::getExistingDirectory()"),
                                                          directoryLabelLineEdit->text(),
                                                          options);
    if (!directory.isEmpty())
        directoryLabelLineEdit->setText(directory);
}
