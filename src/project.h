#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif
#include <QtGui>
#include <QDomDocument>

#include "maintextdocument.h"

class Project : public QObject
{
    Q_OBJECT
public:
    explicit Project(QObject *parent = 0);

    void clearProject();

    QString projectName() const;
    void setProjectName(QString projectName);

    QString projectFileName() const;
    void setProjectFileName(QString projectFileName);
    QString projectWorkPath() const;
    bool isProjectOpened() const;
    void setIsProjectOpened(bool value);

    void lockRefresh();
    void unlockRefresh();
    bool isRefreshLocked();

    void unlockFiles();
    void lockFiles();
    bool areFilesLocked();

    int currentSheetNumber() const;
    void setCurrentSheetNumber(int sheetNumber);

    int currentProjectSettingArrayNumber() const;
    void setCurrentProjectSettingArrayNumber(int projectNumber);

    QDomDocument mainTreeDomDoc();
    void setMainTreeDomDoc(QDomDocument domDoc);
    QHash<MainTextDocument *, QFile *> mainTree_fileForDocHash();
    void mainTree_fileForDocHash_insert(MainTextDocument *doc, QFile *file);
    void set_mainTree_fileForDocHash(QHash<MainTextDocument *, QFile *> fileForDoc);
    QHash<MainTextDocument *, int> mainTree_numForDocHash();
    void mainTree_numForDocHash_insert(MainTextDocument *doc, int id);
    void set_mainTree_numForDocHash(QHash<MainTextDocument *, int> numForDoc);
    QHash<int, QDomElement> mainTree_domElementForNumberHash();
    void set_mainTree_domElementForNumberHash(QHash<int, QDomElement> domElementForNumber);

    QDomDocument attendTreeDomDoc();
    void setAttendTreeDomDoc(QDomDocument doc);
    QHash<QTextDocument *, QFile *> attendTree_fileForDocHash();
    void attendTree_fileForDocHash_insert(QTextDocument *doc, QFile *file);
    void set_attendTree_fileForDocHash(QHash<QTextDocument *, QFile *> fileForDoc);
    QHash<QTextDocument *, int> attendTree_numForDocHash();
    void attendTree_numForDocHash_insert(QTextDocument *doc, int id);
    void set_attendTree_numForDocHash(QHash<QTextDocument *, int> numForDoc);
    QHash<int, QDomElement> attendTree_domElementForNumberHash();
    void set_attendTree_domElementForNumberHash(QHash<int, QDomElement> domElementForNumber);
    QStringList attendTree_namesList();
    void set_attendTree_namesList(QStringList namesList);

    QDomDocument infoTreeDomDoc();
    void setInfoTreeDomDoc(QDomDocument domDoc);

    QHash<int,QString> listOfBooksById();


signals:
    void projectNameChanged();
    void projectFileNameChanged();
    void projectOpenedSignal(bool opened);

    void currentSheetNumberChanged(int currentSheetNumber);

    void mainTree_fileForDocHashChanged();
    void mainTree_numForDocHashChanged();
    void mainTree_numForDocHashChanged(QHash<MainTextDocument *, int> numForDoc);
    void mainTree_domElementForNumberHashChanged();

    void attendTree_fileForDocHashChanged();
    void attendTree_numForDocHashChanged();
    void attendTree_domElementForNumberHashChanged();
    void attendTree_namesListChanged(QStringList namesList);


public slots:


private slots:

private:

    QString m_projectName;
    QString m_projectFileName;
    bool projectOpened;
    QString projectWorkingPath;
    bool refreshIsLocked;

    int m_currentSheetNumber, m_currentProjectSettingArrayNumber;

    QDomDocument m_mainTreeDomDoc;
    QHash<MainTextDocument *, QFile *> m_mainTree_fileForDocHash;
    QHash<MainTextDocument *, int> m_mainTree_numForDocHash;
    QHash<int, QDomElement> m_mainTree_domElementForNumberHash;


    QDomDocument m_attendTreeDomDoc;
    QHash<QTextDocument *, QFile *> m_attendTree_fileForDocHash;
    QHash<QTextDocument *, int> m_attendTree_numForDocHash;
    QHash<int, QDomElement> m_attendTree_domElementForNumberHash;
    QStringList m_attendTree_names;

    QDomDocument m_infoTreeDomDoc;

    QHash<int, QString> listOfBooksByIdHash;

};

#endif // PROJECT_H
