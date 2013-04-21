#include "hub.h"

Hub::Hub(QWidget *parent) :
    QWidget(parent), refreshIsLocked(false), projectOpened(false),
    saveStack(0), m_currentSheetNumber(-1), m_wordGoal(1000), m_achievedWordGoal(0), m_baseWordCount(0)
{
}
//--------------------------------------------------------------------------------

void Hub::lockRefresh()
{
    refreshIsLocked = true;
}

//--------------------------------------------------------------------------------

void Hub::unlockRefresh()
{
    refreshIsLocked = false;
}
//--------------------------------------------------------------------------------

bool Hub::isRefreshLocked()
{
    return refreshIsLocked;
}



//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------



QString Hub::projectFileName() const
{
    return m_projectFileName;
}
//--------------------------------------------------------------------------------

void Hub::setProjectFileName(QString projectFileName)
{
    QString m_old_projectFileName = m_projectFileName;
    m_projectFileName = projectFileName;


    if(!refreshIsLocked)
        if(ChangesTests::test(m_old_projectFileName, m_projectFileName) == false)
            emit projectFileNameChanged();

    //    QFileInfo info(projectFileName);
    //    projectWorkingPath = info.absolutePath() + "/." + projectFileName.split("/").last().remove(".plume");
    projectWorkingPath = QDir::tempPath() + "/Plume/" + projectFileName.split("/").last().remove(".plume");


}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------


