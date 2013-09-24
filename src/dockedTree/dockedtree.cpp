#include "dockedtree.h"

DockedTree::DockedTree(QWidget *parent) :
    QTreeView(parent)
{




    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));

    connect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(itemExpandedSlot(QModelIndex)), Qt::UniqueConnection);
    connect(this, SIGNAL(collapsed(QModelIndex)), this, SLOT(itemCollapsedSlot(QModelIndex)), Qt::UniqueConnection);

    //    QItemSelectionModel *selectModel = new QItemSelectionModel(this->model());
    //    connect(selectModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(updateSelection(QItemSelection,QItemSelection)));

}

void DockedTree::postConstructor()
{


}

//--------------------------------------------------------------------------------
void DockedTree::applySettingsFromData()
{
    disconnect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(itemExpandedSlot(QModelIndex)));
    disconnect(this, SIGNAL(collapsed(QModelIndex)), this, SLOT(itemCollapsedSlot(QModelIndex)));


    QList<QModelIndex> indexList = this->allIndexesFromModel(this->model(), QModelIndex());

    foreach(const QModelIndex &index, indexList)
        this->setExpanded(index, index.data(34).toBool());

    connect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(itemExpandedSlot(QModelIndex)), Qt::UniqueConnection);
    connect(this, SIGNAL(collapsed(QModelIndex)), this, SLOT(itemCollapsedSlot(QModelIndex)), Qt::UniqueConnection);
}
//--------------------------------------------------------------------------------

void DockedTree::itemCollapsedSlot(QModelIndex index)
{




    this->model()->setData(index, false, Qt::DecorationRole );
      this->update(index);


}

//--------------------------------------------------------------------------------

void DockedTree::itemExpandedSlot(QModelIndex index)
{


  this->model()->setData(index, true, Qt::DecorationRole );
    this->update(index);


}
//-----------------------------------------------------------------------------------------



QList<QModelIndex> DockedTree::allIndexesFromModel(QAbstractItemModel *model, const QModelIndex &parent)
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
void DockedTree::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{

    m_selectedIndexes = this->selectedIndexes();


    QTreeView::selectionChanged(selected, deselected);
}


//-----------------------------------------------------------------------------------------
void DockedTree::dragEnterEvent( QDragEnterEvent *event )
{


    const QMimeData *data = event->mimeData();

    if (data->hasFormat("application/x-plumecreator-maintreedata")){





        QString numbersString = QString::fromUtf8(data->data("application/x-plumecreator-maintreedata"));
        QStringList list = numbersString.split("-", QString::SkipEmptyParts);
        QList<int> objectsList;
        foreach(const QString &string, list)
            objectsList.append(string.toInt());


        int draggedInt = objectsList.first();
//        qDebug() << "draggedInt : " << QString::number(draggedInt);
        QModelIndexList indexList = absTreeModel->match(absTreeModel->index(0,0, this->rootIndex()), Qt::UserRole, draggedInt, 1, Qt::MatchExactly | Qt::MatchRecursive | Qt::MatchWrap);
        if(indexList.isEmpty())
            return;
//        qDebug() << "indexList.first().data(Qt::UserRole) : " + QString::number(indexList.first().data(Qt::UserRole).toInt());

        draggedIndex = indexList.first();

        emit modifyFlagsForDropsSignal(indexList.first().data(36).toString());




        QTreeView::dragEnterEvent(event);
    }


}

//-----------------------------------------------------------------------------------------
void DockedTree::dragLeaveEvent( QDragLeaveEvent *event )
{
    qDebug() << "drag leave event";

    emit modifyFlagsForDropsSignal("nothing");
    QTreeView::dragLeaveEvent(event);

}


void DockedTree::dropEvent(QDropEvent* event)
{
    qDebug() << "drop event";

    QString type = draggedIndex.data(36).toString();


    QTreeView::dropEvent(event);
    emit modifyFlagsForDropsSignal("nothing");
}



//----------------------------------------------------------------------------------

void DockedTree::itemClicked(QModelIndex index)
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
void DockedTree::contextMenuEvent(QContextMenuEvent *event)
{
    MainTreeContextMenu *contextMenu = new MainTreeContextMenu(this, this);
    contextMenu->setMainTreeAbstractModel(absTreeModel);
    contextMenu->setHub(hub);
    contextMenu->postConstructor();


    QModelIndex index = this->indexAt(event->pos());
    if(!index.isValid()){
        return;
    }

    int enteredItemId = this->model()->data(index,Qt::UserRole ).toInt();



contextMenu->setId(enteredItemId);

contextMenu->menu(MainTreeContextMenu::Rename | MainTreeContextMenu::Badge
                  | MainTreeContextMenu::Move | MainTreeContextMenu::Delete
                 | MainTreeContextMenu::AddSheet | MainTreeContextMenu::Advanced
                  | MainTreeContextMenu::Status)->exec(event->globalPos());

delete contextMenu;

}

//--------------------------------------------------------------------------------
