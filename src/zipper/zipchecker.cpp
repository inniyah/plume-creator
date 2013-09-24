#include "zipchecker.h"

ZipChecker::ZipChecker(QObject *parent) :
    QObject(parent)
{




    baseList << "tree" << "info" << "attendance"
             << "attend/"
             << "text/"
             << "dicts/" << "dicts/userDict.dict_plume" ;




}

//-----------------------------------------------------------------------------------------

QStringList ZipChecker::list()
{
    QStringList allList;
    allList.append(baseList);
    allList.append(textsList);

    return allList;
}


//-----------------------------------------------------------------------------------------

void ZipChecker::addFile(QString type, int idNumber)
{
    QString path;
    QString firstLetter;
    QString endLetters = ".html";

    if(type == "note"){
        path = "text/";
        firstLetter = "N";
    }
    if(type == "syn"){
        path = "text/";
        firstLetter = "S";
    }
    if(type == "text"){
        path = "text/";
        firstLetter = "T";
    }
    if(type == "attend"){
        path = "attend/";
        firstLetter = "A";
    }

    textsList.append(path + firstLetter + QString::number(idNumber) + endLetters);
}


//-----------------------------------------------------------------------------------------

void ZipChecker::removeFile(QString type, int idNumber)
{
    QString path;
    QString firstLetter;
    QString endLetters = ".html";

    if(type == "note"){
        path = "text/";
        firstLetter = "N";
    }
    if(type == "syn"){
        path = "text/";
        firstLetter = "S";
    }
    if(type == "text"){
        path = "text/";
        firstLetter = "T";
    }
    if(type == "attend"){
        path = "attend/";
        firstLetter = "A";
    }


    textsList.removeOne(path + firstLetter + QString::number(idNumber) + endLetters);

}


//-----------------------------------------------------------------------------------------

void ZipChecker::clearList()
{
    textsList.clear();
}
//-----------------------------------------------------------------------------------------

bool ZipChecker::compareAndClean(QString tempDirectory, QStringList fileList)
{



    int listOfSuccess = 0;
    for(int i = 0; i < fileList.size(); ++i){
        QFile file(tempDirectory + "/" + fileList.at(i));
        if(file.exists())
            listOfSuccess+=1;
    }


    if(listOfSuccess == 0)
        return false;



    //    comapre before cleaning :

    QDir tempDir(tempDirectory);

    QStringList tempList;
    tempList.append(tempDir.entryList(QDir::Files | QDir::NoSymLinks));

    QStringList tempDirsList;
    tempDirsList.append(tempDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks));


    QHash<QString, QString> dirsContent;

    foreach (QString dirName, tempDirsList) {
        QDir dir(tempDirectory + "/" + dirName);
        QStringList contentList = dir.entryList( QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

        for(int j = 0; j < contentList.size() ; ++j )
            dirsContent.insertMulti(dirName, contentList.at(j));

    }

    for(int k = 0; k < tempDirsList.size() ; ++k ){
        QString name = tempDirsList.at(k);
        tempDirsList.replace(k, name + "/");
    }
    tempList.append(tempDirsList);


    QHash<QString, QString>::const_iterator m = dirsContent.constBegin();
    while (m != dirsContent.constEnd()) {

        tempList.append(m.key() + "/" + m.value());

        ++m;
    }



    // clean :

    fileList.removeDuplicates();
    foreach (QString string, tempList) {
        if(fileList.contains(string))
            tempList.removeOne(string);
    }

    if(!tempDirectory.contains(QDir::tempPath() + "/Plume/"))
        return false;

    foreach (QString string, tempList) {
        QString fileString = tempDirectory + "/" + string;
        QFileInfo fileInfo(fileString);
        if(fileInfo.isDir())
            Utils::removeDir(fileString);
        else{
            QFile file(fileString);
            file.remove();
        }
    }


    return true;
}




//-----------------------------------------------------------------------------------------