QString Hub::projectWorkPath() const
{
    return projectWorkingPath;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------


QString Hub::projectName() const
{
    return m_projectName;
}


//--------------------------------------------------------------------------------

void Hub::setProjectName(QString projectName)
{

    QString m_old_projectName = m_projectName;
    m_projectName = projectName;

    if(!refreshIsLocked)
        if(ChangesTests::test(m_old_projectName, m_projectName) == false)
            emit projectNameChanged();
}
//--------------------------------------------------------------------------------

bool Hub::isProjectOpened() const
{
    return projectOpened;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------












QDomDocument Hub::mainTreeDomDoc()
{
    return m_mainTreeDomDoc; // don't forget the data is shared. See Qt doc.
}


//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

QHash<MainTextDocument *, QFile *> Hub::mainTree_fileForDocHash()
{
    return m_mainTree_fileForDocHash;
}

void Hub::set_mainTree_fileForDocHash(QHash<MainTextDocument *, QFile *> fileForDoc)
{
    QHash<MainTextDocument *, QFile *> m_old_fileForDoc = m_mainTree_fileForDocHash;
    m_mainTree_fileForDocHash = fileForDoc;

    if(!refreshIsLocked)
        if(ChangesTests::test(m_old_fileForDoc, m_mainTree_fileForDocHash) == false)
            emit mainTree_fileForDocHashChanged();
}


//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

QHash<MainTextDocument *, int> Hub::mainTree_numForDocHash()
{
    return m_mainTree_numForDocHash;
}

void Hub::set_mainTree_numForDocHash(QHash<MainTextDocument *, int> numForDoc)
{
    QHash<MainTextDocument *, int> m_old_numForDoc = m_mainTree_numForDocHash;
    m_mainTree_numForDocHash = numForDoc;

    if(!refreshIsLocked)
        if(ChangesTests::test(m_old_numForDoc, m_mainTree_numForDocHash) == false)
            emit mainTree_numForDocHashChanged();
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

QHash<int, QDomElement> Hub::mainTree_domElementForNumberHash()
{
    return m_mainTree_domElementForNumberHash;
}

void Hub::set_mainTree_domElementForNumberHash(QHash<int, QDomElement> domElementForNumber)
{
    QHash<int, QDomElement> m_old_domElementForNumberHash = m_mainTree_domElementForNumberHash;
    m_mainTree_domElementForNumberHash = domElementForNumber;

    if(!refreshIsLocked)
        if(ChangesTests::test(m_old_domElementForNumberHash, m_mainTree_domElementForNumberHash) == false)
            emit mainTree_domElementForNumberHashChanged();
}






//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------








QDomDocument Hub::attendTreeDomDoc()
{
    return m_attendTreeDomDoc;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

QHash<QTextDocument *, QFile *> Hub::attendTree_fileForDocHash()
{
//    qDebug() << "m_attendTree_fileForDocHash : " << m_attendTree_fileForDocHash.size();
    return m_attendTree_fileForDocHash;
}

void Hub::set_attendTree_fileForDocHash(QHash<QTextDocument *, QFile *> fileForDoc)
{
    QHash<QTextDocument *, QFile *> m_old_fileForDoc = m_attendTree_fileForDocHash;
    m_attendTree_fileForDocHash = fileForDoc;

    if(!refreshIsLocked)
        if(ChangesTests::test(m_old_fileForDoc, m_attendTree_fileForDocHash) == false)
            emit attendTree_fileForDocHashChanged();
}


//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

QHash<QTextDocument *, int> Hub::attendTree_numForDocHash()
{
    return m_attendTree_numForDocHash;
}

void Hub::set_attendTree_numForDocHash(QHash<QTextDocument *, int> numForDoc)
{
    QHash<QTextDocument *, int> m_old_numForDoc = m_attendTree_numForDocHash;
    m_attendTree_numForDocHash = numForDoc;

    if(!refreshIsLocked)
        if(ChangesTests::test(m_old_numForDoc, m_attendTree_numForDocHash) == false)
            emit attendTree_numForDocHashChanged();
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

QHash<int, QDomElement> Hub::attendTree_domElementForNumberHash()
{
    return m_attendTree_domElementForNumberHash;
}

void Hub::set_attendTree_domElementForNumberHash(QHash<int, QDomElement> domElementForNumber)
{
    QHash<int, QDomElement> m_old_domElementForNumberHash = m_attendTree_domElementForNumberHash;
    m_attendTree_domElementForNumberHash = domElementForNumber;

    if(!refreshIsLocked)
        if(ChangesTests::test(m_old_domElementForNumberHash, m_attendTree_domElementForNumberHash) == false)
            emit attendTree_domElementForNumberHashChanged();
}













//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------




QDomDocument Hub::infoTreeDomDoc()
{
    return m_infoTreeDomDoc;
}

//--------------------------------------------------------------------------------



//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------


int Hub::currentSheetNumber() const
{
    return m_currentSheetNumber;
}


void Hub::setCurrentSheetNumber(int sheetNumber)
{
    int oldSheetNumber = m_currentSheetNumber;
    m_currentSheetNumber = sheetNumber ;


    if(!refreshIsLocked)
        if(ChangesTests::test(oldSheetNumber, m_currentSheetNumber) == false)
            emit currentSheetNumberChanged(m_currentSheetNumber);

}

//--------------------------------------------------------------------------------

int Hub::baseWordCount() const
{
    return m_baseWordCount;
}
void Hub::setBaseWordCount(int base)
{
    m_baseWordCount = base;
    emit baseWordCountSignal(base);
}
//--------------------------------------------------------------------------------

int Hub::wordGoal() const
{
    return m_wordGoal;
}
void Hub::setWordGoal(int goal)
{
    m_wordGoal = goal;
    emit wordGoalSignal(goal);
}
//--------------------------------------------------------------------------------
int Hub::achievedWordGoal() const
{
    return m_achievedWordGoal;
}
void Hub::setAchievedWordGoal(int achievedCount)
{


    if(achievedCount > m_wordGoal)
        this->setWordGoal(m_wordGoal + (achievedCount - m_wordGoal));
    if(achievedCount < 0 )
        m_achievedWordGoal = 0;

    m_achievedWordGoal = achievedCount;
    emit achievedWordGoalSignal(achievedCount);
}

//--------------------------------------------------------------------------------

bool Hub::isWordGoalActivated() const
{
    return m_isWordGoalActivated;
}
void Hub::setWordGoalActivated(bool wordGoalIsActivated)
{
    m_isWordGoalActivated = wordGoalIsActivated;
    emit wordGoalIsActivatedSignal(wordGoalIsActivated);
}

//--------------------------------------------------------------------------------

void Hub::calculatWordCountGoalDelta(int projectCount)
{

    if(projectCount - m_baseWordCount < 0)
        m_baseWordCount = projectCount;
    this->setAchievedWordGoal(projectCount - m_baseWordCount);
}


































//--------------------------------------------------------------------------------------
//------------------------------File Managment----------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

void Hub::startProject(QString file)
{

//    qDebug() << "startProject : " << file;

    //check if it's the right file :

    QFileInfo fileInfo(file);
    if(!fileInfo.fileName().contains(".plume") && !fileInfo.fileName().contains(".plume_backup")){
        int ret = QMessageBox::warning(0, tr("Plume creator"),
                                       tr("The document you are trying to open isn't a *.plume or *.plume_backup file !\n"),
                                       QMessageBox::Ok,
                                       QMessageBox::Ok);
        if(ret == QMessageBox::Ok)
            return;
    }



    if(fileInfo.fileName().contains(".plume_backup")){
        int ret = QMessageBox::warning(0, tr("Plume creator"),
                                       tr("<p>The document you are trying to open is *.plume_backup file !</p>"
                                          "<p>It's a backup created from the last save. This file must be opened in theses cases :</p>"
                                          "<blockquote>- Your computer crashed when saving, corrupting your work . </blockquote>"
                                          "<blockquote>- You can't load your work. </blockquote>"
                                          "<p>This will replace your work.</p>"
                                          "<p>Do you want to proceed ?</p>"),
                                       QMessageBox::Ok | QMessageBox::Cancel,
                                       QMessageBox::Cancel);
        if(ret == QMessageBox::Cancel)
            return;
        else
        {
            //remove the .plume if it exists
            QFile oldFile(file.remove("_backup"));
            if(oldFile.exists())
                oldFile.remove();
            //replace it by the backup
            QFile bckFile(file);
            bckFile.rename(file.remove("_backup"));

            file = bckFile.fileName();
        }


    }


    //check if a project is opened :

    if(projectOpened == true)
        this->closeCurrentProject();








    FileUpdater updater;
    bool zipBool = updater.isZip(file);

    if(zipBool == false){ // means its version is < 0.3
        updater.checkAttendanceFile(file);
        updater.checkInfoFile(file);
        updater.checkTreeFile(file);

        QFileInfo info(file);
        QDir directory = info.dir();
        directory.cdUp();
        JlCompress::compressDir(directory.absolutePath() + "/" + info.fileName(), info.absolutePath());

        file = directory.absolutePath() + "/" + info.fileName();


        //           update the project manager if project is in the manager :

        QSettings settings;
        int size = settings.value("Manager/projects/size").toInt();

        settings.beginWriteArray("Manager/projects");

        for (int i = 0; i < size; ++i) {
            settings.setArrayIndex(i);

            //            qDebug() << settings.value("name").toString();
            //            qDebug() << info.fileName().remove(".plume");
            //                        qDebug() << settings.value("path").toString();
            //                        qDebug() << directory.absolutePath();

            if(settings.value("name").toString() == info.fileName().remove(".plume")
                    && QDir::fromNativeSeparators(settings.value("path").toString()) == directory.absolutePath()){

                settings.setValue("workPath",  "OBSOLETE");
            }
        }
        settings.endArray();

        directory.setPath(directory.absolutePath());
        directory.rename(directory.absolutePath() + "/" + info.fileName().remove(".plume") +"/",
                         directory.absolutePath() + "/obsolete_" +  info.fileName().remove(".plume")+"/");

    }



    this->unlockFiles();
    this->setProjectFileName(file);



    loadProject();

    this->addProjectToPrjManager(this->projectFileName(), this->projectName(), QDateTime::currentDateTime());
// QDateTime::currentDateTime() is temporary;

    // wordCount :

    wcThread = new WordCountEngineThread(this);    // wordCount thread
    wcThread->set_mainTree_numForDocHash(this->mainTree_numForDocHash());
    wcThread->set_mainTree_DomDoc(this->mainTreeDomDoc());
    connect(this, SIGNAL(currentSheetNumberChanged(int)), wcThread, SLOT(changeCurrentSheetNumber(int)));
    connect(wcThread, SIGNAL(currentSheetWordCount(int)), this,  SIGNAL(currentSheetWordCount(int)));
    connect(wcThread, SIGNAL(projectWordCount(int)), this,  SIGNAL(projectWordCount(int)));
    connect(wcThread, SIGNAL(bookWordCount(int)), this,  SIGNAL(bookWordCount(int)));
    connect(wcThread, SIGNAL(chapterWordCount(int)), this,  SIGNAL(chapterWordCount(int)));
    connect(wcThread, SIGNAL(sceneWordCount(int)), this,  SIGNAL(sceneWordCount(int)));




    connectAllSheetsToWordCountThread();





    QFile *prjFile = new QFile(file);
    emit openProjectSignal(prjFile);

    projectOpened = true;

    // wordCount goal :

    this->setBaseWordCount(wcThread->projectWordCount());
    this->setAchievedWordGoal(0);
    this->setWordGoal(1000);
    this->setWordGoalActivated(false);
    connect(wcThread, SIGNAL(projectWordCount(int)), this, SLOT(calculatWordCountGoalDelta(int)));
}

//--------------------------------------------------------------------------------------

void Hub::closeCurrentProject()
{
    //temporary :
    emit closeProjectSignal();
    //
    this->stopSaveTimer();
    this->saveProject("wait");



    bool result = Utils::removeDir(projectWorkingPath);
    //    qDebug() << "result : "<< projectWorkingPath;
    //    qDebug() << "result : "<< result;



    //clear all the docs :


    QHash<MainTextDocument *, QFile *>::iterator i = m_mainTree_fileForDocHash.begin();

    while (i != m_mainTree_fileForDocHash.end()) {
        MainTextDocument *doc = i.key();
        doc->setObjectName("");

        ++i;
    }

    QHash<QTextDocument *, QFile *>::iterator j = m_attendTree_fileForDocHash.begin();

    while (j != m_attendTree_fileForDocHash.end()) {
        QTextDocument *doc = j.key();
        doc->setObjectName("");

        ++j;
    }


    QSettings settings;
    settings.beginWriteArray("Manager/projects");
    settings.setArrayIndex(settingNumber);
    settings.setValue("lastModified", QDateTime::currentDateTime().toString(Qt::ISODate));
    settings.endArray();



    m_mainTree_fileForDocHash.clear();
    refreshIsLocked = false;

    m_projectName.clear();
    m_projectFileName.clear();

    m_mainTreeDomDoc.clear();
    m_mainTree_fileForDocHash.clear();
    m_mainTree_numForDocHash.clear();

    m_infoTreeDomDoc.clear();

    m_attendTreeDomDoc.clear();
    m_attendTree_fileForDocHash.clear();
    m_attendTree_numForDocHash.clear();

    m_currentSheetNumber = -1;


    projectOpened = false;




}

//--------------------------------------------------------------------------------------

void Hub::loadProject()
{
    Zipper *zipper = new Zipper();

    zipper->setJob("extract", this->projectFileName(), projectWorkPath());
    //    connect(zipper, SIGNAL(finished()), this, SLOT(loadTemp()));
    zipper->start();
    zipper->wait(30000);
    this->loadTemp();
}
//--------------------------------------------------------------------------------------

bool Hub::loadTemp()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    this->lockRefresh();

    FileUpdater updater;
    updater.checkAttendanceFile(projectWorkingPath + "/tree");
    updater.checkTreeFile(projectWorkingPath + "/tree");
    updater.checkInfoFile(projectWorkingPath + "/tree");


//    qDebug() << "loading temporary files";

    // ----------------------------   tree :

    QFile *treeFile = new QFile(projectWorkingPath + "/tree");



    QDomDocument domDoc;
    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!domDoc.setContent(treeFile, true, &errorStr, &errorLine,
                           &errorColumn)) {
        QMessageBox::information(this , tr("Plume Creator"),
                                 tr("Tree File. Parse error at line %1, column %2:\n%3\n")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));



        return false;
    }

    QDomElement root = domDoc.documentElement();
    if (root.tagName() != "plume-tree") {
        QMessageBox::information(this, tr("Plume Creator"),
                                 tr("The file is not a Plume Creator tree file."));
        return false;
    } else if (root.hasAttribute("version")
               && root.attribute("version") != updater.getTreeVersion()) {
        QMessageBox::information(this, tr("Plume Creator"),
                                 tr("The file is not a Plume Creator tree file version ", "version added after that") + updater.getTreeVersion());

        return false;
    }

    m_mainTreeDomDoc = domDoc;




    treeFile->close();

    //  open all docs :




    this->loadTextDocs(root.elementsByTagName("book"));
    this->loadTextDocs(root.elementsByTagName("chapter"));
    this->loadTextDocs(root.elementsByTagName("scene"));

    //    qDebug() << "docs  : " <<  QString::number(this->mainTree_fileForDocHash().size());


    // ----------------------------   info :

    QFile *infoFile = new QFile(projectWorkingPath + "/info");




    QDomDocument infoDomDoc;

    if (!infoDomDoc.setContent(infoFile, true, &errorStr, &errorLine,
                               &errorColumn)) {
        QMessageBox::information(this , tr("Plume Creator"),
                                 tr("Info File. Parse error at line %1, column %2:\n%3\n")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));



        return false;
    }

    QDomElement infoRoot = infoDomDoc.documentElement();
    if (infoRoot.tagName() != "plume-information") {
        QMessageBox::information(this, tr("Plume Creator"),
                                 tr("The file is not a Plume Creator info file."));
        return false;
    } else if (infoRoot.hasAttribute("version")
               && infoRoot.attribute("version") != updater.getInfoVersion()) {
        QMessageBox::information(this, tr("Plume Creator"),
                                 tr("The file is not a Plume Creator info file version ", "version added after that") + updater.getInfoVersion());

        return false;
    }


    m_infoTreeDomDoc = infoDomDoc;


    infoFile->close();


    this->setProjectName(infoDomDoc.documentElement().firstChildElement("prj").toElement().attribute("name", "error"));



    //-------------------- Attend :



    QFile *attendFile = new QFile(projectWorkingPath + "/attendance");



    QDomDocument attendDomDoc;

    if (!attendDomDoc.setContent(attendFile, true, &errorStr, &errorLine,
                                 &errorColumn)) {
        QMessageBox::information(this , tr("Plume Creator"),
                                 tr("Attend File. Parse error at line %1, column %2:\n%3\n")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));



        return false;
    }



    QDomElement attendRoot = attendDomDoc.documentElement();


    if (attendRoot.tagName() != "plume-attendance") {
        QMessageBox::information(this, tr("Plume Creator"),
                                 tr("The file is not a Plume Creator attend file."));
        return false;
    } else if (attendRoot.hasAttribute("version")
               && attendRoot.attribute("version") != updater.getAttendVersion()) {
        QMessageBox::information(this, tr("Plume Creator"),
                                 tr("The file is not a Plume Creator attend file version ", "version added after that") + updater.getAttendVersion());

        return false;
    }


    m_attendTreeDomDoc = attendDomDoc;

    attendFile->close();


    //attend docs :





    this->loadAttendDocs(attendRoot.elementsByTagName("group"));
    this->loadAttendDocs(attendRoot.elementsByTagName("obj"));









    QApplication::restoreOverrideCursor();
    this->unlockRefresh();

    return true;
}

