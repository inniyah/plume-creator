#include "fileupdater.h"
#include "JlCompress.h"

FileUpdater::FileUpdater(QWidget *parent) :
    QWidget(parent), attendVersion("0.6"), treeVersion("0.3"), infoVersion("0.3")
{

}

bool FileUpdater::isZip(QString projectFileName)
{
    //necessary check to differenciate old .plume xml and new .plume zip files !

    QStringList list = JlCompress::getFileList(projectFileName);
    if(list.isEmpty())
        return false;
    else
        return true;
}


bool FileUpdater::checkAttendanceFile(QString projectFileName)
{
    QFile *device = new QFile(projectFileName);
    // read file :

    QFileInfo *dirInfo = new QFileInfo(*device);
    devicePath = dirInfo->path();

    QString docType;
    QStringList filters;
    QDir dir(devicePath);


    // check if the "attend" directory is here :

    filters.clear();
    filters << "attend";

    if(dir.entryList(filters, QDir::Dirs).isEmpty())
        dir.mkdir("attend");



    filters.clear();
    filters << "attendance";

    if(dir.entryList(filters, QDir::Files).isEmpty()){





        // check if the .attend file is here (old system) :
        filters.clear();
        filters << "*.attend";
QStringList list = dir.entryList(filters, QDir::Files);
        if(dir.entryList(filters, QDir::Files).isEmpty()){

            QString projectName = dir.dirName();



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




        attFile = new QFile(devicePath + "/" + dir.entryList(filters, QDir::Files).first());
        docType =  "attendance";

    }
    else{
        attFile = new QFile(devicePath + "/attendance");
        docType =  "plume-attendance";
    }

    //opening :


    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!attendDomDocument.setContent(attFile, true, &errorStr, &errorLine,
                                      &errorColumn)) {
        QMessageBox::information(this, tr("Plume Creator Tree"),
                                 tr("Parse error at line %1, column %2:\n%3\n")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));

                qDebug() << "File path:" << device->fileName();
                qDebug() << "File readable:" << device->isReadable();
                qDebug() << "File open:" << device->isOpen();


        return false;
    }

    attFile->close();


    attendRoot = attendDomDocument.documentElement();
    if (attendRoot.tagName() != docType) {
        QMessageBox::information(this, tr("Plume Creator Attendance"),
                                 tr("The file is not a a Plume Creator attendance file."));
        return false;
    } else if (attendRoot.hasAttribute("version")
               && attendRoot.attribute("version") < attendVersion) {


        this->updateAttendanceFile();

        return false;
    }


    return true;
}

