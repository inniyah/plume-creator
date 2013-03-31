#ifndef ATTENDMANAGERTREEPROXYMODEL_H
#define ATTENDMANAGERTREEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "hub.h"
#include "attendtreeitem.h"

class AttendManagerTreeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit AttendManagerTreeProxyModel(QObject *parent = 0);
    void postConstructor();
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void addObject(QModelIndex index);
    void addGroup(QModelIndex index);
    void remove(QModelIndex index);

signals:
    void resetDomDocSignal();
    void setNameSignal(QString value);

public slots:
    void setHub(Hub *varHub){hub = varHub;}


private:
    Hub *hub;
    void domModified();
    int freeNumber();
    void removeAttendNumberFromSheets(int itemNumber);
    int maxNumber;

    QHash<int, QDomElement> domElementForNumber;

};

#endif // ATTENDMANAGERTREEPROXYMODEL_H
