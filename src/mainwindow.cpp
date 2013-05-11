#include <QSettings>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QRect>


#include "mainwindow.h"
#include "newprojectwizard.h"
#include "menubar.h"
#include "maintree.h"
#include "statsbox.h"
#include "itembox.h"
#include "texttab.h"
#include "notezone.h"
#include "ui_mainwindow.h"

//

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),noTabCss(""),textAlreadyChanged(false)
    ,ui(new Ui::MainWindow),  isExternalProjectOpeningBool(false)

{

    ui->setupUi(this);


    systemTray = new QSystemTrayIcon(0);
    systemTray->setIcon(QIcon(":/pics/plume-creator.png"));

    this->setMinimumSize(800, 400);
    setWindowTitle("Plume Creator");

    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);
    setFocusPolicy(Qt::WheelFocus);

    //temporary config
//    setStyleSheet("* {background-color: pink; color: black;}");

    //     netbook mode 10':
    //        setFixedSize(900, 550);

    hub = new Hub(this);
    connect(hub, SIGNAL(openProjectSignal(QFile*)), this, SLOT(openProjectSlot(QFile*)));
    connect(hub, SIGNAL(closeProjectSignal()), this, SLOT(closeProjectSlot()));
    connect(hub, SIGNAL(textAlreadyChangedSignal(bool)), this, SLOT(textAlreadyChangedSlot(bool)));
    connect(hub, SIGNAL(showStatusBarMessageSignal(QString,int)), ui->bar, SLOT(showMessage(QString,int)));
    textStyles = new TextStyles();
    textStyles->setHub(hub);


    ui->mainTabWidget->setTabsClosable(true);
    ui->mainTabWidget->setTabShape(QTabWidget::Triangular);
    ui->mainTabWidget->setDocumentMode(true);
    ui->mainTabWidget->setMovable(false);



    createMenuBar();
    //    createMenuDock();
    createAttendDock();
    createTreeDock();
    createNoteDock();
    createToolDock();
    createDocksToolBar();
    createStatusBar();

menu->setMenusEnabled(false);

    setConnections();


    // Config :
    readSettings();
    connect(menu, SIGNAL(applyConfigSignal()), this,SLOT(applyConfig()));
    applyConfig();


    readDocksSettings();
    giveStyle();



    // Welcome dialog at first start

    if (m_firstStart){
        QMessageBox firstStart;
        firstStart.setWindowTitle(tr("Welcome"));
        firstStart.setText(tr("<center><b>Hello ! Welcome to Plume Creator v") + QApplication::applicationVersion() + tr("!</b></center>"
                                                                                                                         "<p>Plume Creator is a little program for writers"
                                                                                                                         " in quest of a complete yet simple way of"
                                                                                                                         " writing and organizing a fiction.</p>"
                                                                                                                         "<br>"
                                                                                                                         "<p>It allows :"
                                                                                                                         "<blockquote>- fullscreen text editing</blockquote>"
                                                                                                                         "<blockquote>- chapters and scenes outlining</blockquote>"
                                                                                                                         "<blockquote>- note taking</blockquote>"
                                                                                                                         "<blockquote>- items/characters/places managing</blockquote></p>"
                                                                                                                         "<p><b><h1>This is a Beta software ! It's stable but all the features are not finished !</h1></b></p>"


                                                                                                                         ));
        firstStart.exec();



        NewProjectWizard projectWizard;
        projectWizard.exec();

        m_firstStart = false;


    }


    if (m_firstStart_checkDisplay && QApplication::desktop()->availableGeometry().height() < 650){
        QMessageBox firstStart_checkDisplay;
        firstStart_checkDisplay.setWindowTitle(tr("Small screen size detected"));
        firstStart_checkDisplay.setText(tr("<center><b>A small screen has been detected</b></center>"
                                           "<p>Maybe you are running Plume on a netbook."
                                           " Plume will adapt its interface to this type of computer."
                                           "</p>"
                                           "<br>"
                                           "<p>You can change this option in the 'Configure' dialog."
                                           "</p>"
                                           ));
        firstStart_checkDisplay.exec();

        QSettings settings;
        settings.setValue("MainWindow/displayMode", "netbook");
        setDisplayMode("netbook");

        m_firstStart_checkDisplay = false;


    }





    //    if(checkScreenResAtStartupBool){
    //        QRect scrGeom = QDesktopWidget::availableGeometry();
    //        if(scrGeom.x() <= 1024 && scrGeom.x() <= 600)

    //    }





}



void MainWindow::postConstructor()
{
    if(isExternalProjectOpeningBool == false){
        menu->openStartCenter();
    }
    isExternalProjectOpeningBool = false;

    if (checkUpdateAtStartupBool){
        launchSlimUpdater("auto");
    }


}

MainWindow::~MainWindow()
{


    delete ui;
}



//---------------------------------------------------------------------------

void MainWindow::createMenuBar()
{
    menu = new MenuBar;
    menu->setTextStyles(textStyles);
    menu->setHub(hub);
    menu->createContent();



    connect(menu,SIGNAL(exitSignal()), this, SLOT(close()));
    connect(menu, SIGNAL(setDisplayModeSignal(QString)), this, SLOT(setDisplayMode(QString)));
    connect(menu, SIGNAL(changeAllDocsTextStylesSignal()), this, SIGNAL(changeAllDocsTextStylesSignal()));

    connect(menu, SIGNAL(launchCheckUpdateSignal(QString)), this, SLOT(launchSlimUpdater(QString)));


    menu->firstLaunch();

    this->setMenuBar(menu->createMenuBar());

}

//---------------------------------------------------------------------------

void MainWindow::createAttendDock()
{
    attendDock = new QDockWidget(this);
    attendDock->setWindowTitle(tr("Attendance"));
    attendDock->setObjectName("attendDock");
    attendDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);

    attendBase = new AttendBase;
    attendBase->setHub(hub);
    attendBase->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    attendBase->setLineWidth(2);
    attendBase->setMidLineWidth(3);

    attendDock->setWidget(attendBase);
    addDockWidget(Qt::RightDockWidgetArea, attendDock);

    connect(attendDock, SIGNAL(visibilityChanged(bool)), this, SLOT(checkHiddenDocks()));
    attendDock->hide();
}

//---------------------------------------------------------------------------

void MainWindow::createMenuDock()
{

    //    QDockWidget *menuDock = new QDockWidget;

    //    menuDock->setObjectName("menuDock");


    //    menuDock->setWindowTitle(tr("Menus"));
    //    menuDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    //    menuDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    //    menuDock->setMinimumSize(160,350);
    //    //    menuDock->setMaximumWidth(200);

    //    toolBox = new QToolBox;



    // page 1
    //    menu = new MenuBar;

    //    menu->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    //    menu->setLineWidth(2);
    //    menu->setMidLineWidth(3);


    //    toolBox->addItem(menu, tr("Menu"));

    //    connect(menu, SIGNAL(openProjectSignal(QFile*)), this, SLOT(openProjectSlot(QFile*)));

    //    connect(menu,SIGNAL(exitSignal()), this, SLOT(close()));
    //    connect(menu, SIGNAL(closeProjectSignal()), this, SLOT(closeProjectSlot()));


    //    menu->firstLaunch();
    //    toolBox->setCurrentWidget(menu);



    //page 2

    //    editMenu = new EditMenuBar;

    //    editMenu->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    //    editMenu->setLineWidth(2);
    //    editMenu->setMidLineWidth(3);

    //    toolBox->addItem(editMenu, tr("Edit Menu"));

    //page 3

    //    attendList = new AttendBox;

    //    attendList->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    //    attendList->setLineWidth(2);
    //    attendList->setMidLineWidth(3);

    //    toolBox->addItem(attendList, tr("Attendance"));


    //    toolBox->setFrameStyle(QFrame::Panel | QFrame::Raised);
    //    toolBox->setLineWidth(2);
    //    toolBox->setMidLineWidth(3);

    //    menuDock->setWidget(toolBox);

    //    addDockWidget(Qt::RightDockWidgetArea, menuDock);




}