//--------------------------------------------------------------------------------------

void Hub::loadTextDocs(QDomNodeList list)
{
    //                 set up the progress bar :
    //    QWidget *progressWidget = new QWidget(this, Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    //    QHBoxLayout *progressLayout = new QHBoxLayout(progressWidget);
    //    QProgressBar *progressBar = new QProgressBar(progressWidget);
    //    int progressValue = 0;

    //    progressLayout->addWidget(progressBar);
    //    progressWidget->setLayout(progressLayout);

    //    progressBar->setMaximum(domDoc.elementsByTagName("book").size()
    //                            + domDoc.elementsByTagName("chapter").size()
    //                            + domDoc.elementsByTagName("scene").size()
    //                            );
    //    progressBar->setValue(progressValue);
    //    progressWidget->show();


    QDomElement element;
    QString textPath;
    QString synPath;
    QString notePath;


    for(int i = 0; i < list.size(); ++i){

        element = list.at(i).toElement();
        if(element.tagName() != "separator")
        {
            textPath = projectWorkingPath + element.attribute("textPath");
            synPath = projectWorkingPath + element.attribute("synPath");
            notePath = projectWorkingPath + element.attribute("notePath");
            QString number = element.attribute("number");

            QFile *textFile = new QFile;
            textFile->setFileName(textPath);
            textFile->open(QFile::ReadOnly | QFile::Text);
            QTextStream textFileStream( textFile );
            MainTextDocument *textDocument = new MainTextDocument(this);
            textDocument->setIdNumber(number.toInt());
            textDocument->setDocType("text");
            textDocument->setCursorPos(element.attribute("textPos", "0").toInt());
            textDocument->setHtml(textFileStream.readAll());
            textFile->close();
            textDocument->setObjectName("textDoc_" + number);
            m_mainTree_fileForDocHash.insert(textDocument, textFile);
            m_mainTree_numForDocHash.insert(textDocument, number.toInt());

            QFile *synFile = new QFile;
            synFile->setFileName(synPath);
            synFile->open(QFile::ReadOnly | QFile::Text);
            QTextStream synFileStream( synFile );
            MainTextDocument *synDocument = new MainTextDocument(this);
            synDocument->setIdNumber(number.toInt());
            synDocument->setDocType("synopsis");
            synDocument->setCursorPos(element.attribute("synPos", "0").toInt());
            synDocument->setHtml(synFileStream.readAll());
            synFile->close();
            synDocument->setObjectName("synDoc_" + number);
            m_mainTree_fileForDocHash.insert(synDocument, synFile);
            m_mainTree_numForDocHash.insert(synDocument, number.toInt());

            QFile *noteFile = new QFile;
            noteFile->setFileName(notePath);
            noteFile->open(QFile::ReadOnly | QFile::Text);
            QTextStream noteFileStream( noteFile );
            MainTextDocument *noteDocument = new MainTextDocument(this);
            noteDocument->setIdNumber(number.toInt());
            noteDocument->setDocType("note");
            noteDocument->setCursorPos(element.attribute("notePos", "0").toInt());
            noteDocument->setHtml(noteFileStream.readAll());
            noteFile->close();
            noteDocument->setObjectName("noteDoc_" + number);
            m_mainTree_fileForDocHash.insert(noteDocument, noteFile);
            m_mainTree_numForDocHash.insert(noteDocument, number.toInt());

            // wordCount :
            //            qDebug() << "doc opened : "<< element.attribute("number");

            //            QFile *file = fileForDoc.value(noteDocument);
            //            qDebug() << "saveDoc : " << file->fileName();
        }

        //        progressValue += 1;
        //        progressBar->setValue(progressValue);

        //        iterator->operator ++();

    }
}

