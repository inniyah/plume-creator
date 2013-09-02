#ifndef DOCKEDTREEPROXY_H
#define DOCKEDTREEPROXY_H

#include <QSortFilterProxyModel>
#include "utils.h"
#include "hub.h"
#include "mainTree/maintreeitem.h"

class DockedTreeProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit DockedTreeProxy(QObject *parent = 0);
    void postConstructor();
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    Qt::DropActions supportedDropActions() const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
signals:
    void resetAbsModelSignal();

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void modifyFlagsForDrops(QString indexType);
    void displayBadgeSlot(bool value){badgeIsDisplayed = value;}

private:
    Hub *hub;
QString m_indexTypeDragged;
bool badgeIsDisplayed;
};

#endif // DOCKEDTREEPROXY_H
