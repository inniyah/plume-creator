#include "exporter/exportertree.h"

ExporterTree::ExporterTree(QWidget *parent) :
    QTreeView(parent)
{



    //    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));

    connect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(itemExpandedSlot(QModelIndex)), Qt::UniqueConnection);
    connect(this, SIGNAL(collapsed(QModelIndex)), this, SLOT(itemCollapsedSlot(QModelIndex)), Qt::UniqueConnection);

    //    QItemSelectionModel *selectModel = new QItemSelectionModel(this->model());
    //    connect(selectModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(updateSelection(QItemSelection,QItemSelection)));

}

void ExporterTree::postConstructor()
{


}

//--------------------------------------------------------------------------------
void ExporterTree::applySettingsFromData()
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

void ExporterTree::itemCollapsedSlot(QModelIndex index)
{


    int itemId = index.data(Qt::UserRole).toInt();

    QDomElement element = hub->mainTree_domElementForNumberHash().value(itemId);
    element.setAttribute("exporterExpanded", "no");

    this->update(index);

    hub->addToSaveQueue();
}

//--------------------------------------------------------------------------------

void ExporterTree::itemExpandedSlot(QModelIndex index)
{


    int itemId = index.data(Qt::UserRole).toInt();

    QDomElement element = hub->mainTree_domElementForNumberHash().value(itemId);
    element.setAttribute("exporterExpanded", "yes");


    hub->addToSaveQueue();

}
//-----------------------------------------------------------------------------------------



QList<QModelIndex> ExporterTree::allIndexesFromModel(QAbstractItemModel *model, const QModelIndex &parent)
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



