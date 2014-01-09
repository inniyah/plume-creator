#include "hub.h"

Hub::Hub(QWidget *parent) :
    QWidget(parent),
    saveStack(0), m_wordGoal(1000), m_achievedWordGoal(0), m_baseWordCount(0),
    m_userDict(QString())
{
    m_project = new Project(this);
    wcThread = new WordCountEngineThread(this);    // wordCount thread
    zipChecker = new ZipChecker(this);
    m_themes = new Themes(this);

    m_spellChecker = new SpellChecker(this);
    //    m_spellChecker->setDict(this->spellDictPath(), this->userDict(), m_attendTree_names);

    QSettings settings;
    m_themes->loadTheme(settings.value("Themes/theme", "").toString());








}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

Project *Hub::project()
{
    return m_project;
}


























int Hub::baseWordCount() const
{
    return m_baseWordCount;
}
void Hub::setBaseWordCount(int base)
{
    m_baseWordCount = base;
    //    emit baseWordCountSignal(base);
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

//--------------------------------------------------------------------------------

QStringList Hub::userDict()
{
    return m_userDict;
}

void Hub::setUserDict(QStringList userDict)
{
    m_userDict = userDict;


    emit spellDictsChangedSignal(m_spellDictPath , m_userDict);

    this->addToSaveQueue();
}
//--------------------------------------------------------------------------------
QString Hub::spellDictPath()
{
    return m_spellDictPath;
}

void Hub::setSpellDictPath(QString spellDictPath)
{
    m_spellDictPath = spellDictPath;
}
//--------------------------------------------------------------------------------
SpellChecker *Hub::spellChecker()
{
    return m_spellChecker;
}
//--------------------------------------------------------------------------------
void Hub::openSheet(int id)
{
    emit textAndNoteSignal(id, "open");

}

//--------------------------------------------------------------------------------


Themes *Hub::themes()
{
    return m_themes;
}




























//--------------------------------------------------------------------------------------
//------------------------------File Managment----------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

bool Hub::startProject(QString file)
{
    QFile checkFile(file);
    if(!checkFile.exists()){
        int ret = QMessageBox::warning(0, tr("Plume creator"),
                                       tr("The document ")+ file + tr(" you are trying to open doesn't exist !\n"),
                                       QMessageBox::Ok,
                                       QMessageBox::Ok);
        if(ret == QMessageBox::Ok)
            return false;
    }
    if(!file.contains(".plume")){
        int ret = QMessageBox::warning(0, tr("Plume creator"),
                                       tr("The document ")+ file + tr(" you are trying to open isn't a *.plume or .plume_backup file !\n"),
                                       QMessageBox::Ok,
                                       QMessageBox::Ok);
        if(ret == QMessageBox::Ok)
            return false;
    }

    //    qDebug() << "startProject : " << file;

    //check if it's the right file :

    QFileInfo fileInfo(file);
    if(!fileInfo.fileName().contains(".plume") && !fileInfo.fileName().contains(".plume_backup")){
        int ret = QMessageBox::warning(0, tr("Plume creator"),
                                       tr("The document you are trying to open isn't a *.plume or *.plume_backup file !\n"),
                                       QMessageBox::Ok,
                                       QMessageBox::Ok);
        if(ret == QMessageBox::Ok)
            return false;
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
            return false;
        else
        {
            //remove the .plume if it exists
            QString oldFileName = file;
            oldFileName.remove("_backup");
            QFile oldFile(oldFileName);
            if(oldFile.exists())
                oldFile.remove();
            //replace it by the backup
            QFile bckFile(file);
            bckFile.rename(file.remove("_backup"));

            file = bckFile.fileName();
        }


    }


    //check if a project is opened :

    if(m_project->isProjectOpened() == true)
        this->closeCurrentProject();


    Utils::removeDir(QDir::tempPath() + "/Plume");

    if(Utils::isProjectFromOldSystem(file))
        file = Utils::updateProjectIfOldSystem(file);


    m_project->setCurrentProjectSettingArrayNumber(Utils::adaptSettingArrayToProject(file));



    this->unlockFiles();
    m_project->clearProject();


    m_project->setProjectFileName(file);


    loadProject();



    //    this->addProjectToPrjManager(this->projectFileName(), this->projectName(), QDateTime::currentDateTime());
    // QDateTime::currentDateTime() is temporary;

    // wordCount :

    wcThread = new WordCountEngineThread(this);    // wordCount thread
    wcThread->set_mainTree_numForDocHash(m_project->mainTree_numForDocHash());
    wcThread->set_mainTree_DomDoc(m_project->mainTreeDomDoc());
    connect(m_project, SIGNAL(currentSheetNumberChanged(int)), wcThread, SLOT(changeCurrentSheetNumber(int)));
    connect(wcThread, SIGNAL(currentSheetWordCount(int)), this,  SIGNAL(currentSheetWordCount(int)));
    connect(wcThread, SIGNAL(projectWordCount(int)), this,  SIGNAL(projectWordCount(int)));
    connect(wcThread, SIGNAL(bookWordCount(int)), this,  SIGNAL(bookWordCount(int)));
    connect(wcThread, SIGNAL(actWordCount(int)), this,  SIGNAL(actWordCount(int)));
    connect(wcThread, SIGNAL(chapterWordCount(int)), this,  SIGNAL(chapterWordCount(int)));
    connect(wcThread, SIGNAL(sceneWordCount(int)), this,  SIGNAL(sceneWordCount(int)));



    //  connecting :

    connectAllSheetsToWordCountThread();
    connectAllSheetsToSpellChecker();

    // setting spellchecking
    QSettings settings;
    QString defaultDict;
    if(!SpellChecker::dictsList().isEmpty())
        defaultDict = SpellChecker::dictsList().begin().key();
    else
        defaultDict = "";

    QString dict = settings.value("SpellChecking/lang", defaultDict).toString();
    if(!dict.isEmpty()){
        dict =  SpellChecker::dictsList().key(dict);

        this->spellDictsChangedSlot(dict);
    }


    emit openProjectSignal();

    m_project->setIsProjectOpened(true);

    // wordCount goal :
    QApplication::processEvents();

    this->setBaseWordCount(wcThread->projectWordCount());
    this->setAchievedWordGoal(0);
    this->setWordGoal(1000);
    emit setProgressBarValues(0, 0, 1000);
    this->setWordGoalActivated(false);
    connect(wcThread, SIGNAL(projectWordCount(int)), this, SLOT(calculatWordCountGoalDelta(int)));



    return true;
}

//--------------------------------------------------------------------------------------

void Hub::closeCurrentProject()
{
    // for GUI :
    emit closeProjectSignal();
    //



    Utils::modifyProjectModifiedDateInSettingsArray(m_project->currentProjectSettingArrayNumber(),QDateTime::currentDateTime().toString(Qt::ISODate) );

    this->project()->infoTreeDomDoc().firstChildElement("prj").setAttribute("lastModified", QDateTime::currentDateTime().toString(Qt::ISODate));


    this->stopSaveTimer();
    this->saveProject("wait");



    bool result = Utils::removeDir(m_project->projectWorkPath());


    //clear all the docs :

QHash<MainTextDocument *, QFile *> hash = m_project->mainTree_fileForDocHash();
    QHash<MainTextDocument *, QFile *>::iterator i = hash.begin();

    while (i != hash.end()) {
        MainTextDocument *doc = i.key();
        doc->setObjectName("");
        if(doc->docType() == "text")
            disconnect(doc, SIGNAL(wordCountChanged(QString,int,int)), wcThread, SLOT(start()));
        ++i;
    }
    disconnect(m_project, SIGNAL(currentSheetNumberChanged(int)), wcThread, SLOT(changeCurrentSheetNumber(int)));
    disconnect(wcThread, SIGNAL(currentSheetWordCount(int)), this,  SIGNAL(currentSheetWordCount(int)));
    disconnect(wcThread, SIGNAL(projectWordCount(int)), this,  SIGNAL(projectWordCount(int)));
    disconnect(wcThread, SIGNAL(bookWordCount(int)), this,  SIGNAL(bookWordCount(int)));
    disconnect(wcThread, SIGNAL(actWordCount(int)), this,  SIGNAL(actWordCount(int)));
    disconnect(wcThread, SIGNAL(chapterWordCount(int)), this,  SIGNAL(chapterWordCount(int)));
    disconnect(wcThread, SIGNAL(sceneWordCount(int)), this,  SIGNAL(sceneWordCount(int)));


 QHash<QTextDocument *, QFile *> hash2 = m_project->attendTree_fileForDocHash();
    QHash<QTextDocument *, QFile *>::iterator j = hash2.begin();

    while (j != hash2.end()) {
        QTextDocument *doc = j.key();
        doc->setObjectName("");

        ++j;
    }

    project()->clearProject();




    zipChecker->clearList();

    m_project->setIsProjectOpened(false);




}

//--------------------------------------------------------------------------------------

void Hub::loadProject()
{
    Zipper *zipper = new Zipper();

    zipper->setJob("extract", m_project->projectFileName(), m_project->projectWorkPath());
    //    connect(zipper, SIGNAL(finished()), this, SLOT(loadTemp()));
    zipper->start();
    zipper->wait(30000);
    this->loadTemp();
}

//--------------------------------------------------------------------------------------

bool Hub::loadTemp()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_project->lockRefresh();

    FileUpdater updater;
    updater.checkAttendanceFile(m_project->projectWorkPath() + "/tree");
    updater.checkTreeFile(m_project->projectWorkPath() + "/tree");
    updater.checkInfoFile(m_project->projectWorkPath() + "/tree");



    //    qDebug() << "loading temporary files";

    // ----------------------------   tree :

    QFile *treeFile = new QFile(m_project->projectWorkPath() + "/tree");



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

    m_project->setMainTreeDomDoc(domDoc);



    treeFile->close();

    //  open all docs :




    this->loadTextDocs(root.elementsByTagName("book"));
    this->loadTextDocs(root.elementsByTagName("act"));
    this->loadTextDocs(root.elementsByTagName("chapter"));
    this->loadTextDocs(root.elementsByTagName("scene"));

    //    qDebug() << "docs  : " <<  QString::number(this->mainTree_fileForDocHash().size());


    // ----------------------------   info :

    QFile *infoFile = new QFile(m_project->projectWorkPath() + "/info");




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


    m_project->setInfoTreeDomDoc(infoDomDoc);


    infoFile->close();


    m_project->setProjectName(infoDomDoc.documentElement().firstChildElement("prj").toElement().attribute("name", "error"));



    //-------------------- Attend :



    QFile *attendFile = new QFile(m_project->projectWorkPath() + "/attendance");



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


    m_project->setAttendTreeDomDoc(attendDomDoc);

    attendFile->close();


    //attend docs :





    this->loadAttendDocs(attendRoot.elementsByTagName("group"));
    this->loadAttendDocs(attendRoot.elementsByTagName("obj"));




    //    QDir dir(project->projectWorkPath());
    //    QStringList dirList = zipChecker->list();
    //    dirList = dir.entryList();




    // user dictionary :
    // if user dict file gets too complex, add it to FileUpdater.h
    QFile *userDictonaryFile = new QFile(m_project->projectWorkPath() + "/dicts/userDict.dict_plume");



    if(userDictonaryFile->open(QIODevice::ReadOnly)) {
        QTextStream stream(userDictonaryFile);
        QString userString;
        stream >> userString;

        QStringList list  = userString.split(";*$;", QString::SkipEmptyParts);

        this->setUserDict(list);

        userDictonaryFile->close();
    } else {
        //        qWarning() << "User dictionary in " << project->projectWorkPath() + "/dicts/userDict.dict_plume" << "could not be opened";
        this->setUserDict(QStringList());
    }




    //end :


    QApplication::restoreOverrideCursor();
    m_project->unlockRefresh();


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
            textPath = m_project->projectWorkPath() + element.attribute("textPath");
            synPath = m_project->projectWorkPath() + element.attribute("synPath");
            notePath = m_project->projectWorkPath() + element.attribute("notePath");
            QString number = element.attribute("number");

            QFile *textFile = new QFile;
            textFile->setFileName(textPath);
            textFile->open(QFile::ReadOnly | QFile::Text);
            QTextStream textFileStream( textFile );
            MainTextDocument *textDocument = new MainTextDocument(m_project, m_spellChecker);
            textDocument->setIdNumber(number.toInt());
            textDocument->setDocType("text");
            textDocument->setCursorPos(element.attribute("textPos", "0").toInt());
            textDocument->setHtml(textFileStream.readAll());
            textFile->close();
            textDocument->setObjectName("textDoc_" + number);
            m_project->mainTree_fileForDocHash_insert(textDocument, textFile);
            m_project->mainTree_numForDocHash_insert(textDocument, number.toInt());
            zipChecker->addFile("text", number.toInt());

            QFile *synFile = new QFile;
            synFile->setFileName(synPath);
            synFile->open(QFile::ReadOnly | QFile::Text);
            QTextStream synFileStream( synFile );
            MainTextDocument *synDocument = new MainTextDocument(m_project, m_spellChecker);
            synDocument->setIdNumber(number.toInt());
            synDocument->setDocType("synopsis");
            synDocument->setCursorPos(element.attribute("synPos", "0").toInt());
            synDocument->setHtml(synFileStream.readAll());
            synFile->close();
            synDocument->setObjectName("synDoc_" + number);
            m_project->mainTree_fileForDocHash_insert(synDocument, synFile);
            m_project->mainTree_numForDocHash_insert(synDocument, number.toInt());
            zipChecker->addFile("syn", number.toInt());

            QFile *noteFile = new QFile;
            noteFile->setFileName(notePath);
            noteFile->open(QFile::ReadOnly | QFile::Text);
            QTextStream noteFileStream( noteFile );
            MainTextDocument *noteDocument = new MainTextDocument(m_project, m_spellChecker);
            noteDocument->setIdNumber(number.toInt());
            noteDocument->setDocType("note");
            noteDocument->setCursorPos(element.attribute("notePos", "0").toInt());
            noteDocument->setHtml(noteFileStream.readAll());
            noteFile->close();
            noteDocument->setObjectName("noteDoc_" + number);
            m_project->mainTree_fileForDocHash_insert(noteDocument, noteFile);
            m_project->mainTree_numForDocHash_insert(noteDocument, number.toInt());
            zipChecker->addFile("note", number.toInt());


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

        attendPath = m_project->projectWorkPath() + element.attribute("attendPath");

        QString number = element.attribute("number");

        QFile *attendFile = new QFile;
        attendFile->setFileName(attendPath);
        attendFile->open(QFile::ReadOnly | QFile::Text);
        QTextStream attendFileStream( attendFile );
        QTextDocument *textDocument = new QTextDocument(m_project);
        textDocument->setHtml(attendFileStream.readAll());
        attendFile->close();
        textDocument->setObjectName("attendDoc_" + number);
        m_project->attendTree_fileForDocHash_insert(textDocument, attendFile);
        m_project->attendTree_numForDocHash_insert(textDocument, number.toInt());
        zipChecker->addFile("attend", number.toInt());

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
    zipper->setFileListToCheck(zipChecker->list());
    zipper->setJob("compress", m_project->projectFileName(), m_project->projectWorkPath());
    connect(zipper, SIGNAL(zipFinished()), this, SLOT(unlockFiles()));



    if(mode == "wait"){

        zipper->start(QThread::HighestPriority);
        zipper->wait(30000);
    }
    else
        zipper->start(QThread::LowestPriority);


    //    this->showStatusBarMessage(tr("Project saved"));
}
//--------------------------------------------------------------------------------------

