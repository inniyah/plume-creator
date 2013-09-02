#ifndef DOCKEDTREE_H
#define DOCKEDTREE_H

#include <QTreeView>
#include "hub.h"
#include "mainTree/maintreecontextmenu.h"
#include "mainTree/maintreeabstractmodel.h"


class DockedTree : public QTreeView
{
    Q_OBJECT
public:
    explicit DockedTree(QWidget *parent = 0);
    void postConstructor();

protected:
    void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
 void dragEnterEvent(QDragEnterEvent *event);
 void dragLeaveEvent(QDragLeaveEvent *event);
 void dropEvent(QDropEvent *event);
 void contextMenuEvent(QContextMenuEvent *event);

signals:
 void modifyFlagsForDropsSignal(QString type);
 void textAndNoteSignal(int number, QString action);

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void setMainTreeAbstractModel(MainTreeAbstractModel *tree){absTreeModel = tree;}
    void applySettingsFromData();

private slots:
    void itemCollapsedSlot(QModelIndex index);
    void itemExpandedSlot(QModelIndex index);
    void itemClicked(QModelIndex index);

private:
    Hub *hub;
   MainTreeAbstractModel  *absTreeModel;

    MainTreeContextMenu *contextMenu;
    QList<QModelIndex> allIndexesFromModel(QAbstractItemModel *model, const QModelIndex &parent);
QModelIndexList m_selectedIndexes;
QModelIndex draggedIndex;

// clicks :
bool oneClickCheckpoint, twoClicksCheckpoint, threeClicksCheckpoint;
QModelIndex oldIndex;


};

#endif // DOCKEDTREE_H
