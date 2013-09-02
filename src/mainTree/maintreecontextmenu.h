#ifndef MAINTREECONTEXTMENU_H
#define MAINTREECONTEXTMENU_H

#include <QWidget>
#include "mainTree/maintreeabstractmodel.h"

class MainTreeContextMenu : public QObject
{
    Q_OBJECT
public:

    enum Action {
           NoOption = 0x0,
           Badge = 0x1,
           Rename = 0x2,
        Advanced = 0x4,
        EmptyTrash = 0x8,
        Delete = 0x16,
        Move = 0x32,
        AddSheet = 0x64,
        Status = 0x128

       };
       Q_DECLARE_FLAGS(Actions, Action)

    explicit MainTreeContextMenu(QObject *parent = 0, QWidget *baseWidget = new QWidget());
    void postConstructor();
    QMenu *menu(MainTreeContextMenu::Actions flags  = MainTreeContextMenu::NoOption);
    void setId(int id){ m_id = id;}


signals:
void actionSignal(QString action, int idNumber, QVariant var);
void actionSignal(QString action, int idNumber);

public slots:
void setMainTreeAbstractModel(MainTreeAbstractModel *tree){absTreeModel = tree;}
void setHub(Hub *varHub){hub = varHub;}

private slots:

void moveUp(){emit actionSignal("moveUp", m_id);}
void moveDown(){emit actionSignal("moveDown", m_id);}
void sendToTrash(){emit actionSignal("sendToTrash",m_id );}
void autoRenameChildren(){emit actionSignal("autoRenameChildren",m_id );}
void split(){emit actionSignal("split", m_id, splitChoice);}

void addBookNext(){emit actionSignal("addBookNext", m_id);}
void addActNext(){emit actionSignal("addActNext", m_id);}
void addChapterNext(){emit actionSignal("addChapterNext", m_id);}
void addSceneNext(){emit actionSignal("addSceneNext", m_id);}
void addSeparatorNext(){emit actionSignal("addSeparatorNext", m_id);}
void addChildAct(){emit actionSignal("addChildAct", m_id);}
void addChildChapter(){emit actionSignal("addChildChapter", m_id);}
void addChildScene(){emit actionSignal("addChildScene", m_id);}
void addChildSeparator(){emit actionSignal("addChildSeparator", m_id);}



       void displaySplitDialog();
       void rename();
       void addMulti();
       void displayBadge(bool value);
       void setBadgeText();
       void setStatus();
    // for trash :
    void displayEmptyTrashDialog();


    private:
    void prepareContextMenu();
    void splitChoiceChanged(int choice);

    Hub *hub;
MainTreeAbstractModel  *absTreeModel;
QWidget *base;
    int m_id;
QDomElement targetedElement;

    QAction *renameAct,
    *addBookNextAct,
    *addActNextAct,
    *addChapterNextAct,
    *addSceneNextAct,
    *addSeparatorNextAct,
    *addChildActAct,
    *addChildChapterAct,
    *addChildSceneAct,
    *addChildSeparatorAct,
    *autoRenameChildrenAct,
    *sendToTrashAct,
    *moveUpAct,
    *moveDownAct,
    *splitAct,
    *addMultiAct,
    *emptyTrashAct,
    *displayBadgeAct,
    *setBadgeTextAct;

QString splitChoice;


int numberOfStatusActions;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MainTreeContextMenu::Actions)

#endif // MAINTREECONTEXTMENU_H
