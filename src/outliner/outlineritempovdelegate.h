#ifndef OUTLINERITEMPOVDELEGATE_H
#define OUTLINERITEMPOVDELEGATE_H

#include <QStyledItemDelegate>
#include <QtGui>

#include "hub.h"
#include "notezone.h"

class OutlinerItemPoVDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit OutlinerItemPoVDelegate(QObject *parent = 0);

        QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index ) const;
        void setEditorData(QWidget *editor, const QModelIndex &index) const;
        void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
        void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex & index ) const;
    QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const ;
    void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

protected:
        virtual bool editorEvent ( QEvent * event, QAbstractItemModel * model,
                                   const QStyleOptionViewItem & option, const QModelIndex & index );
        virtual bool eventFilter ( QObject * editor, QEvent * event );

signals:

public slots:
    void setHub(Hub *varHub){hub = varHub;}


private slots:


private :
    Hub *hub;
    bool editorLaunched;
    QString qListWidgetCss;
};

#endif // OUTLINERITEMPOVDELEGATE_H
