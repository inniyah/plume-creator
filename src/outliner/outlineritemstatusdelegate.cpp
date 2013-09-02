#include "outlineritemstatusdelegate.h"

OutlinerItemStatusDelegate::OutlinerItemStatusDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    statusCss = "QComboBox{"
            "border-style: none;"
            "background-color: rgba(100,100,100,0)"
            "}"
            "QWidget#base{"
            "border-style: none;"
            "background-color: rgba(100,100,100,0)"
            "}"
            ;
}




QWidget *OutlinerItemStatusDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index ) const
{


    QComboBox *comboBox = new QComboBox(parent);
    comboBox->setStyleSheet(statusCss);
    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(finishStatusEdit()), Qt::UniqueConnection);

    if(index.data(36).toString() == "trash" || index.data(36).toString() == "separator")
        comboBox->hide();



    return(comboBox);




}


void OutlinerItemStatusDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
comboBox->addItems(MainTreeAbstractModel::giveStatusList());





}

void OutlinerItemStatusDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

   QComboBox *comboBox = static_cast<QComboBox*>(editor);






    model->setData(index,comboBox->currentIndex(),  Qt::EditRole);


}

void OutlinerItemStatusDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex & index) const
{
    editor->setGeometry(option.rect);
}




QSize OutlinerItemStatusDelegate::sizeHint( const QStyleOptionViewItem& option,
                                          const QModelIndex& index ) const
{

    return QStyledItemDelegate::sizeHint(option,index);


}

void OutlinerItemStatusDelegate::paint( QPainter* painter,
                                      const QStyleOptionViewItem& option, const QModelIndex& index ) const
{

    QStyleOptionViewItemV4 options = option;
    initStyleOption(&options, index);


    QComboBox *combo = new QComboBox;
    combo->addItems(MainTreeAbstractModel::giveStatusList());
    combo->setCurrentIndex(index.model()->data(index, 38).toInt());
combo->setFixedHeight(20);


//QWidget *widget = new QWidget;
//widget->setObjectName("base");
//QVBoxLayout *layout = new QVBoxLayout;
//layout->addWidget(combo);
//    widget->setStyleSheet(statusCss);
combo->setStyleSheet(statusCss);

    if(index.data(36).toString() == "trash" || index.data(36).toString() == "separator")
        combo->hide();



    painter->save();

    options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);
    combo->setGeometry(option.rect);
    painter->translate(option.rect.x(), option.rect.y());
    combo->render(painter);
    painter->restore();


}



//--------------------------------------------------------------------------------------------------


bool OutlinerItemStatusDelegate::editorEvent ( QEvent * event, QAbstractItemModel * model,
                                             const QStyleOptionViewItem & option, const QModelIndex & index )
{


    return QStyledItemDelegate::editorEvent(event, model,option,index);

}



//--------------------------------------------------------------------------------------------------

bool OutlinerItemStatusDelegate::eventFilter ( QObject * editor, QEvent * event )
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

