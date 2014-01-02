#ifndef OVERVIEWBASE_H
#define OVERVIEWBASE_H

#include <QWidget>
#include "hub.h"
#include "textcell.h"

namespace Ui {
class OverviewBase;
}

class OverviewBase : public QWidget
{
    Q_OBJECT

public:
    explicit OverviewBase(QWidget *parent = 0, Hub *hub = 0, int id = -1);
    ~OverviewBase();

private:
    Ui::OverviewBase *ui;

    QHash< TextCell*, QDomElement> qDomElementForTextCellHash;

};

#endif // OVERVIEWBASE_H
