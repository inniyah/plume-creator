#ifndef ATTENDSHEETTREEPROXYMODEL_H
#define ATTENDSHEETTREEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "attendtreeitem.h"
#include "hub.h"

class AttendSheetTreeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit AttendSheetTreeProxyModel(QObject *parent = 0);

      // drag drop :
    Qt::DropActions  supportedDropActions () const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    QStringList mimeTypes () const;
      bool dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent );
      Qt::ItemFlags flags(const QModelIndex &index) const;
      QVariant data(const QModelIndex &index, int role) const;

      protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

signals:
    
public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void currentSheetModified(int sheetNumber);
    void removeSheetObjects(QList<int> objectsList);
    void addSheetObjects(QList<int> objectsList);
    void setPointOfView();
    void setClickedIndex(QModelIndex proxyIndex);

private:
    Hub *hub;
    QList<int> attendList;
    QList<int> povList;
    QHash<int, QDomElement> domElementForNumber;
    QDomElement openedElement;
    QModelIndex clickedSourceIndex;
    QModelIndex clickedProxyIndex;
    bool nothingWasClicked;
};

#endif // ATTENDSHEETTREEPROXYMODEL_H
