#ifndef STARTCENTER_H
#define STARTCENTER_H

#include <QDialog>
#include "hub.h"

namespace Ui {
class StartCenter;
}

class StartCenter : public QDialog
{
    Q_OBJECT
    
public:
    explicit StartCenter(QWidget *parent = 0);
    ~StartCenter();
    void postConstructor();

signals:
    void newPrjSignal();

public slots:
    void setHub(Hub *varHub){hub = varHub;}

private slots:
    void on_newButton_clicked();

    void on_openButton_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::StartCenter *ui;
    Hub *hub;

    QString defaultOpenPath;

};

#endif // STARTCENTER_H
