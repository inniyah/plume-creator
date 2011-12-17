#include <QSettings>
#include <QtGui>
#include <QRect>


#include "mainwindow.h"
#include "newprojectwizard.h"
#include "menubox.h"
#include "maintree.h"
#include "statsbox.h"
#include "itembox.h"
#include "texttab.h"
#include "notezone.h"
#include "outline.h"
#include "attendbox.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), NoProjectOpened(true)
{
    setMinimumSize(800, 600);
    setWindowTitle("Plume Creator");

    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);


    //temporary config
    //    setStyleSheet("* {background-color: beige}");





    tabWidget = new QTabWidget;
    tabWidget->setTabsClosable(true);
    tabWidget->setTabShape(QTabWidget::Triangular);
    tabWidget->setDocumentMode(true);
    tabWidget->setMovable(false);

    setCentralWidget(tabWidget);



    createMenuDock();
    createTreeDock();
    createNoteDock();
    createToolDock();


    setTextTabConnections();


    // Config :
    readSettings();
    connect(menu, SIGNAL(applyConfigSignal()), this,SLOT(applyConfig()));
    applyConfig();


    // Welcome dialog at first start

    if (m_firstStart){
        QMessageBox firstStart;
        firstStart.setWindowTitle(tr("Welcome"));
        firstStart.setText(tr("<center><b>Hello ! Welcome to Plume Creator v0.39 !</b></center>"
                              "<p>Plume Creator is a little program for writers"
                              " in quest of a complete yet simple way of"
                              " writing and organizing a fiction.</p>"
                              "<br>"
                              "<p>It allows :"
                              "<blockquote>- fullscreen text editing</blockquote>"
                              "<blockquote>- chapters and scenes outlining</blockquote>"
                              "<blockquote>- note taking</blockquote>"
                              "<blockquote>- items/characters/places managing</blockquote></p>"



                              ));
        firstStart.exec();

        NewProjectWizard projectWizard;
        projectWizard.exec();

        m_firstStart = false;

        menu->openManager();

    }
    else{
        menu->openManager();
    }



}

MainWindow::~MainWindow()
{

}




//---------------------------------------------------------------------------

void MainWindow::createMenuDock()
{

    QDockWidget *menuDock = new QDockWidget;

    menuDock->setObjectName("menuDock");


    menuDock->setWindowTitle(tr("Menus"));
    menuDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    menuDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    menuDock->setMinimumSize(160,350);
    menuDock->setMaximumWidth(200);

    QToolBox *toolBox = new QToolBox;



    // page 1
    menu = new MenuBox;

    menu->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    menu->setLineWidth(2);
    menu->setMidLineWidth(3);


    toolBox->addItem(menu, tr("Menu"));

    connect(menu, SIGNAL(openProjectSignal(QFile*)), this, SLOT(openProjectSlot(QFile*)));

    connect(menu,SIGNAL(exitSignal()), this, SLOT(close()));
    connect(menu, SIGNAL(closeProjectSignal()), this, SLOT(closeProjectSlot()));


    menu->firstLaunch();



    //page 2

    editMenu = new EditMenuBox;

    editMenu->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    editMenu->setLineWidth(2);
    editMenu->setMidLineWidth(3);

    toolBox->addItem(editMenu, tr("Edit Menu"));

    //page 3

    attendList = new AttendBox;

    attendList->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    attendList->setLineWidth(2);
    attendList->setMidLineWidth(3);

    toolBox->addItem(attendList, tr("Attendance"));

    toolBox->setCurrentWidget(menu);

    toolBox->setFrameStyle(QFrame::Panel | QFrame::Raised);
    toolBox->setLineWidth(2);
    toolBox->setMidLineWidth(3);

    menuDock->setWidget(toolBox);

    addDockWidget(Qt::RightDockWidgetArea, menuDock);

}

//---------------------------------------------------------------------------