//---------------------------------------------------------------------------


void MainWindow::createTreeDock()
{

    treeDock = new QDockWidget(this);

    treeDock->setObjectName("projectDock");
    treeDock->setWindowTitle(tr("Project"));
    treeDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    treeDock->setMinimumSize(160,200);
    treeDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);

    mainTree = new MainTree;
    mainTree->setHub(hub);

    connect(mainTree, SIGNAL(textAndNoteSignal(int, QString)), this, SLOT(textSlot(int, QString)));
    connect(this, SIGNAL(changeAllDocsTextStylesSignal()), mainTree, SLOT(changeAllDocsTextStyles()));

    treeDock->setWidget(mainTree);

    addDockWidget(Qt::LeftDockWidgetArea, treeDock);

    connect(treeDock, SIGNAL(visibilityChanged(bool)), this, SLOT(checkHiddenDocks()));

    mainTree->setTextStyles(textStyles);
}

//---------------------------------------------------------------------------


void MainWindow::createToolDock()
{

    toolDock = new QDockWidget(this);

    toolDock->setObjectName("toolDock");
    toolDock->setWindowTitle(tr("Tools"));


    toolDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    toolDock->setMinimumSize(160,200);
    //    toolDock->setMaximumWidth(200);

    QToolBox *toolBox = new QToolBox;
    stats = new StatsBox;
    stats->setHub(hub);//    items = new ItemBox;

    // page 1

    toolBox->addItem(stats, tr("Stats"));
    stats->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    stats->setLineWidth(2);
    stats->setMidLineWidth(3);

    //page 2

    //    toolBox->addItem(items, tr("Items"));
    //    items->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    //    items->setLineWidth(2);
    //    items->setMidLineWidth(3);

    toolBox->setCurrentWidget(stats);

    toolBox->setFrameStyle(QFrame::Panel | QFrame::Raised);
    toolBox->setLineWidth(2);
    toolBox->setMidLineWidth(3);

    toolDock->setWidget(toolBox);

    addDockWidget(Qt::LeftDockWidgetArea, toolDock);


    connect(toolDock, SIGNAL(visibilityChanged(bool)), this, SLOT(checkHiddenDocks()));

    //default behaviour :
    toolDock->hide();

}

//---------------------------------------------------------------------------------------------

void MainWindow::createNoteDock()
{

    noteDock = new QDockWidget(this);

    noteDock->setObjectName("noteDock");
    noteDock->setWindowTitle(tr("Notes"));
    noteDock->setAllowedAreas(Qt::BottomDockWidgetArea);


    synLayout = new QStackedLayout;

    noteLayout = new QStackedLayout;
    QGroupBox *synopsisBox = new QGroupBox;
    synopsisBox->setObjectName("synBox");
    synopsisBox->setContentsMargins(0,0,0,0);
    QGroupBox *noteBox = new QGroupBox;
    noteBox->setObjectName("noteBox");
    noteBox->setContentsMargins(0,0,0,0);
    noteSplitter = new QSplitter;
    noteSplitter->setObjectName("notesFrame");
    //    frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
    //    frame->setLineWidth(2);
    //    frame->setMidLineWidth(3);


    //    QFrame *midFrame = new QFrame;
    //    midFrame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    //    midFrame->setLineWidth(1);
    //    midFrame->setMidLineWidth(3);

    //    QGridLayout *midLayout = new QGridLayout;

    //    QToolButton *hideNotesButton = new QToolButton(this);
    //    hideNotesButton->setText(tr("&Hide Notes"));
    //    hideNotesButton->setShortcut(Qt::Key_F10);
    //    hideNotesButton->setToolTip(tr("Hide the notes"));
    //    connect(hideNotesButton, SIGNAL(clicked()), noteDock, SLOT(hide()));

    //    QToolButton *tabFullscreenButton = new QToolButton(this);
    //    tabFullscreenButton->setText(tr("Fullscreen &Edit"));
    //    tabFullscreenButton->setShortcut(Qt::Key_F11);
    //    tabFullscreenButton->setToolTip(tr("Edit this document fullscreen"));
    //    connect(tabFullscreenButton, SIGNAL(clicked()), this, SLOT(editFullscreen()));

    //    QToolButton *outlinerButton = new QToolButton(this);
    //    outlinerButton->setText(tr("Outliner"));
    //    outlinerButton->setShortcut(Qt::Key_F12);
    //    outlinerButton->setToolTip(tr("Launch the project outliner"));
    //    connect(outlinerButton, SIGNAL(clicked()), this, SLOT(launchOutliner()));

    //    QToolButton *keepVisibleButton = new QToolButton(this);
    //    keepVisibleButton->setText(tr("Visible"));
    //    keepVisibleButton->setShortcut(Qt::Key_F11);
    //    keepVisibleButton->setCheckable(true);
    //    keepVisibleButton->setToolTip(tr("Keep this dock visible"));
    //    connect(keepVisibleButton, SIGNAL(toggled(bool)), mainTabWidget, SLOT(showFullScreen()));;

    //    QComboBox *stateCombo = new QComboBox;


    //    midLayout->addWidget(hideNotesButton,0,0, Qt::AlignHCenter);
    //    midLayout->addWidget(tabFullscreenButton,1,0, Qt::AlignHCenter);
    //    midLayout->addWidget(outlinerButton,2,0, Qt::AlignHCenter);
    //   midLayout->addWidget(stateCombo);
    //  midFrame->setLayout(midLayout);

    synopsisBox->setLayout(synLayout);

    noteBox->setLayout(noteLayout);

    QWidget *noteDockLayoutWidget = new QWidget();

    noteDockLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    noteDockLayout->setMargin(0);
    noteDockLayout->setSpacing(0);
    noteDockLayout->setContentsMargins(0,0,0,0);
    noteDockLayout->addWidget(synopsisBox);
    //    layout->addWidget(midFrame);
    noteDockLayout->addWidget(noteBox);
    noteDockLayoutWidget->setLayout(noteDockLayout);
noteSplitter->addWidget(noteDockLayoutWidget);

    noteDock->setWidget(noteSplitter);

    QStringList list;
    list << tr("Draft") << tr("25%") << tr("50%") << tr("75%") << tr("Done") << tr("Corrected");
    //    stateCombo->insertItems(0, list);


    synopsisBox->setTitle(tr("Synopsis"));
    noteBox->setTitle(tr("Note"));

    addDockWidget(Qt::BottomDockWidgetArea, noteDock);



    connect(noteDock, SIGNAL(visibilityChanged(bool)), this, SLOT(checkHiddenDocks()));
    connect(noteDock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(changeOrientationOfNoteDock(Qt::DockWidgetArea)));


    //default behaviour :
    noteDock->hide();



    //    QWidget *widgetToHideWith_syn = new QWidget;
    //    QWidget *widgetToHideWith_note = new QWidget;
    //    QWidget *widgetToHideWith_mid = new QWidget;

    //    widgetToHideWith_syn->setLayout(synLayout);
    //    widgetToHideWith_note->setLayout(noteLayout);
    //    widgetToHideWith_mid->setLayout(midLayout);

}
//---------------------------------------------------------------------------


