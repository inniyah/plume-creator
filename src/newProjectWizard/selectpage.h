#ifndef SELECTPAGE_H
#define SELECTPAGE_H

#include <QWizardPage>
#include <QDir>
#include <QFileDialog>

namespace Ui {
class SelectPage;
}

class SelectPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit SelectPage(QWidget *parent = 0);
    ~SelectPage();


private slots:
   void on_directoryButton_clicked();


private:
    Ui::SelectPage *ui;
};

#endif // SELECTPAGE_H
