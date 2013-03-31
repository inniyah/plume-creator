#include "attendabstractmodel.h"

AttendAbstractModel::AttendAbstractModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

AttendAbstractModel::~AttendAbstractModel()
{
    delete rootItem;

}

int AttendAbstractModel::rowCount(const QModelIndex &parent) const
{

    AttendTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<AttendTreeItem*>(parent.internalPointer());


    return parentItem->childCount();

    //    return 10;
}

int AttendAbstractModel::columnCount(const QModelIndex &parent) const
{

    if (parent.isValid())
        return static_cast<AttendTreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();

    //    return 3;

}

QVariant AttendAbstractModel::headerData(int section, Qt::Orientation orientation,
                                         int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();


    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section != -1){

        return rootItem->data(section);
    }

    else
        return QString("nothing");
}



//--------------------------------------------------------------------------------------------------


QVariant AttendAbstractModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    if (!index.isValid())
        return QVariant();

    if ((role == Qt::DisplayRole || role == Qt::EditRole) && col == 0){
        AttendTreeItem *item = static_cast<AttendTreeItem*>(index.internalPointer());
        return item->data(col).toString();
    }

    if (role == Qt::UserRole){
        AttendTreeItem *item = static_cast<AttendTreeItem*>(index.internalPointer());
        return item->idNumber();
    }
    else
        return QVariant();
}




QModelIndex AttendAbstractModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    //    if (index.column() == 0){
    AttendTreeItem *childItem = static_cast<AttendTreeItem*>(index.internalPointer());
    AttendTreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
    //    }
    //    return QModelIndex();

}


//--------------------------------------------------------------------------------------------------

QModelIndex AttendAbstractModel::index ( int row, int column, const QModelIndex & parent ) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();


    AttendTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<AttendTreeItem*>(parent.internalPointer());

    AttendTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();

}












//--------------------------------------------------------------------------------------------------

void AttendAbstractModel::resetAbsModel()
{
    beginResetModel();

    QList<QVariant> rootData;
    rootData << tr("Titles");
    rootItem = new AttendTreeItem(rootData);
    rootItem->setHub(hub);


    resetDomDoc();

    endResetModel();

}

void AttendAbstractModel::resetDomDoc()
{

    titlesList = new QStringList;
    numberList = new QList<int>;
    domElementForNumber.clear();


    numberOfDomElements = 0;

    treeGroupItemList = new QList<AttendTreeItem *>;
    treeObjectItemList = new QList<AttendTreeItem *>;

    parseFolderElement(hub->attendTreeDomDoc().documentElement());



}



void AttendAbstractModel::parseFolderElement(const QDomElement &element)
{


    QString title = element.attribute("name");
    if (title.isEmpty())
        title = QObject::tr("No Title");





    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {

        titlesList->append(child.attribute("name"));
        numberList->append(child.attribute("number").toInt());
        domElementForNumber.insert(child.attribute("number").toInt(), child);

        if (child.tagName() == "group") {



            QList<QVariant> itemData;

            itemData.append(child.attribute("name", "error"));

            AttendTreeItem *treeItem = new AttendTreeItem(itemData, rootItem);
            treeItem->setHub(hub);
            treeItem->setIdNumber(child.attribute("number").toInt());
            treeItem->setIsGroup(true);

            rootItem->appendChild(treeItem);

            treeGroupItemList->append(treeItem);



            parseFolderElement(child);


        } else if (child.tagName() == "obj") {


            QList<QVariant> itemData;

            itemData.append(child.attribute("name", "error"));


            AttendTreeItem *treeItem = new AttendTreeItem(itemData, treeGroupItemList->last());
            treeItem->setHub(hub);
            treeItem->setIdNumber(child.attribute("number").toInt());
            treeItem->setIsGroup(false);

            treeGroupItemList->last()->appendChild(treeItem);

            treeObjectItemList->append(treeItem);



        }


        int number = element.attribute("number").toInt();
        domElementForNumber.insert(number, element);



        numberOfDomElements += 1;
        child = child.nextSiblingElement();
    }


}

//-----------------------------------------------------------------------------------------------------------