void Hub::saveTemp()
{

    emit savingSignal();

    // ------------------ Tree :

    //    qDebug() << "save temporary files";

    QHash<MainTextDocument *, QFile *> hash = m_project->mainTree_fileForDocHash();
    QHash<MainTextDocument *, QFile *>::iterator i = hash.begin();
    while (i != hash.end()) {
        saveMainDoc(i.key() ,"mainTreeDocs" );
        ++i;
    }

    QFile *treeFile= new QFile(m_project->projectWorkPath() + "/tree");
    treeFile->waitForBytesWritten(500);
    treeFile->close();
    treeFile->open(QFile::ReadWrite | QFile::Text | QFile::Truncate);
    if(treeFile->isWritable())
    {
        treeFile->flush();


        const int IndentSize = 4;

        QTextStream out(treeFile);
        out.flush();
        m_project->mainTreeDomDoc().save(out, IndentSize);
        treeFile->close();

        //TextDocs :

    }



    //--------------- Info :

    QFile *infoFile= new QFile(m_project->projectWorkPath() + "/info");


    infoFile->waitForBytesWritten(500);

    infoFile->close();
    infoFile->open(QIODevice::ReadWrite | QIODevice::Text |QIODevice::Truncate	);
    if(infoFile->isWritable())
    {
        infoFile->flush();

        const int IndentSize = 4;


        QTextStream out(infoFile);
        out.flush();
        m_project->infoTreeDomDoc().save(out, IndentSize);
    }
    infoFile->close();







    //--------------------- Attend :

    QHash<QTextDocument *, QFile *> hash2 = m_project->attendTree_fileForDocHash();
    QHash<QTextDocument *, QFile *>::iterator j =    hash2.begin();
    while (j != hash2.end()) {
        this->saveDoc(j.key(),"attendTreeDocs");

        ++j;
    }


    QFile *attFile= new QFile(m_project->projectWorkPath() + "/attendance");


    attFile->waitForBytesWritten(500);
    attFile->close();
    attFile->open(QFile::ReadWrite | QFile::Text | QFile::Truncate);
    if(attFile->isWritable())
    {
        attFile->flush();


        const int IndentSize = 4;

        QTextStream out(attFile);
        out.flush();
        m_project->attendTreeDomDoc().save(out, IndentSize);
        attFile->close();

        //        qDebug() << "save attendDomDocument()";
    }



    // user dictionary :

    QDir dir(m_project->projectWorkPath());
    if(!dir.cd("dicts"))
        dir.mkdir("dicts");

    QFile *userDictonaryFile = new QFile(m_project->projectWorkPath() + "/dicts/userDict.dict_plume");
    if(userDictonaryFile->open(QIODevice::Truncate | QFile::WriteOnly | QFile::Text)) {
        userDictonaryFile->waitForBytesWritten(500);
        QTextStream stream(userDictonaryFile);
        foreach(QString string, this->userDict())
            stream << string + ";*$;";
        userDictonaryFile->close();
    }



}
//-----------------------------------------------------------------------------------------

