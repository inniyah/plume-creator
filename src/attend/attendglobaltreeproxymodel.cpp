#include "attendglobaltreeproxymodel.h"

AttendGlobalTreeProxyModel::AttendGlobalTreeProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

//-------------------------------------------------------------------------------

Qt::DropActions  AttendGlobalTreeProxyModel::supportedDropActions () const
{
    return Qt::CopyAction;
}

//-------------------------------------------------------------------------------

QMimeData *AttendGlobalTreeProxyModel::mimeData(const QModelIndexList &indexes) const
{
    QByteArray encodedData;
    QMimeData *mimeData = new QMimeData();

    foreach(const QModelIndex &index, indexes){
        QModelIndex index0 = mapToSource(index);
        AttendTreeItem *treeItem = static_cast<AttendTreeItem*>(index0.internalPointer());
        if(treeItem->isGroup()){
            if(!treeItem->childrenItems().isEmpty()){
                QList<AttendTreeItem *> childrenList = treeItem->childrenItems();
                foreach(const AttendTreeItem *item, childrenList){
                    encodedData.append("-" + QString::number(item->idNumber()));
//                    qDebug() << "encodedData.append : " << QString::number(item->idNumber());
                }
            }
        }
        else
            encodedData.append("-" + index.data(Qt::UserRole).toString());
    }
    mimeData->setData("application/x-plumecreatorattendnumberdata", encodedData);
    return mimeData;
}
//-------------------------------------------------------------------------------

QStringList AttendGlobalTreeProxyModel::mimeTypes () const
{
    QStringList list;
    list << "application/x-plumecreatorattendnumberdata";
    return list;
}

//-------------------------------------------------------------------------------

bool AttendGlobalTreeProxyModel::dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent )
{


    if (data->hasFormat("application/x-plumecreatorattendnumberdata"))
    {
        qDebug() << "to global : data->text :  "<< QString::fromUtf8(data->data("application/x-plumecreatorattendnumberdata"));

        QString numbersString = QString::fromUtf8(data->data("application/x-plumecreatorattendnumberdata"));
        QStringList list = numbersString.split("-", QString::SkipEmptyParts);
        QList<int> objectsList;
        foreach(const QString &string, list)
            objectsList.append(string.toInt());


        emit attendSheetDataNumbersDropped(objectsList);

        return true;

    }


    return false;


}

//-------------------------------------------------------------------------------

Qt::ItemFlags AttendGlobalTreeProxyModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QSortFilterProxyModel::flags(index) ;



    if (index.isValid() )
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}