void MainWindow::createTreeDock()
{

    QDockWidget *treeDock = new QDockWidget;

    treeDock->setObjectName("treeDock");
    treeDock->setWindowTitle(tr("Tree"));
    treeDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    treeDock->setMinimumSize(160,200);

    mainTree = new MainTree;

    connect(mainTree, SIGNAL(textAndNoteSignal(QTextDocument*,QTextDocument*,QTextDocument*, int, int, int, QString, int, QString)), this, SLOT(textSlot(QTextDocument*,QTextDocument*,QTextDocument*, int,int,int, QString, int, QString)));
    connect(mainTree, SIGNAL(textAndNoteSignal(int, QString)), this, SLOT(secondTextSlot(int, QString)));

    treeDock->setWidget(mainTree);

    addDockWidget(Qt::LeftDockWidgetArea, treeDock);

}

//---------------------------------------------------------------------------


void MainWindow::createToolDock()
{

    QDockWidget *toolDock = new QDockWidget;

    toolDock->setObjectName("toolDock");
    toolDock->setWindowTitle(tr("Tools"));


    toolDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    toolDock->setMinimumSize(160,200);
    toolDock->setMaximumWidth(200);

    QToolBox *toolBox = new QToolBox;
    stats = new StatsBox;
    items = new ItemBox;

    // page 1

    toolBox->addItem(stats, tr("Stats"));
    stats->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    stats->setLineWidth(2);
    stats->setMidLineWidth(3);

    //page 2

    toolBox->addItem(items, tr("Items"));
    items->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    items->setLineWidth(2);
    items->setMidLineWidth(3);

    toolBox->setCurrentWidget(stats);

    toolBox->setFrameStyle(QFrame::Panel | QFrame::Raised);
    toolBox->setLineWidth(2);
    toolBox->setMidLineWidth(3);

    toolDock->setWidget(toolBox);

    addDockWidget(Qt::LeftDockWidgetArea, toolDock);

}

//---------------------------------------------------------------------------------------------

void MainWindow::createNoteDock()
{

    QDockWidget *noteDock = new QDockWidget;

    noteDock->setObjectName("noteDock");
    noteDock->setWindowTitle(tr("Notes"));
    noteDock->setAllowedAreas(Qt::BottomDockWidgetArea);
    noteDock->setMinimumSize(800,200);
    noteDock->setMaximumSize(2000,800);

    synLayout = new QStackedLayout;
    noteLayout = new QStackedLayout;
    QGroupBox *synopsisBox = new QGroupBox;
    QGroupBox *noteBox = new QGroupBox;

    QFrame *frame = new QFrame;
    frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
    frame->setLineWidth(2);
    frame->setMidLineWidth(3);


    QFrame *midFrame = new QFrame;
    midFrame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    midFrame->setLineWidth(1);
    midFrame->setMidLineWidth(3);

    QGridLayout *midLayout = new QGridLayout;

    QToolButton *tabFullscreenButton = new QToolButton(this);
    tabFullscreenButton->setText(tr("Fullscreen &Edit"));
    tabFullscreenButton->setShortcut(Qt::Key_F11);
    tabFullscreenButton->setToolTip(tr("Edit this document fullscreen"));
    connect(tabFullscreenButton, SIGNAL(pressed()), this, SLOT(editFullscreen()));;

    QToolButton *outlinerButton = new QToolButton(this);
    outlinerButton->setText(tr("Outliner"));
    outlinerButton->setShortcut(Qt::Key_F12);
    outlinerButton->setToolTip(tr("Launch the project outliner"));
    connect(outlinerButton, SIGNAL(pressed()), this, SLOT(launchOutliner()));

    //    QToolButton *keepVisibleButton = new QToolButton(this);
    //    keepVisibleButton->setText(tr("Visible"));
    //    keepVisibleButton->setShortcut(Qt::Key_F11);
    //    keepVisibleButton->setCheckable(true);
    //    keepVisibleButton->setToolTip(tr("Keep this dock visible"));
    //    connect(keepVisibleButton, SIGNAL(toggled(bool)), tabWidget, SLOT(showFullScreen()));;


    QComboBox *stateCombo = new QComboBox;
    midLayout->addWidget(tabFullscreenButton,0,0, Qt::AlignHCenter);
    midLayout->addWidget(outlinerButton,1,0, Qt::AlignHCenter);
    //   midLayout->addWidget(stateCombo);
    midFrame->setLayout(midLayout);

    synopsisBox->setLayout(synLayout);

    noteBox->setLayout(noteLayout);


    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(synopsisBox);
    layout->addWidget(midFrame);
    layout->addWidget(noteBox);
    frame->setLayout(layout);


    noteDock->setWidget(frame);

    QStringList list;
    list << tr("Draft") << tr("25%") << tr("50%") << tr("75%") << tr("Done") << tr("Corrected");
    stateCombo->insertItems(0, list);


    synopsisBox->setTitle(tr("Synopsis"));
    noteBox->setTitle(tr("Note"));

    addDockWidget(Qt::BottomDockWidgetArea, noteDock);


}