//--------------------------------------------------------------------------------------
void Hub::loadAttendDocs(QDomNodeList list)
{

    QDomElement element;
    QString attendPath;



    for(int i = 0; i < list.size(); ++i){

        element = list.at(i).toElement();

        attendPath = projectWorkingPath + element.attribute("attendPath");

        QString number = element.attribute("number");

        QFile *attendFile = new QFile;
        attendFile->setFileName(attendPath);
        attendFile->open(QFile::ReadOnly | QFile::Text);
        QTextStream attendFileStream( attendFile );
        QTextDocument *textDocument = new QTextDocument(this);
        textDocument->setHtml(attendFileStream.readAll());
        attendFile->close();
        textDocument->setObjectName("attendDoc_" + number);
        m_attendTree_fileForDocHash.insert(textDocument, attendFile);
        m_attendTree_numForDocHash.insert(textDocument, number.toInt());

    }
}

//--------------------------------------------------------------------------------------

void Hub::saveProject(QString mode)
{


//    qDebug() << "saveProject";


    //    qDebug() << "Hub::areFileLocked() : "<< this->areFilesLocked();
    if(this->areFilesLocked() == true)
        return;

    this->lockFiles();
    //    qDebug() << "POST_Lock Hub::areFileLocked() : "<< this->areFilesLocked();

    this->saveTemp();

    Zipper *zipper = new Zipper();
    zipper->setJob("compress", this->projectFileName(), projectWorkPath());
    connect(zipper, SIGNAL(zipFinished()), this, SLOT(unlockFiles()));


    zipper->start();

    if(mode == "wait")
        zipper->wait(30000);

    //    this->showStatusBarMessage(tr("Project saved"));
}
//--------------------------------------------------------------------------------------

