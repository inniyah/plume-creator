#ifndef ATTENDGLOBALTREEPROXYMODEL_H
#define ATTENDGLOBALTREEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "attendtreeitem.h"
#include "hub.h"

class AttendGlobalTreeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit AttendGlobalTreeProxyModel(QObject *parent = 0);
    
    // drag drop :
  Qt::DropActions  supportedDropActions () const;
  QMimeData *mimeData(const QModelIndexList &indexes) const;
  QStringList mimeTypes () const;
    bool dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent );
    Qt::ItemFlags flags(const QModelIndex &index) const;


signals:
    void attendSheetDataNumbersDropped(QList<int> list);

public slots:
    void setHub(Hub *varHub){hub = varHub;}

private:
    Hub *hub;
};

#endif // ATTENDGLOBALTREEPROXYMODEL_H