//---------------------------------------------------------------------------

void MainWindow::openProjectSlot(QFile *projectFile)
{


    closeAllDocsSlot();
    file = projectFile;
    attendList->readProjectAttendance(projectFile);
    mainTree->read(projectFile);
    configTimer();
}

//---------------------------------------------------------------------------

void MainWindow::closeProjectSlot()
{

    closeAllDocsSlot();
    mainTree->write(file);

    mainTree->closeTree();
    NoProjectOpened = true;

    attendList->accept(); //to close the manager;

    QSettings settings;
    settings.beginWriteArray("Manager/projects");
    settings.setArrayIndex(settingNumber);
    settings.setValue("lastModified", QDateTime::currentDateTime().toString());
    settings.endArray();

    timer->stop();
}

void MainWindow::setProjectNumberSlot(int prjNumber)
{
    settingNumber = prjNumber;
}




//---------------------------------------------------------------------------





void MainWindow::textSlot(QTextDocument *textDoc, QTextDocument *noteDoc, QTextDocument *synDoc, int textCursorPosition, int synCursorPosition, int noteCursorPosition, QString name, int number, QString action)
{
    if(action == "open"){


        // verify if tree item not already opened, and if yes focus on it :

        if(textWidgetList->size() != 0){
            for(int i = 0; i < textWidgetList->size(); ++i){
                if(textWidgetList->at(i)->document() == textDoc){
                    qDebug() << "tree item already opened, focus on it :";
                         tabWidget->setCurrentWidget(textWidgetList->at(i));
                    return;
                }
            }
        }
        if(textWidgetList->size() == 0){
            tabWidget->clear();
            textDocList->clear();
            noteDocList->clear();
            synDocList->clear();
            nameList->clear();
            textWidgetList->clear();
            noteWidgetList->clear();
            synWidgetList->clear();
            tabNumList->clear();
            numList->clear();
        }



        // open and mem in :

        TextTab *tab = new TextTab;
        connect(tab,SIGNAL(wordCountSignal(int)),stats,SLOT(setWordCount(int)));
        tab->openText(textDoc);
        //       tab->setAttribute(Qt::WA_DeleteOnClose);
        //        qDebug() << "name : " << name;


        QWidget *noteWidget = new QWidget(this);
        QVBoxLayout *nLayout = new QVBoxLayout(noteWidget);
        NoteZone *noteStack = new NoteZone(noteWidget);
        nLayout->addWidget(noteStack);
        noteStack->openNote(noteDoc);
        noteWidget->setLayout(nLayout);


        QWidget *synWidget = new QWidget(this);
        QVBoxLayout *sLayout = new QVBoxLayout(synWidget);
        NoteZone *synStack = new NoteZone(synWidget);
        sLayout->addWidget(synStack);
        synStack->openSyn(synDoc);
        synWidget->setLayout(sLayout);


        textDocList->append(textDoc);
        noteDocList->append(noteDoc);
        synDocList->append(synDoc);
        nameList->append(name);
        textWidgetList->append(tab);
        noteWidgetList->append(noteStack);
        synWidgetList->append(synStack);


        //set objectnames
        QString string;

        tab->setObjectName("tab_" + string.setNum(number,10));
        noteWidget->setObjectName("note_" + string.setNum(number,10));
        synWidget->setObjectName("syn_" + string.setNum(number,10));
        noteStack->setObjectName(noteWidget->objectName() + "-NoteZone");
        synStack->setObjectName(synWidget->objectName() + "-NoteZone");



        // adding to GUI :
        noteLayout->addWidget(noteWidget);
        synLayout->addWidget(synWidget);
        tabWidget->addTab(tab, name);




        numList->append(number);
        //        qDebug() << "added objectname value : " << string.setNum(number,10);



        //display the opened tab (config setting)

        tabWidget->setCurrentWidget(tab);


        //connect edit menu to tab
        connect(editMenu, SIGNAL(widthChangedSignal(int)), tab, SLOT(changeWidthSlot(int)));

        connect(editMenu,SIGNAL(textFontChangedSignal(QFont)),tab,SLOT(changeTextFontSlot(QFont)));
        connect(editMenu,SIGNAL(textHeightChangedSignal(int)),tab,SLOT(changeTextHeightSlot(int)));
        connect(tab,SIGNAL(charFormatChangedSignal(QTextCharFormat)),editMenu,SLOT(charFormatChangedSlot(QTextCharFormat)));


        //connect note & syn to maintree :
        connect(synStack, SIGNAL(connectUpdateTextsSignal()), mainTree, SIGNAL(connectUpdateTextsSignal()));
        connect(synStack, SIGNAL(disconnectUpdateTextsSignal()), mainTree, SIGNAL(disconnectUpdateTextsSignal()));
        connect(noteStack, SIGNAL(connectUpdateTextsSignal()), mainTree, SIGNAL(connectUpdateTextsSignal()));
        connect(noteStack, SIGNAL(disconnectUpdateTextsSignal()), mainTree, SIGNAL(disconnectUpdateTextsSignal()));

        //launch autosaving :
        if(NoProjectOpened)
            autosaveTimer();

        //other :
        NoProjectOpened = false;

        //focus on text :
        tab->setTextFocus();

        //set cursor position :
        tab->setCursorPos(textCursorPosition);
        synStack->setCursorPos(synCursorPosition);
        noteStack->setCursorPos(noteCursorPosition);

        //        QString debug;
        //        qDebug() << "cursorPosition tab : " << debug.setNum(textCursorPosition);
        //        qDebug() << "cursorPosition syn : " << debug.setNum(synCursorPosition);
        //        qDebug() << "cursorPosition note : " << debug.setNum(noteCursorPosition);



        //apply config :

        connect(this, SIGNAL(applyConfigSignal()), tab,SLOT(applyConfig()));
        connect(this, SIGNAL(applyConfigSignal()), synStack,SLOT(applySynConfig()));
        connect(this, SIGNAL(applyConfigSignal()), noteStack,SLOT(applyNoteConfig()));


        editMenu->loadSliderValue();









    }
    //    if(action == "tempSave"){
    //        tempSaveNote(noteFile, noteFile, synFile);
    //    }




    if(action == "save"){
        //      emit properSaveTabTextSignal(textFile, name);



        // Saving

        mainTree->write(file);
        attendList->saveAll();
        //        for(int i = nameList->size()-1; i >= 0; --i){



        //            mainTree->saveDoc(textWidgetList->at(i)->document());
        //            mainTree->saveDoc(noteWidgetList->at(i)->document());
        //            mainTree->saveDoc(synWidgetList->at(i)->document());

        //        }
        qDebug() << "saving all";
    }


    //    qDebug() << "Action Note :" << action;

}


