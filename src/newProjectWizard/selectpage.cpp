#include "selectpage.h"
#include "ui_selectpage.h"

SelectPage::SelectPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::SelectPage)
{
    ui->setupUi(this);


    ui->projectNameLabelLineEdit->setValidator(new QRegExpValidator(QRegExp("[^\x002F\\\\:\x002A\?\x0022<>|]+"), ui->projectNameLabelLineEdit));
    ui->directoryLabelLineEdit->setText(QDir::toNativeSeparators(QDir::homePath()));


    this->registerField("projectNameField*", ui->projectNameLabelLineEdit);
     this->registerField("projectDirectoryField*", ui->directoryLabelLineEdit);


}

SelectPage::~SelectPage()
{
    delete ui;
}
void SelectPage::on_directoryButton_clicked()
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    QString directory = QFileDialog::getExistingDirectory(this, tr("QFileDialog::getExistingDirectory()"),
                                                          ui->directoryLabelLineEdit->text(),
                                                          options);
    if (!directory.isEmpty())
        ui->directoryLabelLineEdit->setText(directory);
}

