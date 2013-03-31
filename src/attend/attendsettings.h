#ifndef ATTENDSETTINGS_H
#define ATTENDSETTINGS_H

#include <QDialog>
#include "hub.h"

namespace Ui {
class AttendSettings;
}

class AttendSettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit AttendSettings(QWidget *parent = 0);
    ~AttendSettings();
    void postConstructor();

protected:
    void accept();

public slots:
    void setHub(Hub *varHub){hub = varHub;}

private slots:
    void itemActivatedSlot_1(QListWidgetItem *item);
    void itemActivatedSlot_2(QListWidgetItem *item);
    void itemActivatedSlot_3(QListWidgetItem *item);
    void addItem_1();
    void addItem_2();
    void addItem_3();
    void removeItem_1();
    void removeItem_2();
    void removeItem_3();
    void renameItem_1();
    void renameItem_2();
    void renameItem_3();

private:
    Ui::AttendSettings *ui;
    Hub *hub;

    QListWidgetItem *itemActivated_1;
    QListWidgetItem *itemActivated_2;
    QListWidgetItem *itemActivated_3;

};

#endif // ATTENDSETTINGS_H
