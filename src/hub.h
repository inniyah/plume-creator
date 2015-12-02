/***************************************************************************
 *   Copyright (C) 2012 by Cyril Jacquet                                   *
 *   cyril.jacquet@plume-creator.eu                                                 *
 *                                                                         *
 *  This file is part of Plume Creator.                                    *
 *                                                                         *
 *  Plume Creator is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  Plume Creator is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with Plume Creator.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/
#ifndef HUB_H
#define HUB_H

#include <QObject>
#include <QDomDocument>
#include <QHash>
#include <QFile>
#include <QTextDocument>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   

#include "zipper/zipper.h"
#include "changestests.h"
#include "fileupdater.h"
#include <quazip/JlCompress.h>
#include "common/utils.h"
#include "maintextdocument.h"
#include "wordcountenginethread.h"
#include "zipper/zipchecker.h"
#include "themes.h"
#include "spellchecker.h"
#include "project.h"

class Hub : public QWidget
{
    Q_OBJECT
public:
    explicit Hub(QWidget *parent = 0);


    void lockRefresh();
    void unlockRefresh();
    bool isRefreshLocked();

    Project *project();




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

    // spell check :
    QStringList userDict();
    QString spellDictPath();
    void setSpellDictPath(QString spellDictPath);



    // files managment :
    void closeCurrentProject();
    void loadProject();
    void clearBin();
    void removeFileFromZipList(QString type, int number);
    void addFileToZipList(QString type, int number);

    void connectAllSheetsToWordCountThread();
    void connectAllSheetsToSpellChecker();

    void showStatusBarMessage(QString string = "", int time = 3000);

    MainTextDocument *prevText(int num);
    MainTextDocument *nextText(int num);
    void saveCursorPos(int textCursorPosition, int synCursorPosition, int noteCursorPosition, int number);



    Themes *themes();

protected:
    void timerEvent(QTimerEvent *event);

signals:

    void openProjectSignal();
    void closeProjectSignal();
    void closeAllChildrenWindowsSignal();

    void textAndNoteSignal(int number, QString action);

    void textAlreadyChangedSignal(bool textChanged);
    void showStatusBarMessageSignal(QString string = "", int time = 3000);


    void savingSignal();

    //    wordCount :
    void projectWordCount(int count);
    void bookWordCount(int count);
    void actWordCount(int count);
    void chapterWordCount(int count);
    void sceneWordCount(int count);
    void currentSheetWordCount(int count);

    // wordCount goal :
    void baseWordCountSignal(int count);
    void wordGoalSignal(int count);
    void achievedWordGoalSignal(int count);
    void wordGoalIsActivatedSignal(bool wordGoalIsActivated);
    void setProgressBarValues(int base,int achieved,int goal);

    void resetSpreadsheetOutlinerSignal();

    // spell check :
    void spellDictsChangedSignal(QString dictionaryPath, QStringList userDictionary);


public slots:
    bool startProject(QString file);
    void addToSaveQueue();
    void resetSpreadsheetOutliner(){emit resetSpreadsheetOutlinerSignal();}

    // spell check :
    void spellDictsChangedSlot(const QString dictionaryPath);
    void setUserDict(QStringList userDict);
    SpellChecker *spellChecker();


    void openSheet(int id);

private slots:
    void saveProject(QString mode = "");
    void unlockFiles();
    void lockFiles();
    bool loadTemp();

    void calculatWordCountGoalDelta(int projectCount);

    void debuggg(int count){ qDebug() << "debuggg : " << QString::number(count);}

    void attendTree_namesListChangedSlot(QStringList namesList);



private:





    int m_baseWordCount , m_wordGoal, m_achievedWordGoal;
    bool m_isWordGoalActivated;


    Project *m_project;


    // file managment :
    void saveTemp();
    bool saveDoc(QTextDocument *doc, QString mode);
    bool saveMainDoc(MainTextDocument *doc, QString mode);
    bool areFilesLocked();
bool filesLocked;

    void loadTextDocs(QDomNodeList list);
    void loadAttendDocs(QDomNodeList list);
    int saveStack;
    QList<int> timerIdList;
    void stopSaveTimer();

    int settingNumber;

    // wordcount :
    WordCountEngineThread *wcThread;

    //zipChecker :
    ZipChecker *zipChecker;

    // spell check :
    QStringList m_userDict;
    QString m_spellDictPath;
    SpellChecker *m_spellChecker;

    // themes
    Themes *m_themes;

};

#endif // HUB_H
