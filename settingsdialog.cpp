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

    QGroupBox *textBox = new QGroupBox(tr("Main Text Area :"));
    QGridLayout *gridTextLayout = new QGridLayout;
    QLabel *textFontLabel = new QLabel(tr("Default Text Font :"));
    textFontCombo = new QFontComboBox;
    textFontCombo->setCurrentFont(textFont);
    textSpin = new QSpinBox;
    textSpin->setRange(6,30);
    textSpin->setValue(textSpinValue);   
    QCheckBox *textApplyWideBox = new QCheckBox(tr("Apply project wide"));

showScrollbarBox = new QCheckBox(tr("Show scrollbar"));




    gridTextLayout->addWidget(textFontLabel,0,0);
    gridTextLayout->addWidget(textFontCombo,0,1);
    gridTextLayout->addWidget(textSpin,0,2);
    gridTextLayout->addWidget(textApplyWideBox,0,3);
     gridTextLayout->addWidget(showScrollbarBox,1,0);
    textBox->setLayout(gridTextLayout);







    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(textBox);
    setLayout(mainLayout);

    readSettings();

}

//---------------------------------------------------------------------------------

void TextSettingTab::readSettings()
{
    settings.beginGroup( "Settings" );
    textSpinValue = settings.value("TextArea/textHeight", 12).toInt();
textFont.setFamily(settings.value("TextArea/textFontFamily", "Liberation Serif").toString());
showScrollbarBox->setChecked(settings.value("TextArea/showScrollbar", true).toBool());
settings.endGroup();


textSpin->setValue(textSpinValue);
textFontCombo->setCurrentFont(textFont);
}

//---------------------------------------------------------------------------------

void TextSettingTab::accept()
{
    settings.beginGroup( "Settings" );
    settings.setValue("TextArea/textHeight", textSpin->value());
    settings.setValue("TextArea/textFontFamily", textFontCombo->currentFont());
    settings.setValue("TextArea/showScrollbar", showScrollbarBox->isChecked());
    settings.endGroup();

 //   qDebug()<< "textFont" << textFont.family();







}