void MainWindow::changeOrientationOfNoteDock(Qt::DockWidgetArea noteDockArea)
{
    if(noteDockArea == Qt::LeftDockWidgetArea || noteDockArea == Qt::RightDockWidgetArea){
        noteDockLayout->setDirection(QBoxLayout::TopToBottom);
        noteSplitter->setOrientation(Qt::Vertical);
    }
    else if(noteDockArea == Qt::TopDockWidgetArea || noteDockArea == Qt::BottomDockWidgetArea){
        noteDockLayout->setDirection(QBoxLayout::LeftToRight);
        noteSplitter->setOrientation(Qt::Horizontal);
    }
}

//---------------------------------------------------------------------------

void MainWindow::createStatusBar()
{


    sceneWCLabel = new QLabel();

    QWidget *stretcher1 = new QWidget();
    stretcher1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
    QWidget *stretcher2 = new QWidget();
    stretcher2->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);

    wordGoalBar = new WordGoalProgressBar(this);
    wordGoalBar->setHub(hub);
    wordGoalBar->postConstructor();

    projectWCLabel = new QLabel();
    projectWCLabel->setToolTip(tr("Project word count"));
    bookWCLabel = new QLabel();
    bookWCLabel->setToolTip(tr("Book word count"));
    chapterWCLabel = new QLabel();
    chapterWCLabel->setToolTip(tr("Chapter word count"));
    currentWCLabel = new QLabel();
    currentWCLabel->setToolTip(tr("Current sheet word count"));

    connect(hub, SIGNAL(projectWordCount(int)), this, SLOT(updateProjectWCLabel(int)));
    connect(hub, SIGNAL(bookWordCount(int)), this, SLOT(updateBookWCLabel(int)));
    connect(hub, SIGNAL(chapterWordCount(int)), this, SLOT(updateChapterWCLabel(int)));
    connect(hub, SIGNAL(currentSheetWordCount(int)), this, SLOT(updateCurrentSheetWCLabel(int)));


    ui->bar->addWidget(projectWCLabel);
    ui->bar->addWidget(bookWCLabel);
    ui->bar->addWidget(chapterWCLabel);
    ui->bar->addWidget(stretcher1, 5);
    ui->bar->addWidget(currentWCLabel);


    //    ui->bar->addPermanentWidget(sceneWCLabel,1);
    //    ui->bar->addPermanentWidget(stretcher2,10);
    ui->bar->addWidget(stretcher2,10);
    //    bar->addPermanentWidget(showPrevSceneButton,2);
    //    bar->addPermanentWidget(status_tabFullscreenButton,2);
    ui->bar->addPermanentWidget(wordGoalBar);
    //    bar->addPermanentWidget(stretcher2);





    this->setStatusBar(ui->bar);





}




//---------------------------------------------------------------------------

void MainWindow::createDocksToolBar()
{

    //    For menu bar :
    connect(menu, SIGNAL(showNotesDockSignal(bool)), noteDock, SLOT(setVisible(bool)));
    connect(menu, SIGNAL(showAttendDockSignal(bool)), attendDock, SLOT(setVisible(bool)));
    connect(menu, SIGNAL(showToolsDockSignal(bool)), toolDock, SLOT(setVisible(bool)));
    connect(menu, SIGNAL(launchOutlinerSignal()), this, SLOT(launchOutliner()));
    connect(menu, SIGNAL(showFullscreenSignal()), this, SLOT(editFullscreen()));





    docksToolBar = new QToolBar(tr("Side buttons"), this);
    docksToolBar->setObjectName("docksToolBar");
    docksToolBar->setOrientation(Qt::Vertical);
    docksToolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);
    docksToolBar->setMovable(true);
    docksToolBar->setFloatable(false);


    treeDockButton = new OrientationButton(QIcon(":/pics/view-list-tree-90.png"),tr("Project"));
    treeDockButton->setObjectName("showTreeBt");
    treeDockButton->setOrientation(Qt::Vertical);
    treeDockButton->setCheckable(true);
    treeDockButton->setMirrored(true);
    connect(treeDockButton, SIGNAL(toggled(bool)), treeDock, SLOT(setVisible(bool)));


    noteDockButton = new OrientationButton(QIcon(":/pics/im-status-message-edit-90.png"),tr("Notes"));
    noteDockButton->setObjectName("showNotesBt");
    noteDockButton->setOrientation(Qt::Vertical);
    noteDockButton->setCheckable(true);
    noteDockButton->setMirrored(true);
    connect(noteDockButton, SIGNAL(toggled(bool)), noteDock, SLOT(setVisible(bool)));

    toolDockButton = new OrientationButton(QIcon(":/pics/preferences-system-90.png"),tr("Tools"));
    toolDockButton->setObjectName("showToolsBt");
    toolDockButton->setOrientation(Qt::Vertical);
    toolDockButton->setCheckable(true);
    toolDockButton->setMirrored(true);
    connect(toolDockButton, SIGNAL(toggled(bool)), toolDock, SLOT(setVisible(bool)));

    attendDockButton = new OrientationButton(QIcon(":/pics/meeting-organizer-90.png"),tr("Attendance"));
    attendDockButton->setObjectName("showAttendBt");
    attendDockButton->setOrientation(Qt::Vertical);
    attendDockButton->setCheckable(true);
    attendDockButton->setMirrored(true);
    connect(attendDockButton, SIGNAL(toggled(bool)), attendDock, SLOT(setVisible(bool)));

    OrientationButton *outlinerButton = new OrientationButton(QIcon(":/pics/view-time-schedule-90.png"),tr("Outliner"));
    outlinerButton->setObjectName("outlinerBt");
    outlinerButton->setOrientation(Qt::Vertical);
    outlinerButton->setCheckable(false);
    outlinerButton->setMirrored(true);
    connect(outlinerButton, SIGNAL(clicked()), this, SLOT(launchOutliner()));

    OrientationButton *fullscreenButton = new OrientationButton(QIcon(":/pics/view-fullscreen-90.png"),tr("Fullscreen"));
    fullscreenButton->setObjectName("fullscreenBt");
    fullscreenButton->setToolTip(tr("Edit this document fullscreen"));
    fullscreenButton->setOrientation(Qt::Vertical);
    fullscreenButton->setCheckable(false);
    fullscreenButton->setMirrored(true);
    connect(fullscreenButton, SIGNAL(clicked()), this, SLOT(editFullscreen()));

    //    showPrvScnButton = new OrientationButton(QIcon(":/pics/zoom-previous-90.png"),tr("&Show Prev. Scene"));
    //    showPrvScnButton->setObjectName("showPrvScnBt");
    //    showPrvScnButton->setToolTip(tr("Show the end of the previous scene"));
    //    showPrvScnButton->setOrientation(Qt::Vertical);
    //    showPrvScnButton->setShortcut(Qt::Key_F10);
    //    showPrvScnButton->setCheckable(true);
    //    showPrvScnButton->setMirrored(true);
    //    connect(showPrvScnButton, SIGNAL(toggled(bool)), this, SLOT(showPrevText(bool)));

    //    showNxtScnButton = new OrientationButton(tr("&Show Next Scene"));
    //    showNxtScnButton->setObjectName("showNxtScnBt");
    //    showNxtScnButton->setToolTip(tr("Show the start of the next scene"));
    //    showNxtScnButton->setOrientation(Qt::Vertical);
    //    showNxtScnButton->setShortcut(Qt::Key_F10);
    //    showNxtScnButton->setCheckable(true);
    //    showNxtScnButton->setMirrored(true);
    //    connect(showNxtScnButton, SIGNAL(toggled(bool)), this, SLOT(showNextText(bool)));

    //    docksToolBar->addWidget(treeDockButton);
    docksToolBar->addWidget(noteDockButton);
    docksToolBar->addWidget(toolDockButton);
    docksToolBar->addWidget(attendDockButton);
    docksToolBar->addSeparator();
    docksToolBar->addWidget(outlinerButton);
    docksToolBar->addWidget(fullscreenButton);
    //    docksToolBar->addWidget(showPrvScnButton);
    //    docksToolBar->addWidget(showNxtScnButton);



    //    this->addToolBar(Qt::LeftToolBarArea, docksToolBar);

}

