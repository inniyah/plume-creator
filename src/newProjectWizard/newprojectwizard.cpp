#include <QFileDialog>

#include "newprojectwizard.h"
#include "intropage.h"
#include "selectpage.h"
#include "structurepage.h"


NewProjectWizard::NewProjectWizard(QWidget *parent) :
    QWizard(parent)
{


         this->resize(534, 365);
         this->setMinimumSize(QSize(400, 300));
         this->setMaximumSize(QSize(600, 400));
         QIcon icon;
         icon.addFile(QString::fromUtf8(":/pics/plume-creator.png"), QSize(), QIcon::Normal, QIcon::Off);
         this->setWindowIcon(icon);
         this->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
         this->setModal(true);
         this->setWizardStyle(QWizard::ModernStyle);

         this->setWindowTitle(tr("New Project"));


//         this->addPage(createIntroPage());
         this->addPage(createSelectPage());
            this->addPage(createStructurePage());

}


QHash<QString, int> NewProjectWizard::structureToCreate()
{
    return m_structureToCreate;
}

QString NewProjectWizard::newProjectFileName()
{
    return m_newProjectFileName;

}


QWizardPage *NewProjectWizard::createIntroPage()
{
    IntroPage *page = new IntroPage(this);



    return page;
}

QWizardPage *NewProjectWizard::createSelectPage()
{
    SelectPage *page = new SelectPage(this);



    return page;
}

QWizardPage *NewProjectWizard::createStructurePage()
{
    StructurePage *page = new StructurePage(this);



    return page;
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

    QDomElement trash = domDoc.createElement("trash");
    trash.setTagName("trash");
    trash.setAttribute("name", "");
    trash.setAttribute("number", "20000");
    root.appendChild(trash);

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
   infoDomDoc.appendChild(infoRoot);

    QDomElement prjInfoElem = infoDomDoc.createElement("prj");
    prjInfoElem.setAttribute("name", projectNameFinish);
    prjInfoElem.setAttribute("lastModified", QDateTime::currentDateTime().toString(Qt::ISODate));
    prjInfoElem.setAttribute("creationDate", QDateTime::currentDateTime().toString(Qt::ISODate));
    infoRoot.appendChild(prjInfoElem);


    infoFile.write(infoDomDoc.toByteArray(3));
    infoFile.close();




    // compress :



        JlCompress::compressDir(projectFileNameFinish, workingPath);






m_structureToCreate.insert("bookCount", field("bookCountSpinBox").toInt());
m_structureToCreate.insert("actCount", field("actCountSpinBox").toInt());
m_structureToCreate.insert("chapterCount", field("chapterCountSpinBox").toInt());
m_structureToCreate.insert("sceneCount", field("sceneCountSpinBox").toInt());



m_newProjectFileName = projectFileNameFinish;














    QDialog::accept();


}