//---------------------------------------------------------------------------
void MainWindow::secondTextSlot(int number, QString action)
{
    if(action == "close"){

        if(!numList->contains(number))
            return;


//        disconnect(tabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));

        QString string;
        TextTab *tab = tabWidget->findChild<TextTab *>("tab_" + string.setNum(number,10));
//        QWidget *noteWidget = noteLayout->findChild<QWidget *>("note_" + string.setNum(number,10));
//        QWidget *synWidget = synLayout->findChild<QWidget *>("syn_" + string.setNum(number,10));
//        //        qDebug() << "objectname value : " << string.setNum(number,10);


        //disconnect edit menu to tab
        disconnect(editMenu, SIGNAL(widthChangedSignal(int)), tab, SLOT(changeWidthSlot(int)));

        disconnect(editMenu,SIGNAL(textFontChangedSignal(QFont)),tab,SLOT(changeTextFontSlot(QFont)));
        disconnect(editMenu,SIGNAL(textHeightChangedSignal(int)),tab,SLOT(changeTextHeightSlot(int)));
        disconnect(tab,SIGNAL(charFormatChangedSignal(QTextCharFormat)),editMenu,SLOT(charFormatChangedSlot(QTextCharFormat)));


int tabNum = tabWidget->indexOf(tab);
tabCloseRequest(tabNum);

//tabWidget->removeTab();

//        noteLayout->removeWidget(noteWidget);
//        synLayout->removeWidget(synWidget);















//        connect(tabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));

    }
}

