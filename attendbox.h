#ifndef ATTENDBOX_H
#define ATTENDBOX_H

#include <QtGui>
#include <QWidget>
#include <QDomDocument>

class AttendBox : public QFrame
{
    Q_OBJECT
public:
    explicit AttendBox(QWidget *parent = 0);
bool readProjectAttendance(QFile *device);

signals:

public slots:
void openSheetAttendList(int number ,QString attendString);
void setCurrentList(int number);
void setManagerSheetList(int number);

 private slots:
   void buildList();
   QListWidgetItem *createItem(const QDomElement &element,
                                         QListWidgetItem *item);

   QList<QListWidgetItem *> *sortItems(QList<int> *attend, QString sorting = "nothing");
   bool showSheetAttendanceList(QList<QListWidgetItem *> *itemList);


   QListWidgetItem *separator(QString separatorName);
   void addAttendManagerButton();
void itemDoubleClick(QListWidgetItem* itemClicked);
void launchAttendManager();

void accept();
void readSettings();
void writeSettings();




private:
 QListWidget *attendList;
 QDomDocument domDocument;
 QDomElement root;
QListWidgetItem *managerLauncher;


int currentSheetNumber;
QListWidget *managerSheetList;

QHash<QListWidgetItem *, QDomElement> domElementForItem;
QHash<int, QDomElement> domElementForItemNumber;
QHash<int, QString> attendStringForNumber;

QWidget* attendManager;
bool attendManagerLaunched;
};

#endif // ATTENDBOX_H
