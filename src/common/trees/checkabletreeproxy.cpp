#include "checkabletreeproxy.h"

CheckableTreeProxy::CheckableTreeProxy(QObject *parent, QString expandElementAttributeName, QString checkStateElementAttributeName, MainTreeItem::Tree tree) :
    QSortFilterProxyModel(parent), m_indexTypeDragged("nothing"), badgeIsDisplayed(false),
    expandAttributeName(expandElementAttributeName), checkStateAttributeName(checkStateElementAttributeName),
    treeType(tree), m_columnCount(1), secondaryOpenedSheet(-1)
{
}
void CheckableTreeProxy::postConstructor()
{
}

int CheckableTreeProxy::columnCount(const QModelIndex &parent) const
{
    return m_columnCount;
}



bool CheckableTreeProxy::filterAcceptsRow(int sourceRow,
                                          const QModelIndex &sourceParent) const
{
    QModelIndex indexToFilter = sourceModel()->index(sourceRow, 0, sourceParent);
    QString type = indexToFilter.data(36).toString();
    if(type == "book" || type == "act" || type == "chapter" || type == "scene" || type == "separator")
        return true;

    return false;

}








Qt::ItemFlags CheckableTreeProxy::flags(const QModelIndex &index) const
{

    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);


    if (!index.isValid()){
        return defaultFlags;
    }

    if (index.column() == 0){
        if(index.data(36).toString() == "separator" && !treeFlag.testFlag(CheckableTreeProxy::SeparatorCheckable))
            return defaultFlags;
        else
            return defaultFlags | Qt::ItemIsTristate | Qt::ItemIsUserCheckable;
    }


    return defaultFlags;
}

QVariant CheckableTreeProxy::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    if (role == Qt::CheckStateRole && col == 0){
        MainTreeItem *item = static_cast<MainTreeItem*>(this->mapToSource(index).internalPointer());

        if(item->type() == "separator" && !treeFlag.testFlag(CheckableTreeProxy::SeparatorCheckable))
            return QSortFilterProxyModel::data(index,role);
        else
            return item->checkState(treeType);
    }
    if (role == Qt::DecorationRole && col == 0){
        return MainTreeAbstractModel::giveDecoration(this->mapToSource(index), treeType);

    }
    if(role == Qt::DisplayRole && index.column() == 1){
        MainTreeItem *item = static_cast<MainTreeItem*>(this->mapToSource(index).internalPointer());

        int number = numberForSheetIdHash.value(item->idNumber(), -1);
        if(number = 0 || number == -1)
            return "";
        else
            return QString::number(number);
    }
    if(role == Qt::TextAlignmentRole && index.column() == 1){
        return Qt::AlignRight;
    }
    if (role == Qt::BackgroundRole && col == 0){
        MainTreeItem *item = static_cast<MainTreeItem*>(this->mapToSource(index).internalPointer());
        if(item->idNumber() == secondaryOpenedSheet)
            return QBrush(Qt::green);
        else
            return QVariant();
    }



    return QSortFilterProxyModel::data(index,role);

}
bool CheckableTreeProxy::setData(const QModelIndex &index,
                                 const QVariant &value, int role)
{

    QVector<int> vector(1, role);
    QModelIndex sourceIndex = this->mapToSource(index);

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


    if (index.isValid() && role == Qt::DecorationRole && index.column() == 0) {


        int itemId = sourceIndex.data(Qt::UserRole).toInt();

        QDomElement element = hub->mainTree_domElementForNumberHash().value(itemId);
        if(value.toBool() == true)
            element.setAttribute(expandAttributeName, "yes");
        else
            element.setAttribute(expandAttributeName, "no");


        MainTreeItem *item = static_cast<MainTreeItem*>(sourceIndex.internalPointer());
        item->setIsExpanded(value.toBool(), treeType);

        emit dataChanged(index, index);

        hub->addToSaveQueue();

        return true;
    }






    return QSortFilterProxyModel::setData(index, value, role);
}
//------------------------------------------------------------------------------------

void CheckableTreeProxy::checkBoxChanged(QModelIndex index, Qt::CheckState state)
{

    setChildrenCheckState(index, state);
    setParentCheckState(index.parent(), state, index.data(Qt::UserRole).toInt());


}

//------------------------------------------------------------------------------------

void CheckableTreeProxy::setSecondaryOpenedSheetSlot(int id)
{
secondaryOpenedSheet = id;

//this->resetInternalData();

}




//------------------------------------------------------------------------------------

