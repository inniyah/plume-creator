#ifndef ATTENDBASE_H
#define ATTENDBASE_H

#include <QtGui>
#include <QWidget>
#include <QFrame>

#include "hub.h"
#include "attend/attendabstractmodel.h"
#include "attend/attendsheettreeproxymodel.h"
#include "attend/attendglobaltreeproxymodel.h"
#include "attend/attendmanager.h"
//#include "attend/globaltreeview.h"
//#include "attend/sheettreeview.h"

namespace Ui {
class AttendBase;
}

class AttendBase : public QFrame
{
    Q_OBJECT
    
public:
    explicit AttendBase(QWidget *parent = 0);
    ~AttendBase();
    void startAttendance();

signals:


public slots:
    void setHub(Hub *varHub){hub = varHub;}

private slots:
    void launchAttendManager();
    void setManagerLaunched(){isManagerLaunched = false;}
    void on_collapseButton_clicked();

    void on_splitter_splitterMoved(int pos, int index);

    void expandAll();


private:
    Ui::AttendBase *ui;
    Hub *hub;

    AttendAbstractModel *absModel;
    bool isManagerLaunched;
};

#endif // ATTENDBASE_H
