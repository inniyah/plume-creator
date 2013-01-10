#ifndef ATTENDBOX_H
#define ATTENDBOX_H

#include <QtGui>
#include <QWidget>
#include <QFrame>

namespace Ui {
class AttendBox;
}

class AttendBox : public QFrame
{
    Q_OBJECT
    
public:
    explicit AttendBox(QWidget *parent = 0);
    ~AttendBox();
    bool readProjectAttendance(QFile *device){return true;}

public slots:
    bool saveAll(){return true;}
    bool closeAll(){return true;}
    void accept(){}

    void setCurrentList(int number){}
    void setCurrentListName(QString currentTabName){}
private:
    Ui::AttendBox *ui;
};

#endif // ATTENDBOX_H