void CheckableTreeProxy::setChildrenCheckState(QModelIndex index, Qt::CheckState state)
{

    if(!index.isValid())
        return;

    MainTreeItem *item = static_cast<MainTreeItem*>(this->mapToSource(index).internalPointer());
    int number = index.data(Qt::UserRole).toInt();
    QDomElement element = hub->mainTree_domElementForNumberHash().value(number);

    int count = item->childCount();


    if((index.data(36) == "book" || index.data(36) == "act" || index.data(36) == "chapter" || index.data(36) == "scene"  || index.data(36) == "separator") &&
            (element.attribute(checkStateAttributeName) == "0" && index.parent().isValid() &&
             index.parent().data(Qt::CheckStateRole).toInt() == 2)){
        state = Qt::Checked;
    }
    else if((index.data(36) == "book" || index.data(36) == "act" || index.data(36) == "chapter"|| index.data(36) == "scene" || index.data(36) == "separator") &&
            element.attribute(checkStateAttributeName) == "0"){   //allow tristate clicking
        state = Qt::PartiallyChecked;
        count = 0;
    }
    else if((index.data(36) == "book" || index.data(36) == "act" || index.data(36) == "chapter"|| index.data(36) == "scene" || index.data(36) == "separator") &&
            element.attribute(checkStateAttributeName) == "1"){
        state = Qt::Checked;

    }
    else if((index.data(36) == "book" || index.data(36) == "act" || index.data(36) == "chapter"|| index.data(36) == "scene" || index.data(36) == "separator") &&
            element.attribute(checkStateAttributeName) == "2"){
        state = Qt::Unchecked;
    }
    //    else if((index.data(36) == "scene" || index.data(36) == "separator") &&
    //            (element.attribute(checkStateAttributeName) == "0" || element.attribute(checkStateAttributeName) == "1" )){
    //        state = Qt::Checked;
    //    }
    //    else if((index.data(36) == "scene" || index.data(36) == "separator") &&
    //            element.attribute(checkStateAttributeName) == "2"){
    //        state = Qt::Unchecked;
    //    }


    if(index.data(36) == "separator" && !treeFlag.testFlag(CheckableTreeProxy::SeparatorCheckable))
            state = Qt::Unchecked;



    element.setAttribute(checkStateAttributeName, QString::number(state));
    item->setCheckState(state, treeType);


#if QT_VERSION < 0x050000
    emit dataChanged(index, index);
#endif
#if QT_VERSION >= 0x050000
    QVector<int> vector(1, Qt::CheckStateRole);
    emit dataChanged(index, index, vector);
#endif

    //    MainTreeItem *treeItem = static_cast<MainTreeItem*>(sourceIndex.internalPointer());

    //    this->setData(treeItem->index(), state ,Qt::CheckStateRole);


    for(int i = 0 ; i < count; ++i){

        QModelIndex childIndex = item->childrenItems().at(i)->index();
        childIndex = this->mapFromSource(childIndex);
        if(childIndex.isValid()){
            setChildrenCheckState(childIndex, state);
        }

    }

}
void CheckableTreeProxy::setParentCheckState(QModelIndex parentIndex, Qt::CheckState state, int idToForgetIfPartiallyChecked)
{
    if(!parentIndex.isValid())
        return;

    MainTreeItem *item = static_cast<MainTreeItem*>(this->mapToSource(parentIndex).internalPointer());

    //check if direct children are or are not checked

    bool partiallyChecked = false;
    bool checked = false;
    bool unchecked = false;

    foreach(MainTreeItem *childItem, item->childrenItems()){
        if(childItem->type() == "separator" && !treeFlag.testFlag(CheckableTreeProxy::SeparatorCheckable))
            continue;

        if(childItem->checkState(treeType) == Qt::Checked)
            checked = true;
        if(childItem->checkState(treeType) == Qt::PartiallyChecked && childItem->idNumber() != idToForgetIfPartiallyChecked)
            partiallyChecked = true;
        if(childItem->checkState(treeType) == Qt::Unchecked)
            unchecked = true;


    }
    if(checked == true && partiallyChecked == true && unchecked == true){
        state = Qt::PartiallyChecked;
    }
    else if(checked == true && partiallyChecked == true && unchecked == false){
        state = Qt::PartiallyChecked;
    }
    else if(checked == true && partiallyChecked == false && unchecked == true){
        state = Qt::PartiallyChecked;
    }
    else if(checked == true && partiallyChecked == false && unchecked == false){
        state = Qt::Checked;
    }
    else if(checked == false && partiallyChecked == true && unchecked == true){
        state = Qt::PartiallyChecked;
    }
    else if(checked == false && partiallyChecked == false && unchecked == true){
        state = Qt::Unchecked;
    }
    //    else if(checked == false && partiallyChecked == true && unchecked == false){
    //        state = Qt::PartiallyChecked;
    //    }
    //    else if(checked == false && partiallyChecked == false && unchecked == false){
    //        state = Qt::Unchecked;
    //    }
    //    else if(checked == false && partiallyChecked == false && unchecked == false &&){
    //        state = Qt::Unchecked;
    //    }
    //


    int number = parentIndex.data(Qt::UserRole).toInt();
    QDomElement element = hub->mainTree_domElementForNumberHash().value(number);




    element.setAttribute(checkStateAttributeName, QString::number(state));

    item->setCheckState(state, treeType);
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
void CheckableTreeProxy::setNumberForSheetIdHash(const QHash<int, int> &value)
{
    numberForSheetIdHash = value;

//this->resetInternalData();
}
