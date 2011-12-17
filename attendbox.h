#ifndef ATTENDBOX_H
#define ATTENDBOX_H

#include <QtGui>
#include <QWidget>
#include <QDomDocument>

#include "notezone.h"

class AttendBox : public QFrame
{
    Q_OBJECT
public:
    explicit AttendBox(QWidget *parent = 0);
    bool readProjectAttendance(QFile *device);

signals:
    void projectAttendanceList(QHash<QListWidgetItem *, QDomElement> domElementForItem_, QHash<int, QDomElement> domElementForItemNumber_);
    void removeAttendNumberSignal(int itemNumber);
    void addAttendNumberToSheetSignal(QList<int> list);
    void removeAttendNumberFromSheetSignal(QList<int> list);

public slots:
    void openSheetAttendList(int number ,QString attendString);
    bool saveAll();
    bool saveThisDoc();
    bool saveDomDocument();
    void setCurrentList(int number);
    void setManagerSheetList(int number);
    void updateAllAttendances(QHash<int,QString> allAttendancesForNumber);
    void accept();

private slots:
    void buildList();
    QListWidgetItem *createItem(const QDomElement &element,
                                QListWidgetItem *item);

    QList<QListWidgetItem *> *sortItems(QList<int> *attend, QString sorting = "nothing");
    bool showSheetAttendanceList(QList<QListWidgetItem *> *itemList);


    QListWidgetItem *separator(QString separatorName);
    void addAttendManagerButton();
    void itemActivatedSlot(QListWidgetItem* itemActivated);
    void projectItemActivated(QListWidgetItem* itemActivated);
    void launchAttendManager();
    void setProjectList();

    void showDetailAnimation();
    void hideDetailAnimation();
    void showDetails();
    void hideDetails();
    void openDetail(QListWidgetItem* item);

    void readSettings();
    void writeSettings();

    void centerWindow(){    if(attendManager->x() < 0)
            attendManager->move(0, attendManager->y());}

    void attendManagerDestroyed();

    void saveAndUpdate();
    void firstnameChanged();
    void lastnameChanged();
    void nameChanged();
    void levelChanged();
    void roleChanged();

    void newCharSlot();
    void newItemSlot();
    void newPlaceSlot();
    void newAttendElementSlot(QString tagName);
    void deleteItems();

    void toSheetSlot();
    void toAllSlot();

private:
    QListWidget *attendList, *abstractList;
    QDomDocument domDocument;
    QDomElement root;
    QListWidgetItem *managerLauncher;
    QFile *attFile;
    QString devicePath;

    QHash<QTextDocument *, QFile *> fileForDoc;
    QHash<QTextDocument *, QFile *>::iterator u;

    int currentSheetNumber;
    QListWidget *projectList;
    QListWidget *managerSheetList;
    QGridLayout *detailLayout;
    QPushButton *showDetailButton, *hideDetailButton, *newCharButton, *newItemButton, *newPlaceButton, *deleteButton
    ,*toSheetButton, *toAllButton;
    QComboBox *levelComboBox, *roleComboBox;

    bool hideDetailsBool;
    bool firstDetailOpening;

    NoteZone *editZone;

    QLineEdit *firstnameEdit, *lastnameEdit, *nameEdit;
    QDomElement currentElement;
    QHash<QListWidgetItem *, QDomElement> domElementForItem;
    QHash<int, QDomElement> domElementForItemNumber;
    QHash<int, QString> attendStringForNumber;

    QWidget* attendManager;
    bool attendManagerLaunched;

    int currentManagerSheetList;

    QString newAttendName;
};

#endif // ATTENDBOX_H