void FileUpdater::updateAttendanceFile()
{

    QString oldVersion = attendRoot.attribute("version");
    QDomDocument newDomDocument;

    if(oldVersion == "0.1"){


        oldVersion = "0.2";

    }
    if(oldVersion == "0.2"){


        //change file name :
        newDomDocument = attendDomDocument;
        QDomNode newRoot = newDomDocument.documentElement();
        newRoot.toElement().setAttribute("version", "0.3");
        newRoot.toElement().setTagName("plume-attendance");

        attFile->remove();
        QFileInfo info(attFile->fileName());
        attFile->setFileName(info.absolutePath() + "/attendance");

        attendDomDocument = newDomDocument;



        oldVersion = "0.3";

    }
    if(oldVersion == "0.3"){



        newDomDocument = attendDomDocument;
        QDomNode newRoot = newDomDocument.documentElement();
        newRoot.toElement().setAttribute("version", "0.4");


        QDomElement charGroup = newDomDocument.createElement("group");
        charGroup.setAttribute("name", tr("Characters"));
        newRoot.appendChild(charGroup);
        QDomElement itemGroup = newDomDocument.createElement("group");
        itemGroup.setAttribute("name", tr("Items"));
        newRoot.appendChild(itemGroup);
        QDomElement placeGroup = newDomDocument.createElement("group");
        placeGroup.setAttribute("name", tr("Places"));
        newRoot.appendChild(placeGroup);



        QDomNodeList charList = attendDomDocument.elementsByTagName("char");
        for(int i = 0; i < charList.count(); ++i){
            charList.at(i).toElement().setAttribute("name", charList.at(i).toElement().attribute("firstName", "") + " " + charList.at(i).toElement().attribute("lastName", ""));
            charList.at(i).toElement().setTagName("obj");
            charGroup.appendChild(charList.at(i));

        }

        QDomNodeList itemList = attendDomDocument.elementsByTagName("item");
        for(int i = 0; i < itemList.count(); ++i){
            itemList.at(i).toElement().setTagName("obj");
            itemGroup.appendChild(itemList.at(i));

        }

        QDomNodeList placeList = attendDomDocument.elementsByTagName("place");
        for(int i = 0; i < placeList.count(); ++i){
            placeList.at(i).toElement().setTagName("obj");
            placeGroup.appendChild(placeList.at(i));

        }

        attendDomDocument = newDomDocument;

        oldVersion = "0.4";

    }
    if(oldVersion == "0.4"){

        newDomDocument = attendDomDocument;
        QDomElement newRoot = newDomDocument.documentElement().toElement();
        newRoot.setAttribute("version", "0.5");

        newRoot.setAttribute("box_1", newRoot.attribute("levelsNames", "nothing"));
        newRoot.setAttribute("box_2", newRoot.attribute("rolesNames", "nothing"));

        QDomNodeList list = attendDomDocument.elementsByTagName("obj");
        for(int i = 0; i < list.count(); ++i){
            list.at(i).toElement().setAttribute("box_1", list.at(i).toElement().attribute("level", "0"));
            list.at(i).toElement().setAttribute("box_2", list.at(i).toElement().attribute("role", "0"));
        }

        attendDomDocument = newDomDocument;


        oldVersion = "0.5";
    }
    if(oldVersion == "0.5"){

        newDomDocument = attendDomDocument;
        QDomElement newRoot = newDomDocument.documentElement().toElement();
        newRoot.setAttribute("version", "0.6");

        QList<int> intList;
        QDomNodeList list = attendDomDocument.elementsByTagName("obj");
        for(int i = 0; i < list.count(); ++i){
            intList.append(list.at(i).toElement().attribute("number", "1").toInt());
        }


        int maxNumber;
        if(!intList.isEmpty()){
            maxNumber = intList.first();
            QListIterator<int> i(intList);
            while (i.hasNext()){

                maxNumber = qMax(maxNumber, i.next());

            }
        }
        else
            maxNumber = 1;


        int groupNumber = maxNumber + 1;

        QDomNodeList groupList = attendDomDocument.elementsByTagName("group");
        for(int i = 0; i < groupList.count(); ++i){
            QDomElement element = groupList.at(i).toElement();
                    element.setAttribute("number",  groupNumber );

            // create docs :

                    element.setAttribute("attendPath", "/attend/A" + QString::number(groupNumber) + ".html");

            QFile *attendDocFile = new QFile(devicePath + element.attribute("attendPath"));


            QTextDocument *attendDocument = new QTextDocument();
            attendDocument->toHtml();
            QTextDocumentWriter docWriter(attendDocFile, "HTML");
            docWriter.write(attendDocument);


            delete attendDocument;

            groupNumber += 1;



        }




        attendDomDocument = newDomDocument;


        oldVersion = "0.6";

    }
    if(oldVersion == "0.6"){
        //        future updates
    }

    attFile->open(QIODevice::ReadWrite | QIODevice::Text | QFile::Truncate);
    attFile->write(attendDomDocument.toByteArray(3));

    attFile->close();
}




//-----------------------------------------------------------------------------------------------------------------
//---------------------------Tree : --------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------


