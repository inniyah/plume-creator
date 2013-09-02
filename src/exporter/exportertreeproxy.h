#ifndef EXPORTERTREEPROXY_H
#define EXPORTERREEPROXY_H

#include <QSortFilterProxyModel>
#include "utils.h"
#include "hub.h"
#include "mainTree/maintreeitem.h"

class ExporterTreeProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ExporterTreeProxy(QObject *parent = 0);
    void postConstructor();
    int columnCount(const QModelIndex &parent) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
signals:

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void checkBoxChanged(QModelIndex index, Qt::CheckState state);

private slots:

    void setChildrenCheckState(QModelIndex index, Qt::CheckState state);
    void setParentCheckState(QModelIndex index, Qt::CheckState state);

private:
    Hub *hub;
QString m_indexTypeDragged;
bool badgeIsDisplayed;
};

#endif // EXPORTERTREEPROXY_H
