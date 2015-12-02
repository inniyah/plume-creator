#include  <QtCore>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QSettings>

#include "themes.h"
#include "common/utils.h"
#include <quazip/JlCompress.h>
#include "zipper/zipchecker.h"

Themes::Themes(QObject *parent) :
    QObject(parent)
{

    m_currentTheme = new Theme(this);

}

QHash<QString,QString> Themes::themesList()
{
    QStringList paths = Utils::addonsPathsList();

    for(int i = 0; i < paths.size(); ++i){
        QString path = paths.at(i);
        paths.replace(i, path + "/themes/" );
    }


    QHash<QString, QString> hash;

    foreach (const QString themePath, paths) {
        QDir themeDir;
        themeDir.setPath(themePath);

        QStringList filters;
        filters << "*.plume_theme";


        // for themes in dirs :

        QFileInfoList infoList = themeDir.entryInfoList(filters, QDir::Dirs);
        foreach(const QFileInfo theme, infoList){

            QFile file(theme.absoluteFilePath() + "/info.xml");
            QDomDocument domDoc = Themes::readInfoXML(file.fileName());

            if(domDoc.isNull()){
                continue;
            }
            QDomElement root = domDoc.documentElement();
            QString name = root.attribute("name", "no name");

            hash.insert(theme.absoluteFilePath(), name);


        }

        // for themes in zip :

        infoList = themeDir.entryInfoList(filters, QDir::Files);
        foreach(const QFileInfo theme, infoList){


            QString tempFileName = QDir::tempPath() + "/Plume/info";
            QString infoFileName = JlCompress::extractFile(theme.absoluteFilePath(), "info.xml", tempFileName );


            QDomDocument domDoc = Themes::readInfoXML(infoFileName);

            if(domDoc.isNull()){
                continue;
            }
            QDomElement root = domDoc.documentElement();
            QString name = root.attribute("name", "no name");

            hash.insert(theme.absoluteFilePath(), name);
        }


    }


    return hash;
}

bool Themes::loadTheme(QString themeFileOrPath)
{


    Utils::removeDir(QDir::tempPath() + "/Plume/currentTheme");
    QDir dir;
    dir.setPath(QDir::tempPath() + "/Plume/currentTheme");


    if(themeFileOrPath == ""){
        unloadTheme();
        return true;
    }
    QFileInfo fileInfo(themeFileOrPath);
    if(!fileInfo.exists())
        return false;


    m_currentTheme = new Theme(this);




    if(fileInfo.isDir()){
        m_currentTheme->setTempPath(themeFileOrPath);

        dir.setPath(themeFileOrPath);


    }
    if(fileInfo.isFile() && ZipChecker::isZip(themeFileOrPath)){

        m_currentTheme->setTempPath(QDir::tempPath() + "/Plume/currentTheme");

        dir.mkpath(m_currentTheme->tempPath());

        QStringList list;
        list =  JlCompress::extractDir(themeFileOrPath, m_currentTheme->tempPath());



    }

    m_currentTheme->setPath(themeFileOrPath);

    QFileInfo info(m_currentTheme->tempPath() + "/info.xml");
    if(!info.exists()){
        qWarning() << "no info.xml file in theme";
        return false;
    }
    QDomDocument domDoc = this->readInfoXML(info.absoluteFilePath());

    if(domDoc.isNull()){
        return false;
    }
    QDomElement root = domDoc.documentElement();
    m_currentTheme->setName(root.attribute("name", "no name"));



    QFileInfo cssFile(m_currentTheme->tempPath() + "/stylesheet");
    if(!cssFile.exists()){
        qWarning() << "no css file in theme";
        return false;
    }

 QDir::setSearchPaths("themePics", QStringList(m_currentTheme->tempPath() + "/themePics"));

    QFile *styleFile = new QFile(cssFile.absoluteFilePath());

    if(styleFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(styleFile);
        QString string;
        string = stream.readAll();
        m_currentTheme->setQss(string);
        qApp->setStyleSheet(string);

    }

    return true;
}






Theme *Themes::currentTheme() const
{
    return m_currentTheme;
}

QDomDocument Themes::readInfoXML(QString fileString)
{
    QFile *themeFile = new QFile(fileString);

    QDomDocument domDoc;
    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!domDoc.setContent(themeFile, true, &errorStr, &errorLine,
                           &errorColumn)) {
        qWarning() << "Theme File "+ themeFile->fileName() + QString(". Parse error at line %1, column %2:\n%3\n")
                      .arg(errorLine)
                      .arg(errorColumn)
                      .arg(errorStr);



        return QDomDocument();
    }

    QDomElement root = domDoc.documentElement();
    if (root.tagName() != "plume-theme") {
        qWarning() << "The file is not a Plume Creator tree file.";
        return QDomDocument();
    }/* else if (root.hasAttribute("version")
                   && root.attribute("version") != updater.getTreeVersion()) {
            QMessageBox::information(this, tr("Plume Creator"),
                                     tr("The file is not a Plume Creator file version ", "version added after that ") + updater.getTreeVersion());

            continue;
        }*/



    themeFile->close();

    return domDoc;
}

//QDir picsDir;
//picsDir.setPath(themeDir.path() + "/pics/");
//foreach(QFileInfo picInfo, themeDir.entryInfoList(filters, QDir::Files)){
//}
void Themes::unloadTheme()
{

    m_currentTheme = new Theme(this);

    qApp->setStyleSheet("");
}