bool Hub::saveMainDoc(MainTextDocument *doc, QString mode)
{
    if(mode != "mainTreeDocs")
        return false;

    QTextDocument *m_doc =  doc;

    QFile *file = new QFile;
    file = m_project->mainTree_fileForDocHash().value(doc);

    file->close();
    QTextDocumentWriter *docWriter = new QTextDocumentWriter(file, "HTML");
    QTextDocument *clonedDoc = m_doc->clone(this);
    bool written = docWriter->write(clonedDoc);

    QDomElement element = m_project->mainTree_domElementForNumberHash().value(doc->idNumber());
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
    file = m_project->attendTree_fileForDocHash().value(doc);


    file->close();
    QTextDocumentWriter *docWriter = new QTextDocumentWriter(file, "HTML");
    QTextDocument *clonedDoc = doc->clone(this);
    bool written = docWriter->write(clonedDoc);



    delete docWriter;
    delete clonedDoc;

    return written;
}

void Hub::addToSaveQueue()
{
    if(this->areFilesLocked()){
        saveStack += 1; // add more time before the next saving...
    }

    saveStack += 1;

    if(saveStack == 1)
        timerIdList.append(this->startTimer(2000));
    else if(saveStack > 1){  // reset if more than one save demand
        for(int i = 0; i < timerIdList.size(); ++i){
            this->killTimer(timerIdList.at(i));
        }
        timerIdList.clear();
        timerIdList.append(this->startTimer(4000));
    }



}
void Hub::stopSaveTimer()
{
    saveStack = 0;

    for(int i = 0; i < timerIdList.size(); ++i){
        this->killTimer(timerIdList.at(i));
    }
    timerIdList.clear();
}
void Hub::timerEvent(QTimerEvent *event)
{
    //    timerIdList.removeAll(event->timerId());


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
    wcThread->set_mainTree_numForDocHash(m_project->mainTree_numForDocHash());

   QHash<MainTextDocument *, QFile *> hash = m_project->mainTree_fileForDocHash();
    QHash<MainTextDocument *, QFile *>::iterator i = hash.begin();
    while (i != hash.end()) {
        MainTextDocument *doc = i.key();
        if(doc->docType() == "text")
            connect(doc, SIGNAL(wordCountChanged(QString,int,int)), wcThread, SLOT(start()), Qt::UniqueConnection);
        ++i;
    }

}