//---------------------------------------------------------------------------

void MainWindow::checkHiddenDocks()
{



    if(treeDock->isVisible())
        treeDockButton->setChecked(true);
    else
        treeDockButton->setChecked(false);
    //    menu->setTreeDockAct(treeDock->isVisible());

    if(noteDock->isVisible()){
        noteDockButton->setChecked(true);
    }
    else
        noteDockButton->setChecked(false);
    menu->setNoteDockAct(noteDock->isVisible());

    if(toolDock->isVisible())
        toolDockButton->setChecked(true);
    else
        toolDockButton->setChecked(false);
    menu->setToolDockAct(toolDock->isVisible());

    if(attendDock->isVisible()){
        attendDockButton->setChecked(true);
    }
    else
        attendDockButton->setChecked(false);
    menu->setAttendDockAct(attendDock->isVisible());


}



//---------------------------------------------------------------------------

void MainWindow::setDisplayMode(QString mode)
{
    QSettings settings;
    settings.beginGroup( "MainWindow" );

    if(mode == "desktop"){

        if(displayMode == "netbook")
            settings.setValue( "netbook_wmState", saveState(1) );

        disconnect(attendDock, SIGNAL(visibilityChanged(bool)), this, SLOT(attendDockHidesOthers(bool)));
        disconnect(noteDock, SIGNAL(visibilityChanged(bool)), this, SLOT(noteDockHidesOthers(bool)));

        docksToolBar->setAllowedAreas(Qt::RightToolBarArea | Qt::LeftToolBarArea);
        docksToolBar->setOrientation(Qt::Vertical);
        docksToolBar->setMovable(true);
        docksToolBar->setFloatable(false);
        this->addToolBar(Qt::LeftToolBarArea, docksToolBar);

        attendDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
        attendDock->setFloating(false);
        this->addDockWidget(Qt::RightDockWidgetArea, attendDock, Qt::Vertical);

        noteDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
        noteDock->setFloating(false);
        this->addDockWidget(Qt::RightDockWidgetArea, noteDock, Qt::Horizontal);
        this->changeOrientationOfNoteDock(Qt::RightDockWidgetArea);

        treeDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
        this->addDockWidget(Qt::LeftDockWidgetArea, treeDock, Qt::Vertical);

        restoreState( settings.value( "desktop_wmState" ).toByteArray(),1 );


    }

    if(mode == "netbook"){

        if(displayMode == "desktop")
            settings.setValue( "desktop_wmState", saveState(1) );

        docksToolBar->setAllowedAreas(Qt::RightToolBarArea);
        docksToolBar->setOrientation(Qt::Vertical);
        docksToolBar->setMovable(false);
        docksToolBar->setFloatable(false);
        this->addToolBar(Qt::RightToolBarArea, docksToolBar);

        attendDock->setAllowedAreas(Qt::RightDockWidgetArea);
        attendDock->setFloating(false);
        this->addDockWidget(Qt::RightDockWidgetArea, attendDock, Qt::Vertical);

        noteDock->setAllowedAreas(Qt::RightDockWidgetArea);
        noteDock->setFloating(false);
        this->changeOrientationOfNoteDock(Qt::RightDockWidgetArea);
        this->addDockWidget(Qt::RightDockWidgetArea, noteDock, Qt::Vertical);

        treeDock->setAllowedAreas(Qt::LeftDockWidgetArea);
        treeDock->setFloating(false);
        this->addDockWidget(Qt::LeftDockWidgetArea, treeDock, Qt::Vertical);


        connect(attendDock, SIGNAL(visibilityChanged(bool)), this, SLOT(attendDockHidesOthers(bool)));
        connect(noteDock, SIGNAL(visibilityChanged(bool)), this, SLOT(noteDockHidesOthers(bool)));

        restoreState( settings.value( "netbook_wmState" ).toByteArray(),1 );

    }


    displayMode = mode;
    checkHiddenDocks();


    settings.endGroup();

}




































//---------------------------------------------------------------------------

void MainWindow::openExternalProject(QString externalFile)
{

//    qDebug() << "openExternalProject : " << externalFile->fileName();

    //    if(!externalFile->exists()){
    //        int ret = QMessageBox::warning(this, tr("Plume Creator"),
    //                                       tr("Your are maybe using Windows."
    //                                          "The file path may contain special characters.\n"
    //                                          "Please rename the folders and file without these characters :\n"
    //                                          "Ã©,Ã¨,Ã ,Ã¯,Ã§ et cetera... \n"
    //                                          "Then, try opening it again !"),
    //                                       QMessageBox::Ok,
    //                                       QMessageBox::Ok);

    //        return;
    //    }

    isExternalProjectOpeningBool = true;  //needed to avoid the opening of the project manager at startup
    hub->startProject(externalFile);
}
//---------------------------------------------------------------------------

void MainWindow::openProjectSlot(QFile *projectFile)
{

    closeAllDocsSlot();

    menu->setMenusEnabled(true);
    textStyles->setProjectInfoFile();
    attendBase->startAttendance();
    mainTree->startTree();
    configTimer();
}


//---------------------------------------------------------------------------

void MainWindow::closeProjectSlot()
{
    if(!hub->isProjectOpened())
        return;

    menu->setMenusEnabled(false);





    writeDocksSettings();
    closeAllDocsSlot();

    mainTree->closeTree();

    //    attendList->accept(); //to close the manager;


//    hub->closeCurrentProject();
    //    attendList->closeAll();
    //    timer->stop();
}

void MainWindow::setProjectNumberSlot(int prjNumber)
{
    settingNumber = prjNumber;
}




//---------------------------------------------------------------------------





