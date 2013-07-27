#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    styleInfoModified(false)
{
    ui->setupUi(this);

    ui->isPortableToBeCreated->hide();
    ui->isPortableToBeCreatedLabel->hide();
#ifdef Q_OS_WIN32
    ui->isPortableToBeCreated->hide();
    ui->isPortableToBeCreatedLabel->hide();
#endif
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}
//---------------------------------------------------------------------------------
void SettingsDialog::createContent()
{
    createGeneralTab();
    createTextTab();

    ui->stylesTab->setEnabled(false);
    if(hub->isProjectOpened()){
        createStyleTab();
        ui->stylesTab->setEnabled(true);
    }

    readSettings();
}
//---------------------------------------------------------------------------------
void SettingsDialog::setCurrentTab(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

void SettingsDialog::createGeneralTab()
{



    langs << "Français" << "English" << "Italiano" << "Deutsch" << "Português (Brasil)";
    langCodes << "fr_FR" << "en_US" << "it_IT" << "de_DE" << "pt_BR";
    ui->langComboBox->addItems(langs);


    styles <<  tr("System default") << tr("Plastique (KDE)") << tr("Cleanlooks");
#ifdef Q_OS_LINUX
    styles << tr("Gtk (Gnome/XFCE)");
#endif
#ifdef Q_OS_WIN32
    styles  << tr("Windows Vista") << tr("Windows XP");
#endif
#ifdef Q_OS_MAC
    styles << tr("Macintosh (OSX)");
#endif
    styleCodes << "default" << "plastique" << "cleanlooks";
#ifdef Q_OS_LINUX
    styleCodes << "gtk";
#endif
#ifdef Q_OS_WIN32
    styleCodes << "vista" << "xp";
#endif
#ifdef Q_OS_MAC
    styleCodes << "osx";
#endif
    ui->styleComboBox->addItems(styles);




    displayModes << tr("Desktop") << tr("Netbook");
    displayModeCodes << "desktop" << "netbook";
    ui->displayModeComboBox->addItems(displayModes);



}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

void SettingsDialog::langChanged()
{
    QMessageBox msgBox;
    msgBox.setText(tr("A different language has been selected.<br>The change will be effective after restarting the program."));
    msgBox.exec();
}

//---------------------------------------------------------------------------------

void SettingsDialog::programStyleChanged()
{
    QMessageBox msgBox;
    msgBox.setText(tr("A different style has been selected.<br>The change will be effective after restarting the program."));
    msgBox.exec();
}

//---------------------------------------------------------------------------------

void SettingsDialog::displayModeChanged(int dispModeIndex)
{

    emit setDisplayModeSignal(displayModeCodes.at(dispModeIndex), ui->isToolBarInStatusBarCheckBox->isChecked());

}

//---------------------------------------------------------------------------------
void SettingsDialog::toolBarInToolBarChanged(bool isToolBarInStatusBar)
{
    emit setDisplayModeSignal(displayModeCodes.at(ui->displayModeComboBox->currentIndex()), isToolBarInStatusBar);
}
    //---------------------------------------------------------------------------------

void SettingsDialog::portableModeChanged(bool mode)
{

    QMessageBox msgBox;
    msgBox.setText(tr("A different portable mode has been selected.<br>The change will be effective after restarting the program."));
    msgBox.exec();

}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

void SettingsDialog::createTextTab()
{


    // Synopsis :


    ui->synFontCombo->setCurrentFont(synFont);


    // Note :


    ui->noteFontCombo->setCurrentFont(noteFont);



}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

void SettingsDialog::createStyleTab()
{

    connect(ui->styleListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(setStyleInfos(int)));
    connect(ui->addStyleButton, SIGNAL(clicked()), this, SLOT(addStyle()));
    connect(ui->removeStyleButton, SIGNAL(clicked()), this, SLOT(removeStyle()));
    connect(ui->renameStyleButton, SIGNAL(clicked()), this, SLOT(renameStyle()));


    connect(ui->styleFontCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(infoModified()));
    connect(ui->styleSizeSpin, SIGNAL(valueChanged(int)), this, SLOT(infoModified()));
    connect(ui->styleFirstLineIndentSpin, SIGNAL(valueChanged(int)), this, SLOT(infoModified()));
    connect(ui->styleBottomMarginSpin, SIGNAL(valueChanged(int)), this, SLOT(infoModified()));
    connect(ui->styleLeftMarginSpin, SIGNAL(valueChanged(int)), this, SLOT(infoModified()));
    connect(ui->styleAlignComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(infoModified()));
    connect(ui->styleItalicCheckBox , SIGNAL(toggled(bool)), this, SLOT(infoModified()));
    connect(ui->styleBoldCheckBox, SIGNAL(toggled(bool)), this, SLOT(infoModified()));
    connect(ui->styleUnderlineCheckBox, SIGNAL(toggled(bool)), this, SLOT(infoModified()));
    connect(ui->styleStrikeCheckBox , SIGNAL(toggled(bool)), this, SLOT(infoModified()));
    connect(ui->defaultStyleCheckBox , SIGNAL(toggled(bool)), this, SLOT(infoModified()));
}

//---------------------------------------------------------------------------------

void SettingsDialog::setStyleInfos(int index)
{

    //    disconnect :

    disconnect(ui->styleFontCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(infoModified()));
    disconnect(ui->styleSizeSpin, SIGNAL(valueChanged(int)), this, SLOT(infoModified()));
    disconnect(ui->styleFirstLineIndentSpin, SIGNAL(valueChanged(int)), this, SLOT(infoModified()));
    disconnect(ui->styleBottomMarginSpin, SIGNAL(valueChanged(int)), this, SLOT(infoModified()));
    disconnect(ui->styleLeftMarginSpin, SIGNAL(valueChanged(int)), this, SLOT(infoModified()));
    disconnect(ui->styleAlignComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(infoModified()));
    disconnect(ui->styleItalicCheckBox , SIGNAL(toggled(bool)), this, SLOT(infoModified()));
    disconnect(ui->styleBoldCheckBox, SIGNAL(toggled(bool)), this, SLOT(infoModified()));
    disconnect(ui->styleUnderlineCheckBox, SIGNAL(toggled(bool)), this, SLOT(infoModified()));
    disconnect(ui->styleStrikeCheckBox , SIGNAL(toggled(bool)), this, SLOT(infoModified()));






    //    set Style Infos :

    if(index != -1){ // if list isn't empty

        currentStyleIndex = index;



        ui->styleFontCombo->setCurrentFont(QFont(textStyles->fontFamilyAt(index)));
        ui->styleSizeSpin->setValue(textStyles->fontSizeAt(index));
        ui->styleFirstLineIndentSpin->setValue(textStyles->blockFirstLineIndentAt(index));
        ui->styleBottomMarginSpin->setValue(textStyles->blockBottomMarginAt(index));
        ui->styleLeftMarginSpin->setValue(textStyles->blockLeftMarginAt(index));
        ui->styleAlignComboBox->setCurrentIndex(textStyles->blockAlignmentAt(index));
        ui->styleItalicCheckBox->setChecked(textStyles->fontItalicAt(index));
        ui->styleBoldCheckBox->setChecked(textStyles->fontBoldAt(index));
        ui->styleUnderlineCheckBox->setChecked(textStyles->fontUnderlineAt(index));
        ui->styleStrikeCheckBox->setChecked(textStyles->fontStrikeOutAt(index));
        ui->defaultStyleCheckBox->setChecked(textStyles->isDefaultStyle(index));
    }

    //    reconnect :

    connect(ui->styleFontCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(infoModified()));
    connect(ui->styleSizeSpin, SIGNAL(valueChanged(int)), this, SLOT(infoModified()));
    connect(ui->styleFirstLineIndentSpin, SIGNAL(valueChanged(int)), this, SLOT(infoModified()));
    connect(ui->styleBottomMarginSpin, SIGNAL(valueChanged(int)), this, SLOT(infoModified()));
    connect(ui->styleLeftMarginSpin, SIGNAL(valueChanged(int)), this, SLOT(infoModified()));
    connect(ui->styleAlignComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(infoModified()));
    connect(ui->styleItalicCheckBox , SIGNAL(toggled(bool)), this, SLOT(infoModified()));
    connect(ui->styleBoldCheckBox, SIGNAL(toggled(bool)), this, SLOT(infoModified()));
    connect(ui->styleUnderlineCheckBox, SIGNAL(toggled(bool)), this, SLOT(infoModified()));
    connect(ui->styleStrikeCheckBox , SIGNAL(toggled(bool)), this, SLOT(infoModified()));

}

//---------------------------------------------------------------------------------

void SettingsDialog::infoModified()
{
    styleInfoModified = true;

    int index = currentStyleIndex;

    textStyles->setFontFamilyAt(index, ui->styleFontCombo->currentFont().toString());
    textStyles->setFontSizeAt(index, ui->styleSizeSpin->value());
    textStyles->setBlockFirstLineIndentAt(index, ui->styleFirstLineIndentSpin->value());
    textStyles->setBlockBottomMarginAt(index, ui->styleBottomMarginSpin->value());
    textStyles->setBlockLeftMarginAt(index, ui->styleLeftMarginSpin->value());
    textStyles->setBlockAlignmentAt(index, ui->styleAlignComboBox->currentIndex());
    textStyles->setFontItalicAt(index, ui->styleItalicCheckBox->isChecked());
    textStyles->setFontBoldAt(index, ui->styleBoldCheckBox->isChecked());
    textStyles->setFontUnderlineAt(index, ui->styleUnderlineCheckBox->isChecked());
    textStyles->setFontStrikeOutAt(index, ui->styleStrikeCheckBox->isChecked());

    if(ui->defaultStyleCheckBox->isChecked())
        textStyles->setDefaultStyle(index);
}

//---------------------------------------------------------------------------------
void SettingsDialog::addStyle()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Name new style"),
                                         tr("Style name :"), QLineEdit::Normal,
                                         tr("New Style"), &ok);
    if (ok && !text.isEmpty()){
        textStyles->addDefaultStyle(text);
        ui->styleListWidget->addItem(text);
    }
}
//---------------------------------------------------------------------------------
void SettingsDialog::renameStyle()
{

    bool ok;
    QString text = QInputDialog::getText(this, tr("Style Renaming"),
                                         tr("Style new name :"), QLineEdit::Normal,
                                         textStyles->nameAt(currentStyleIndex), &ok);
    if (ok && !text.isEmpty()){
        textStyles->renameStyle(currentStyleIndex, text);
        ui->styleListWidget->item(currentStyleIndex)->setText(text);
    }

}

