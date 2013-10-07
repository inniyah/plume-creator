#ifndef STRUCTUREPAGE_H
#define STRUCTUREPAGE_H

#include <QWizardPage>
#include <QTreeWidgetItem>
#include <QListWidgetItem>

namespace Ui {
class StructurePage;
}

class StructurePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit StructurePage(QWidget *parent = 0);
    ~StructurePage();

protected:
    void initializePage();

private slots:
    void on_bookCountSpinBox_valueChanged(int value);

    void on_actCountSpinBox_valueChanged(int value);

    void on_chapterCountSpinBox_valueChanged(int value);

    void on_sceneCountSpinBox_valueChanged(int value);

    void on_bookTemplatesListWidget_itemActivated(QListWidgetItem *item);

private:
    Ui::StructurePage *ui;

    int m_bookCount, m_actCount, m_chapterCount, m_sceneCount;
    void resetPreviewTree();
    QTreeWidgetItem *createItem(QString type, QTreeWidgetItem *parentItem);
    void fillTemplateList();
};


#endif // STRUCTUREPAGE_H