void Hub::saveTemp()
{

    emit savingSignal();

    // ------------------ Tree :

//    qDebug() << "save temporary files";

    QHash<MainTextDocument *, QFile *>::iterator i = m_mainTree_fileForDocHash.begin();
    while (i != m_mainTree_fileForDocHash.end()) {
        saveMainDoc(i.key() ,"mainTreeDocs" );
        ++i;
    }

    QFile *treeFile= new QFile(projectWorkingPath + "/tree");
    treeFile->waitForBytesWritten(500);
    treeFile->close();
    treeFile->open(QFile::ReadWrite | QFile::Text | QFile::Truncate);
    if(treeFile->isWritable())
    {
        treeFile->flush();


        const int IndentSize = 4;

        QTextStream out(treeFile);
        out.flush();
        this->mainTreeDomDoc().save(out, IndentSize);
        treeFile->close();

        //TextDocs :

    }



    //--------------- Info :

    QFile *infoFile= new QFile(projectWorkingPath + "/info");


    infoFile->waitForBytesWritten(500);

    infoFile->close();
    infoFile->open(QIODevice::ReadWrite | QIODevice::Text |QIODevice::Truncate	);
    if(infoFile->isWritable())
    {
        infoFile->flush();

        const int IndentSize = 4;


        QTextStream out(infoFile);
        out.flush();
        this->infoTreeDomDoc().save(out, IndentSize);
    }
    infoFile->close();







    //--------------------- Attend :

    QHash<QTextDocument *, QFile *>::iterator j =     m_attendTree_fileForDocHash.begin();

    while (j != m_attendTree_fileForDocHash.end()) {
        this->saveDoc(j.key(),"attendTreeDocs");

        ++j;
    }


    QFile *attFile= new QFile(projectWorkingPath + "/attendance");


    attFile->waitForBytesWritten(500);
    attFile->close();
    attFile->open(QFile::ReadWrite | QFile::Text | QFile::Truncate);
    if(attFile->isWritable())
    {
        attFile->flush();


        const int IndentSize = 4;

        QTextStream out(attFile);
        out.flush();
        this->attendTreeDomDoc().save(out, IndentSize);
        attFile->close();

        //        qDebug() << "save attendDomDocument()";
    }






}
//-----------------------------------------------------------------------------------------

