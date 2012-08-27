#ifndef OUTLINERITEMDELEGATE_H
#define OUTLINERITEMDELEGATE_H

#include "notezone.h"
#include <QStyledItemDelegate>
#include <QtGui>

class OutlinerItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit OutlinerItemDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index ) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex & index ) const;
    QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const ;
    void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

//    void paintBody( QPainter* painter,
//                                        const QStyleOptionViewItem& option,
//                                        const QModelIndex& index ) const;

protected:
    virtual bool editorEvent ( QEvent * event, QAbstractItemModel * model,
                                                      const QStyleOptionViewItem & option, const QModelIndex & index );
    virtual bool eventFilter ( QObject * editor, QEvent * event );

    signals:
    
public slots:


private slots:


private :
    NoteZone *note;
    NoteZone *syn;
    bool editorLaunched;
};

#endif // OUTLINERITEMDELEGATE_H