void MainWindow::textSlot(int number, QString action)
{
    if(action == "open"){

        MainTextDocument *textDoc = hub->findChild<MainTextDocument *>("textDoc_" + QString::number(number));
        MainTextDocument *synDoc =  hub->findChild<MainTextDocument *>("synDoc_" + QString::number(number));
        MainTextDocument *noteDoc = hub->findChild<MainTextDocument *>("noteDoc_" + QString::number(number));

        QString name = hub->mainTree_domElementForNumberHash().value(number).attribute("name");

        // verify if tree item not already opened, and if yes focus on it :

        if(textWidgetList->size() != 0){
            for(int i = 0; i < textWidgetList->size(); ++i){
                if(textWidgetList->at(i)->document() == textDoc){
                    ui->mainTabWidget->setCurrentWidget(textWidgetList->at(i));
                    return;
                }
            }
        }
        if(textWidgetList->size() == 0){
            ui->mainTabWidget->clear();
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

        TextTab *tab = new TextTab(this);
        tab->setHub(hub);

        //set text Styles :
        tab->setTextStyles(textStyles);

        tab->openText(textDoc);


        QWidget *noteWidget = new QWidget(this);
        QVBoxLayout *nLayout = new QVBoxLayout(noteWidget);
        NoteZone *noteStack = new NoteZone(noteWidget);
        noteStack->setHub(hub);
        nLayout->addWidget(noteStack);
        noteStack->openNote(noteDoc);
        noteWidget->setLayout(nLayout);


        QWidget *synWidget = new QWidget(this);
        QVBoxLayout *sLayout = new QVBoxLayout(synWidget);
        NoteZone *synStack = new NoteZone(synWidget);
        synStack->setHub(hub);
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
        ui->mainTabWidget->addTab(tab, name);




        numList->append(number);
        //        qDebug() << "added objectname value : " << string.setNum(number,10);



        //display the opened tab (config setting)

        ui->mainTabWidget->setCurrentWidget(tab);


        //connect edit menu to tab

        connect(menu,SIGNAL(textFontChangedSignal(QFont)),tab,SLOT(changeTextFontSlot(QFont)));
        connect(menu,SIGNAL(textHeightChangedSignal(int)),tab,SLOT(changeTextHeightSlot(int)));
        connect(tab,SIGNAL(charFormatChangedSignal(QTextCharFormat)),menu,SIGNAL(charFormatChangedSlotSignal(QTextCharFormat)));
        connect(menu,SIGNAL(styleSelectedSignal(int)), tab, SLOT(changeTextStyleSlot(int)));
        connect(tab,SIGNAL(setStyleSelectionSignal(int)), menu, SIGNAL(setStyleSelectionSignal(int)));
        connect(tab,SIGNAL(manageStylesSignal()), menu, SLOT(manageStyles()));


        //connect note & syn to maintree :
        connect(synStack, SIGNAL(connectUpdateTextsSignal()), mainTree, SIGNAL(connectUpdateTextsSignal()));
        connect(synStack, SIGNAL(disconnectUpdateTextsSignal()), mainTree, SIGNAL(disconnectUpdateTextsSignal()));
        connect(noteStack, SIGNAL(connectUpdateTextsSignal()), mainTree, SIGNAL(connectUpdateTextsSignal()));
        connect(noteStack, SIGNAL(disconnectUpdateTextsSignal()), mainTree, SIGNAL(disconnectUpdateTextsSignal()));

        //connect note & syn to maintree (for Outliner) :

        connect(noteStack, SIGNAL(noteFocusOutSignal()), mainTree, SLOT(updateOutliner()));
        connect(synStack, SIGNAL(noteFocusOutSignal()), mainTree, SLOT(updateOutliner()));

        //connect maintree to note & syn (for Outliner) :
        connect(mainTree, SIGNAL(applySynNoteFontConfigSignal()), noteStack, SLOT(applyNoteFontConfig()));
        connect(mainTree, SIGNAL(applySynNoteFontConfigSignal()), synStack, SLOT(applySynFontConfig()));

        //launch autosaving :
        if(!hub->isProjectOpened())
            autosaveTimer();


        //focus on text :
        tab->setTextFocus();

        //set cursor position :
        tab->setCursorPos(textDoc->cursorPos());
        synStack->setCursorPos(synDoc->cursorPos());
        noteStack->setCursorPos(noteDoc->cursorPos());

        //        QString debug;
        //        qDebug() << "cursorPosition tab : " << debug.setNum(textCursorPosition);
        //        qDebug() << "cursorPosition syn : " << debug.setNum(synCursorPosition);
        //        qDebug() << "cursorPosition note : " << debug.setNum(noteCursorPosition);

        // show previous and nest text :

        //apply config :

        connect(this, SIGNAL(applyConfigSignal()), tab,SLOT(applyConfig()));
        connect(this, SIGNAL(applyConfigSignal()), synStack,SLOT(applySynConfig()));
        connect(this, SIGNAL(applyConfigSignal()), noteStack,SLOT(applyNoteConfig()));


        //connect save when text change :
        connect(tab, SIGNAL(textChangedSignal()), this, SLOT(textChangedSlot()));
        connect(noteStack, SIGNAL(textChanged()), this, SLOT(textChangedSlot()));
        connect(synStack, SIGNAL(textChanged()), this, SLOT(textChangedSlot()));


        QTimer::singleShot(0, tab, SLOT(changeWidthSlot()));






        //    temporary config

        //            tab->setObjectName("textTabWidget");
        //            tab->setStyleSheet("QWidget#textTabWidget { background: white }");


        // if option "one tab only" is activated :
        if(oneTabOnly){
            int i = 0 ;
            while( numList->size() > 1 ){
                this->textSlot(numList->at(0), "close");
                ++i;
            }
        }


    }
    else if(action == "close"){

        if(!numList->contains(number))
            return;


        QString string;
        TextTab *tab = ui->mainTabWidget->findChild<TextTab *>("tab_" + string.setNum(number,10));

        //disconnect edit menu to tab
        disconnect(menu, SIGNAL(widthChangedSignal(int)), tab, SLOT(changeWidthSlot(int)));

        disconnect(menu,SIGNAL(textFontChangedSignal(QFont)),tab,SLOT(changeTextFontSlot(QFont)));
        disconnect(menu,SIGNAL(textHeightChangedSignal(int)),tab,SLOT(changeTextHeightSlot(int)));
        disconnect(tab,SIGNAL(charFormatChangedSignal(QTextCharFormat)),menu,SIGNAL(charFormatChangedSlotSignal(QTextCharFormat)));


        int tabNum = ui->mainTabWidget->indexOf(tab);
        tabCloseRequest(tabNum);


    }
    else
        return;

}


//---------------------------------------------------------------------------


void MainWindow::setConnections()
{
    textDocList = new QList<MainTextDocument *> ;
    noteDocList = new QList<MainTextDocument *> ;
    synDocList = new QList<MainTextDocument *> ;
    nameList = new QStringList;
    textWidgetList = new QList<TextTab *>;
    noteWidgetList = new QList<NoteZone *>;
    synWidgetList = new QList<NoteZone *>;

    tabNumList = new QList<int>;
    numList = new QList<int>;

    connect(ui->mainTabWidget,SIGNAL(currentChanged(int)),noteLayout,SLOT(setCurrentIndex(int)));
    connect(ui->mainTabWidget,SIGNAL(currentChanged(int)),synLayout,SLOT(setCurrentIndex(int)));
    connect(ui->mainTabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));
    connect(ui->mainTabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(tabCloseRequest(int)));
    connect(mainTree, SIGNAL(nameChangedSignal(QString,int)), this, SLOT(tabRenamingSlot(QString, int)));
    connect(menu, SIGNAL(openProjectNumberSignal(int)), this, SLOT(setProjectNumberSlot(int)));
    connect(menu, SIGNAL(saveProjectSignal()), this, SLOT(saveAllDocsSlot()));



    //    //to show previous text :
    //    connect(menu, SIGNAL(showPrevTextSignal(bool)), this, SLOT(showPrevText(bool)));


    //for attendance :

    //    connect(mainTree, SIGNAL(attendStringSignal(int,QString)), attendBase, SLOT(openSheetAttendList(int,QString)));
    //    connect(mainTree, SIGNAL(allAttendancesSignal(QHash<int,QString>)), attendBase, SLOT(updateAllAttendances(QHash<int,QString>)));
    //    connect(attendBase, SIGNAL(projectAttendanceList(QHash<QListWidgetItem*,QDomElement>,QHash<int,QDomElement>)),
    //            mainTree,SLOT(setOutlinerProjectAttendList(QHash<QListWidgetItem*,QDomElement>,QHash<int,QDomElement>)));
    connect(ui->mainTabWidget, SIGNAL(currentChanged(int)), this, SLOT(setCurrentAttendList(int)));
    //    connect(attendBase, SIGNAL(removeAttendNumberSignal(int)), mainTree, SLOT(removeAttendNumberSlot(int)));
    //    connect(attendBase, SIGNAL(removeAttendNumberFromSheetSignal(QList<int>, int)), mainTree, SLOT(removeAttendNumberFromSheetSlot(QList<int>, int)));
    //    connect(attendBase, SIGNAL(addAttendNumberToSheetSignal(QList<int>, int)), mainTree, SLOT(addAttendNumberToSheetSlot(QList<int>, int)));




    // for previous and next texts :
    connect(ui->mainTabWidget,SIGNAL(currentChanged(int)),this,SLOT(showPrevAndNextTexts()));
    connect(mainTree, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(showPrevAndNextTexts()));


}





