#include "dockedtreeproxy.h"

DockedTreeProxy::DockedTreeProxy(QObject *parent) :
    QSortFilterProxyModel(parent), m_indexTypeDragged("nothing"), badgeIsDisplayed(false)
{
}
void DockedTreeProxy::postConstructor()
{
    QSettings settings;
    badgeIsDisplayed = settings.value("MainTree/badgeDisplayed", false).toBool();
}

int DockedTreeProxy::columnCount(const QModelIndex &parent) const
{
    return 1;
}



bool DockedTreeProxy::filterAcceptsRow(int sourceRow,
                                       const QModelIndex &sourceParent) const
{
    QModelIndex indexToFilter = sourceModel()->index(sourceRow, 0, sourceParent);
    QString type = indexToFilter.data(36).toString();
    if(type == "book" || type == "act" || type == "chapter" || type == "scene" || type == "separator")
        return true;

    return false;

}

QVariant DockedTreeProxy::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();


    if( role == Qt::EditRole  && col == 0){
        MainTreeItem *item = static_cast<MainTreeItem*>(this->mapToSource(index).internalPointer());
        return item->data(col).toString();
    }
    if (role == Qt::DisplayRole && col == 0){
        MainTreeItem *item = static_cast<MainTreeItem*>(this->mapToSource(index).internalPointer());


        if(badgeIsDisplayed && item->badge() != "")
            return item->data(col).toString() + " - " + item->badge();
        else
            return item->data(col).toString();

    }
    if (role == Qt::DecorationRole && col == 0){
        return MainTreeAbstractModel::giveDecoration(this->mapToSource(index), MainTreeItem::DockedTree);

    }


    return QSortFilterProxyModel::data(index,role);

}
//------------------------------------------------------------------------------



bool DockedTreeProxy::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QModelIndex sourceIndex = this->mapToSource(index);
    QVector<int> vector(1, role);

    if (sourceIndex.isValid() && role == Qt::DecorationRole && sourceIndex.column() == 0) {


        int itemId = sourceIndex.data(Qt::UserRole).toInt();

        QDomElement element = hub->project()->mainTree_domElementForNumberHash().value(itemId);
        if(value.toBool() == true)
            element.setAttribute("dockedTreeExpanded", "yes");
        else
            element.setAttribute("dockedTreeExpanded", "no");


        MainTreeItem *item = static_cast<MainTreeItem*>(sourceIndex.internalPointer());
        item->setIsExpanded(value.toBool(), MainTreeItem::DockedTree);

#if QT_VERSION < 0x050000
        emit dataChanged(index, index);
#endif
#if QT_VERSION >= 0x050000
        emit dataChanged(index, index, vector);
#endif

        hub->addToSaveQueue();

        return true;
    }


    return QSortFilterProxyModel::setData(index, value, role)  ;
}

//------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//---------------------------Drag & Drop-----------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------










Qt::DropActions DockedTreeProxy::supportedDropActions() const
{
    return /*Qt::CopyAction | */Qt::MoveAction;
}
//-------------------------------------------------------------------------------

QMimeData *DockedTreeProxy::mimeData(const QModelIndexList &indexes) const
{
    QByteArray encodedData;
    QMimeData *mimeData = new QMimeData();

    //    // up to 4 levels : book, act, chapter, scenes

    foreach(const QModelIndex &index, indexes){
        encodedData.append("-" + index.data(Qt::UserRole).toString());


    }


    qDebug() << "from maintree : encodedData.append : " << encodedData;
    mimeData->setData("application/x-plumecreator-maintreedata", encodedData);
    return mimeData;
}
//-------------------------------------------------------------------------------

QStringList DockedTreeProxy::mimeTypes () const
{
    QStringList list;
    list << "application/x-plumecreator-maintreedata";
    return list;
}

//-------------------------------------------------------------------------------

bool DockedTreeProxy::dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent )
{

    if (data->hasFormat("application/x-plumecreator-maintreedata") && action == Qt::MoveAction)
    {
        qDebug() << "to maintree : data->text :  "<< QString::fromUtf8(data->data("application/x-plumecreator-maintreedata"));


        QString numbersString = QString::fromUtf8(data->data("application/x-plumecreator-maintreedata"));
        QStringList list = numbersString.split("-", QString::SkipEmptyParts);
        QList<int> objectsList;
        foreach(const QString &string, list)
            objectsList.append(string.toInt());


        int draggedInt = objectsList.first();

        QHash<int, QDomElement> domElementForNumber = hub->project()->mainTree_domElementForNumberHash();

        QDomElement draggedElement = domElementForNumber.value(draggedInt);

        QDomElement parentTargetElement = domElementForNumber.value(parent.data(Qt::UserRole).toInt());

        if(draggedElement.tagName() == "book")
            parentTargetElement = hub->project()->mainTreeDomDoc().documentElement();
        else if(!parent.isValid())
            return false;

        if(row == -1 && column == -1 ){

            parentTargetElement.appendChild(draggedElement);
        }
        else{

            if(parentTargetElement.hasChildNodes()){

                if(parentTargetElement.childNodes().size() > row){

                    parentTargetElement.insertBefore(draggedElement, parentTargetElement.childNodes().at(row));

                }
                else{
                    parentTargetElement.appendChild(draggedElement);
                }

            }

        }

        Utils::applyAttributeRecursively(draggedElement, "isTrashed", "no");


        hub->addToSaveQueue();
        emit resetAbsModelSignal();




        return true;

    }


    return false;


}

//-------------------------------------------------------------------------------


bool DockedTreeProxy::removeRows(int row, int count, const QModelIndex & parent)
{




    beginRemoveRows(parent, row, row + count);
    endRemoveRows();

    qDebug() << "remove rows";

    return true;
}

Qt::ItemFlags DockedTreeProxy::flags(const QModelIndex &index) const
{

    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    QStringList myOptions;
    myOptions << "book" << "act" << "chapter" << "scene" << "separator";

    if (!index.isValid()){
        return defaultFlags| Qt::ItemIsDropEnabled;
    }
    QString type = index.data(36).toString();

    if (index.column() == 0 || index.column() == 1 || index.column() == 2 || index.column() == 3){

        switch (myOptions.indexOf(m_indexTypeDragged)) {

        case 0 :
            return defaultFlags ;
            break;

        case 1 :
            if(type == "book")
                return defaultFlags  | Qt::ItemIsDropEnabled ;
            else
                return defaultFlags ;
            break;

        case 2 :
            if(type == "book" || type == "act")
                return defaultFlags | Qt::ItemIsDropEnabled ;
            else
                return defaultFlags ;
            break;

        case 3 :
            if(type == "chapter")
                return defaultFlags | Qt::ItemIsDropEnabled;

            break;

        case 4 :
            if(type == "chapter")
                return defaultFlags | Qt::ItemIsDropEnabled ;

            break;

        default:
            if(type == "separator")
                return defaultFlags | Qt::ItemIsDragEnabled;
            else
                return defaultFlags| Qt::ItemIsEditable | Qt::ItemIsDragEnabled;

            break;

        }

    }

    return defaultFlags;
}

void DockedTreeProxy::modifyFlagsForDrops(QString indexType)
{
    m_indexTypeDragged = indexType;
}

