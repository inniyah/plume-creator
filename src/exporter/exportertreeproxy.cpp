#include "exportertreeproxy.h"

ExporterTreeProxy::ExporterTreeProxy(QObject *parent) :
    QSortFilterProxyModel(parent), m_indexTypeDragged("nothing"), badgeIsDisplayed(false)
{
}
void ExporterTreeProxy::postConstructor()
{
}

int ExporterTreeProxy::columnCount(const QModelIndex &parent) const
{
    return 1;
}



bool ExporterTreeProxy::filterAcceptsRow(int sourceRow,
                                         const QModelIndex &sourceParent) const
{
    QModelIndex indexToFilter = sourceModel()->index(sourceRow, 0, sourceParent);
    QString type = indexToFilter.data(36).toString();
    if(type == "book" || type == "act" || type == "chapter" || type == "scene" || type == "separator")
        return true;

    return false;

}








Qt::ItemFlags ExporterTreeProxy::flags(const QModelIndex &index) const
{

    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);


    if (!index.isValid()){
        return defaultFlags;
    }

    if (index.column() == 0)

        return defaultFlags | Qt::ItemIsTristate | Qt::ItemIsUserCheckable;



    return defaultFlags;
}

QVariant ExporterTreeProxy::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    if (role == Qt::CheckStateRole && col == 0){
        MainTreeItem *item = static_cast<MainTreeItem*>(this->mapToSource(index).internalPointer());
        return item->checkState();
    }



    return QSortFilterProxyModel::data(index,role);

}
bool ExporterTreeProxy::setData(const QModelIndex &index,
                                const QVariant &value, int role)
{

    QVector<int> vector(1, role);

    if (index.isValid() && role == Qt::CheckStateRole && index.column() == 0){

        this->checkBoxChanged(index, static_cast<Qt::CheckState>(value.toInt()));




#if QT_VERSION < 0x050000
    emit dataChanged(index, index);
#endif
#if QT_VERSION >= 0x050000
    emit dataChanged(index, index, vector);
#endif

        this->invalidateFilter();
        return true;
    }

    return QSortFilterProxyModel::setData(index, value, role);
}
//------------------------------------------------------------------------------------

void ExporterTreeProxy::checkBoxChanged(QModelIndex index, Qt::CheckState state)
{

    setChildrenCheckState(index, state);
    setParentCheckState(index.parent(), state);


}




//------------------------------------------------------------------------------------

void ExporterTreeProxy::setChildrenCheckState(QModelIndex index, Qt::CheckState state)
{

    if(!index.isValid())
        return;



    MainTreeItem *item = static_cast<MainTreeItem*>(this->mapToSource(index).internalPointer());

    int number = index.data(Qt::UserRole).toInt();
    QDomElement element = hub->mainTree_domElementForNumberHash().value(number);
    element.setAttribute("exporterCheckState", QString::number(state));

//    qDebug() << "value : " + QString::number(state);
    item->setCheckState(state);

    QVector<int> vector(1, Qt::CheckStateRole);

#if QT_VERSION < 0x050000
    emit dataChanged(index, index);
#endif
#if QT_VERSION >= 0x050000
    emit dataChanged(index, index, vector);
#endif

    //    MainTreeItem *treeItem = static_cast<MainTreeItem*>(sourceIndex.internalPointer());

    //    this->setData(treeItem->index(), state ,Qt::CheckStateRole);


    for(int i = 0 ; i < item->childCount(); ++i){

        QModelIndex childIndex = item->childrenItems().at(i)->index();
        childIndex = this->mapFromSource(childIndex);
        if(childIndex.isValid()){
            setChildrenCheckState(childIndex, state);
        }

    }

}
void ExporterTreeProxy::setParentCheckState(QModelIndex parentIndex, Qt::CheckState state)
{
    if(!parentIndex.isValid())
        return;

    MainTreeItem *item = static_cast<MainTreeItem*>(this->mapToSource(parentIndex).internalPointer());

    //check if direct children are or are not checked

    bool partiallyChecked = false;
    bool checked = false;
    bool unchecked = false;

    foreach(const MainTreeItem *childItem, item->childrenItems()){
        if(childItem->checkState() == Qt::Checked)
            checked = true;
        if(childItem->checkState() == Qt::PartiallyChecked)
            partiallyChecked = true;
        if(childItem->checkState() == Qt::Unchecked)
            unchecked = true;
    }
    if(checked == true && partiallyChecked == true && unchecked == true){
        state = Qt::PartiallyChecked;
    }
    if(checked == true && partiallyChecked == true && unchecked == false){
        state = Qt::PartiallyChecked;
    }
    if(checked == true && partiallyChecked == false && unchecked == true){
        state = Qt::PartiallyChecked;
    }
    if(checked == true && partiallyChecked == false && unchecked == false){
        state = Qt::Checked;
    }
    if(checked == false && partiallyChecked == true && unchecked == true){
        state = Qt::PartiallyChecked;
    }
    if(checked == false && partiallyChecked == false && unchecked == true){
        state = Qt::Unchecked;
    }
    if(checked == false && partiallyChecked == true && unchecked == false){
        state = Qt::PartiallyChecked;
    }
    if(checked == false && partiallyChecked == false && unchecked == false){
        state = Qt::Unchecked;  // never happen
    }

    //


    int number = parentIndex.data(Qt::UserRole).toInt();
    QDomElement element = hub->mainTree_domElementForNumberHash().value(number);
    element.setAttribute("exporterCheckState", QString::number(state));

    item->setCheckState(state);
//    qDebug() << "value parent : " + QString::number(state);

    QVector<int> vector(1, Qt::CheckStateRole);

#if QT_VERSION < 0x050000
    emit this->dataChanged(parentIndex, parentIndex);
#endif
#if QT_VERSION >= 0x050000
    emit dataChanged(parentIndex, parentIndex, vector);
#endif

    parentIndex = parentIndex.parent();
    while(parentIndex.isValid()){
//            parentIndex = this->mapFromSource(parentIndex);
//            qDebug() << "ddddddd : " + QString::number(parentIndex.data(Qt::UserRole).toInt());
            setParentCheckState(parentIndex, state);

        parentIndex = parentIndex.parent();
    }
}