void MainWindow::setTextTabConnections()
{
    textDocList = new QList<QTextDocument *> ;
    noteDocList = new QList<QTextDocument *> ;
    synDocList = new QList<QTextDocument *> ;
    nameList = new QStringList;
    textWidgetList = new QList<TextTab *>;
    noteWidgetList = new QList<NoteZone *>;
    synWidgetList = new QList<NoteZone *>;

    tabNumList = new QList<int>;
    numList = new QList<int>;

    connect(tabWidget,SIGNAL(currentChanged(int)),noteLayout,SLOT(setCurrentIndex(int)));
    connect(tabWidget,SIGNAL(currentChanged(int)),synLayout,SLOT(setCurrentIndex(int)));
    connect(tabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));
    connect(tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(tabCloseRequest(int)));
    connect(mainTree, SIGNAL(nameChangedSignal(QString,int)), this, SLOT(tabRenamingSlot(QString, int)));
    connect(menu, SIGNAL(openProjectNumberSignal(int)), this, SLOT(setProjectNumberSlot(int)));
    connect(menu, SIGNAL(saveProjectSignal()), this, SLOT(saveAllDocsSlot()));

    connect(this, SIGNAL(tabWidgetWidth(int)), editMenu,SLOT(tabWitdhChangedSlot(int)));
    emit tabWidgetWidth(tabWidget->width());



    //to show previous text :
    connect(editMenu, SIGNAL(showPrevTextSignal(bool)), this, SLOT(showPrevText(bool)));


    //for attendance :

    connect(mainTree, SIGNAL(attendStringSignal(int,QString)), attendList, SLOT(openSheetAttendList(int,QString)));
    connect(mainTree, SIGNAL(allAttendancesSignal(QHash<int,QString>)), attendList, SLOT(updateAllAttendances(QHash<int,QString>)));
    connect(attendList, SIGNAL(projectAttendanceList(QHash<QListWidgetItem*,QDomElement>,QHash<int,QDomElement>)),
            mainTree,SLOT(setOutlinerProjectAttendList(QHash<QListWidgetItem*,QDomElement>,QHash<int,QDomElement>)));
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(setCurrentAttendList(int)));
    connect(attendList, SIGNAL(removeAttendNumberSignal(int)), mainTree, SLOT(removeAttendNumberSlot(int)));
    connect(attendList, SIGNAL(removeAttendNumberFromSheetSignal(QList<int>)), mainTree, SLOT(removeAttendNumberFromSheetSlot(QList<int>)));
    connect(attendList, SIGNAL(addAttendNumberToSheetSignal(QList<int>)), mainTree, SLOT(addAttendNumberToSheetSlot(QList<int>)));
}





