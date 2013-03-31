#include "outliner/outlinerspreadsheet.h"

#include <QtGui>
#include "notezone.h"
#include "outliner/outlinertreeitem.h"
#include "outliner/outlinerspreadsheetheader.h"

OutlinerSpreadsheet::OutlinerSpreadsheet(QWidget *parent) :
    QTreeView(parent)
{
    QSettings settings;

    if(settings.value("Outline/dontWarnMe_1", false).toBool() == false){

        QDialog *warningDialog = new QDialog(this);
        warningDialog->setWindowTitle(tr("Outliner"));
        QVBoxLayout *layout = new QVBoxLayout;

        QLabel *warningText = new QLabel(tr("This speadsheet outliner is at an early stage of development. <br>"
                                            "For now, you can rename titles, write synopses and notes. "
                                            "Right-click and a menu will appear ! "
                                            ));
        QCheckBox *doNotWarnAgain = new QCheckBox(tr("Do not warn me again"));
        connect(doNotWarnAgain, SIGNAL(toggled(bool)), this, SLOT(doNotWarnAgainSlot(bool)));
        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);

        connect(buttonBox, SIGNAL(accepted()), warningDialog, SLOT(close()));


        layout->addWidget(warningText);
        layout->addWidget(doNotWarnAgain);
        layout->addWidget(buttonBox);

        warningDialog->setLayout(layout);

        warningDialog->exec();



    }








    prepareContextMenu();

    this->setFocusPolicy(Qt::WheelFocus);
    this->setEditTriggers(QAbstractItemView::AnyKeyPressed);
    this->setContextMenuPolicy(Qt::DefaultContextMenu);
    this->setAutoExpandDelay(1000);
    this->setAnimated(true);
    this->setUniformRowHeights(false);

    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::ContiguousSelection);

    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    setMouseTracking(true);
    //    connect(this, SIGNAL(entered(QModelIndex)), this, SLOT(itemEntered(QModelIndex)));
    connect(this, SIGNAL(activated(QModelIndex)), this, SLOT(itemEntered(QModelIndex)));
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(itemEntered(QModelIndex)));


    OutlinerSpreadsheetHeader *hHeader = new OutlinerSpreadsheetHeader(Qt::Horizontal);
hHeader->setHub(hub);
   this->setHeader(hHeader);



    connect(hHeader, SIGNAL(sectionResized(int,int,int)), this, SLOT(columnResizedSlot(int,int,int)));

    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));


    giveStyle();

}


//----------------------------------------------------------------------------------

void OutlinerSpreadsheet::doNotWarnAgainSlot(bool dontWarnMe)
{
    QSettings settings;
    settings.beginGroup( "Outline" );
    settings.setValue("dontWarnMe_1", dontWarnMe);
}

//----------------------------------------------------------------------------------
void OutlinerSpreadsheet::columnResizedSlot(int index, int oldSize, int newSize)
{
    if(index == 1){
        emit columnOneResizedSignal(newSize - 30);
        this->expandAll();

    }
    if(index == 2){
        emit columnTwoResizedSignal(newSize - 30);
        this->expandAll();
    }
}

//----------------------------------------------------------------------------------

void OutlinerSpreadsheet::itemClicked(QModelIndex index)
{
    int itemId = index.data(Qt::UserRole).toInt();
    if (index.column() == 0){
        if(wasClickedOnce == true && itemIdClickedOnce == itemId){
            this->edit(index);
            wasClickedOnce = false;
            itemIdClickedOnce = 99999;
        }
        else
            wasClickedOnce = true;
        itemIdClickedOnce = itemId;
    }
    else if (index.column() == 1 || index.column() == 2)
        this->edit(index);
}

//----------------------------------------------------------------------------------
void OutlinerSpreadsheet::focusInEvent ( QFocusEvent * event )
{

    event->accept();
}

//void OutlinerSpreadsheet::mouseEvent(QMouseEvent* event)
//{
//    qDebug() << "rrrrrrrr";
//   int itemColumn = this->indexAt(event->pos()).column();