bool Hub::saveMainDoc(MainTextDocument *doc, QString mode)
{
    if(mode != "mainTreeDocs")
        return false;

    QTextDocument *m_doc =  doc;

    QFile *file = new QFile;
    file = m_mainTree_fileForDocHash.value(doc);

    file->close();
    QTextDocumentWriter *docWriter = new QTextDocumentWriter(file, "HTML");
    QTextDocument *clonedDoc = m_doc->clone();
    bool written = docWriter->write(clonedDoc);

    QDomElement element = m_mainTree_domElementForNumberHash.value(doc->idNumber());
    if(doc->docType() == "text")
        element.setAttribute("textPos", doc->cursorPos());
    else if(doc->docType() == "synopsis")
        element.setAttribute("synPos", doc->cursorPos());
    else if(doc->docType() == "note")
        element.setAttribute("notePos", doc->cursorPos());


    delete docWriter;
    delete clonedDoc;

    return written;
}

bool Hub::saveDoc(QTextDocument *doc, QString mode)
{

    QFile *file = new QFile;
    //    if(mode == "mainTreeDocs"){
    //        qDebug() << "saveDoc error !";
    //return false;
    //    }
    //    else if(mode == "attendTreeDocs")
    //
    file = m_attendTree_fileForDocHash.value(doc);


    file->close();
    QTextDocumentWriter *docWriter = new QTextDocumentWriter(file, "HTML");
    QTextDocument *clonedDoc = doc->clone();
    bool written = docWriter->write(clonedDoc);



    delete docWriter;
    delete clonedDoc;

    return written;
}
//--------------------------------------------------------------------------------------
void Hub::lockFiles()
{
    filesLocked = true;
}

