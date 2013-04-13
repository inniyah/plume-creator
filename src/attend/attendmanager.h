#ifndef ATTENDMANAGER_H
#define ATTENDMANAGER_H

#include <QDialog>
#include "attendabstractmodel.h"
#include "attendmanagertreeproxymodel.h"
#include "hub.h"
#include "attendsettings.h"

namespace Ui {
class AttendManager;
}

class AttendManager : public QDialog
{
    Q_OBJECT
    
public:
    explicit AttendManager(QWidget *parent, AttendAbstractModel *absModel);
    ~AttendManager();
    void postConstructor();

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void applySettings();

private slots:
    void setItemActivated(QModelIndex index);
    void launchAttendSettings();
    void attendSettingsAccepted();
    void saveToElement();
    void saveText();
    void textAlreadyChangedSlot(bool textChanged){textAlreadyChanged = textChanged;}
    void editItemTitle(QModelIndex index);
    void setNameSlot(QString value);
    void resetDomElementForNumber();

    void on_addObjectAction_triggered();

    void on_addGroupAction_triggered();

    void on_removeAction_triggered();

    void expandAll();
    void nameEditingFinished();

private:
    void connectAll();
    void disconnectAll();
    void saveToAttendTreeItem();

    Ui::AttendManager *ui;
    Hub *hub;
    AttendManagerTreeProxyModel *managerProxyModel;
    AttendAbstractModel *abstractModel;
    QHash<int, QDomElement> domElementForNumber;
    QDomElement openedElement;
    bool textAlreadyChanged;
    bool oneClickCheckpoint, twoClicksCheckpoint, threeClicksCheckpoint;
    QModelIndex oldIndex;

    AttendTreeItem *attendTreeItemActivated;
};

#endif // ATTENDMANAGER_H
