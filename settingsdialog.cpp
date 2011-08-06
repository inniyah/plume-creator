#include <QtGui>

#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    QTabWidget *tabWidget = new QTabWidget;
    generalSettingTab = new GeneralSettingTab;
    textSettingTab = new TextSettingTab;
    tabWidget->addTab(generalSettingTab, tr("General"));
    tabWidget->addTab(textSettingTab, tr("Text"));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Tab Dialog"));
}

void SettingsDialog::accept()
{
    generalSettingTab->accept();
    textSettingTab->accept();
    QDialog::accept();
}


//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$



GeneralSettingTab::GeneralSettingTab(QWidget *parent)
    : QWidget(parent)
{
    readSettings();


    //->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    QGroupBox *generalBox = new QGroupBox(tr("Main Text Area :"));
    QHBoxLayout *generalBoxLayout = new QHBoxLayout;
    QLabel *autosaveLabel = new QLabel(tr("Save project every :"));
    autosaveTimeSpin = new QSpinBox;
    autosaveTimeSpin->setRange(10,3600);
    autosaveTimeSpin->setValue(autosaveTime / 1000);
    autosaveTimeSpin->setSingleStep(10);
    autosaveTimeSpin->setAccelerated(true);
    autosaveTimeSpin->setSuffix(tr(" s"));

    generalBoxLayout->addWidget(autosaveLabel);
    generalBoxLayout->addWidget(autosaveTimeSpin);
    generalBox->setLayout(generalBoxLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(generalBox);
    setLayout(mainLayout);


}

//---------------------------------------------------------------------------------

void GeneralSettingTab::readSettings()
{
    settings.beginGroup( "Settings" );
    autosaveTime = settings.value("autosaveTime", 20000).toInt();
    settings.endGroup();
}

//---------------------------------------------------------------------------------

void GeneralSettingTab::accept()
{
    settings.beginGroup( "Settings" );
    settings.setValue("autosaveTime", autosaveTimeSpin->value() * 1000);
    settings.endGroup();




}

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$



TextSettingTab::TextSettingTab(QWidget *parent)
    : QWidget(parent)
{
    readSettings();

    QGroupBox *textBox = new QGroupBox(tr("Main Text Area :"));
    QHBoxLayout *hTextLayout = new QHBoxLayout;
    QLabel *textFontLabel = new QLabel(tr("Default Text Font :"));
    textFontCombo = new QFontComboBox;
    textFontCombo->setCurrentFont(textFont);
    textSpin = new QSpinBox;
    textSpin->setRange(6,30);
    textSpin->setValue(textSpinValue);   
    QCheckBox *textApplyWideBox = new QCheckBox(tr("Apply project wide"));

    hTextLayout->addWidget(textFontLabel);
    hTextLayout->addStretch(20);
    hTextLayout->addWidget(textFontCombo);
    hTextLayout->addWidget(textSpin);
    hTextLayout->addWidget(textApplyWideBox);
    textBox->setLayout(hTextLayout);







    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(textBox);
    setLayout(mainLayout);


}

//---------------------------------------------------------------------------------

void TextSettingTab::readSettings()
{
    settings.beginGroup( "Settings" );
    textSpinValue = settings.value("TextArea/textHeight", 12).toInt();
    textFont.setFamily(settings.value("TextArea/textFontFamily", "Liberation Serif").toString());
    settings.endGroup();
}

//---------------------------------------------------------------------------------

void TextSettingTab::accept()
{
    settings.beginGroup( "Settings" );
    settings.setValue("TextArea/textHeight", textSpin->value());
    settings.setValue("TextArea/textFontFamily", textFontCombo->currentFont());
    settings.endGroup();

 //   qDebug()<< "textFont" << textFont.family();







}