void Hub::unlockFiles()
{
    filesLocked = false;
}
bool Hub::areFilesLocked()
{
    return filesLocked;
}
//--------------------------------------------------------------------------------------

void Hub::addToSaveQueue()
{
    saveStack += 1;

    if(saveStack == 1)
        timerIdList.append(this->startTimer(2000));
    else if(saveStack > 1){  // reset if more than one save demand
        for(int i = 0; i < timerIdList.size(); ++i)
            this->killTimer(timerIdList.at(i));
        timerIdList.append(this->startTimer(2000));
    }



}
void Hub::stopSaveTimer()
{
    saveStack = 0;

    for(int i = 0; i < timerIdList.size(); ++i)
        this->killTimer(timerIdList.at(i));
}
void Hub::timerEvent(QTimerEvent *event)
{
    qDebug() << " -------- time to save !";
    this->saveProject();
    emit textAlreadyChangedSignal(false);
    stopSaveTimer();

}



void Hub::showStatusBarMessage(QString string, int time)
{
    emit showStatusBarMessageSignal(string, time);
}














void Hub::connectAllSheetsToWordCountThread()
{
    wcThread->set_mainTree_numForDocHash(m_mainTree_numForDocHash);

    QHash<MainTextDocument *, QFile *>::iterator i = m_mainTree_fileForDocHash.begin();
    while (i != m_mainTree_fileForDocHash.end()) {
        MainTextDocument *doc = i.key();
        if(doc->docType() == "text")
            connect(doc, SIGNAL(wordCountChanged(QString,int,int)), wcThread, SLOT(start()), Qt::UniqueConnection);
        ++i;
    }

}





