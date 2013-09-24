#ifndef IMPORTUSERDICTDIALOG_H
#define IMPORTUSERDICTDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class ImportUserDictDialog;
}

class ImportUserDictDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImportUserDictDialog(QWidget *parent = 0);
    ~ImportUserDictDialog();

    // result :
    QStringList importedList();

private slots:
    void populateWordsList(QListWidgetItem *projectItem);

    void on_browseButton_clicked();

private:
    Ui::ImportUserDictDialog *ui;

    QStringList m_importedList;
    QHash<QString, QString> projectsListHash;
};

#endif // IMPORTUSERDICTDIALOG_H