//---------------------------------------------------------------------------------

void SettingsDialog::removeStyle()
{
    QMessageBox msgBox(this);
    msgBox.setText(tr("Do you want to remove this style ?"));
    msgBox.setInformativeText(tr("The texts written with this style will not be linked anymore."));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    int index;

    switch (ret){
    case QMessageBox::Ok:

        index = currentStyleIndex;
        ui->styleListWidget->takeItem(index);

        textStyles->removeStyleAt(index);

        break;

    case QMessageBox::Cancel:

        return;

        break;
    default:
        // should never be reached
        break;
    }
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

void SettingsDialog::readSettings()
{
    // general tab :

    settings.beginGroup("MainWindow");
    ui->langComboBox->setCurrentIndex(langCodes.indexOf(settings.value("lang", "en_US").toString()));
    ui->styleComboBox->setCurrentIndex(styleCodes.indexOf(settings.value("style", "default").toString()));

    displayMode = settings.value("displayMode", "desktop").toString();
    prev_displayMode = displayMode;
    ui->displayModeComboBox->setCurrentIndex(displayModeCodes.indexOf(displayMode));
#ifdef Q_OS_WIN32
    ui->isPortableToBeCreated->setChecked(settings.value("isPortable", false).toBool());
    previousIsPortable = settings.value("isPortable", false).toBool();
#endif
    settings.endGroup();
    settings.beginGroup( "Settings" );
    autosaveTime = settings.value("autosaveTime", 20000).toInt();
    ui->autosaveTimeSpinBox->setValue(autosaveTime / 1000);
    ui->preventDoubleSpaceCheckBox->setChecked(settings.value("preventDoubleSpace", false).toBool());
    ui->oneTabOnlyCheckBox->setChecked(settings.value("oneTabOnly", false).toBool());
    ui->noTabCheckBox->setChecked(settings.value("TextArea/noTab", false).toBool());
    settings.endGroup();
    settings.beginGroup("Updater");
    ui->checkUpdateAtStartupCheckBox->setChecked(settings.value("checkAtStartup_1", true).toBool());
    settings.endGroup();
    settings.beginGroup("MainWindow");
    ui->isToolBarInStatusBarCheckBox->setChecked(settings.value("isToolBarInStatusBar", false).toBool());
    prevIsToolBarInStatusBar = ui->isToolBarInStatusBarCheckBox->isChecked();
    settings.endGroup();

    connect(ui->langComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(langChanged()), Qt::UniqueConnection);
    connect(ui->styleComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(programStyleChanged()), Qt::UniqueConnection);
    connect(ui->displayModeComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(displayModeChanged(int)), Qt::UniqueConnection);
    connect(ui->isToolBarInStatusBarCheckBox,SIGNAL(toggled(bool)), this, SLOT(toolBarInToolBarChanged(bool)), Qt::UniqueConnection);
    connect(ui->isPortableToBeCreated,SIGNAL(toggled(bool)),this,SLOT(portableModeChanged(bool)));


    // text tab :

    settings.beginGroup( "Settings" );

    textWidthValue = settings.value("TextArea/textWidth", 700).toInt();
    previous_textWidthValue = textWidthValue;
    ui->widthSpin->setValue(textWidthValue);


    // Fullscreen text :

    ui->showFullScrollbarBox->setChecked(settings.value("FullTextArea/showScrollbar", false).toBool());

    // Synopsis :

    synSpinValue = settings.value("SynArea/textHeight", 12).toInt();
    synFont.setFamily(settings.value("SynArea/textFontFamily", "Liberation Serif").toString());
    ui->synShowScrollbarBox->setChecked(settings.value("SynArea/showScrollbar", true).toBool());
    synIndentValue = settings.value("SynArea/textIndent", 20).toInt();
    synMarginValue = settings.value("SynArea/bottomMargin", 10).toInt();

    ui->synSpin->setValue(synSpinValue);
    ui->synFontCombo->setCurrentFont(synFont);
    ui->synIndentSpin->setValue(synIndentValue);
    ui->synMarginSpin->setValue(synMarginValue);

    // Note :

    noteSpinValue = settings.value("NoteArea/textHeight", 12).toInt();
    noteFont.setFamily(settings.value("NoteArea/textFontFamily", "Liberation Serif").toString());
    ui->noteShowScrollbarBox->setChecked(settings.value("NoteArea/showScrollbar", true).toBool());
    noteIndentValue = settings.value("NoteArea/textIndent", 20).toInt();
    noteMarginValue = settings.value("NoteArea/bottomMargin", 10).toInt();

    ui->noteSpin->setValue(noteSpinValue);
    ui->noteFontCombo->setCurrentFont(noteFont);
    ui->noteIndentSpin->setValue(noteIndentValue);
    ui->noteMarginSpin->setValue(noteMarginValue);

    settings.endGroup();





    // style tab :


    if(hub->isProjectOpened()){

        ui->styleListWidget->addItems(textStyles->namesList());
        ui->styleListWidget->setCurrentRow(0);
    }
    // proxy tab :

    settings.beginGroup("Updater");
    ui->proxyBox->setChecked(settings.value("Proxy/proxyEnabled", false).toBool());
    ui->proxySystemSettingsCheckBox->setChecked(settings.value("Proxy/proxySystemEnabled", true).toBool());

    QString type = settings.value("Proxy/proxyType", "http").toString();
    if(type == "http")
        ui->proxyTypeComboBox->setCurrentIndex(0);
    else if(type == "socks5")
        ui->proxyTypeComboBox->setCurrentIndex(1);
    else
        ui->proxyTypeComboBox->setCurrentIndex(0);
    ui->proxyHostNameLineEdit->setText(settings.value("Proxy/proxyHostName", "").toString());
    ui->proxyPortSpinBox->setValue(settings.value("Proxy/proxyPort", 1080).toInt());
    ui->proxyUserNameLineEdit->setText(settings.value("Proxy/proxyUserName", "").toString());
    ui->proxyPasswordLineEdit->setText(settings.value("Proxy/proxyPassword", "").toString());
    settings.endGroup();




}


//---------------------------------------------------------------------------------

void SettingsDialog::accept()
{
    //    general tab :

    settings.beginGroup("MainWindow");
    settings.setValue("lang", langCodes.at(langs.indexOf(ui->langComboBox->currentText())));
    settings.setValue("style", styleCodes.at(styles.indexOf(ui->styleComboBox->currentText())));
    settings.setValue("displayMode", displayModeCodes.at(displayModes.indexOf(ui->displayModeComboBox->currentText())));

    settings.setValue("isPortable", ui->isPortableToBeCreated->isChecked());
    if(ui->isPortableToBeCreated->isChecked() && !previousIsPortable)
        settings.setValue("isPortableToBeCreated", true);
    else if(previousIsPortable)
        settings.setValue("isPortableToBeUnset", true);

    settings.endGroup();

    settings.beginGroup( "Settings" );
    settings.setValue("autosaveTime", ui->autosaveTimeSpinBox->value() * 1000);
    settings.setValue("preventDoubleSpace", ui->preventDoubleSpaceCheckBox->isChecked());
    settings.setValue("oneTabOnly", ui->oneTabOnlyCheckBox->isChecked());
    settings.setValue("TextArea/noTab", ui->noTabCheckBox->isChecked());
    settings.endGroup();
    settings.beginGroup("Updater");
    settings.setValue("checkAtStartup_1", ui->checkUpdateAtStartupCheckBox->isChecked());
    settings.endGroup();
    settings.beginGroup("MainWindow");
    settings.setValue("isToolBarInStatusBar", ui->isToolBarInStatusBarCheckBox->isChecked());
    settings.endGroup();



    // text tab :


    settings.beginGroup( "Settings" );

    settings.setValue( "TextArea/textWidth", ui->widthSpin->value());



    settings.setValue("FullTextArea/showScrollbar", ui->showFullScrollbarBox->isChecked());


    // Synopsis :

    settings.setValue("SynArea/textHeight", ui->synSpin->value());
    settings.setValue("SynArea/textFontFamily", ui->synFontCombo->currentFont());
    settings.setValue("SynArea/showScrollbar", ui->synShowScrollbarBox->isChecked());
    settings.setValue("SynArea/textIndent", ui->synIndentSpin->value());
    settings.setValue("SynArea/bottomMargin", ui->synMarginSpin->value());

    // Note :

    settings.setValue("NoteArea/textHeight", ui->noteSpin->value());
    settings.setValue("NoteArea/textFontFamily", ui->noteFontCombo->currentFont());
    settings.setValue("NoteArea/showScrollbar", ui->noteShowScrollbarBox->isChecked());
    settings.setValue("NoteArea/textIndent", ui->noteIndentSpin->value());
    settings.setValue("NoteArea/bottomMargin", ui->noteMarginSpin->value());

    settings.endGroup();



    // style tab :

    if(hub->isProjectOpened()){
        textStyles->saveStyles();
        if(styleInfoModified){
            emit changeAllDocsTextStylesSignal();
        }
    }
    // proxy tab :

    settings.beginGroup("Updater");
    settings.setValue("Proxy/proxyEnabled", ui->proxyBox->isChecked());
    settings.setValue("Proxy/proxySystemEnabled", ui->proxySystemSettingsCheckBox->isChecked());

    if(ui->proxyTypeComboBox->currentIndex() == 0)
        settings.setValue("Proxy/proxyType", "http");
    if(ui->proxyTypeComboBox->currentIndex() == 1)
        settings.setValue("Proxy/proxyType", "socks5");
    settings.setValue("Proxy/proxyHostName", ui->proxyHostNameLineEdit->text());
    settings.setValue("Proxy/proxyPort", ui->proxyPortSpinBox->value());
    settings.setValue("Proxy/proxyUserName", ui->proxyUserNameLineEdit->text());
    settings.setValue("Proxy/proxyPassword", ui->proxyPasswordLineEdit->text());
    settings.endGroup();



    if(ui->proxyBox->isChecked() && !ui->proxySystemSettingsCheckBox->isChecked()){
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::Socks5Proxy);
        proxy.setHostName(ui->proxyHostNameLineEdit->text());
        proxy.setPort(ui->proxyPortSpinBox->value());
        proxy.setUser(ui->proxyUserNameLineEdit->text());
        proxy.setPassword(ui->proxyPasswordLineEdit->text());
        QNetworkProxy::setApplicationProxy(proxy);
    }




    QDialog::accept();
}
//---------------------------------------------------------------------------------

void SettingsDialog::reject()
{

    //    general tab :

    emit setDisplayModeSignal(prev_displayMode, prevIsToolBarInStatusBar);

    // text tab :
    ui->widthSpin->setValue(previous_textWidthValue);


    QDialog::reject();

}