//---------------------------------------------------------------------------
void MainWindow::tabChangeSlot(int tabNum)
{

    int preTabNum = -1;

    if(!tabNumList->isEmpty()){
        preTabNum = tabNumList->last();
        textWidgetList->at(tabNum)->updateWordCounts();
        textWidgetList->at(tabNum)->setTextFocus();

    }

    tabNumList->append(tabNum);






    if(preTabNum != -1){

        //        bool textBool = mainTree->saveDoc(textWidgetList->at(preTabNum)->document());
        //        bool noteBool = mainTree->saveDoc(noteWidgetList->at(preTabNum)->document());
        //        bool synBool = mainTree->saveDoc(synWidgetList->at(preTabNum)->document());

        //        qDebug() << "tabChangeRequest textName :" << textWidgetList->at(preTabNum)->objectName() << "----------- saved :" << textBool;
        //        qDebug() << "tabChangeRequest noteName :" << noteWidgetList->at(preTabNum)->objectName() << "----------- saved :" << noteBool;
        //        qDebug() << "tabChangeRequest synName :" << synWidgetList->at(preTabNum)->objectName() << "----------- saved :" << synBool;
        //        qDebug() << "tabChangeRequest pre :" << preTabNum;
        //        qDebug() << "tabChangeRequest name :" << nameList->at(preTabNum);


        //to initialize edit menu fonts:

        TextTab *tab = textWidgetList->at(tabNum);
        editMenu->tabChangedSlot(tab->tabFontChangedSlot());


        setCurrentAttendList(tabNum);


    }
}
//---------------------------------------------------------------------------
void MainWindow::tabCloseRequest(int tabNum)
{
    disconnect(tabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));

    // Saving

    bool textBool = mainTree->saveDoc(textWidgetList->at(tabNum)->document());
    bool noteBool = mainTree->saveDoc(noteWidgetList->at(tabNum)->document());
    bool synBool = mainTree->saveDoc(synWidgetList->at(tabNum)->document());

    mainTree->saveCursorPos(textWidgetList->at(tabNum)->saveCursorPos(),
                            synWidgetList->at(tabNum)->saveCursorPos(),
                            noteWidgetList->at(tabNum)->saveCursorPos(),
                            numList->at(tabNum));


    qDebug() << "tabCloseRequest textName :" << textWidgetList->at(tabNum)->objectName() << "----------- saved :" << textBool;
    qDebug() << "tabCloseRequest noteName :" << noteWidgetList->at(tabNum)->objectName() << "----------- saved :" << noteBool;
    qDebug() << "tabCloseRequest synName :" << synWidgetList->at(tabNum)->objectName() << "----------- saved :" << synBool;
    qDebug() << "tabCloseRequest pre :" << tabNum;
    qDebug() << "tabCloseRequest name :" << nameList->at(tabNum);


    // Closing / removing

    tabWidget->widget(tabNum)->setObjectName("");
    tabWidget->removeTab(tabNum);

    noteLayout->widget(tabNum)->setObjectName("");
    noteLayout->removeWidget(noteLayout->widget(tabNum));

    synLayout->widget(tabNum)->setObjectName("");
    synLayout->removeWidget(synLayout->widget(tabNum));



    textDocList->removeAt(tabNum);
    noteDocList->removeAt(tabNum);
    synDocList->removeAt(tabNum);
    nameList->removeAt(tabNum);
    textWidgetList->removeAt(tabNum);
    noteWidgetList->removeAt(tabNum);
    synWidgetList->removeAt(tabNum);
    numList->removeAt(tabNum);
    tabNumList->removeAt(tabNum);

    //    qDebug() << "tabCloseRequest post :" << tabNum;

    QTimer::singleShot(500, this, SLOT(reconnectAFterTabClose()));

    //    connect(tabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)),Qt::UniqueConnection);

}

void MainWindow::reconnectAFterTabClose()
{
    connect(tabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)),Qt::UniqueConnection);

}

//---------------------------------------------------------------------------

