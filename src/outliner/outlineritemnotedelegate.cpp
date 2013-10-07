#include "outliner/outlineritemnotedelegate.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   

OutlinerItemNoteDelegate::OutlinerItemNoteDelegate(QObject *parent) :
    QStyledItemDelegate(parent),editorLaunched(false)
{
    noteZoneCss = "NoteZone{"
            "border-style: none;"
            "background-color: rgba(100,100,100,0)"
            "}"
            ;
}




QWidget *OutlinerItemNoteDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index ) const
{


    NoteZone *note = new NoteZone(parent);
    note->setHub(hub);  //     te->setFixedHeight(maxHeight+maxHeight/8);
    note->document()->setDocumentMargin(0);
    note->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //        qDebug()<< "createEditor";
    note->setStyleSheet(noteZoneCss);

    return(note);

}


void OutlinerItemNoteDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

    NoteZone * note = static_cast<NoteZone*>(editor);

    QTextDocument *noteDoc = new QTextDocument;
    noteDoc->setHtml(index.data(33).toString());
    note->openOutlinerDoc(noteDoc);



}

void OutlinerItemNoteDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

    NoteZone * textEdit = static_cast<NoteZone*>(editor);






    model->setData(index,textEdit->toHtml(),  Qt::EditRole);


}

void OutlinerItemNoteDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex & index) const
{
    editor->setGeometry(option.rect);
}




QSize OutlinerItemNoteDelegate::sizeHint( const QStyleOptionViewItem& option,
                                          const QModelIndex& index ) const
{

    return QStyledItemDelegate::sizeHint(option,index);


}

void OutlinerItemNoteDelegate::paint( QPainter* painter,
                                      const QStyleOptionViewItem& option, const QModelIndex& index ) const
{



    QStyleOptionViewItemV4 options = option;
    initStyleOption(&options, index);


    NoteZone te;
    QTextDocument *textDoc = new QTextDocument;
    textDoc->setHtml(index.model()->data(index, 33).toString());
    //           te.openSyn(textDoc);
    te.openOutlinerDoc(textDoc);


    te.setStyleSheet(noteZoneCss);



    painter->save();

    options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);
    te.setGeometry(option.rect);
    painter->translate(option.rect.x(), option.rect.y());
    te.render(painter);
    painter->restore();


}



//--------------------------------------------------------------------------------------------------


bool OutlinerItemNoteDelegate::editorEvent ( QEvent * event, QAbstractItemModel * model,
                                             const QStyleOptionViewItem & option, const QModelIndex & index )
{

    return QStyledItemDelegate::editorEvent(event, model,option,index);

}



//--------------------------------------------------------------------------------------------------

bool OutlinerItemNoteDelegate::eventFilter ( QObject * editor, QEvent * event )
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
