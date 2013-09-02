#ifndef DOCKEDTRASHTREE_H
#define DOCKEDTRASHTREE_H

#include <QTreeView>
#include "hub.h"
#include "mainTree/maintreecontextmenu.h"
#include "mainTree/maintreeabstractmodel.h"


class DockedTrashTree : public QTreeView
{
    Q_OBJECT
public:
    explicit DockedTrashTree(QWidget *parent = 0);
    void postConstructor();

protected:
    void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
 void dragEnterEvent(QDragEnterEvent *event);
 void dragLeaveEvent(QDragLeaveEvent *event);
 void dropEvent(QDropEvent *event);
 void contextMenuEvent(QContextMenuEvent *event);
 void leaveEvent(QEvent *event);
 void enterEvent(QEvent *event);

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

    void extendTrashTree();
    void shrinkTrashTree();

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

bool isLeaveEventStopped;
};

#endif // DOCKEDTRASHTREE_H