void MainWindow::closeAllDocsSlot()
{
    disconnect(tabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));

    for(int i = nameList->size()-1; i >= 0; --i){


        // Saving


        //        bool textBool = mainTree->saveDoc(textWidgetList->at(i)->document());
        //        bool noteBool = mainTree->saveDoc(noteWidgetList->at(i)->document());
        //        bool synBool = mainTree->saveDoc(synWidgetList->at(i)->document());

        mainTree->saveCursorPos(textWidgetList->at(i)->saveCursorPos(),
                                synWidgetList->at(i)->saveCursorPos(),
                                noteWidgetList->at(i)->saveCursorPos(),
                                numList->at(i));

        qDebug() << "closeAllRequest name :" << nameList->at(i);
        //        qDebug() << "closeAllRequest textName :" << textWidgetList->at(i)->objectName() << "----------- saved :" << textBool;
        //        qDebug() << "closeAllRequest noteName :" << noteWidgetList->at(i)->objectName() << "----------- saved :" << noteBool;
        //        qDebug() << "closeAllRequest synName :" << synWidgetList->at(i)->objectName() << "----------- saved :" << synBool;


        // Closing / removing


        noteWidgetList->at(i)->closeNote();
        synWidgetList->at(i)->closeSyn();

        tabWidget->widget(i)->setObjectName("");
        tabWidget->removeTab(i);

        noteLayout->widget(i)->setObjectName("");
        noteLayout->removeWidget(noteLayout->widget(i));

        synLayout->widget(i)->setObjectName("");
        synLayout->removeWidget(synLayout->widget(i));

    }
    tabWidget->clear();

    textDocList->clear();
    noteDocList->clear();
    synDocList->clear();
    nameList->clear();
    textWidgetList->clear();
    noteWidgetList->clear();
    synWidgetList->clear();
    tabNumList->clear();
    numList->clear();


    connect(tabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));

}

//---------------------------------------------------------------------------

void MainWindow::saveAllDocsSlot()
{

    for(int i = nameList->size()-1; i >= 0; --i){


        // Saving


        //        bool textBool = mainTree->saveDoc(textWidgetList->at(i)->document());
        //        bool noteBool = mainTree->saveDoc(noteWidgetList->at(i)->document());
        //        bool synBool = mainTree->saveDoc(synWidgetList->at(i)->document());

        mainTree->saveCursorPos(textWidgetList->at(i)->saveCursorPos(),
                                synWidgetList->at(i)->saveCursorPos(),
                                noteWidgetList->at(i)->saveCursorPos(),
                                numList->at(i));

        qDebug() << "tabSaveRequest name :" << nameList->at(i);
        //        qDebug() << "tabSaveRequest textName n° " << i << " ---> " << textWidgetList->at(i)->objectName() << "----- saved :" << textBool;
        //        qDebug() << "tabSaveRequest noteName n° " << i << " ---> " << noteWidgetList->at(i)->objectName() << "----- saved :" << noteBool;
        //        qDebug() << "tabSaveRequest synName  n° " << i << " ---> " << synWidgetList->at(i)->objectName() << "----- saved :" << synBool;
    }
    mainTree->write(file);
}

//---------------------------------------------------------------------------

void MainWindow::tabRenamingSlot(QString newName, int number)
{


    if(!numList->contains(number))
        return;


    //        disconnect(tabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));

    QString string;
    TextTab *tab = tabWidget->findChild<TextTab *>("tab_" + string.setNum(number,10));

    qDebug() << "tabRenamingSlot : " << string.setNum(number,10);

    tabWidget->setTabText(tabWidget->indexOf(tab),newName);

    //    connect(tabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));


}





//---------------------------------------------------------------------------

void MainWindow::readSettings()
{
    QSettings settings;
    settings.beginGroup( "MainWindow" );
    restoreState( settings.value( "wmState" ).toByteArray(),1 );
    resize(settings.value( "size", QSize( 1000, 750 ) ).toSize() );
    move(settings.value( "pos" ).toPoint() );
    m_firstStart = settings.value("firstStart", true).toBool();
    settings.endGroup();




}

//---------------------------------------------------------------------------

void MainWindow::writeSettings()
{

    QSettings settings;
    settings.beginGroup( "MainWindow" );
    settings.setValue( "wmState", saveState(1) );
    settings.setValue( "size", size() );
    settings.setValue( "pos", pos() );
    settings.setValue( "firstStart", false);
    settings.endGroup();

    qDebug() << "main settings saved";
}


//---------------------------------------------------------------------------


