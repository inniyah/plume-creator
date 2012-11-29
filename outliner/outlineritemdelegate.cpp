#include "outliner/outlineritemdelegate.h"

#include <QtGui>

OutlinerItemDelegate::OutlinerItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent),editorLaunched(false)
{
}
//const qreal OutlinerItemDelegate::THUMB_HEIGHT = 50.0;
//const qreal OutlinerItemDelegate::THUMB_WIDTH = 50.0;
//const qreal OutlinerItemDelegate::PADDING = 10.0;
//const qreal OutlinerItemDelegate::ITEM_HEIGHT = THUMB_HEIGHT+20;
//const qreal OutlinerItemDelegate::ITEM_WIDTH = THUMB_WIDTH+20;


//OutlinerItemDelegate::OutlinerItemDelegate(QObject* parent)
//    : QStyledItemDelegate(parent), downloadInfo(true) {
//    boldFont.setBold(true);
//    smallerBoldFont = FontUtils::smallBold();
//    smallerFont = FontUtils::small();


//}

// This function is only called to paint the text





QWidget *OutlinerItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index ) const
{
    //  QTextEdit *editor = new QTextEdit(parent);
    ////  editor->setGeometry(QRect(40, 30, 401, 31));
    ////  editor->setLayoutDirection(Qt::LeftToRight);
    ////  //editor->setAutoFillBackground(false);
    ////  //editor->setStyleSheet(QString::fromUtf8("background-color: transparent;;"));
    ////  editor->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
    ////  editor->setFrameShape(QFrame::WinPanel);
    //// // editor->setFrameShadow(QFrame::Plain);
    ////  editor->setLineWidth(2);
    ////  editor->setReadOnly(true);
    //  editor->setText("This is text test!!1");

    //  return editor;
    //    if(index.column() == 1 || index.column() == 2){


    NoteZone *note = new NoteZone(parent);
    //     te->setFixedHeight(maxHeight+maxHeight/8);
    note->document()->setDocumentMargin(0);
    note->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //        qDebug()<< "createEditor";

    return(note);
    //    }
    //    else
    //        return QStyledItemDelegate::createEditor(parent,option,index);

}


void OutlinerItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    //    if(index.column() == 1 || index.column() == 2){

    NoteZone * note = static_cast<NoteZone*>(editor);
    //int value = index.model()->data(index, Qt::EditRole).toUInt();
    // do somthing with the widgets

    QTextDocument *noteDoc = new QTextDocument;
    noteDoc->setHtml(index.data(33).toString());
    note->openOutlinerDoc(noteDoc);


    //    }
    //    else QStyledItemDelegate::setEditorData(editor,index);
}

void OutlinerItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    //    if(index.column() == 1 || index.column() == 2){

    NoteZone * textEdit = static_cast<NoteZone*>(editor);
    //model->setData(index, comboBox->currentIndex(), Qt::EditRole);



    //         QTextCharFormat charFormat;
    //         charFormat.setFontPointSize(14);
    //         charFormat.setFontFamily("Libération");
    //         QTextBlockFormat blockFormat;
    //         blockFormat.setBottomMargin(10);
    //         blockFormat.setTextIndent(10);




    //         QTextCursor *tCursor = new QTextCursor(textEdit->document());
    //         tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    //         tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
    //         tCursor->mergeCharFormat(charFormat);
    //         tCursor->mergeBlockFormat(blockFormat);

    //         QFont font;
    //         font.setFamily(synDefaultFont.family());
    //         font.setPointSize(synDefaultHeight);
    //         synOutlineDoc->setDefaultFont(font);




    model->setData(index,textEdit->toHtml(),  Qt::EditRole);

    //    }
    //    else QStyledItemDelegate::setModelData(editor,model,index);
}

void OutlinerItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex & index) const
{
    //    if(index.column() == 1 || index.column() == 2){
    editor->setGeometry(option.rect);
    //    }
}



//OutlinerItemDelegate::~OutlinerItemDelegate() { }

QSize OutlinerItemDelegate::sizeHint( const QStyleOptionViewItem& option,
                                      const QModelIndex& index ) const
{


    //    if(index.column() == 0){
    //            QStyledItemDelegate::sizeHint(option,index);
    //    }



    //    return QSize( 256, THUMB_HEIGHT+20.0);
    //    QStyleOptionViewItemV4 options = option;
    //initStyleOption(&options, index);

    //QTextDocument doc;
    //doc.setHtml(options.text);
    //doc.setTextWidth(options.rect.width());
    //return QSize(doc.idealWidth(), doc.size().height());


    // bold and underlined characters take more space
    // so you have to redefine this function as well
    // (if you have a checkbox or an icon in the item, you will have
    // to include their size to the returned value)


//    QTextDocument doc;
//    doc.setDefaultFont(option.font);
//    doc.setHtml(index.data(33).toString());
//    doc.setDocumentMargin(1);
//    QTextCursor cursor(&doc);
//    cursor.setPosition(0);
//    cursor.select(QTextCursor::BlockUnderCursor);
//    doc.setHtml(cursor.selection().toHtml());
//    if(doc.size().toSize().height() < 200)
//    return doc.size().toSize();
//else
    QStyledItemDelegate::sizeHint(option,index);


}