void Hub::connectAllSheetsToSpellChecker()
{
    connect(m_project, SIGNAL(attendTree_namesListChanged(QStringList)), this, SLOT(attendTree_namesListChangedSlot(QStringList)), Qt::UniqueConnection);
    connect(m_spellChecker, SIGNAL(userDictSignal(QStringList)), this, SLOT(setUserDict(QStringList)), Qt::UniqueConnection);

    QHash<MainTextDocument *, QFile *> hash = m_project->mainTree_fileForDocHash();
    QHash<MainTextDocument *, QFile *>::iterator i = hash.begin();
    while (i != hash.end()) {
        MainTextDocument *doc = i.key();
        if(doc->docType() == "text" /*|| doc->docType() == "synopsis" || doc->docType() == "note"*/)

            connect(this, SIGNAL(spellDictsChangedSignal(QString, QStringList)), doc, SLOT(setDicts()), Qt::UniqueConnection);

        ++i;
    }

}


//--------------------------------------------------------------------------------------
void Hub::spellDictsChangedSlot(QString dictionaryPath)
{


    this->setSpellDictPath(dictionaryPath);

    m_spellChecker->setDict(dictionaryPath, this->userDict(), m_project->attendTree_namesList());
    // send to all docs:
    emit spellDictsChangedSignal(dictionaryPath, this->userDict());

}