void MainWindow::closeEvent(QCloseEvent* event)
{




    QMessageBox msgBox(this);
    msgBox.setText("Do you want to quit ?");
    msgBox.setInformativeText("Your changes are already saved.");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Ok:


        writeSettings();

        closeProjectSlot();
        event->accept();

        break;

    case QMessageBox::Cancel:
        event->ignore();


        break;
    default:
        // should never be reached
        break;
    }






}

//----------------------------------------------------------------------------------------

void MainWindow::resizeEvent(QResizeEvent* event)
{
    emit tabWidgetWidth(tabWidget->width());
}

//----------------------------------------------------------------------------------------

void MainWindow::autosaveTimer()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(textSlot()));
    timer->start(autosaveTime);
}









//---------------------------------------------------------------------------
//----------Apply Config---------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------


void MainWindow::applyConfig()
{
    emit applyConfigSignal();

    QSettings settings;
    settings.beginGroup( "Settings" );
    autosaveTime = settings.value("autosaveTime", 20000).toInt();
    settings.endGroup();

    if(!NoProjectOpened)
        configTimer();


    editMenu->applyConfig();
}

//---------------------------------------------------------------------------

void MainWindow::configTimer()
{
    timer->stop();
    timer->start(autosaveTime);

    QString debug;
    qDebug() << "autosaveTime" << debug.setNum(autosaveTime);
}
























//----------------------------------------------------------------------------



void MainWindow::editFullscreen()
{




    qDebug() << tabWidget->currentWidget()->objectName();


    QString tabName = tabWidget->currentWidget()->objectName();
    TextTab *tab = tabWidget->findChild<TextTab *>(tabName);

    QString synName = synLayout->currentWidget()->objectName();
    QWidget *syn = this->findChild<QWidget *>(synName);

    QString noteName = noteLayout->currentWidget()->objectName();
    QWidget *note = this->findChild<QWidget *>(noteName);


    //    if(this->findChild<QWidget *>(noteName) == 0)
    //        qDebug() << "note == 0";

    qDebug() << "noteName : " << noteName;

    NoteZone *synStack = syn->findChild<NoteZone *>(synName + "-NoteZone");
    NoteZone *noteStack = note->findChild<NoteZone *>(noteName + "-NoteZone");


    qDebug() << "synStack name : " << synStack->objectName();

    fullEditor = new FullscreenEditor(tab->document(), tab->saveCursorPos(), 0);

    fullEditor->setSyn(synStack->document(), synStack->textCursor().position());
    fullEditor->setNote(noteStack->document(), noteStack->textCursor().position());


    connect(tab,SIGNAL(wordCountSignal(int)),fullEditor,SLOT(setWordCount(int)));
    connect(stats,SIGNAL(timerSignal(QString)),fullEditor,SLOT(setTimer(QString)));
    connect(fullEditor, SIGNAL(closeSignal()),tab, SLOT(updateTextZone()));
    connect(fullEditor, SIGNAL(closeSignal()),synStack, SLOT(updateTextZone()));
    connect(fullEditor, SIGNAL(closeSignal()),noteStack, SLOT(updateTextZone()));

}

//----------------------------------------------------------------------------

void MainWindow::launchOutliner()
{

    mainTree->launchOutliner();



}

//----------------------------------------------------------------------------


void MainWindow::showPrevText(bool showPrevTextBool)
{

    int number = tabWidget->currentWidget()->objectName().mid(tabWidget->currentWidget()->objectName().indexOf("_") + 1).toInt();

    QString string;
    TextTab *tab = tabWidget->findChild<TextTab *>("tab_" + string.setNum(number,10));

    tab->showPrevText(showPrevTextBool);

    tab->setPrevText(mainTree->prevText(number));
}

//----------------------------------------------------------------------------

void MainWindow::setCurrentAttendList(int tabNum)
{
    if(tabNum == -1)
    tabNum = 0;

    if(tabWidget->count() == 0)
    return;

    int number = tabWidget->widget(tabNum)->objectName().mid(tabWidget->widget(tabNum)->objectName().indexOf("_") + 1).toInt();

    attendList->setCurrentList(number);
}