////   if(itemColumn == 1 || itemColumn == 2)
////       this->select;
//   event->accept();
//}

void OutlinerSpreadsheet::wheelEvent(QWheelEvent* event)
{
    QModelIndex index = this->indexAt(event->pos());
    int indexColumn = index.column();



    if(indexColumn == 1 | indexColumn == 2){
        //           this->setFocusProxy(this->indexWidget(index));
        this->setCurrentIndex(index);
        this->edit(index);


        event->accept();
    }
    else
        QTreeView::wheelEvent(event);
}

void OutlinerSpreadsheet::contextMenuEvent(QContextMenuEvent *event)
{
    enteredItemId = this->model()->data(this->indexAt(event->pos()),Qt::UserRole ).toInt();



    QMenu menu(this);
    menu.addSeparator();

    menu.addAction(renameAct);
    menu.addAction(addItemNextAct);
    menu.addAction(addChildAct);
    menu.addAction(addSeparatorAct);

    menu.addSeparator();

    menu.addAction(moveUpAct);
    menu.addAction(moveDownAct);

    menu.addSeparator();

    delItemMenu = menu.addMenu(tr("&Delete"));
    delItemMenu->addAction(delYesAct);

    menu.addSeparator();

    advancedMenu = menu.addMenu(tr("&Advanced"));
    advancedMenu->addAction(autoRenameChildsAct);
    advancedMenu->addAction(addMultiAct);

    menu.exec(event->globalPos());
}

void OutlinerSpreadsheet::prepareContextMenu()
{

    renameAct = new QAction(QIcon(":/pics/edit-rename.png"),tr("&Rename"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(renameAct, SIGNAL(triggered()), this, SLOT(rename()));

    addItemNextAct = new QAction(QIcon(":/pics/folder-new.png"),tr("Add &next"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(addItemNextAct, SIGNAL(triggered()), this, SLOT(addItemNext()));

    addChildAct = new QAction(QIcon(":/pics/document-new.png"),tr("Add &child"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(addChildAct, SIGNAL(triggered()), this, SLOT(addChild()));

    addSeparatorAct = new QAction(QIcon(":/pics/insert-horizontal-rule.png"),tr("Add &scene break"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(addSeparatorAct, SIGNAL(triggered()), this, SLOT(addSeparator()));

    moveUpAct = new QAction(QIcon(":/pics/arrow-up.png"),tr("Move Up"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(moveUpAct, SIGNAL(triggered()), this, SLOT(moveUp()));

    moveDownAct = new QAction(QIcon(":/pics/arrow-down.png"),tr("Move Down"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(moveDownAct, SIGNAL(triggered()), this, SLOT(moveDown()));

    delYesAct = new QAction(QIcon(":/pics/knotes_delete.png"),tr("C&onfirm"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(delYesAct, SIGNAL(triggered()), this, SLOT(removeItem()));

    autoRenameChildsAct = new QAction(QIcon(":/pics/edit-rename.png"),tr("&Auto Rename Childs"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(autoRenameChildsAct, SIGNAL(triggered()), this, SLOT(autoRenameChilds()));

    addMultiAct = new QAction(QIcon(":/pics/document-multiple.png"),tr("Add &X Children"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(addMultiAct, SIGNAL(triggered()), this, SLOT(addMulti()));



}
//--------------------------------------------------------------------------------

void OutlinerSpreadsheet::itemEntered(QModelIndex index)
{
    enteredItemId = this->model()->data(index,Qt::UserRole ).toInt();
//    qDebug() << "itemEntered";
}

//--------------------------------------------------------------------------------

void OutlinerSpreadsheet::giveStyle()
{

    this->setAlternatingRowColors(true);
    QString css = "QTreeView {"
            "alternate-background-color: rgba(100,100,100,10);"
            "selection-background-color: rgba(100,100,100,20);"
            "selection-color: black;"
            "}"
            " QTreeView::item {"
            "border-top: 1px solid rgba(100,100,100,40);"
            "border-bottom: 1px solid rgba(100,100,100,40);"
            "}"
            ;

    this->setStyleSheet(css);
}
