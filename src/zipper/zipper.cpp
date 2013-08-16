#include "zipper.h"
#include "JlCompress.h"

Zipper::Zipper(QObject *parent) :
    currentJob("extract"), oneTime(true)
{
}

void Zipper::setJob(QString job, QString fileName, QString workPath)
{
    currentJob = job;
    QFile file(fileName);
    m_fileName = file.fileName();
    QDir dir(workPath);
    m_workPath = dir.absolutePath();

    qDebug() << "##### JOB ##########";
    qDebug() << "job : " << currentJob;
    qDebug() << "file : " << m_fileName;
    qDebug() << "post path : " << workPath;
    qDebug() << "path : " << m_workPath;

}

void Zipper::setFileListToCheck(QStringList fileList)
{
    fileListToZip = fileList;


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

        //        QString projectName = zipFileName.split("/").last().remove(".plume");
        bool isCompressionSuccessful = JlCompress::compressDir(zipFileName, m_workPath);


        bool isZipGood = this->checkZip(zipFileName);

        if((!isCompressionSuccessful || !isZipGood) && oneTime){

            oneTime = false;
            qWarning() << "retrying to save one more time";
            this->compress(zipFileName);
        }
        else if((!isCompressionSuccessful || !isZipGood) && !oneTime){

            qWarning() << "ERROR : save failed ! Compress :" << isCompressionSuccessful <<
                          "; zip :" << isZipGood;


        }

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

bool Zipper::checkZip(QString zipFileName)
{

    QStringList filesList = JlCompress::getFileList(zipFileName);

    QStringList rootFileList;
    rootFileList << "attendance"
                 << "info"
                 << "tree";

    //    for (int i = 0 ; i < filesList.size(); ++i)
    //        qDebug()<< filesList.at(i);

    while(!rootFileList.isEmpty()){
        if(!filesList.contains(rootFileList.takeFirst()))
            return false;

    }

    return true;

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
            if(!ZipChecker::compareAndClean(m_workPath, fileListToZip)){
                qWarning() << "None of the listed files found. Saving operation failed" ;
                lock.unlock();
                emit zipFinished();

                return;
            }
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