//---------------------------------------------------------------------------
void MainWindow::tabChangeSlot(int tabNum)
{

    int preTabNum = -1;

    if(!tabNumList->isEmpty()){
        preTabNum = tabNumList->last();
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
        //                qDebug() << "tabChangeRequest pre :" << preTabNum;
        //        qDebug() << "tabChangeRequest name :" << nameList->at(preTabNum);


        //to initialize edit menu fonts:

        //        TextTab *tab = textWidgetList->at(tabNum);
        //        menu->tabChangedSlot(tab->tabFontChangedSlot());


        //        setCurrentAttendList(tabNum);


    }
}
//---------------------------------------------------------------------------
void MainWindow::tabCloseRequest(int tabNum)
{
    disconnect(ui->mainTabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));

    // Saving


    mainTree->saveCursorPos(textWidgetList->at(tabNum)->cursorPos(),
                            synWidgetList->at(tabNum)->saveCursorPos(),
                            noteWidgetList->at(tabNum)->saveCursorPos(),
                            numList->at(tabNum));


    //    qDebug() << "tabCloseRequest textName :" << textWidgetList->at(tabNum)->objectName() << "----------- saved :" << textBool;
    //    qDebug() << "tabCloseRequest noteName :" << noteWidgetList->at(tabNum)->objectName() << "----------- saved :" << noteBool;
    //    qDebug() << "tabCloseRequest synName :" << synWidgetList->at(tabNum)->objectName() << "----------- saved :" << synBool;
    //    qDebug() << "tabCloseRequest pre :" << tabNum;
    //    qDebug() << "tabCloseRequest name :" << nameList->at(tabNum);


    // Closing / removing

    QWidget* widget = ui->mainTabWidget->widget(tabNum);
    widget->setObjectName("");
    ui->mainTabWidget->removeTab(tabNum);
    delete widget;

    QWidget* noteWidget = noteLayout->widget(tabNum);
    noteWidget->setObjectName("");
    noteLayout->removeWidget(noteLayout->widget(tabNum));
    delete noteWidget;

    QWidget* synWidget = synLayout->widget(tabNum);
    synWidget->setObjectName("");
    synLayout->removeWidget(synLayout->widget(tabNum));
    delete synWidget;


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

    //    connect(mainTabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)),Qt::UniqueConnection);
    hub->addToSaveQueue();

}

void MainWindow::reconnectAFterTabClose()
{
    connect(ui->mainTabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)),Qt::UniqueConnection);

}

//---------------------------------------------------------------------------

void MainWindow::closeAllDocsSlot()
{
    disconnect(ui->mainTabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));

    for(int i = nameList->size()-1; i >= 0; --i){


        // Saving


        //        bool textBool = mainTree->saveDoc(textWidgetList->at(i)->document());
        //        bool noteBool = mainTree->saveDoc(noteWidgetList->at(i)->document());
        //        bool synBool = mainTree->saveDoc(synWidgetList->at(i)->document());

        mainTree->saveCursorPos(textWidgetList->at(i)->cursorPos(),
                                synWidgetList->at(i)->saveCursorPos(),
                                noteWidgetList->at(i)->saveCursorPos(),
                                numList->at(i));

        //        qDebug() << "closeAllRequest name :" << nameList->at(i);
        //        qDebug() << "closeAllRequest textName :" << textWidgetList->at(i)->objectName() << "----------- saved :" << textBool;
        //        qDebug() << "closeAllRequest noteName :" << noteWidgetList->at(i)->objectName() << "----------- saved :" << noteBool;
        //        qDebug() << "closeAllRequest synName :" << synWidgetList->at(i)->objectName() << "----------- saved :" << synBool;


        // Closing / removing


        noteWidgetList->at(i)->closeNote();
        synWidgetList->at(i)->closeSyn();

        QWidget* widget = ui->mainTabWidget->widget(i);
        widget->setObjectName("");
        ui->mainTabWidget->removeTab(i);
        delete widget;

        QWidget* noteWidget = noteLayout->widget(i);
        noteWidget->setObjectName("");
        noteLayout->removeWidget(noteLayout->widget(i));
        delete noteWidget;

        QWidget* synWidget = synLayout->widget(i);
        synWidget->setObjectName("");
        synLayout->removeWidget(synLayout->widget(i));
        delete synWidget;

    }
    ui->mainTabWidget->clear();

    textDocList->clear();
    noteDocList->clear();
    synDocList->clear();
    nameList->clear();
    textWidgetList->clear();
    noteWidgetList->clear();
    synWidgetList->clear();
    tabNumList->clear();
    numList->clear();


    connect(ui->mainTabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));

}

//---------------------------------------------------------------------------

void MainWindow::saveAllDocsSlot()
{

    for(int i = nameList->size()-1; i >= 0; --i){


        // Saving


        //        bool textBool = mainTree->saveDoc(textWidgetList->at(i)->document());
        //        bool noteBool = mainTree->saveDoc(noteWidgetList->at(i)->document());
        //        bool synBool = mainTree->saveDoc(synWidgetList->at(i)->document());

        mainTree->saveCursorPos(textWidgetList->at(i)->cursorPos(),
                                synWidgetList->at(i)->saveCursorPos(),
                                noteWidgetList->at(i)->saveCursorPos(),
                                numList->at(i));

        //        qDebug() << "tabSaveRequest name :" << nameList->at(i);
        //        qDebug() << "tabSaveRequest textName nÂ° " << i << " ---> " << textWidgetList->at(i)->objectName() << "----- saved :" << textBool;
        //        qDebug() << "tabSaveRequest noteName nÂ° " << i << " ---> " << noteWidgetList->at(i)->objectName() << "----- saved :" << noteBool;
        //        qDebug() << "tabSaveRequest synName  nÂ° " << i << " ---> " << synWidgetList->at(i)->objectName() << "----- saved :" << synBool;
    }
}

//---------------------------------------------------------------------------

void MainWindow::tabRenamingSlot(QString newName, int number)
{


    if(!numList->contains(number))
        return;


    //        disconnect(mainTabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));

    QString string;
    TextTab *tab = ui->mainTabWidget->findChild<TextTab *>("tab_" + string.setNum(number,10));

    //    qDebug() << "tabRenamingSlot : " << string.setNum(number,10);

    ui->mainTabWidget->setTabText(ui->mainTabWidget->indexOf(tab),newName);

    //    connect(mainTabWidget, SIGNAL(currentChanged(int)), this,SLOT(tabChangeSlot(int)));


}





//---------------------------------------------------------------------------

void MainWindow::readSettings()
{
    QSettings settings;
    settings.beginGroup( "MainWindow" );
    resize(settings.value( "size", QSize( 800, 500 ) ).toSize() );
    move(settings.value( "pos" ).toPoint() );
    m_firstStart = settings.value("firstStart", true).toBool();
    m_firstStart_checkDisplay = settings.value("firstStart_checkDisplay", true).toBool();
    checkScreenResAtStartupBool = settings.value("checkScreenResAtStartup", true).toBool();
    settings.endGroup();
    settings.beginGroup( "Updater" );
    checkUpdateAtStartupBool = settings.value("checkAtStartup_1", true).toBool();
    settings.endGroup();


    //    checkHiddenDocks();


}
//---------------------------------------------------------------------------

void MainWindow::readDocksSettings()
{
    QSettings settings;
    settings.beginGroup( "MainWindow" );
    displayMode = settings.value("displayMode", "desktop").toString();
    this->setDisplayMode(settings.value("displayMode", "desktop").toString() );
    settings.endGroup();


}

//---------------------------------------------------------------------------

