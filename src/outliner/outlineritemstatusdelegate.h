#ifndef OUTLINERITEMSTATUSDELEGATE_H
#define OUTLINERITEMSTATUSDELEGATE_H

#include <QStyledItemDelegate>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif
#include <QtGui>

#include "hub.h"
#include "notezone.h"
#include "mainTree/maintreeabstractmodel.h"

class OutlinerItemStatusDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit OutlinerItemStatusDelegate(QObject *parent = 0);

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
void finishStatusEdit();

public slots:


private slots:


private :
    bool editorLaunched;
    QString statusCss;
    
};

#endif // OUTLINERITEMSTATUSDELEGATE_H
