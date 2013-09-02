#ifndef OUTLINERSPREADSHEETPROXY_H
#define OUTLINERSPREADSHEETPROXY_H

#include <QSortFilterProxyModel>
#include "hub.h"

class OutlinerSpreadheetProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit OutlinerSpreadheetProxy(QObject *parent = 0);
    void postConstructor();
    ~OutlinerSpreadheetProxy();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole);
    QVariant data(const QModelIndex &index, int role) const;

    Qt::DropActions supportedDropActions() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    QStringList mimeTypes() const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
signals:
    void resetAbsModelSignal();

public slots:
     void setHub(Hub *varHub){hub = varHub;}

     // set the size hint :
     void shrinkRow(){itemHeight += 20; beginResetModel(); endResetModel();}
     void expandRow(){if(itemHeight > 40)itemHeight -= 20; beginResetModel(); endResetModel();}
     void setRowHeight(int height){itemHeight = height; beginResetModel(); endResetModel();}

     void columnOneResizedSlot(int newSize){textZoneColumnOneWidth = newSize;}
     void columnTwoResizedSlot(int newSize){textZoneColumnTwoWidth = newSize;}

     void modifyFlagsForDrops(QString indexType);

private:
    Hub *hub;

    int itemHeight;
    int textZoneColumnOneWidth;
    int textZoneColumnTwoWidth;

    QString m_indexTypeDragged;
};

#endif // OUTLINERSPREADSHEETPROXY_H
