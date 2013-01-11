#ifndef SLIMUPDATER_H
#define SLIMUPDATER_H

#include <QWidget>

namespace Ui {
class SlimUpdater;
}

class SlimUpdater : public QWidget
{
    Q_OBJECT
    
public:
    explicit SlimUpdater(QWidget *parent = 0);
    ~SlimUpdater();
    
signals:
    void closeUpdaterSignal();


private:
    Ui::SlimUpdater *ui;
};

#endif // SLIMUPDATER_H
