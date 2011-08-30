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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), firstOpen(true)
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
    tabWidget->setMovable(true);

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
        firstStart.setText(tr("<center><b>Hello ! Welcome to Plume Creator v0.2 !</b></center>"
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

    connect(mainTree, SIGNAL(textAndNoteSignal(QFile*,QFile*,QFile*, int, int, int, QString, int, QString)), this, SLOT(textSlot(QFile*,QFile*,QFile*, int,int,int, QString, int, QString)));
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
    QGroupBox *synopsysBox = new QGroupBox;
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


    //    QToolButton *keepVisibleButton = new QToolButton(this);
    //    keepVisibleButton->setText(tr("Visible"));
    //    keepVisibleButton->setShortcut(Qt::Key_F11);
    //    keepVisibleButton->setCheckable(true);
    //    keepVisibleButton->setToolTip(tr("Keep this dock visible"));
    //    connect(keepVisibleButton, SIGNAL(toggled(bool)), tabWidget, SLOT(showFullScreen()));;


    QComboBox *stateCombo = new QComboBox;
    midLayout->addWidget(tabFullscreenButton,0,0);
    midLayout->addWidget(stateCombo);
    midFrame->setLayout(midLayout);

    synopsysBox->setLayout(synLayout);

    noteBox->setLayout(noteLayout);


    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(synopsysBox);
    layout->addWidget(midFrame);
    layout->addWidget(noteBox);
    frame->setLayout(layout);


    noteDock->setWidget(frame);

    QStringList list;
    list << tr("Draft") << tr("25%") << tr("50%") << tr("75%") << tr("Done") << tr("Corrected");
    stateCombo->insertItems(0, list);


    synopsysBox->setTitle(tr("Synopsys"));
    noteBox->setTitle(tr("Note"));

    addDockWidget(Qt::BottomDockWidgetArea, noteDock);


}




//---------------------------------------------------------------------------

void MainWindow::openProjectSlot(QFile *projectFile)
{
    closeAllDocsSlot();
    file = projectFile;
    mainTree->read(projectFile);
}

//---------------------------------------------------------------------------

void MainWindow::closeProjectSlot()
{

    closeAllDocsSlot();

    mainTree->write(file);
    mainTree->closeTree();
    firstOpen = true;

    QSettings settings;
    settings.beginWriteArray("Manager/projects");
    settings.setArrayIndex(settingNumber);
    settings.setValue("lastModified", QDateTime::currentDateTime().toString());
    settings.endArray();


}

void MainWindow::setProjectNumberSlot(int prjNumber)
{
    settingNumber = prjNumber;
}




//---------------------------------------------------------------------------





void MainWindow::textSlot(QFile *textFile, QFile *noteFile, QFile *synFile, int textCursorPosition, int synCursorPosition, int noteCursorPosition, QString name, int number, QString action)
{
    if(action == "open"){


        // verify if tree item not already opened, and if yes focus on it :

        if(!textFileList->isEmpty()){
            for(int i = 0; i < textFileList->size(); ++i){
                if(textFileList->at(i)->fileName() == textFile->fileName()){
                    tabWidget->setCurrentWidget(textWidgetList->at(i));
                    return;
                }
            }
        }
        // open and mem in :

        TextTab *tab = new TextTab(textFile, name);
        connect(tab,SIGNAL(wordCountSignal(int)),stats,SLOT(setWordCount(int)));
        tab->openText(textFile, name);
        tabWidget->addTab(tab, name);

        NoteZone *noteStack = new NoteZone;
        noteStack->openNote(noteFile, name);
        noteLayout->addWidget(noteStack);

        NoteZone *synStack = new NoteZone;
        synStack->openSyn(synFile, name);
        synLayout->addWidget(synStack);

        textFileList->append(textFile);
        noteFileList->append(noteFile);
        synFileList->append(synFile);
        nameList->append(name);
        textWidgetList->append(tab);
        noteWidgetList->append(noteStack);
        synWidgetList->append(synStack);


        //set objectnames
        QString string;

        tab->setObjectName("tab_" + string.setNum(number,10));
        noteStack->setObjectName("note_" + string.setNum(number,10));
        synStack->setObjectName("syn_" + string.setNum(number,10));

        numList->append(number);
        qDebug() << "added objectname value : " << string.setNum(number,10);



        //display the opened tab (config setting)

        tabWidget->setCurrentWidget(tab);


        //connect edit menu to tab
        connect(editMenu, SIGNAL(widthChangedSignal(int)), tab, SLOT(changeWidthSlot(int)));
        editMenu->loadSliderValue();

        connect(editMenu,SIGNAL(textFontChangedSignal(QFont)),tab,SLOT(changeTextFontSlot(QFont)));
        connect(editMenu,SIGNAL(textHeightChangedSignal(int)),tab,SLOT(changeTextHeightSlot(int)));
        connect(tab,SIGNAL(charFormatChangedSignal(QTextCharFormat)),editMenu,SLOT(charFormatChangedSlot(QTextCharFormat)));


        //launch autosaving :
        if(firstOpen)
            autosaveTimer();



        //focus on text :
        tab->setTextFocus();

        //set cursor position :
        tab->setCursorPos(textCursorPosition);
        synStack->setCursorPos(synCursorPosition);
        noteStack->setCursorPos(noteCursorPosition);

        QString debug;
        qDebug() << "cursorPosition tab : " << debug.setNum(textCursorPosition);
        qDebug() << "cursorPosition syn : " << debug.setNum(synCursorPosition);
        qDebug() << "cursorPosition note : " << debug.setNum(noteCursorPosition);
        //other :
        firstOpen = false;


        //apply config :

        connect(this, SIGNAL(applyConfigSignal()), tab,SLOT(applyConfig()));











    }
    //    if(action == "tempSave"){
    //        tempSaveNote(noteFile, noteFile, synFile);
    //    }




    if(action == "save"){
        //      emit properSaveTabTextSignal(textFile, name);



        // Saving


        for(int i = nameList->size()-1; i >= 0; --i){




            textWidgetList->at(i)->saveText(textFileList->at(i),nameList->at(i));
            noteWidgetList->at(i)->saveNote(noteFileList->at(i),nameList->at(i));
            synWidgetList->at(i)->saveSyn(synFileList->at(i), nameList->at(i));


        }
        qDebug() << "saving all";
    }


    qDebug() << "Action Note :" << action;

}


//---------------------------------------------------------------------------
void MainWindow::secondTextSlot(int number, QString action)
{
    if(action == "close"){

        if(!numList->contains(number))
            return;


        disconnect(tabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));

        QString string;
        TextTab *tab = tabWidget->findChild<TextTab *>("tab_" + string.setNum(number,10));
        NoteZone *noteStack = noteLayout->findChild<NoteZone *>("note_" + string.setNum(number,10));
        NoteZone *synStack = synLayout->findChild<NoteZone *>("syn_" + string.setNum(number,10));
        qDebug() << "objectname value : " << string.setNum(number,10);

        tabWidget->removeTab(tabWidget->indexOf(tab));
        noteLayout->removeWidget(noteStack);
        synLayout->removeWidget(synStack);

        connect(tabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));

    }
}

