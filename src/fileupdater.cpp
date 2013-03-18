#include "fileupdater.h"

FileUpdater::FileUpdater(QWidget *parent) :
    QWidget(parent), attendVersion("0.2")
{

}

bool FileUpdater::checkAttendanceFile(QString projectFileName)
{
    QFile *device = new QFile(projectFileName);
    // read file :

    QFileInfo *dirInfo = new QFileInfo(*device);
    devicePath = dirInfo->path();


    QStringList filters;
    QDir dir(devicePath);


    // check if the "attend" directory is here :

    filters.clear();
    filters << "attend";

    if(dir.entryList(filters, QDir::Dirs).isEmpty())
        dir.mkdir("attend");


    // check if the .attend file is here :
    filters.clear();
    filters << "*.attend";

    if(dir.entryList(filters, QDir::Files).isEmpty()){
        filters.clear();
        filters << "*.plume";

        QFile plumeFile(dir.entryList(filters).first());
        QString projectName = plumeFile.fileName().split("/").last().remove(".plume");



        // create an empty file :

        QFile file(devicePath + "/" + projectName + ".attend");
        file.open(QIODevice::ReadWrite | QIODevice::Text);


        QDomDocument domDoc("plume-attendance");

        QDomElement root = domDoc.createElement("root");
        root.setTagName("attendance");
        root.setAttribute("projectName", projectName);
        root.setAttribute("version","0.2");

        QString defaultLevelsNames = tr("Main") + "--" + tr("Secondary") + "--" + tr("None");
        root.setAttribute("levelsNames", defaultLevelsNames);
        QString defaultRolesNames = tr("None") + "--" + tr("Protagonist") + "--" + tr("Supporting") + "--" + tr("Neutral") + "--" + tr("Antagonist");
        root.setAttribute("rolesNames", defaultRolesNames);
        domDoc.appendChild(root);

        file.write(domDoc.toByteArray(3));
        file.close();
    }




    filters.clear();
    filters << "*.attend";


    attFile = new QFile(devicePath + "/" + dir.entryList(filters).first());




    //opening :


    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!domDocument.setContent(attFile, true, &errorStr, &errorLine,
                                &errorColumn)) {
        QMessageBox::information(this, tr("Plume Creator Tree"),
                                 tr("Parse error at line %1, column %2:\n%3\n")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));

        //        qDebug() << "File path:" << device->fileName();
        //        qDebug() << "File readable:" << device->isReadable();
        //        qDebug() << "File open:" << device->isOpen();


        return false;
    }

    attFile->close();


    root = domDocument.documentElement();
    if (root.tagName() != "attendance") {
        QMessageBox::information(this, tr("Plume Creator Tree"),
                                 tr("The file is not a a Plume Creator attendance file."));
        return false;
    } else if (root.hasAttribute("version")
               && root.attribute("version") < attendVersion) {


        this->updateAttendanceFile();

        return false;
    }


    return true;
}

void FileUpdater::updateAttendanceFile()
{

    QString oldVersion = root.attribute("version");
    QDomDocument newDomDocument;

    if(oldVersion == "0.1"){

        newDomDocument = domDocument;
        QDomNode newRoot = newDomDocument.documentElement();
        newRoot.toElement().setAttribute("version", "0.2");


        QDomElement charGroup = newDomDocument.createElement("group");
        charGroup.setAttribute("name", tr("Characters"));
        newRoot.appendChild(charGroup);
        QDomElement itemGroup = newDomDocument.createElement("group");
        itemGroup.setAttribute("name", tr("Items"));
        newRoot.appendChild(itemGroup);
        QDomElement placeGroup = newDomDocument.createElement("group");
        placeGroup.setAttribute("name", tr("Places"));
        newRoot.appendChild(placeGroup);



        QDomNodeList charList = domDocument.elementsByTagName("char");
        for(int i = 0; i < charList.count(); ++i){
            charList.at(i).toElement().setTagName("obj");
            charGroup.appendChild(charList.at(i));

        }

        QDomNodeList itemList = domDocument.elementsByTagName("item");
        for(int i = 0; i < itemList.count(); ++i){
            itemList.at(i).toElement().setTagName("obj");
            itemGroup.appendChild(itemList.at(i));

        }

        QDomNodeList placeList = domDocument.elementsByTagName("place");
        for(int i = 0; i < placeList.count(); ++i){
            placeList.at(i).toElement().setTagName("obj");
            placeGroup.appendChild(placeList.at(i));

        }

        domDocument = newDomDocument;
        oldVersion = "0.2";

//        qDebug() << domDocument.toByteArray(3);
    }
    if(oldVersion == "0.2"){
//        future updates
}

    attFile->open(QIODevice::ReadWrite | QIODevice::Text | QFile::Truncate);
    attFile->write(domDocument.toByteArray(3));

    attFile->close();
}
