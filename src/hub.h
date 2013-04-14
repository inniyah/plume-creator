#ifndef HUB_H
#define HUB_H

#include <QObject>
#include <QDomDocument>
#include <QHash>
#include <QFile>
#include <QTextDocument>
#include <QtGui>

#include "zipper.h"
#include "changestests.h"
#include "fileupdater.h"
#include "JlCompress.h"
#include "fileutils.h"
#include "maintextdocument.h"

class Hub : public QWidget
{
    Q_OBJECT
public:
    explicit Hub(QWidget *parent = 0);


    void lockRefresh();
    void unlockRefresh();
    bool isRefreshLocked();

    QString projectName();
    void setProjectName(QString projectName);

    QString projectFileName();
    void setProjectFileName(QString projectFileName);
    QString projectWorkPath();

    QDomDocument mainTreeDomDoc();
    void setMainTreeDomDoc(QDomDocument doc);
    QHash<MainTextDocument *, QFile *> mainTree_fileForDocHash();
    void set_mainTree_fileForDocHash(QHash<MainTextDocument *, QFile *> fileForDoc);
    QHash<MainTextDocument *, int> mainTree_numForDocHash();
    void set_mainTree_numForDocHash(QHash<MainTextDocument *, int> numForDoc);

    QDomDocument infoTreeDomDoc();

    QDomDocument attendTreeDomDoc();
    QHash<QTextDocument *, QFile *> attendTree_fileForDocHash();
    void set_attendTree_fileForDocHash(QHash<QTextDocument *, QFile *> fileForDoc);
    QHash<QTextDocument *, int> attendTree_numForDocHash();
    void set_attendTree_numForDocHash(QHash<QTextDocument *, int> numForDoc);


    int currentSheetNumber();
    void setCurrentSheetNumber(int sheetNumber);

    // files managment :
    void startProject(QString file);
    void closeCurrentProject();
    void loadProject();

protected:
    void timerEvent(QTimerEvent *event);

signals:
    void projectNameChanged();
    void projectFileNameChanged();

    void mainTree_fileForDocHashChanged();
    void mainTree_numForDocHashChanged();

    void attendTree_fileForDocHashChanged();
    void attendTree_numForDocHashChanged();

    void openProjectSignal(QFile *file);
    void closeProjectSignal();

    void currentSheetNumberChanged(int currentSheetNumber);

    void textAlreadyChangedSignal(bool textChanged);

public slots:
    void addToSaveQueue();

private slots:
    void saveProject(QString mode = "");
    void unlockFiles();
    void lockFiles();
    bool loadTemp();

private:
    bool refreshIsLocked;

    QString m_projectName;
    QString m_projectFileName;

    QDomDocument m_mainTreeDomDoc;
    QHash<MainTextDocument *, QFile *> m_mainTree_fileForDocHash;
    QHash<MainTextDocument *, int> m_mainTree_numForDocHash;

    QDomDocument m_infoTreeDomDoc;

    QDomDocument m_attendTreeDomDoc;
    QHash<QTextDocument *, QFile *> m_attendTree_fileForDocHash;
    QHash<QTextDocument *, int> m_attendTree_numForDocHash;

    int m_currentSheetNumber;

// file managment :
    void saveTemp();
    bool areFilesLocked();
    bool filesLocked;
    bool projectOpened;
    QString projectWorkingPath;
 bool saveDoc(QTextDocument *doc, QString mode);
 bool saveMainDoc(MainTextDocument *doc, QString mode);

void loadTextDocs(QDomNodeList list);
void loadAttendDocs(QDomNodeList list);
int saveStack;
QList<int> timerIdList;
void stopSaveTimer();
};

#endif // HUB_H
