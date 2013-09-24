#include "checkabletree.h"

CheckableTree::CheckableTree(QWidget *parent) :
    QTreeView(parent)
{



    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));

    connect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(itemExpandedSlot(QModelIndex)), Qt::UniqueConnection);
    connect(this, SIGNAL(collapsed(QModelIndex)), this, SLOT(itemCollapsedSlot(QModelIndex)), Qt::UniqueConnection);

    //    QItemSelectionModel *selectModel = new QItemSelectionModel(this->model());
    //    connect(selectModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(updateSelection(QItemSelection,QItemSelection)));

}

void CheckableTree::postConstructor()
{


}

//--------------------------------------------------------------------------------
void CheckableTree::applySettingsFromData()
{
    disconnect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(itemExpandedSlot(QModelIndex)));
    disconnect(this, SIGNAL(collapsed(QModelIndex)), this, SLOT(itemCollapsedSlot(QModelIndex)));


    QList<QModelIndex> indexList = this->allIndexesFromModel(this->model(), QModelIndex());

    foreach(const QModelIndex &index, indexList)
        this->setExpanded(index, index.data(40).toBool());

    connect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(itemExpandedSlot(QModelIndex)), Qt::UniqueConnection);
    connect(this, SIGNAL(collapsed(QModelIndex)), this, SLOT(itemCollapsedSlot(QModelIndex)), Qt::UniqueConnection);
}
//--------------------------------------------------------------------------------

void CheckableTree::itemCollapsedSlot(QModelIndex index)
{



    this->model()->setData(index, false, Qt::DecorationRole );
    this->update(index);


}

//--------------------------------------------------------------------------------

void CheckableTree::itemExpandedSlot(QModelIndex index)
{


    this->model()->setData(index, true, Qt::DecorationRole );
    this->update(index);



}
//-----------------------------------------------------------------------------------------



QList<QModelIndex> CheckableTree::allIndexesFromModel(QAbstractItemModel *model, const QModelIndex &parent)
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


//----------------------------------------------------------------------------------

void CheckableTree::itemClicked(QModelIndex index)
{

    if(index != oldIndex){ // reset if change
        oneClickCheckpoint = false;
        twoClicksCheckpoint = false;
    }
    oldIndex = index;


    if( oneClickCheckpoint && twoClicksCheckpoint){ // third click
        //            this->edit(index);
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

