#include "outliner/outlinerspreadsheet.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include "notezone.h"
#include "outliner/outlinerspreadsheetheader.h"

OutlinerSpreadsheet::OutlinerSpreadsheet(QWidget *parent) :
    QTreeView(parent)
{
//    QSettings settings;

//    if(settings.value("Outline/dontWarnMe_1", false).toBool() == false){

//        QDialog *warningDialog = new QDialog(this);
//        warningDialog->setWindowTitle(tr("Outliner"));
//        QVBoxLayout *layout = new QVBoxLayout;

//        QLabel *warningText = new QLabel(tr("This speadsheet outliner is at an early stage of development. <br>"
//                                            "For now, you can rename titles, write synopses and notes. "
//                                            "Right-click and a menu will appear ! "
//                                            ));
//        QCheckBox *doNotWarnAgain = new QCheckBox(tr("Do not warn me again"));
//        connect(doNotWarnAgain, SIGNAL(toggled(bool)), this, SLOT(doNotWarnAgainSlot(bool)));
//        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);

//        connect(buttonBox, SIGNAL(accepted()), warningDialog, SLOT(close()));


//        layout->addWidget(warningText);
//        layout->addWidget(doNotWarnAgain);
//        layout->addWidget(buttonBox);

//        warningDialog->setLayout(layout);

//        warningDialog->exec();



//    }







    contextMenu = new MainTreeContextMenu(this, this);


    this->setFocusPolicy(Qt::WheelFocus);
    this->setEditTriggers(QAbstractItemView::AnyKeyPressed);
    this->setContextMenuPolicy(Qt::DefaultContextMenu);
    this->setAutoExpandDelay(1000);
    this->setAnimated(true);
    this->setUniformRowHeights(false);

    this->setDragEnabled(true);
    this->setDragDropMode(QAbstractItemView::DragDrop);
    this->setDefaultDropAction(Qt::MoveAction);

    this->setAcceptDrops(true);
    this->setDropIndicatorShown(true);


    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::ExtendedSelection	);

    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    setMouseTracking(true);
    connect(this, SIGNAL(activated(QModelIndex)), this, SLOT(itemEntered(QModelIndex)));
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(itemEntered(QModelIndex)));


    OutlinerSpreadsheetHeader *hHeader = new OutlinerSpreadsheetHeader(Qt::Horizontal);
    hHeader->setHub(hub);
    this->setHeader(hHeader);



    connect(hHeader, SIGNAL(sectionResized(int,int,int)), this, SLOT(columnResizedSlot(int,int,int)));

    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));


    connect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(itemExpandedSlot(QModelIndex)), Qt::UniqueConnection);
    connect(this, SIGNAL(collapsed(QModelIndex)), this, SLOT(itemCollapsedSlot(QModelIndex)), Qt::UniqueConnection);



    giveStyle();

}


//----------------------------------------------------------------------------------
void OutlinerSpreadsheet::postConstructor()
{
    contextMenu->setMainTreeAbstractModel(absTreeModel);
    contextMenu->setHub(hub);
    contextMenu->postConstructor();
}

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
        //        this->expandAll();

    }
    if(index == 2){
        emit columnTwoResizedSignal(newSize - 30);
        //        this->expandAll();
    }

}

//----------------------------------------------------------------------------------

