#ifndef INFOSHEETBASE_H
#define INFOSHEETBASE_H

#include <QWidget>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif
#include <QtGui>

#include "hub.h"

namespace Ui {
class InfoSheetBase;
}

class InfoSheetBase : public QWidget
{
    Q_OBJECT

public:
    explicit InfoSheetBase(QWidget *parent = 0);
    ~InfoSheetBase();
    void postConstructor();

    QList<QAction *> toolButtons();

public slots:
    void setHub(Hub *varHub){hub = varHub;}

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void applyInfoSheetConfig();

private:
    Hub *hub;
    Ui::InfoSheetBase *ui;
    QAction *applyChangesAct, *cancelChangesAct;


};

#endif // INFOSHEETBASE_H