void MainWindow::writeSettings()
{

    QSettings settings;
    settings.beginGroup( "MainWindow" );

    if(settings.value( "displayMode", "desktop" ).toString() == "netbook")
        settings.setValue( "netbook_wmState", saveState(1) );
    else if(settings.value( "displayMode", "desktop" ).toString() == "desktop")
        settings.setValue( "desktop_wmState", saveState(1) );


    settings.setValue( "size", size() );
    settings.setValue( "pos", pos() );
    settings.setValue( "firstStart", false);
    settings.setValue( "firstStart_checkDisplay", false);
    settings.endGroup();

    //    qDebug() << "main settings saved";
}


//---------------------------------------------------------------------------

void MainWindow::writeDocksSettings()
{

    QSettings settings;
    settings.beginGroup( "MainWindow" );

    if(settings.value( "displayMode", "desktop" ).toString() == "netbook")
        settings.setValue( "netbook_wmState", saveState(1) );
    else if(settings.value( "displayMode", "desktop" ).toString() == "desktop")
        settings.setValue( "desktop_wmState", saveState(1) );


    settings.endGroup();

}

//---------------------------------------------------------------------------

void MainWindow::closeEvent(QCloseEvent* event)
{

    if(!hub->isProjectOpened()){
        writeSettings();
        event->accept();
        return;
    }


    QMessageBox msgBox(this);
    msgBox.setText(tr("Do you want to quit ?"));
    msgBox.setInformativeText(tr("Your changes are already saved."));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Ok:


        writeSettings();
        hub->closeCurrentProject();
        event->accept();

        systemTray->show();
        systemTray->showMessage("Plume Creator", tr("Your project was successfully saved."), QSystemTrayIcon::Information, 3000);

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
    emit tabWidgetWidth(ui->mainTabWidget->width());
}

//----------------------------------------------------------------------------------------
void MainWindow::paintEvent(QPaintEvent *)
 {
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
 }

//----------------------------------------------------------------------------------------

bool MainWindow::eventFilter(QObject *obj, QEvent *event) //for MAc o
{
    if(event->type() == QEvent::FileOpen)
    {
        QFile *extFile = new QFile(((QFileOpenEvent*)event)->file());
        hub->startProject(extFile->fileName());
        return true;
    }
    else
    {
        // else : nothing
        return QObject::eventFilter(obj, event);
    }
}



//----------------------------------------------------------------------------------------

void MainWindow::autosaveTimer()
{
    //    timer = new QTimer(this);
    //    connect(timer, SIGNAL(timeout()), this, SLOT(textSlot()));
    //    timer->start(autosaveTime);
}


//----------------------------------------------------------------------------------------
void MainWindow::textChangedSlot()
{
    if(!textAlreadyChanged){
        textAlreadyChanged = true;

        QTimer::singleShot(autosaveTime, hub, SLOT(addToSaveQueue()));

    }
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
    oneTabOnly = settings.value("oneTabOnly", false).toBool();
    noTabBool = settings.value("TextArea/noTab", false).toBool();
    settings.endGroup();
    settings.beginGroup( "MainWindow" );
    //    menuBarOnTop = settings.value("menuBarOnTop", true).toBool();
    settings.endGroup();

    if(hub->isProjectOpened())
        configTimer();

    if(oneTabOnly){
        int i = 0 ;
        while( numList->size() > 1 ){
            this->textSlot(numList->at(0), "close");
            ++i;
        }
    }
    noTabCss  = "QTabWidget::tab-bar#mainTabWidget {"
            "height: 0px;"
            "}"
            "QTabWidget::pane#mainTabWidget {"
            "padding-top : -20px"

                "}";

    if(!noTabBool){
        QString css = this->styleSheet();
        this->setStyleSheet(css.replace(noTabCss, ""));
        noTabCss = "";
    }

    giveStyle();
    //    if(menuBarOnTop == true){
    //        menu->hide();
    //        toolBox->removeItem(toolBox->indexOf(menu));
    //    }

    //    menu->applyConfig();
}

//---------------------------------------------------------------------------

void MainWindow::configTimer()
{
    //    timer->stop();
    //    timer->start(autosaveTime);

    //    QString debug;
    //    qDebug() << "autosaveTime" << debug.setNum(autosaveTime);
}



















//----------------------------------------------------------------------------



void MainWindow::editFullscreen()
{
    if(ui->mainTabWidget->count() == 0)
        return;




    fullEditor = new FullscreenEditor(0);
    fullEditor->setHub(hub);
    fullEditor->setTextStyles(textStyles);
    fullEditor->postConstructor();
    fullEditor->openBySheetNumber(hub->currentSheetNumber());


    connect(stats,SIGNAL(timerSignal(QString)),fullEditor,SLOT(setTimer(QString)));
    connect(fullEditor, SIGNAL(manageStylesSignal()), menu, SLOT(manageStyles()));
    connect(menu, SIGNAL(resetFullscreenTextWidthSignal()), fullEditor, SLOT(resetFullscreenTextWidthSlot()));
    connect(fullEditor, SIGNAL(newSheetSignal(int)), mainTree, SLOT(addItemNext(int)));


}

//----------------------------------------------------------------------------

void MainWindow::launchOutliner()
{
    if(ui->mainTabWidget->count() == 0)
        return;

    mainTree->launchOutliner();



}

//----------------------------------------------------------------------------


void MainWindow::showPrevText(bool showPrevTextBool)
{
    if(ui->mainTabWidget->count() == 0)
        return;

    int number = ui->mainTabWidget->currentWidget()->objectName().mid(ui->mainTabWidget->currentWidget()->objectName().indexOf("_") + 1).toInt();

    QString string;
    TextTab *tab = ui->mainTabWidget->findChild<TextTab *>("tab_" + string.setNum(number,10));

    tab->showPrevText(showPrevTextBool);

    tab->setPrevText(mainTree->prevText(number));
}

//----------------------------------------------------------------------------
void MainWindow::showNextText(bool showNextTextBool)
{
    if(ui->mainTabWidget->count() == 0)
        return;

    int number = ui->mainTabWidget->currentWidget()->objectName().mid(ui->mainTabWidget->currentWidget()->objectName().indexOf("_") + 1).toInt();

    QString string;
    TextTab *tab = ui->mainTabWidget->findChild<TextTab *>("tab_" + string.setNum(number,10));

    tab->showNextText(showNextTextBool);

    tab->setNextText(mainTree->nextText(number));
}
//----------------------------------------------------------------------------

void MainWindow::showPrevAndNextTexts(bool showTextsBool)
{
    showPrevText(showTextsBool);
    showNextText(showTextsBool);
}

//----------------------------------------------------------------------------

void MainWindow::setCurrentAttendList(int tabNum)
{
    if(tabNum == -1)
        tabNum = 0;

    if(ui->mainTabWidget->count() == 0)
        return;



    int number = ui->mainTabWidget->widget(tabNum)->objectName().mid(ui->mainTabWidget->widget(tabNum)->objectName().indexOf("_") + 1).toInt();
    hub->setCurrentSheetNumber(number); //updates also the attendance list

    //    attendList->setCurrentList(number);
    //    QString currentTabName = ui->mainTabWidget->tabText(tabNum);

    //    attendList->setCurrentListName(currentTabName);
}

//----------------------------------------------------------------------------

int MainWindow::setCurrentNumber()
{

    if(ui->mainTabWidget->count() == 0)
        return 0;

    int number = ui->mainTabWidget->currentWidget()->objectName().mid(ui->mainTabWidget->currentWidget()->objectName().indexOf("_") + 1).toInt();

    emit currentNumber(number);

    return number;
}

//----------------------------------------------------------------------------
//---------------------Updater-----------------------------------
//----------------------------------------------------------------------------



void MainWindow::launchSlimUpdater(QString mode)
{

    SlimUpdater *updater = new SlimUpdater;
    updater->setHub(hub);
    connect(this, SIGNAL(applyConfigSignal()), updater, SLOT(readSettings()));
    connect(updater, SIGNAL(closeUpdaterSignal()), this, SLOT(closeSlimUpdater()));
    ui->baseWidget->layout()->addWidget(updater);
    updater->setMode(mode);
}

