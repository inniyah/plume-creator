#include "edittoolbar.h"
#include "ui_edittoolbar.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif
#include <QtGui>

EditToolBar::EditToolBar(QWidget *parent) :
    QWidget(parent),ui(new Ui::EditToolBar), isOpened(false), m_height(this->height()), isPressed(false), timerIsActivated(true)
{
    ui->setupUi(this);
    toolBar = new QToolBar(this);
    ui->verticalLayout->addWidget(toolBar);

    this->setMouseTracking(true);

    createActions();
    applyConfig();
    //    connect(action, SIGNAL(triggered()), this, SLOT(closeToolBar()));

    closeZoneRect = QRect(0, this->height() -4, this->width(), 4);
    closeToolBar();

    this->hide();
}

EditToolBar::~EditToolBar()
{
    delete ui;
}

void EditToolBar::mouseMoveEvent(QMouseEvent *event)
{
    addToTimerQueue();
    //    if(closeZoneRect.contains(event->pos()))
    //        QApplication::setOverrideCursor(QCursor(Qt::UpArrowCursor));
    //    else{
    //     isPressed = false;
    //        QApplication::restoreOverrideCursor();
    //    }
}

void EditToolBar::leaveEvent(QEvent *event)
{
    addToTimerQueue();
    //    QApplication::restoreOverrideCursor();
    isPressed = false;
    event->ignore();

}

void EditToolBar::paintEvent(QPaintEvent *event)
{

    //    QRect closeZoneRect(0, this->height() -4, this->width(), 4);

    //    QPainter hoverPainter(this);
    //    hoverPainter.setRenderHint(QPainter::Antialiasing);
    //    hoverPainter.setPen(QPen(Qt::red));
    //    hoverPainter.drawRect(closeZoneRect);

}

//----------------------------------------------------------------

void EditToolBar::mousePressEvent(QMouseEvent *event)
{

    addToTimerQueue();

    //    if(closeZoneRect.contains(event->pos()))
    //        isPressed = true;
    //    else
    //        isPressed = false;

}

//----------------------------------------------------------------

void EditToolBar::mouseReleaseEvent(QMouseEvent *event)
{
    addToTimerQueue();

    //    if(closeZoneRect.contains(event->pos()) && isPressed == true){
    //        closeToolBar();

    //        //        QApplication::restoreOverrideCursor();
    //    }
    //    else
    //        isPressed = false;
}

//----------------------------------------------------------------

void EditToolBar::openToolBar()
{
    addToTimerQueue();

    if(this->isVisible())
        return;
    isOpened = true;

    this->show();

    int animDuration = 500;


    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setDuration(animDuration);

    int xManager = this->geometry().x();
    int yManager = this->geometry().y();
    int wManager = m_editZoneRect.width();
    int hManager = this->height();


    animation->setStartValue(QRect(xManager,yManager,wManager,0));
    animation->setEndValue(QRect(xManager,yManager,wManager,m_height));
    animation->start();


}
//---------------------------------------------------------------------------

void EditToolBar::closeToolBar()
{
    stopTimer();

    isOpened = false;

    int animDuration = 500;
    QTimer::singleShot(animDuration, this, SLOT(hide()));

    //    QTimer::singleShot(animDuration, this, SIGNAL(closeUpdaterSignal()));

    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setDuration(animDuration);

    int xManager = this->geometry().x();
    int yManager = this->geometry().y();
    int wManager = this->width();
    int hManager = this->height();


    animation->setStartValue(QRect(xManager,yManager,wManager,hManager));
    animation->setEndValue(QRect(xManager,yManager,wManager,0));
    animation->start();



}

//---------------------------------------------------------------------------------------

void EditToolBar::editZoneMoved(QRect editZoneRect)
{

    QPoint point = this->mapFromGlobal(QPoint(editZoneRect.x(), editZoneRect.y()));
    m_editZoneRect.setX(point.x());
    m_editZoneRect.setY(point.y());
    m_editZoneRect.setWidth(editZoneRect.width());
    m_editZoneRect.setHeight(editZoneRect.height());




}
void EditToolBar::createActions()
{

    // actions made in the UI.




}

void EditToolBar::populateToolBar(QStringList actionsStringList)
{
    toolBar->clear();

    foreach (QString actionString, actionsStringList) {
        if(actionString == "toolBar_separator")
            toolBar->addSeparator();
        else if(actionString.startsWith("toolBarWidget_")){
            QWidget *widget = this->findChild<QWidget *>(actionString);
            toolBar->addWidget(widget);
        }
        else{
            QAction *action = this->findChild<QAction *>(actionString);
            if(action)
                toolBar->addAction(action);
        }
    }
}

void EditToolBar::applyConfig()
{
    QStringList defaultList;
    defaultList /*<< "goBack" << "goForward" << "toolBar_separator"*/ <<"undo" << "redo"
                                                                     << "toolBar_separator" << "bold" << "italic"
                                                                     << "toolBar_separator"<< "cut" <<"copy" << "paste" << "pasteUnformated"
                                                                     << "toolBar_separator"  << "spellChecker"
                                                                     << "toolBar_separator"  << "pinToolBar" ;

    QSettings settings;
    QList<QVariant> toolBarVariantList = settings.value("Settings/TextArea/ToolBarList", defaultList).toList();

    QStringList toolBarList;
    foreach (QVariant var, toolBarVariantList) {
        toolBarList.append(var.toString());
    }
    populateToolBar(toolBarList);
}
void EditToolBar::actionSlot()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action){
        emit actionSignal(action->objectName(), true);
    }
}
void EditToolBar::actionSlot(bool value)
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action){
        emit actionSignal(action->objectName(), value);
    }
}


void EditToolBar::cursorStateSlot(QString actionName, bool value)
{
    QAction *action = this->findChild<QAction *>(actionName);

    if(action)
        action->setChecked(value);
}
void EditToolBar::timerEvent(QTimerEvent *)
{
    if(timerIsActivated)
        closeToolBar();
    stopTimer();
}
//--------------------------------------------------------------------------------------

void EditToolBar::addToTimerQueue()
{


    timerStack += 1;

    if(timerStack == 1)
        timerIdList.append(this->startTimer(4000));
    else if(timerStack > 1){  // reset if more than one save demand
        for(int i = 0; i < timerIdList.size(); ++i){
            this->killTimer(timerIdList.at(i));
        }
        timerIdList.clear();
        timerIdList.append(this->startTimer(4000));
    }



}
void EditToolBar::stopTimer()
{
    timerStack = 0;

    for(int i = 0; i < timerIdList.size(); ++i){
        this->killTimer(timerIdList.at(i));
    }
    timerIdList.clear();
}

void EditToolBar::pinToolBar(bool value)
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action){
        action->setChecked(value);
        if(value){
            stopTimer();
            timerIsActivated = false;
        }
        else{
            addToTimerQueue();
            timerIsActivated = true;
        }
    }
}
