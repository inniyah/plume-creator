#include "dockedtrashtree.h"

DockedTrashTree::DockedTrashTree(QWidget *parent) :
    QTreeView(parent), isLeaveEventStopped(false)
{
    contextMenu = new MainTreeContextMenu(this, this);

connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));

connect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(itemExpandedSlot(QModelIndex)), Qt::UniqueConnection);
connect(this, SIGNAL(collapsed(QModelIndex)), this, SLOT(itemCollapsedSlot(QModelIndex)), Qt::UniqueConnection);

//    QItemSelectionModel *selectModel = new QItemSelectionModel(this->model());
//    connect(selectModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(updateSelection(QItemSelection,QItemSelection)));

}

void DockedTrashTree::postConstructor()
{
contextMenu->setMainTreeAbstractModel(absTreeModel);
contextMenu->setHub(hub);
contextMenu->postConstructor();
}

//--------------------------------------------------------------------------------
void DockedTrashTree::applySettingsFromData()
{
disconnect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(itemExpandedSlot(QModelIndex)));
disconnect(this, SIGNAL(collapsed(QModelIndex)), this, SLOT(itemCollapsedSlot(QModelIndex)));


QList<QModelIndex> indexList = this->allIndexesFromModel(this->model(), QModelIndex());

foreach(const QModelIndex &index, indexList)
    this->setExpanded(index, index.data(34).toBool());

shrinkTrashTree();


connect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(itemExpandedSlot(QModelIndex)), Qt::UniqueConnection);
connect(this, SIGNAL(collapsed(QModelIndex)), this, SLOT(itemCollapsedSlot(QModelIndex)), Qt::UniqueConnection);
}
//--------------------------------------------------------------------------------

void DockedTrashTree::itemCollapsedSlot(QModelIndex index)
{



    this->model()->setData(index, false, Qt::DecorationRole );
      this->update(index);


}

//--------------------------------------------------------------------------------

void DockedTrashTree::itemExpandedSlot(QModelIndex index)
{



    this->model()->setData(index, true, Qt::DecorationRole );
      this->update(index);



}
//-----------------------------------------------------------------------------------------



QList<QModelIndex> DockedTrashTree::allIndexesFromModel(QAbstractItemModel *model, const QModelIndex &parent)
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
void DockedTrashTree::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{

m_selectedIndexes = this->selectedIndexes();


QTreeView::selectionChanged(selected, deselected);
}


//-----------------------------------------------------------------------------------------
void DockedTrashTree::dragEnterEvent( QDragEnterEvent *event )
{


const QMimeData *data = event->mimeData();

if (data->hasFormat("application/x-plumecreator-maintreedata")){

    extendTrashTree();




    QString numbersString = QString::fromUtf8(data->data("application/x-plumecreator-maintreedata"));
    QStringList list = numbersString.split("-", QString::SkipEmptyParts);
    QList<int> objectsList;
    foreach(const QString &string, list)
        objectsList.append(string.toInt());


    int draggedInt = objectsList.first();


    QModelIndexList indexList = absTreeModel->match(absTreeModel->index(0,0, this->rootIndex()), Qt::UserRole, draggedInt, 1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);

    if(indexList.isEmpty())
        return;


    draggedIndex = indexList.first();

    emit modifyFlagsForDropsSignal(indexList.first().data(36).toString());




    QTreeView::dragEnterEvent(event);
}


}

//-----------------------------------------------------------------------------------------
void DockedTrashTree::dragLeaveEvent( QDragLeaveEvent *event )
{

    shrinkTrashTree();
emit modifyFlagsForDropsSignal("nothing");
QTreeView::dragLeaveEvent(event);

}
//-----------------------------------------------------------------------------------------

void DockedTrashTree::dropEvent(QDropEvent* event)
{
qDebug() << "drop event";

QString type = draggedIndex.data(36).toString();


QTreeView::dropEvent(event);
emit modifyFlagsForDropsSignal("nothing");
}



//----------------------------------------------------------------------------------

void DockedTrashTree::itemClicked(QModelIndex index)
{

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

         emit textAndNoteSignal(index.data(Qt::UserRole).toInt(), "open");
        emit currentOpenedSheetSignal(index.data(Qt::UserRole).toInt());

        twoClicksCheckpoint = true;
        }
    else
        oneClickCheckpoint = true; // first click

}

//----------------------------------------------------------------------------------
void DockedTrashTree::contextMenuEvent(QContextMenuEvent *event)
{
QModelIndex index = this->indexAt(event->pos());
if(!index.isValid() || index.data(36) != "trash"){
    return;
}

int enteredItemId = this->model()->data(index,Qt::UserRole ).toInt();



contextMenu->setId(enteredItemId);


isLeaveEventStopped = true;
contextMenu->menu(MainTreeContextMenu::EmptyTrash | MainTreeContextMenu::Rename)->exec(event->globalPos());



}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

void DockedTrashTree::leaveEvent(QEvent * event)
{
    if(isLeaveEventStopped == true){
        event->ignore();
        isLeaveEventStopped = false;
        return;
    }


        shrinkTrashTree();


    event->accept();
}

//--------------------------------------------------------------------------------

void DockedTrashTree::enterEvent(QEvent * event)
{
         extendTrashTree();

    event->accept();
}



void DockedTrashTree::extendTrashTree()
{
    QModelIndexList indexList = this->model()->match(this->model()->index(0,0, this->rootIndex()), 36, "trash" , 1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);

    if(indexList.isEmpty())
        return;


this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->expand(indexList.first());

    int rowHeight = this->sizeHintForRow(0);
    this->setFixedHeight(8*rowHeight);
}

void DockedTrashTree::shrinkTrashTree()
{

    QModelIndexList indexList = this->model()->match(this->model()->index(0,0, this->rootIndex()), 36, "trash" , 1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);

    if(indexList.isEmpty())
        return;


    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->collapse(indexList.first());

    int rowHeight = this->sizeHintForRow(0);
    this->setFixedHeight(rowHeight);
    QApplication::processEvents();
    this->scrollTo(indexList.first(),QAbstractItemView::PositionAtTop);

}