void MainWindow::setTextTabConnections()
{
    textFileList = new QList<QFile *> ;
    noteFileList = new QList<QFile *> ;
    synFileList = new QList<QFile *> ;
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

    connect(this, SIGNAL(tabWidgetWidth(int)), editMenu,SLOT(tabWitdhChangedSlot(int)));
    emit tabWidgetWidth(tabWidget->width());
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
        bool textBool = textWidgetList->at(preTabNum)->saveText(textFileList->at(preTabNum),nameList->at(preTabNum));
        bool noteBool = noteWidgetList->at(preTabNum)->saveNote(noteFileList->at(preTabNum),nameList->at(preTabNum));
        bool synBool = synWidgetList->at(preTabNum)->saveSyn(synFileList->at(preTabNum), nameList->at(preTabNum));


        qDebug() << "tabChangeRequest textFile :" << textFileList->at(preTabNum)->fileName() << "----------- saved :" << textBool;
        qDebug() << "tabChangeRequest noteFile :" << noteFileList->at(preTabNum)->fileName() << "----------- saved :" << noteBool;
        qDebug() << "tabChangeRequest synFile :" << synFileList->at(preTabNum)->fileName() << "----------- saved :" << synBool;
        qDebug() << "tabChangeRequest pre :" << preTabNum;
        qDebug() << "tabChangeRequest name :" << nameList->at(preTabNum);


        //to initialize edit menu fonts:

        TextTab *tab = textWidgetList->at(tabNum);
        editMenu->tabChangedSlot(tab->tabChangedSlot());
    }
}
//---------------------------------------------------------------------------
void MainWindow::tabCloseRequest(int tabNum)
{
    disconnect(tabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));

    // Saving

    bool textBool = textWidgetList->at(tabNum)->saveText(textFileList->at(tabNum),nameList->at(tabNum));
    bool noteBool = noteWidgetList->at(tabNum)->saveNote(noteFileList->at(tabNum),nameList->at(tabNum));
    bool synBool = synWidgetList->at(tabNum)->saveSyn(synFileList->at(tabNum), nameList->at(tabNum));

    mainTree->saveCursorPos(textWidgetList->at(tabNum)->saveCursorPos(),
                            synWidgetList->at(tabNum)->saveCursorPos(),
                            noteWidgetList->at(tabNum)->saveCursorPos(),
                            numList->at(tabNum));


    qDebug() << "tabCloseRequest textFile :" << textFileList->at(tabNum)->fileName() << "----------- saved :" << textBool;
    qDebug() << "tabCloseRequest noteFile :" << noteFileList->at(tabNum)->fileName() << "----------- saved :" << noteBool;
    qDebug() << "tabCloseRequest synFile :" << synFileList->at(tabNum)->fileName() << "----------- saved :" << synBool;
    qDebug() << "tabCloseRequest pre :" << tabNum;
    qDebug() << "tabCloseRequest name :" << nameList->at(tabNum);


    // Closing / removing

    tabWidget->removeTab(tabNum);
    noteLayout->removeWidget(noteLayout->widget(tabNum));
    synLayout->removeWidget(synLayout->widget(tabNum));

    textFileList->removeAt(tabNum);
    noteFileList->removeAt(tabNum);
    synFileList->removeAt(tabNum);
    nameList->removeAt(tabNum);
    textWidgetList->removeAt(tabNum);
    noteWidgetList->removeAt(tabNum);
    synWidgetList->removeAt(tabNum);
    numList->removeAt(tabNum);
    tabNumList->removeAt(tabNum);

    qDebug() << "tabCloseRequest post :" << tabNum;

    connect(tabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));

}
//---------------------------------------------------------------------------