//--------------------------------------------------------------------------------------


void Hub::attendTree_namesListChangedSlot(QStringList namesList)
{

    QSettings settings;

    if(namesList.isEmpty() || !settings.value("SpellChecking/includeNamesFromTheMiseEnScene", true).toBool())
        return;




    if(m_spellChecker->isActive()){
        m_spellChecker->setDict(this->spellDictPath(), this->userDict(), m_project->attendTree_namesList());
        emit spellDictsChangedSignal(this->spellDictPath(), this->userDict());
    }
}

//--------------------------------------------------------------------------------------
void Hub::clearBin()
{

    // todo before that :
    //Model/view for mainTree


}

//temporary :
void Hub::removeFileFromZipList(QString type, int number)
{
    zipChecker->removeFile(type, number);
}

void Hub::addFileToZipList(QString type, int number)
{
    zipChecker->addFile(type, number);
}













MainTextDocument *Hub::prevText(int num)
{
    //find directly before :
    int prevNum = m_project->mainTree_domElementForNumberHash().key(m_project->mainTree_domElementForNumberHash().value(num).previousSiblingElement(m_project->mainTree_domElementForNumberHash().value(num).tagName()));


    //find before the father :
    if(prevNum == 0){
        QDomElement father = m_project->mainTree_domElementForNumberHash().value(num).parentNode().toElement();
        QDomElement prevFather = father.previousSiblingElement(father.tagName());
        QDomElement lastChild = prevFather.lastChild().toElement();
        prevNum = m_project->mainTree_domElementForNumberHash().key(lastChild);
    }
    //cancel :
    else if(prevNum == 0)
        return 0;

    QString string;
    MainTextDocument *textDoc = this->project()->findChild<MainTextDocument *>("textDoc_" + string.setNum(prevNum,10));
    //    qDebug() << "prevNum : " << string;

    return textDoc;
}