bool FileUpdater::checkTreeFile(QString projectFileName)
{
    QString docType;
    QFile *device = new QFile(projectFileName);
    // read file :

    QFileInfo *dirInfo = new QFileInfo(*device);
    devicePath = dirInfo->path();


    QStringList filters;
    QDir dir(devicePath);


    // check if the "attend" directory is here :

    filters.clear();
    filters << "text";

    if(dir.entryList(filters, QDir::Dirs).isEmpty())
        dir.mkdir("text");

    filters.clear();
    filters << "tree";

    if(dir.entryList(filters, QDir::Files).isEmpty()){


        // check if the (old verison) .plume file is here :
        filters.clear();
        filters << "*.plume";

        //    if(dir.entryList(filters, QDir::Files).isEmpty()){
        //        filters.clear();
        //        filters << "*.plume";

        //        QFile plumeFile(dir.entryList(filters).first());
        //        QString projectName = plumeFile.fileName().split("/").last().remove(".plume");



        //        // create an empty file :

        //        QFile file(devicePath + "/" + projectName + ".plume");
        //        file.open(QIODevice::ReadWrite | QIODevice::Text);


        //        QDomDocument domDoc("plume-attendance");

        //        QDomElement root = domDoc.createElement("root");
        //        root.setTagName("plume");
        //        root.setAttribute("projectName", projectName);
        //        root.setAttribute("version","0.2");

        //        QString defaultLevelsNames = tr("Main") + "--" + tr("Secondary") + "--" + tr("None");
        //        root.setAttribute("levelsNames", defaultLevelsNames);
        //        QString defaultRolesNames = tr("None") + "--" + tr("Protagonist") + "--" + tr("Supporting") + "--" + tr("Neutral") + "--" + tr("Antagonist");
        //        root.setAttribute("rolesNames", defaultRolesNames);
        //        domDoc.appendChild(root);

        //        file.write(domDoc.toByteArray(3));
        //        file.close();
        //    }




        //    filters.clear();
        //    filters << "*.attend";


        treeFile = new QFile(devicePath + "/" + dir.entryList(filters).first());
        docType = "plume";
    }
    else{
        treeFile = new QFile(devicePath + "/tree");
        docType =  "plume-tree";
    }



    //opening :


    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!treeDomDocument.setContent(treeFile, true, &errorStr, &errorLine,
                                    &errorColumn)) {
        QMessageBox::information(this, tr("Plume Creator Tree File"),
                                 tr("Parse error at line %1, column %2:\n%3\n")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));

                qDebug() << "File path:" << device->fileName();
                qDebug() << "File readable:" << device->isReadable();
                qDebug() << "File open:" << device->isOpen();


        return false;
    }

    treeFile->close();


    treeRoot = treeDomDocument.documentElement();
    if (treeRoot.tagName() != docType) {
        QMessageBox::information(this, tr("Plume Creator Tree"),
                                 tr("The file is not a a Plume Creator tree file."));
        return false;
    } else if (treeRoot.hasAttribute("version")
               && treeRoot.attribute("version") < treeVersion) {


        this->updateTreeFile();

        return false;
    }


    return true;
}

void FileUpdater::updateTreeFile()
{

    QString oldVersion = treeRoot.attribute("version");
    QDomDocument newDomDocument;

    if(oldVersion == "0.2"){

        //change file name :
        newDomDocument = treeDomDocument;
        QDomNode newRoot = newDomDocument.documentElement();
        newRoot.toElement().setAttribute("version", "0.3");
        newRoot.toElement().setTagName("plume-tree");


        treeFile->remove();
        QFileInfo info(treeFile->fileName());
        treeFile->setFileName(info.absolutePath() + "/tree");


        treeDomDocument = newDomDocument;
        oldVersion = "0.3";

        //        qDebug() << domDocument.toByteArray(3);
    }
    if(oldVersion == "0.3"){

        //        future updates


    }


    treeFile->open(QIODevice::ReadWrite | QIODevice::Text | QFile::Truncate);
    treeFile->write(treeDomDocument.toByteArray(3));

    treeFile->close();
}