void MainWindow::closeAllDocsSlot()
{
    disconnect(tabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));

    for(int i = nameList->size()-1; i >= 0; --i){


        // Saving


        bool textBool = textWidgetList->at(i)->saveText(textFileList->at(i),nameList->at(i));
        bool noteBool = noteWidgetList->at(i)->saveNote(noteFileList->at(i),nameList->at(i));
        bool synBool = synWidgetList->at(i)->saveSyn(synFileList->at(i), nameList->at(i));

        mainTree->saveCursorPos(textWidgetList->at(i)->saveCursorPos(),
                                synWidgetList->at(i)->saveCursorPos(),
                                noteWidgetList->at(i)->saveCursorPos(),
                                numList->at(i));

        qDebug() << "tabCloseRequest name :" << nameList->at(i);
        qDebug() << "tabCloseRequest textFile n° " << i << " ---> " << textFileList->at(i)->fileName() << "----- saved :" << textBool;
        qDebug() << "tabCloseRequest noteFile n° " << i << " ---> " << noteFileList->at(i)->fileName() << "----- saved :" << noteBool;
        qDebug() << "tabCloseRequest synFile  n° " << i << " ---> " << synFileList->at(i)->fileName() << "----- saved :" << synBool;




        // Closing / removing


        noteWidgetList->at(i)->closeNote(noteFileList->at(i),nameList->at(i));
        synWidgetList->at(i)->closeSyn(synFileList->at(i), nameList->at(i));

        tabWidget->widget(i)->setObjectName("");
        tabWidget->removeTab(i);
        noteLayout->removeWidget(noteLayout->widget(i));
        synLayout->removeWidget(synLayout->widget(i));



    }
    tabWidget->clear();

    textFileList->clear();
    noteFileList->clear();
    synFileList->clear();
    nameList->clear();
    textWidgetList->clear();
    noteWidgetList->clear();
    synWidgetList->clear();
    tabNumList->clear();
    numList->clear();


    connect(tabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));

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
    autosaveTime = settings.value("autosaveTime", 20).toInt();
    settings.endGroup();

    if(!firstOpen)
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

    fullEditor = new FullscreenEditor(tab->document(), tab->saveCursorPos(), 0);


    connect(tab,SIGNAL(wordCountSignal(int)),fullEditor,SLOT(setWordCount(int)));
    connect(stats,SIGNAL(timerSignal(QString)),fullEditor,SLOT(setTimer(QString)));
    connect(fullEditor, SIGNAL(closeSignal()),tab, SLOT(updateTextZone()));


}