void OutlinerItemDelegate::paint( QPainter* painter,
                                  const QStyleOptionViewItem& option, const QModelIndex& index ) const
{



    //    //    int itemType = index.data(ItemTypeRole).toInt();
    //    //    if (itemType == ItemTypeVideo) {
    //    //        QApplication::style()->drawPrimitive( QStyle::PE_PanelItemViewItem, &option, painter );
    //    //        paintBody( painter, option, index );
    //    //   }
    //    if(index.column() == 1){

    //        qDebug() << "paint 1";
    //        QVariant value = index.data(Qt::DisplayRole);
    //        if (value.isValid() && !value.isNull())
    //        {
    //            qDebug() << "paint valid a";


    //            QTextDocument document;
    //            document.setDocumentMargin(2);
    //            document.setHtml(value.toString());
    //            painter->translate(option.rect.topLeft());
    //            document.drawContents(painter);
    //            painter->translate(-option.rect.topLeft());

    //qDebug() << "paint valid b";
    //            //            QApplication::style()->drawControl(QStyle::CE_ProgressBar,
    //            //                                               &progressBarOption, painter);
    //        }
    //        qDebug() << "paint valid 2";

    //    }
    //    else
    //        QStyledItemDelegate::paint( painter, option, index );

    //    if(index.column() == 1 || index.column() == 2){

    QStyleOptionViewItemV4 options = option;
    initStyleOption(&options, index);

    //    QString s = index.model()->data(index, Qt::DisplayRole).toString();
    NoteZone te;
    QTextDocument *textDoc = new QTextDocument;
    textDoc->setHtml(index.model()->data(index, 33).toString());
    //           te.openSyn(textDoc);
    te.openOutlinerDoc(textDoc);





    painter->save();

    //        textDoc->drawContents(painter,option.rect);
    options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);
    //              QRect clip(0, 0, options.rect.width()/*+iconSize.width()*/, options.rect.height());
    //                     painter->setClipRect(clip);
    te.setGeometry(option.rect);
    painter->translate(option.rect.x(), option.rect.y());
    te.render(painter);
    painter->restore();
    //    }
    //    else QStyledItemDelegate::paint( painter, option, index );








    //    QStyleOptionViewItemV4 options = option;
    //       initStyleOption(&options, index);

    //       painter->save();

    //       QTextDocument doc;
    //       doc.setHtml(options.text);

    //       options.text = "";
    //       options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

    //       // shift text right to make icon visible
    ////       QSize iconSize = options.icon.actualSize(options.rect.size());
    //       painter->translate(options.rect.left()/*+iconSize.width()*/, options.rect.top());
    //       QRect clip(0, 0, options.rect.width()/*+iconSize.width()*/, options.rect.height());

    //       //doc.drawContents(painter, clip);

    //       painter->setClipRect(clip);
    //       QAbstractTextDocumentLayout::PaintContext ctx;
    //       // set text color to red for selected item
    //       if (option.state & QStyle::State_Selected)
    //           ctx.palette.setColor(QPalette::Text, QColor("red"));
    //       ctx.clip = clip;
    //       doc.documentLayout()->draw(painter, ctx);

    //       painter->restore();


}

//void OutlinerItemDelegate::paintBody( QPainter* painter,
//                                    const QStyleOptionViewItem& option,
//                                    const QModelIndex& index ) const {

//    painter->save();
//    painter->translate( option.rect.topLeft() );


//    QRectF line(0, 0, option.rect.width(), option.rect.height());

//     if (downloadInfo) line.setWidth(line.width());
//    painter->setClipRect(line);
//     // get the video metadata
//    const StreamItemPointer pStreamItemPointer = index.data( VideoRole ).value<StreamItemPointer>();
//    const StreamItem *pStreamItem = pStreamItemPointer.data();

//    // thumb
//    if (!pStreamItem->thumbnail().isNull()) {
//        painter->drawImage(QRect(0, 0, THUMB_WIDTH, THUMB_HEIGHT), pStreamItem->thumbnail());
//     }
//    // media thumb
//    if(!pStreamItem->Mediathumbnail().isNull())
//    {
//        painter->drawImage(QRect(THUMB_WIDTH+10, 0, THUMB_WIDTH, THUMB_HEIGHT), pStreamItem->Mediathumbnail());
//    }
//save state
//     painter->save();
//     painter->restore();
//     // separator button line on each item
//    painter->setClipping(false);
//    painter->setPen(option.palette.color(QPalette::Midlight));
//    painter->drawLine(0, ITEM_HEIGHT-1, option.rect.width(), ITEM_HEIGHT-1);


//    painter->restore();



//}


//--------------------------------------------------------------------------------------------------


bool OutlinerItemDelegate::editorEvent ( QEvent * event, QAbstractItemModel * model,
                                         const QStyleOptionViewItem & option, const QModelIndex & index )
{

     return QStyledItemDelegate::editorEvent(event, model,option,index);

}



//--------------------------------------------------------------------------------------------------

bool OutlinerItemDelegate::eventFilter ( QObject * editor, QEvent * event )
{


    NoteZone * textEdit = static_cast<NoteZone*>(editor);


    if(event->type() == QEvent::MouseButtonRelease)
    {

        return true;
    }
    if(event->type() == QEvent::Wheel)
    {


        return true;
    }
    else

        return QStyledItemDelegate::eventFilter(editor, event);

}