//-----------------------------------------------------------------------------------------------------------------
//---------------------------Info : --------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------


bool FileUpdater::checkInfoFile(QString projectFileName)
{
    QFile *device = new QFile(projectFileName);
    // read file :

    QFileInfo *dirInfo = new QFileInfo(*device);
    devicePath = dirInfo->path();


    QStringList filters;
    QDir dir(devicePath);


    filters.clear();
    filters << "info";

    if(dir.entryList(filters, QDir::Files).isEmpty()){


        // check if the (old verison) .info file is here :
        filters.clear();
        filters << "*.prjinfo";

        if(dir.entryList(filters, QDir::Files).isEmpty()){


            QString projectName = dir.dirName();


            // create an empty file :

            QFile file( dir.path() + "/" + projectName + ".prjinfo");
            file.waitForBytesWritten(500);
            file.close();
            file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);


            QDomDocument domDoc("plume-information");

            QDomElement root = domDoc.createElement("root");
            root.setTagName("plume-information");
            root.setAttribute("projectName", projectName);
            root.setAttribute("version","0.2");
            domDoc.appendChild(root);

            QDomElement prjInfoElem = domDoc.createElement("prj");
            prjInfoElem.setAttribute("name", projectName);
            prjInfoElem.setAttribute("path", dir.path().left(dir.path().size() - projectName.size() - 1 ));
            prjInfoElem.setAttribute("workPath", dir.path());
            prjInfoElem.setAttribute("lastModified", QDateTime::currentDateTime().toString());
            prjInfoElem.setAttribute("creationDate", QDateTime::currentDateTime().toString());
            root.appendChild(prjInfoElem);

            if(file.isWritable()){
                file.flush();

                const int IndentSize = 4;

                QTextStream out(&file);
                out.flush();
                domDoc.save(out, IndentSize);

            }


        }

        filters.clear();
        filters << "*.prjinfo";


        infoFile = new QFile(devicePath + "/" + dir.entryList(filters).first());
    }
    else
        infoFile = new QFile(devicePath + "/info");




    //opening :


    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!infoDomDocument.setContent(infoFile, true, &errorStr, &errorLine,
                                    &errorColumn)) {
        QMessageBox::information(this, tr("Plume Creator Info File"),
                                 tr("Parse error at line %1, column %2:\n%3\n")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));

        //        qDebug() << "File path:" << device->fileName();
        //        qDebug() << "File readable:" << device->isReadable();
        //        qDebug() << "File open:" << device->isOpen();


        return false;
    }

    infoFile->close();


    infoRoot = infoDomDocument.documentElement();
    if (infoRoot.tagName() != "plume-information") {
        QMessageBox::information(this, tr("Plume Creator Info"),
                                 tr("The file is not a a Plume Creator info file."));
        return false;
    } else if (infoRoot.hasAttribute("version")
               && infoRoot.attribute("version") < infoVersion) {


        this->updateInfoFile();

        return false;
    }


    return true;
}

void FileUpdater::updateInfoFile()
{

    QString oldVersion = infoRoot.attribute("version");
    QDomDocument newDomDocument;

    if(oldVersion == "0.2"){

        //change file name :
        newDomDocument = infoDomDocument;
        QDomNode newRoot = newDomDocument.documentElement();
        newRoot.toElement().setAttribute("version", "0.3");

        infoFile->remove();
        QFileInfo info(infoFile->fileName());
        infoFile->setFileName(info.absolutePath() + "/info");

        infoDomDocument = newDomDocument;
        oldVersion = "0.3";

        qDebug() << infoDomDocument.toByteArray(3);
    }
    if(oldVersion == "0.3"){

        //        future updates


    }


    infoFile->open(QIODevice::ReadWrite | QIODevice::Text | QFile::Truncate);
    infoFile->write(infoDomDocument.toByteArray(3));

    infoFile->close();
}
