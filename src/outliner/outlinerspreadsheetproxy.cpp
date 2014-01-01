#include "outlinerspreadsheetproxy.h"

OutlinerSpreadheetProxy::OutlinerSpreadheetProxy(QObject *parent) :
    QSortFilterProxyModel(parent),itemHeight(40),
    textZoneColumnOneWidth(300), textZoneColumnTwoWidth(300),
    m_indexTypeDragged("nothing")
{
}

//------------------------------------------------------------------------------


void OutlinerSpreadheetProxy::postConstructor()
{

}

//------------------------------------------------------------------------------



OutlinerSpreadheetProxy::~OutlinerSpreadheetProxy()
{
}

//------------------------------------------------------------------------------



int OutlinerSpreadheetProxy::rowCount(const QModelIndex &parent) const
{

    return QSortFilterProxyModel::rowCount(parent);
}

//------------------------------------------------------------------------------



int OutlinerSpreadheetProxy::columnCount(const QModelIndex &parent) const
{
    return QSortFilterProxyModel::columnCount(parent);
}

//------------------------------------------------------------------------------
bool OutlinerSpreadheetProxy::filterAcceptsRow(int sourceRow,
                                               const QModelIndex &sourceParent) const
{
    QModelIndex indexToFilter = sourceModel()->index(sourceRow, 0, sourceParent);
    QString type = indexToFilter.data(36).toString();
    if(type == "book" || type == "act" || type == "chapter" ||type == "scene"  || type == "separator")
        return true;

    return false;

}


QVariant OutlinerSpreadheetProxy::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QSortFilterProxyModel::headerData(section, orientation, role);
}

//------------------------------------------------------------------------------



Qt::ItemFlags OutlinerSpreadheetProxy::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    QStringList myOptions;
    myOptions << "book" << "act" << "chapter" << "scene" << "separator";

    if (!index.isValid()){
        return defaultFlags| Qt::ItemIsDropEnabled;
    }
    QString type = index.data(36).toString();

    if (index.column() == 0 || index.column() == 1 || index.column() == 2 || index.column() == 3
            || index.column() == 4 || index.column() == 5){

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
            return defaultFlags| Qt::ItemIsEditable | Qt::ItemIsDragEnabled;

            break;

        }

    }

    return defaultFlags;

}

//------------------------------------------------------------------------------



bool OutlinerSpreadheetProxy::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QModelIndex sourceIndex = this->mapToSource(index);
    QVector<int> vector(1, role);

    if (sourceIndex.isValid() && role == Qt::DecorationRole && sourceIndex.column() == 0) {


        int itemId = sourceIndex.data(Qt::UserRole).toInt();

        QDomElement element = hub->mainTree_domElementForNumberHash().value(itemId);
        if(value.toBool() == true)
            element.setAttribute("outlinerExpanded", "yes");
        else
            element.setAttribute("outlinerExpanded", "no");


        MainTreeItem *item = static_cast<MainTreeItem*>(sourceIndex.internalPointer());
        item->setIsExpanded(value.toBool(), MainTreeItem::Outliner);




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


QVariant OutlinerSpreadheetProxy::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();



    if (!index.isValid())
        return QVariant();


    if (role == Qt::SizeHintRole){
        QTextDocument synDoc;
        synDoc.setTextWidth(textZoneColumnOneWidth);
        synDoc.setHtml(this->index(row,1, index.parent()).data(33).toString());
        synDoc.setDocumentMargin(1);
        QTextCursor cursor(&synDoc);
        cursor.setPosition(0);
        cursor.select(QTextCursor::BlockUnderCursor);
        synDoc.setHtml(cursor.selection().toHtml());
        QSize synSize(synDoc.size().toSize());
        synSize.setHeight(synSize.height() + 5);

        QTextDocument noteDoc;
        noteDoc.setTextWidth(textZoneColumnTwoWidth);
        noteDoc.setHtml(this->index(row,2, index.parent()).data(33).toString());
        noteDoc.setDocumentMargin(1);
        QTextCursor tcursor(&noteDoc);
        tcursor.setPosition(0);
        tcursor.select(QTextCursor::BlockUnderCursor);
        noteDoc.setHtml(tcursor.selection().toHtml());
        QSize noteSize(noteDoc.size().toSize());
        noteSize.setHeight(noteSize.height() + 5);

        if(qMax(synSize.height(), noteSize.height()) < itemHeight + 200
                && qMax(synSize.height(), noteSize.height()) > itemHeight){
            //            qDebug() << "qMax";
            if(synSize.height() >= noteSize.height())
                return synSize;
            else
                return noteSize;
        }
        else if((synSize.height() > itemHeight + 200 || noteSize.height() > itemHeight + 200)
                && (synSize.height() < itemHeight + 200 || noteSize.height() < itemHeight + 200)
                && (synSize.height() > itemHeight || noteSize.height() > itemHeight)){
            //            qDebug() << "minimum";
            if(synSize.height() >= noteSize.height())
                return noteSize;
            else
                return synSize;
        }
        else if(col == 3){
            return (QSize(50,itemHeight));
        }
        else{
            return (QSize(100,itemHeight));
        }
    }

    if (role == Qt::DecorationRole && col == 0){
        return MainTreeAbstractModel::giveDecoration(this->mapToSource(index), MainTreeItem::Outliner);

    }
    if (role == Qt::ForegroundRole && col == 0){
        return giveTextColor(this->mapToSource(index));
    }


    return QSortFilterProxyModel::data(index,role);
}


QBrush OutlinerSpreadheetProxy::giveTextColor(QModelIndex index)
{
    MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());
    if(item->type() == "book")
        return QBrush(Qt::black);
    if(item->type() == "act")
        return QBrush(Qt::red);
    if(item->type() == "chapter")
        return QBrush(Qt::blue);
    if(item->type() == "scene")
        return QBrush(Qt::black);
    if(item->type() == "separator")
        return QBrush(Qt::black);
    else
        return QBrush(Qt::black);
}



//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//---------------------------Drag & Drop-----------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------










Qt::DropActions OutlinerSpreadheetProxy::supportedDropActions() const
{
    return /*Qt::CopyAction | */Qt::MoveAction;
}
//-------------------------------------------------------------------------------

QMimeData *OutlinerSpreadheetProxy::mimeData(const QModelIndexList &indexes) const
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

QStringList OutlinerSpreadheetProxy::mimeTypes () const
{
    QStringList list;
    list << "application/x-plumecreator-maintreedata";
    return list;
}

//-------------------------------------------------------------------------------

bool OutlinerSpreadheetProxy::dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent )
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

        if(draggedElement.tagName() == "book")
            parentTargetElement = hub->mainTreeDomDoc().documentElement();
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

        hub->addToSaveQueue();
        emit resetAbsModelSignal();




        return true;

    }


    return false;


}

//-------------------------------------------------------------------------------


bool OutlinerSpreadheetProxy::removeRows(int row, int count, const QModelIndex & parent)
{




    beginRemoveRows(parent, row, row + count);
    endRemoveRows();

    qDebug() << "remove rows";

    return true;
}

void OutlinerSpreadheetProxy::modifyFlagsForDrops(QString indexType)
{
    m_indexTypeDragged = indexType;
}
