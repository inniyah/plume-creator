#include "project.h"
#include "changestests.h"

Project::Project(QObject *parent) :
    QObject(parent)
{
    clearProject();
}

void Project::clearProject()
{
    m_projectName.clear();
    m_projectFileName.clear();
    projectOpened = false;
    projectWorkingPath.clear();
    refreshIsLocked = false;
    m_currentSheetNumber = -1;
    m_currentProjectSettingArrayNumber = 9999;


    m_mainTreeDomDoc.clear();
    m_mainTree_fileForDocHash.clear();
    m_mainTree_fileForDocHash.clear();
    m_mainTree_numForDocHash.clear();

    m_attendTreeDomDoc.clear();
    m_attendTree_fileForDocHash.clear();
    m_attendTree_numForDocHash  .clear();
    m_attendTree_domElementForNumberHash.clear();
    m_attendTree_names.clear();

    m_infoTreeDomDoc.clear();

    listOfBooksByIdHash.clear();
}

//----------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------



QString Project::projectFileName() const
{
    return m_projectFileName;
}
//--------------------------------------------------------------------------------

void Project::setProjectFileName(QString projectFileName)
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


QString Project::projectWorkPath() const
{
    return projectWorkingPath;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------


QString Project::projectName() const
{
    return m_projectName;
}


//--------------------------------------------------------------------------------

void Project::setProjectName(QString projectName)
{

    QString m_old_projectName = m_projectName;
    m_projectName = projectName;

    if(!refreshIsLocked)
        if(ChangesTests::test(m_old_projectName, m_projectName) == false)
            emit projectNameChanged();
}
//--------------------------------------------------------------------------------

bool Project::isProjectOpened() const
{

    return projectOpened;
}

void Project::setIsProjectOpened(bool value)
{
    projectOpened = value;
    emit projectOpenedSignal(value);

}

//--------------------------------------------------------------------------------

void Project::lockRefresh()
{
    refreshIsLocked = true;
}

//--------------------------------------------------------------------------------

void Project::unlockRefresh()
{
    refreshIsLocked = false;
}
//--------------------------------------------------------------------------------

bool Project::isRefreshLocked()
{
    return refreshIsLocked;
}



//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------


int Project::currentSheetNumber() const
{
    return m_currentSheetNumber;
}


void Project::setCurrentSheetNumber(int sheetNumber)
{
    int oldSheetNumber = m_currentSheetNumber;
    m_currentSheetNumber = sheetNumber ;


    if(!refreshIsLocked)
        if(ChangesTests::test(oldSheetNumber, m_currentSheetNumber) == false)
            emit currentSheetNumberChanged(m_currentSheetNumber);

}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------




int Project::currentProjectSettingArrayNumber() const
{
    return m_currentProjectSettingArrayNumber;
}


void Project::setCurrentProjectSettingArrayNumber(int projectNumber)
{
    m_currentProjectSettingArrayNumber = projectNumber ;

}


//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------





QDomDocument Project::mainTreeDomDoc()
{
    return m_mainTreeDomDoc; // don't forget the data is shared. See Qt doc.
}

void Project::setMainTreeDomDoc(QDomDocument domDoc)
{
    m_mainTreeDomDoc = domDoc;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------



QHash<MainTextDocument *, QFile *> Project::mainTree_fileForDocHash()
{
    return m_mainTree_fileForDocHash;
}

void Project::mainTree_fileForDocHash_insert(MainTextDocument *doc, QFile *file)
{
    m_mainTree_fileForDocHash.insert(doc, file);
}

void Project::set_mainTree_fileForDocHash(QHash<MainTextDocument *, QFile *> fileForDoc)
{
    QHash<MainTextDocument *, QFile *> m_old_fileForDoc = m_mainTree_fileForDocHash;
    m_mainTree_fileForDocHash = fileForDoc;

    if(!refreshIsLocked)
        if(ChangesTests::test(m_old_fileForDoc, m_mainTree_fileForDocHash) == false)
            emit mainTree_fileForDocHashChanged();
}


//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

QHash<MainTextDocument *, int> Project::mainTree_numForDocHash()
{
    return m_mainTree_numForDocHash;
}

void Project::mainTree_numForDocHash_insert(MainTextDocument *doc, int id)
{
    m_mainTree_numForDocHash.insert(doc, id);
}

void Project::set_mainTree_numForDocHash(QHash<MainTextDocument *, int> numForDoc)
{
    QHash<MainTextDocument *, int> m_old_numForDoc = m_mainTree_numForDocHash;
    m_mainTree_numForDocHash = numForDoc;

    if(!refreshIsLocked)
        if(ChangesTests::test(m_old_numForDoc, m_mainTree_numForDocHash) == false){
            emit mainTree_numForDocHashChanged(m_mainTree_numForDocHash);
            emit mainTree_numForDocHashChanged();
            //            qDebug() << "mainTree_numForDocHashChanged"   ;
        }
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

QHash<int, QDomElement> Project::mainTree_domElementForNumberHash()
{
    return m_mainTree_domElementForNumberHash;
}

void Project::set_mainTree_domElementForNumberHash(QHash<int, QDomElement> domElementForNumber)
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
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------





QDomDocument Project::attendTreeDomDoc()
{
    return m_attendTreeDomDoc;
}

void Project::setAttendTreeDomDoc(QDomDocument doc)
{
    m_attendTreeDomDoc = doc;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

QHash<QTextDocument *, QFile *> Project::attendTree_fileForDocHash()
{
    //    qDebug() << "m_attendTree_fileForDocHash : " << m_attendTree_fileForDocHash.size();
    return m_attendTree_fileForDocHash;
}

void Project::attendTree_fileForDocHash_insert(QTextDocument *doc, QFile *file)
{
    m_attendTree_fileForDocHash.insert(doc, file);
}

void Project::set_attendTree_fileForDocHash(QHash<QTextDocument *, QFile *> fileForDoc)
{
    QHash<QTextDocument *, QFile *> m_old_fileForDoc = m_attendTree_fileForDocHash;
    m_attendTree_fileForDocHash = fileForDoc;

    if(!refreshIsLocked)
        if(ChangesTests::test(m_old_fileForDoc, m_attendTree_fileForDocHash) == false)
            emit attendTree_fileForDocHashChanged();
}


//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

QHash<QTextDocument *, int> Project::attendTree_numForDocHash()
{
    return m_attendTree_numForDocHash;
}

void Project::attendTree_numForDocHash_insert(QTextDocument * doc, int id)
{
    m_attendTree_numForDocHash.insert(doc, id);
}

void Project::set_attendTree_numForDocHash(QHash<QTextDocument *, int> numForDoc)
{
    QHash<QTextDocument *, int> m_old_numForDoc = m_attendTree_numForDocHash;
    m_attendTree_numForDocHash = numForDoc;

    if(!refreshIsLocked)
        if(ChangesTests::test(m_old_numForDoc, m_attendTree_numForDocHash) == false)
            emit attendTree_numForDocHashChanged();
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

QHash<int, QDomElement> Project::attendTree_domElementForNumberHash()
{
    return m_attendTree_domElementForNumberHash;
}

void Project::set_attendTree_domElementForNumberHash(QHash<int, QDomElement> domElementForNumber)
{
    QHash<int, QDomElement> m_old_domElementForNumberHash = m_attendTree_domElementForNumberHash;
    m_attendTree_domElementForNumberHash = domElementForNumber;

    if(!refreshIsLocked)
        if(ChangesTests::test(m_old_domElementForNumberHash, m_attendTree_domElementForNumberHash) == false)
            emit attendTree_domElementForNumberHashChanged();
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------


QStringList Project::attendTree_namesList()
{
    return m_attendTree_names;
}

void Project::set_attendTree_namesList(QStringList namesList)
{
    m_attendTree_names.clear();

    for(int i = 0; i < namesList.size() ; ++i){

        QString name = namesList.at(i);

        //break names into parts :
        QStringList fragmentList = name.split(" ", QString::SkipEmptyParts);
        if(!fragmentList.isEmpty())
            foreach (QString fragment, fragmentList) {
                m_attendTree_names.append( fragment );
            }


    }


    emit attendTree_namesListChanged(m_attendTree_names);
}


//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------




QDomDocument Project::infoTreeDomDoc()
{
    return m_infoTreeDomDoc;
}

void Project::setInfoTreeDomDoc(QDomDocument domDoc)
{
    m_infoTreeDomDoc = domDoc;
}




//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------



QHash<int, QString> Project::listOfBooksById()
{


    // dom maintree listbytagname("book")



    return listOfBooksByIdHash;
}



