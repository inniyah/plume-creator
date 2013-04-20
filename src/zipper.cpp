#include "zipper.h"
#include "JlCompress.h"

Zipper::Zipper(QObject *parent) :
    currentJob("extract")
{
}

void Zipper::setJob(QString job, QString fileName, QString workPath)
{
    currentJob = job;
    m_fileName = fileName;
    m_workPath = workPath;

}
void Zipper::compress(QString zipFileName)
{
    //make backup :
    QFile backupFile(zipFileName + "_backup");
    if(backupFile.exists())
            backupFile.remove();
    QFile file(zipFileName);
    file.copy(zipFileName, zipFileName + "_backup");


    // compress :

    QReadWriteLock lock;
    bool isLockingSuccessfull = lock.tryLockForWrite();

    if(isLockingSuccessfull == true){

        QString projectName = zipFileName.split("/").last().remove(".plume");
//    QFileInfo fileInfo(zipFileName);
//    JlCompress::compressDir(zipFileName, fileInfo.dir().path()  + "/." + projectName);
    JlCompress::compressDir(zipFileName, m_workPath);

    }
    else
        qWarning() << "Another part of the program is reading/writing. Loading operation failed" ;

    lock.unlock();


}

void Zipper::extract(QString zipFileName)
{
    //    QFileInfo fileInfo(fileName) ;


    //    QStringList list;
    //    list =  JlCompress::extractDir(fileName, fileInfo.dir().path()  + "/essai");

    //        qDebug() << "path : " + fileInfo.dir().path()  + "/essai" ;
    //        for(int i = 0; i < list.size(); ++i){
    //            qDebug() << list.at(i);
    //        }
    //        if(list.size() == 0)
    //            qDebug() << "file = 0";


//    qDebug() << zipFileName;


//    qDebug() <<  "fileInfo.dir().path() : "<< fileInfo.dir().path();


    QStringList list;
    list =  JlCompress::extractDir(zipFileName, m_workPath);

//    qDebug() << "path : " + fileInfo.dir().path()  + "/essai" ;
//    for(int i = 0; i < list.size(); ++i){
//        qDebug() << list.at(i);
//    }
//    if(list.size() == 0)
//        qDebug() << "file = 0";


//    QStringList list1;
//    list1 =  JlCompress::getFileList(zipFileName);

//    for(int i = 0; i < list1.size(); ++i){
//        qDebug() << "file = " << list1.at(i);
//    }
//    if(list1.size() == 0)
//        qDebug() << "file = 0";

}

void Zipper::run()
{
    emit zipStarted(currentJob);

//    qDebug() << "run : " << fileName_ ;

    QReadWriteLock lock;
    bool isLockingSuccessfull = lock.tryLockForWrite();
    if(isLockingSuccessfull == true){

        if(currentJob == "compress"){
//            qDebug() << "compressing...";
            this->compress(m_fileName);
        }
       else  if(currentJob == "extract"){
//            qDebug() << "extracting...";
            this->extract(m_fileName);
        }
    }
    else
        qWarning() << "Another part of the program is reading/writing. Saving operation failed" ;

    lock.unlock();


    emit zipFinished();

}