void OutlinerSpreadsheet::itemClicked(QModelIndex index)
{

    if (index.column() == 0){

        if(index != oldIndex){ // reset if change
            oneClickCheckpoint = false;
            twoClicksCheckpoint = false;
        }
        oldIndex = index;


        if( oneClickCheckpoint && twoClicksCheckpoint){ // third click
            this->edit(index);
            oneClickCheckpoint = false; //reset
            twoClicksCheckpoint = false;
        }
        else if(oneClickCheckpoint == true){ // second click

//             emit textAndNoteSignal(index.data(Qt::UserRole).toInt(), "open");
            twoClicksCheckpoint = true;
            }
        else
            oneClickCheckpoint = true; // first click

    }
    else if (index.column() == 1 || index.column() == 2|| index.column() == 3 || index.column() == 4|| index.column() == 5)
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



    if(indexColumn == 1 || indexColumn == 2 || indexColumn == 3 || indexColumn == 5){
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
    QModelIndex index = this->indexAt(event->pos());
    if(!index.isValid())
        return;


    int enteredItemId = this->model()->data(index,Qt::UserRole ).toInt();


    if(index.column() == 0){
contextMenu->setId(enteredItemId);

contextMenu->menu(MainTreeContextMenu::Rename
                  | MainTreeContextMenu::Move | MainTreeContextMenu::Delete
                 | MainTreeContextMenu::AddSheet | MainTreeContextMenu::Advanced)->exec(event->globalPos());

  }
}

//--------------------------------------------------------------------------------

void OutlinerSpreadsheet::itemEntered(QModelIndex index)
{
    if(!index.isValid())
        return;

enteredItemModelIndex = index;
    enteredItemId = this->model()->data(index,Qt::UserRole ).toInt();
//        qDebug() << "itemEntered : " +  QString::number(enteredItemId);
}



//--------------------------------------------------------------------------------

void OutlinerSpreadsheet::giveStyle()
{
    this->setAlternatingRowColors(true);
    QString css = "QTreeView {"
            "alternate-background-color: rgba(100,100,100,20);"
            "selection-background-color: rgba(100,100,100,30);"
            "selection-color: black;"
            "}"
            ;

    this->setStyleSheet(css);
}

void OutlinerSpreadsheet::finishStatusEdit()
{
    // only for status from delegate
    this->commitData(this->indexWidget(enteredItemModelIndex));
    this->closeEditor(this->indexWidget(enteredItemModelIndex),QAbstractItemDelegate::NoHint );
}


//--------------------------------------------------------------------------------
void OutlinerSpreadsheet::applySettingsFromData()
{
    disconnect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(itemExpandedSlot(QModelIndex)));
    disconnect(this, SIGNAL(collapsed(QModelIndex)), this, SLOT(itemCollapsedSlot(QModelIndex)));


    QList<QModelIndex> indexList = this->allIndexesFromModel(this->model(), QModelIndex());

    foreach(const QModelIndex &index, indexList)
        this->setExpanded(index, index.data(39).toBool());

    connect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(itemExpandedSlot(QModelIndex)), Qt::UniqueConnection);
    connect(this, SIGNAL(collapsed(QModelIndex)), this, SLOT(itemCollapsedSlot(QModelIndex)), Qt::UniqueConnection);
}
//--------------------------------------------------------------------------------

void OutlinerSpreadsheet::itemCollapsedSlot(QModelIndex index)
{



    this->model()->setData(index, false, Qt::DecorationRole );
      this->update(index);


}

//--------------------------------------------------------------------------------

void OutlinerSpreadsheet::itemExpandedSlot(QModelIndex index)
{



    this->model()->setData(index, true, Qt::DecorationRole );
      this->update(index);



}
//-----------------------------------------------------------------------------------------


QList<QModelIndex> OutlinerSpreadsheet::allIndexesFromModel(QAbstractItemModel *model, const QModelIndex &parent)
{
    QList<QModelIndex> indexList;


    int rowCount = model->rowCount(parent);

    for(int i = 0; i < rowCount; ++i)
    {
        QModelIndex index = model->index(i, 0, parent);

        if(index.isValid())
        {
            indexList << index;
            indexList << allIndexesFromModel(model, index);
        }
    }
    return indexList;
}




//-----------------------------------------------------------------------------------------
void OutlinerSpreadsheet::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{

    m_selectedIndexes = this->selectedIndexes();


    QTreeView::selectionChanged(selected, deselected);
}


//-----------------------------------------------------------------------------------------
void OutlinerSpreadsheet::dragEnterEvent( QDragEnterEvent *event )
{


    const QMimeData *data = event->mimeData();

    if (data->hasFormat("application/x-plumecreator-maintreedata")){





        QString numbersString = QString::fromUtf8(data->data("application/x-plumecreator-maintreedata"));
        QStringList list = numbersString.split("-", QString::SkipEmptyParts);
        QList<int> objectsList;
        foreach(const QString &string, list)
            objectsList.append(string.toInt());


        int draggedInt = objectsList.first();

        QModelIndexList indexList = this->model()->match(this->model()->index(0,0, this->rootIndex()), Qt::UserRole, draggedInt, 1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
        if(indexList.isEmpty())
            return;
        qDebug() << "indexList.first().data(Qt::UserRole) : " + QString::number(indexList.first().data(Qt::UserRole).toInt());

        draggedIndex = indexList.first();

        emit modifyFlagsForDropsSignal(indexList.first().data(36).toString());




        QTreeView::dragEnterEvent(event);
    }


}

//-----------------------------------------------------------------------------------------
void OutlinerSpreadsheet::dragLeaveEvent( QDragLeaveEvent *event )
{
    qDebug() << "drag leave event";

    emit modifyFlagsForDropsSignal("nothing");
    QTreeView::dragLeaveEvent(event);

}
//-----------------------------------------------------------------------------------------
void OutlinerSpreadsheet::dragMoveEvent( QDragMoveEvent *event )
{
    //    qDebug() << "drag move event";

    QTreeView::dragMoveEvent(event);

}

void OutlinerSpreadsheet::dropEvent(QDropEvent* event)
{
    qDebug() << "drop event";

    QString type = draggedIndex.data(36).toString();


    QTreeView::dropEvent(event);
    emit modifyFlagsForDropsSignal("nothing");
}




//----------------------------------------------------------------------------------

