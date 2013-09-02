#include "dockedtrashtreeproxy.h"

DockedTrashTreeProxy::DockedTrashTreeProxy(QObject *parent) :
    QSortFilterProxyModel(parent), m_indexTypeDragged("nothing")
{
}
void DockedTrashTreeProxy::postConstructor()
{
}

int DockedTrashTreeProxy::columnCount(const QModelIndex &parent) const
{
    return 1;
}
//--------------------------------------------------------------------------------------------------


bool DockedTrashTreeProxy::filterAcceptsRow(int sourceRow,
                                            const QModelIndex &sourceParent) const
{
    QModelIndex indexToFilter = sourceModel()->index(sourceRow, 0, sourceParent);
    QString type = indexToFilter.data(36).toString();
    bool isTrashed = indexToFilter.data(37).toBool();

    if( (isTrashed == true )  && (type == "book" || type == "act" || type == "chapter" ||type == "scene")  || type == "separator")
        return true;
    else if(type == "trash")
        return true;
    else
        return false;

    return false;

}

//----------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//---------------------------Drag & Drop-----------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------










Qt::DropActions DockedTrashTreeProxy::supportedDropActions() const
{
    return /*Qt::CopyAction | */Qt::MoveAction;
}
//-------------------------------------------------------------------------------

QMimeData *DockedTrashTreeProxy::mimeData(const QModelIndexList &indexes) const
{
    QByteArray encodedData;
    QMimeData *mimeData = new QMimeData();


    foreach(const QModelIndex &index, indexes){
        encodedData.append("-" + index.data(Qt::UserRole).toString());


    }


    //    qDebug() << "from maintree : encodedData.append : " << encodedData;
    mimeData->setData("application/x-plumecreator-maintreedata", encodedData);
    return mimeData;
}
//-------------------------------------------------------------------------------

QStringList DockedTrashTreeProxy::mimeTypes () const
{
    QStringList list;
    list << "application/x-plumecreator-maintreedata";
    return list;
}

//-------------------------------------------------------------------------------

bool DockedTrashTreeProxy::dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent )
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

        QHash<int, QDomElement> domElementForNumber = hub->mainTree_domElementForNumberHash();

        QDomElement draggedElement = domElementForNumber.value(draggedInt);

        QDomElement parentTargetElement = domElementForNumber.value(parent.data(Qt::UserRole).toInt());

        // if(!parent.isValid())
        parentTargetElement = hub->mainTreeDomDoc().documentElement().elementsByTagName("trash").at(0).toElement();

        parentTargetElement.appendChild(draggedElement);

        if(parentTargetElement.hasChildNodes())
            parentTargetElement.insertBefore(draggedElement, parentTargetElement.firstChild());

        //     if((row == -1 && column == -1) || !parent.isValid()){

        //         parentTargetElement.appendChild(draggedElement);
        //     }
        //     else{

        //         if(parentTargetElement.hasChildNodes()){

        //             if(parentTargetElement.childNodes().size() > row){

        //                 parentTargetElement.insertBefore(draggedElement, parentTargetElement.childNodes().at(row));

        //             }
        //             else{
        //                 parentTargetElement.appendChild(draggedElement);
        //             }

        //         }

        //     }


        Utils::applyAttributeRecursively(draggedElement, "isTrashed", "yes");


        hub->addToSaveQueue();
        emit resetAbsModelSignal();




        return true;

    }


    return false;


}

//-------------------------------------------------------------------------------


bool DockedTrashTreeProxy::removeRows(int row, int count, const QModelIndex & parent)
{




    beginRemoveRows(parent, row, row + count);
    endRemoveRows();

    qDebug() << "remove rows";

    return true;
}

Qt::ItemFlags DockedTrashTreeProxy::flags(const QModelIndex &index) const
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
            if(type == "trash")
                return defaultFlags  | Qt::ItemIsDropEnabled ;
            else
                return (defaultFlags | Qt::ItemIsDropEnabled) & ~(Qt::ItemIsSelectable);
            break;

        case 1 :
            if(type == "trash")
                return defaultFlags  | Qt::ItemIsDropEnabled ;
            else
                return (defaultFlags | Qt::ItemIsDropEnabled) & ~(Qt::ItemIsSelectable);
            break;

        case 2 :
            if(type == "trash")
                return defaultFlags   | Qt::ItemIsDropEnabled ;
            else
                return (defaultFlags | Qt::ItemIsDropEnabled) & ~(Qt::ItemIsSelectable);
            break;

        case 3 :
            if(type == "trash")
                return defaultFlags  | Qt::ItemIsDropEnabled ;
            else
                return (defaultFlags | Qt::ItemIsDropEnabled) & ~(Qt::ItemIsSelectable);
            break;

        case 4 :
            if(type == "trash"  )
                return defaultFlags | Qt::ItemIsDropEnabled ;
            else
                return (defaultFlags | Qt::ItemIsDropEnabled) & ~(Qt::ItemIsSelectable);
            break;

        default:
            if(type == "trash")
                return defaultFlags & ~(Qt::ItemIsSelectable);
            else
                return defaultFlags | Qt::ItemIsDragEnabled;

            break;

        }

    }

    return defaultFlags;
}

void DockedTrashTreeProxy::modifyFlagsForDrops(QString indexType)
{
    m_indexTypeDragged = indexType;
    qDebug() <<  "m_indexTypeDragged : " + m_indexTypeDragged;
}
