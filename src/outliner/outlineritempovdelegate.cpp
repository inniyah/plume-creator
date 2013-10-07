#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include "outlineritempovdelegate.h"

OutlinerItemPoVDelegate::OutlinerItemPoVDelegate(QObject *parent) :
    QStyledItemDelegate(parent),editorLaunched(false)
{
    qListWidgetCss =    "QListWidget{"
            "border-style: none;"
            "background-color: rgba(100,100,100,0)"
            "}"
            ;
}



QWidget *OutlinerItemPoVDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index ) const
{


    QListWidget *listWidget = new QListWidget(parent);
    //        qDebug()<< "createEditor";
    listWidget->setStyleSheet(qListWidgetCss);

    return(listWidget);
    //       }
    //    else
    //        return QStyledItemDelegate::createEditor(parent,option,index);

}


void OutlinerItemPoVDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

    QListWidget * listWidget = static_cast<QListWidget*>(editor);

    QStringList stringList = index.model()->data(index, 35).toStringList();

    foreach(const QString &name, stringList){
        QListWidgetItem *item = new QListWidgetItem(name, listWidget);
        item->setTextAlignment(Qt::AlignCenter);
    }
    listWidget->setStyleSheet(qListWidgetCss);


}

void OutlinerItemPoVDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

    //    NoteZone * textEdit = static_cast<NoteZone*>(editor);

    //    model->setData(index,textEdit->toHtml(),  Qt::EditRole);
}

void OutlinerItemPoVDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex & index) const
{
    editor->setGeometry(option.rect);
}




QSize OutlinerItemPoVDelegate::sizeHint( const QStyleOptionViewItem& option,
                                         const QModelIndex& index ) const
{



    return QStyledItemDelegate::sizeHint(option,index);


}

void OutlinerItemPoVDelegate::paint( QPainter* painter,
                                     const QStyleOptionViewItem& option, const QModelIndex& index ) const
{



    QStyleOptionViewItemV4 options = option;
    initStyleOption(&options, index);

    //    QString s = index.model()->data(index, Qt::DisplayRole).toString();


    QListWidget *listWidget = new QListWidget;

    QStringList stringList = index.model()->data(index, 35).toStringList();

    foreach(const QString &name, stringList){
        QListWidgetItem *item = new QListWidgetItem(name, listWidget);
        item->setTextAlignment(Qt::AlignCenter);
    }
    listWidget->setStyleSheet(qListWidgetCss);




    painter->save();

    //        textDoc->drawContents(painter,option.rect);
    options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);
    //              QRect clip(0, 0, options.rect.width()/*+iconSize.width()*/, options.rect.height());
    //                     painter->setClipRect(clip);
    listWidget->setGeometry(option.rect);
    painter->translate(option.rect.x(), option.rect.y());
    listWidget->render(painter);
    initStyleOption( &options, index );
    painter->restore();
    //    }
    //    else QStyledItemDelegate::paint( painter, option, index );

    //    QStyledItemDelegate::paint(painter,options,index);







}
//--------------------------------------------------------------------------------------------------
//void OutlinerItemPoVDelegate::initStyleOption(QStyleOptionViewItem *option,
//                       const QModelIndex &index) const
//  {
//      QStyledItemDelegate::initStyleOption(option, index);

//      QStyleOptionViewItemV4 *optionV4 =
//              qstyleoption_cast<QStyleOptionViewItemV4*>(option);

//      optionV4->backgroundBrush = QBrush(calculateColorForRow(index.row()));
//  }
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------


bool OutlinerItemPoVDelegate::editorEvent ( QEvent * event, QAbstractItemModel * model,
                                            const QStyleOptionViewItem & option, const QModelIndex & index )
{

    return QStyledItemDelegate::editorEvent(event, model,option,index);

}



//--------------------------------------------------------------------------------------------------

bool OutlinerItemPoVDelegate::eventFilter ( QObject * editor, QEvent * event )
{


    if(event->type() == QEvent::MouseButtonRelease)
    {

        return true;
    }
    if(event->type() == QEvent::Wheel)
    {

        //        qDebug(" QEvent::Wheel");
        return true;
    }
    else
        return QStyledItemDelegate::eventFilter(editor, event);


}