//-----------------------------------------------------------------------------------------------

MainTextDocument *Hub::nextText(int num)
{
    //find directly before :
    int nextNum = m_project->mainTree_domElementForNumberHash().key(m_project->mainTree_domElementForNumberHash().value(num).nextSiblingElement(m_project->mainTree_domElementForNumberHash().value(num).tagName()));


    //find after the father :
    if(nextNum == 0){
        QDomElement father = m_project->mainTree_domElementForNumberHash().value(num).parentNode().toElement();
        QDomElement nextFather = father.nextSiblingElement(father.tagName());
        QDomElement firstChild = nextFather.firstChild().toElement();
        nextNum = m_project->mainTree_domElementForNumberHash().key(firstChild);
    }
    //cancel :
    else if(nextNum == 0)
        return 0;

    QString string;
    MainTextDocument *textDoc = this->project()->findChild<MainTextDocument *>("textDoc_" + string.setNum(nextNum,10));
    //    qDebug() << "nextNum : " << string;

    return textDoc;
}

//-----------------------------------------------------------------------------------------------





//-----------------------------------------------------------------------------------------

void Hub::saveCursorPos(int textCursorPosition, int synCursorPosition, int noteCursorPosition, int number)
{
    QDomElement element = m_project->mainTree_domElementForNumberHash().value(number);
    element.setAttribute("textCursorPos", textCursorPosition);
    element.setAttribute("synCursorPos", synCursorPosition);
    element.setAttribute("noteCursorPos", noteCursorPosition);
    //   this->write(deviceFile);

    this->addToSaveQueue();

}



//--------------------------------------------------------------------------------
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



