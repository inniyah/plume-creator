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
#include "utils.h"
#include "maintextdocument.h"
#include "wordcountenginethread.h"

class Hub : public QWidget
{
    Q_OBJECT
public:
    explicit Hub(QWidget *parent = 0);


    void lockRefresh();
    void unlockRefresh();
    bool isRefreshLocked();

    QString projectName() const;
    void setProjectName(QString projectName);

    QString projectFileName() const;
    void setProjectFileName(QString projectFileName);
    QString projectWorkPath() const;
    bool isProjectOpened() const;

    QDomDocument mainTreeDomDoc();
    void setMainTreeDomDoc(QDomDocument doc);
    QHash<MainTextDocument *, QFile *> mainTree_fileForDocHash();
    void set_mainTree_fileForDocHash(QHash<MainTextDocument *, QFile *> fileForDoc);
    QHash<MainTextDocument *, int> mainTree_numForDocHash();
    void set_mainTree_numForDocHash(QHash<MainTextDocument *, int> numForDoc);
    QHash<int, QDomElement> mainTree_domElementForNumberHash();
    void set_mainTree_domElementForNumberHash(QHash<int, QDomElement> domElementForNumber);

    QDomDocument infoTreeDomDoc();

    QDomDocument attendTreeDomDoc();
    QHash<QTextDocument *, QFile *> attendTree_fileForDocHash();
    void set_attendTree_fileForDocHash(QHash<QTextDocument *, QFile *> fileForDoc);
    QHash<QTextDocument *, int> attendTree_numForDocHash();
    void set_attendTree_numForDocHash(QHash<QTextDocument *, int> numForDoc);
    QHash<int, QDomElement> attendTree_domElementForNumberHash();
    void set_attendTree_domElementForNumberHash(QHash<int, QDomElement> domElementForNumber);


    int currentSheetNumber() const;
    void setCurrentSheetNumber(int sheetNumber);

    // wordCount goal :
    int baseWordCount() const;
    void setBaseWordCount(int base);
    int wordGoal() const;
    void setWordGoal(int goal);
    int achievedWordGoal() const;
    void setAchievedWordGoal(int achievedCount);
    bool isWordGoalActivated() const;
    void setWordGoalActivated(bool wordGoalIsActivated);

    int projectWordCount() const{return wcThread->projectWordCount();}

    // files managment :
    void startProject(QString file);
    void closeCurrentProject();
    void loadProject();

    void connectAllSheetsToWordCountThread();

    void showStatusBarMessage(QString string = "", int time = 3000);

protected:
    void timerEvent(QTimerEvent *event);

signals:
    void projectNameChanged();
    void projectFileNameChanged();

    void mainTree_fileForDocHashChanged();
    void mainTree_numForDocHashChanged();
    void mainTree_domElementForNumberHashChanged();

    void attendTree_fileForDocHashChanged();
    void attendTree_numForDocHashChanged();
    void attendTree_domElementForNumberHashChanged();

    void openProjectSignal(QFile *file);
    void closeProjectSignal();

    void currentSheetNumberChanged(int currentSheetNumber);

    void textAlreadyChangedSignal(bool textChanged);
    void showStatusBarMessageSignal(QString string, int time);

    void savingSignal();

//    wordCount :
    void projectWordCount(int count);
    void bookWordCount(int count);
    void chapterWordCount(int count);
    void sceneWordCount(int count);
    void currentSheetWordCount(int count);

    // wordCount goal :
    void baseWordCountSignal(int count);
    void wordGoalSignal(int count);
    void achievedWordGoalSignal(int count);
    void wordGoalIsActivatedSignal(bool wordGoalIsActivated);

    void resetSpreadsheetOutlinerSignal();

public slots:
    void addToSaveQueue();
    void resetSpreadsheetOutliner(){emit resetSpreadsheetOutlinerSignal();}

private slots:
    void saveProject(QString mode = "");
    void unlockFiles();
    void lockFiles();
    bool loadTemp();

    void calculatWordCountGoalDelta(int projectCount);

    void debuggg(int count){ qDebug() << "debuggg : " << QString::number(count);}

private:
   void addProjectToPrjManager(QString fileName, QString _name, QDateTime creationDate);
   bool refreshIsLocked;

    QString m_projectName;
    QString m_projectFileName;

    QDomDocument m_mainTreeDomDoc;
    QHash<MainTextDocument *, QFile *> m_mainTree_fileForDocHash;
    QHash<MainTextDocument *, int> m_mainTree_numForDocHash;
    QHash<int, QDomElement> m_mainTree_domElementForNumberHash;

    QDomDocument m_infoTreeDomDoc;

    QDomDocument m_attendTreeDomDoc;
    QHash<QTextDocument *, QFile *> m_attendTree_fileForDocHash;
    QHash<QTextDocument *, int> m_attendTree_numForDocHash;
    QHash<int, QDomElement> m_attendTree_domElementForNumberHash;

    int m_currentSheetNumber;
int m_baseWordCount , m_wordGoal, m_achievedWordGoal;
bool m_isWordGoalActivated;

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

int settingNumber;

// wordcount :
 WordCountEngineThread *wcThread;

};

#endif // HUB_H