void MainWindow::closeSlimUpdater()
{



    QWidget *widget = ui->baseWidget->findChild<QWidget *>("SlimUpdater");
    ui->baseWidget->layout()->removeWidget(widget);
    widget->deleteLater();
}










//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


void MainWindow::giveStyle()
{
    QString  css =
            "QDockWidget {"
            "border: 1px solid lightgray;"
            //            "titlebar-close-icon: url(close.png);"
            //     e       "titlebar-normal-icon: url(undock.png);"
            "margin: 3px"
            "}"

            "QDockWidget::title {"
            "text-align: left;" /* align the text to the left */
            //            "background: gray;"
            "padding-left: 5px;"
            "}"

            "QDockWidget::close-button, QDockWidget::float-button {"
            "border: 1px solid transparent;"
            "background: transparent;"
            " padding: 0px;"
            "}"

            "QDockWidget::close-button:hover, QDockWidget::float-button:hover {"
            "background: transparent;"
            "}"

            "QDockWidget::close-button:pressed, QDockWidget::float-button:pressed {"
            "padding: 1px -1px -1px 1px;"

            "}"

            //            "QTabWidget::pane {" /* The tab widget frame */
            //            "border-top: 2px solid #C2C7CB;"
            //            "}"

            //            "QTabWidget::tab-bar {"
            //            "left: 5px;" /* move to the right by 5px */
            //            "}"

            //            /* Style the tab using the tab sub-control. Note that
            //                it reads QTabBar _not_ QTabWidget */
            //            "QTabBar::tab {"
            //            "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            //            "stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,"
            //            "stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);"
            //            "border: 2px solid #C4C4C3;"
            //            "border-bottom-color: #C2C7CB; /* same as the pane color */"
            //            "border-top-left-radius: 4px;"
            //            "border-top-right-radius: 4px;"
            //            "min-width: 8ex;"
            //            "padding: 2px;"
            //            "}"

            //            "QTabBar::tab:selected, QTabBar::tab:hover {"
            //            "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            //            "stop: 0 #fafafa, stop: 0.4 #f4f4f4,"
            //            "stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);"
            //            "}"

            //            "QTabBar::tab:selected {"
            //            "border-color: #9B9B9B;"
            //            "border-bottom-color: #C2C7CB;" /* same as pane color */
            //            "}"

            //            "QTabBar::tab:!selected {"
            //            "margin-top: 2px;" /* make non-selected tabs look smaller */
            //            "}"

            //            "QPushButton#showNotesBt, QPushButton#showToolsBt, "
            //            "QPushButton#showAttendBt, QPushButton#fullscreenBt, QPushButton#outlinerBt, "
            //            "QPushButton#showTreeBt, QPushButton#showPrvScnBt {"
            //            "border: 1px groove #8f8f91;"
            //            "border-radius: 3px;"
            //            "font: bold normal 10pt ""Times New Roman"";"
            //            "padding-top: 4px;"
            //            "padding-bottom: 4px;"
            //            "background: mid(midlight);"
            //            //            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            //            //            "stop: 0 #f6f7fa, stop: 1 #dadbde);"
            //            //                        "min-width: 1em;"
            //            "}"

            //            "QPushButton#showNotesBt:checked, QPushButton#showToolsBt:checked, "
            //            "QPushButton#showAttendBt:checked, QPushButton#fullscreenBt:checked, QPushButton#outlinerBt:checked,"
            //            "QPushButton#showTreeBt:checked, QPushButton#showPrvScnBt:checked {"
            //            "background-color: palette(mid);"
            //            "border: 2px inset #8f8f91;"
            //            "}"

            //            "QPushButton#showNotesBt:pressed, QPushButton#showToolsBt:pressed, "
            //            "QPushButton#showAttendBt:pressed, QPushButton#fullscreenBt:pressed, QPushButton#outlinerBt:pressed,"
            //            "QPushButton#showTreeBt:pressed, QPushButton#showPrvScnBt:pressed {"
            //            "background-color: palette(mid);"
            //            "border: 2px inset #8f8f91;"
            //            "}"

            //            "QToolButton#showFullscreenBt, QToolButton#showPrvSceneBt, QToolButton#showOutlinerBt {"
            //            "border: 2px outset #8f8f91;"
            //            "border-radius: 1px;"
            //            "font: bold normal 12pt ""Times New Roman"";"
            //            "padding-left: 4px;"
            //            "padding-right: 4px;"
            //            "}"

            //"QToolButton#showFullscreenBt:checked, QToolButton#showPrvSceneBt:checked, QToolButton#showOutlinerBt:checked {"
            //            "background-color: palette(mid);"
            //            "border: 2px inset #8f8f91;"
            //            "}"

            //            "QToolButton#showFullscreenBt:pressed, QToolButton#showPrvSceneBt:pressed, QToolButton#showOutlinerBt:pressed {"
            //                        "background-color: palette(mid);"
            //                        "border: 2px inset #8f8f91;"
            //                        "}"



            "QStatusBar::item {"
            "border: 0px none transparent;"
            "}"



            "QGroupBox#noteBox, QGroupBox#synBox {"
            //            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            //            "stop: 0 #E0E0E0, stop: 1 #FFFFFF);"
            //            "border: 0px none transparent ;"
            //            "border-radius: 0px;"
            "margin-left: -5px;"
            "margin-right: -5px;"
            "margin-bottom: -5px;"
            "margin-top: 1ex;" /* leave space at the top for the title */
            "spacing: 0px;"
            "padding-left: -6px;"
            "padding-right: -6px;"
            "padding-bottom: -6px;"
            "padding-top: -3px;"
            "}"

            "QGroupBox::title {"
            "subcontrol-origin: margin;"
            "subcontrol-position: top center;" /* position at the top center */
            ////            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            ////            "stop: 0 #FFOECE, stop: 1 #FFFFFF);"
            "}"

            "QFrame#notesFrame { "
            "border: 0px none transparent;"
            "border-radius: 0px;"
            "spacing: 0px;"
            "padding: 0px;"
            "margin: 0px;"
            "}"

            "NoteZone {"
            //            "border: 0px none transparent;"
            "spacing: 0px;"
            "padding: 0px;"
            "margin: 0px;"
            "}"

            "AttendBase {"
            //            "border: 0px none transparent;"
            "border-radius: 1px;"
            "spacing: 0px;"
            "padding: -6px;"
            //            "margin: -4px;"
            "}"

            "QToolBar#docksToolBar {"
            "spacing: 5px;"
            "padding: 5px;"

            "}"

            //            "QToolBar#docksToolBar::separator {"
            //            "}"

            ;

    this->setStyleSheet(this->styleSheet() + noTabCss);
}





















//-----------------------------------------------------------------
//--------------Word Count-----------------------------------------
//-----------------------------------------------------------------



void MainWindow::updateProjectWCLabel(int count)
{

    projectWCLabel->setText(tr("Project: ") + Utils::spaceInNumber(QString::number(count)));

}
void MainWindow::updateBookWCLabel(int count)
{
    bookWCLabel->setText(tr("Book: ") + Utils::spaceInNumber(QString::number(count)));

}
void MainWindow::updateChapterWCLabel(int count)
{
    if(count == -1)
        chapterWCLabel->hide();
    else {
        chapterWCLabel->show();
        chapterWCLabel->setText(tr("Chapter: ") + Utils::spaceInNumber(QString::number(count)));
    }
}
void MainWindow::updateCurrentSheetWCLabel(int count)
{

    currentWCLabel->setText(tr("Words: ") + Utils::spaceInNumber(QString::number(count)));

}