void Hub::addProjectToPrjManager(QString fileName, QString _name, QDateTime creationDate)
{
QString name = fileName.split("/").last().remove(".plume");

    QSettings settings;

    settings.beginGroup("Manager/projects");

    QStringList groups = settings.childGroups();
    int size = groups.size();
    settings.setValue("size", size);
    settings.endGroup();

    // check if the project already exist :
    QString iName;
    QString iPath;
QDir iDir;
    QDir dir;
    QFileInfo info(fileName);
        //    projectWorkingPath = info.absolutePath()
    for(int i = 0 ; i < size; ++i){
    settings.beginReadArray("Manager/projects");
    settings.setArrayIndex(i);
    iName = settings.value("name").toString();
    iPath = settings.value("path").toString();
    settings.endArray();
iDir.setPath(iPath);
dir.setPath(info.absolutePath());
    if(iName == name && iDir == dir){
        settingNumber = i;
        return;
    }

    }







    // add project


    settingNumber = size;

    settings.beginWriteArray("Manager/projects");

    settings.setArrayIndex(size);
    settings.setValue("name", name);
    settings.setValue("path", info.absolutePath());
    settings.setValue("workPath", "OBSOLETE");
    settings.setValue("lastModified", QDateTime::currentDateTime().toString(Qt::ISODate));
    settings.setValue("creationDate", creationDate.toString(Qt::ISODate));

    settings.endArray();

}

























