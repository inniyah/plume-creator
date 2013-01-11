#ifndef SLIMUPDATER_H
#define SLIMUPDATER_H

#include <QWidget>

namespace Ui {
class slimUpdater;
}

class slimUpdater : public QWidget
{
    Q_OBJECT
    
public:
    explicit slimUpdater(QWidget *parent = 0);
    ~slimUpdater();
    
private:
    Ui::slimUpdater *ui;
};

#endif // SLIMUPDATER_H
