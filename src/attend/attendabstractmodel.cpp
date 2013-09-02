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
    if (role == Qt::ToolTipRole && col == 0){
        AttendTreeItem *item = static_cast<AttendTreeItem*>(index.internalPointer());
        return AttendAbstractModel::createToolTipText(item);
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

QString AttendAbstractModel::createToolTipText(AttendTreeItem *treeItem)
{





    QString toolTip(treeItem->name() + "\n" +
                    treeItem->quickDetails() + "\n" +
                    treeItem->aliases() + "\n" +
                    treeItem->box_1Value() + "\n" +
                    treeItem->box_2Value() + "\n" +
                    treeItem->box_3Value() + "\n" +
                    treeItem->spinBox_1_label() + " " + QString::number(treeItem->spinBox_1Value()));





    return toolTip;
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

    root = domDoc.documentElement().toElement();
    parseFolderElement(domDoc.documentElement());

    hub->set_attendTree_domElementForNumberHash(domElementForNumber);


}

QDomDocument AttendAbstractModel::getDomDocument()
{
    return domDoc;
}

void AttendAbstractModel::setDomDocument(QDomDocument domDocument)
{
    domDoc = domDocument;
}

void AttendAbstractModel::parseFolderElement(const QDomElement &element)
{

    QStringList box1List = root.attribute("box_1", tr("None")).split("--", QString::SkipEmptyParts);
    if(box1List.isEmpty()){
        box1List << tr("None");
        root.setAttribute("box_1", tr("None"));
    }

    QStringList box2List = root.attribute("box_2", tr("None")).split("--", QString::SkipEmptyParts);
    if(box2List.isEmpty()){
        box2List << tr("None");
        root.setAttribute("box_1", tr("None"));
    }

    QStringList box3List = root.attribute("box_3", tr("None")).split("--", QString::SkipEmptyParts);
    if(box3List.isEmpty()){
        box3List << tr("None");
        root.setAttribute("box_1", tr("None"));
    }

    QString spinBox_1_label = root.attribute("spinBox_1_label", tr("Age :"));



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





            treeItem->setName(child.attribute("name", ""));

            treeItem->setAliases(child.attribute("aliases", ""));
            treeItem->setQuickDetails(child.attribute("quickDetails", ""));

            treeItem->setBox_1Value(boxListValueAt(box1List,child.attribute("box_1", "0").toInt()));
            treeItem->setBox_2Value(boxListValueAt(box2List,child.attribute("box_2", "0").toInt()));
            treeItem->setBox_3Value(boxListValueAt(box3List,child.attribute("box_3", "0").toInt()));

            treeItem->setSpinbox_1_label(spinBox_1_label);
            treeItem->setSpinbox_1Value(child.attribute("spinBox_1", "0").toInt());








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





            treeItem->setName(child.attribute("name", ""));

            treeItem->setAliases(child.attribute("aliases", ""));
            treeItem->setQuickDetails(child.attribute("quickDetails", ""));

            treeItem->setBox_1Value(boxListValueAt(box1List,child.attribute("box_1", "0").toInt()));
            treeItem->setBox_2Value(boxListValueAt(box2List,child.attribute("box_2", "0").toInt()));
            treeItem->setBox_3Value(boxListValueAt(box3List,child.attribute("box_3", "0").toInt()));

            treeItem->setSpinbox_1_label(spinBox_1_label);
            treeItem->setSpinbox_1Value(child.attribute("spinBox_1", "0").toInt());






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

QString AttendAbstractModel::boxListValueAt(QStringList boxList, int index)
{
    if(boxList.size() == 0)
        boxList << tr("None");

    if(index >= boxList.size() || index < 0)
        index = 0;

    return boxList.at(index);
}
