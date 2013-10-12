#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "spellchecker.h"
#include "importuserdictdialog.h"
#include "themes.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    styleInfoModified(false),
    spellLangIsModified(false),
    includeNamesFromTheMiseEnSceneIsModified(false)
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
    createThemeTab();

    ui->stylesTab->setEnabled(false);
    if(hub->isProjectOpened()){
        createStyleTab();
        ui->stylesTab->setEnabled(true);
    }

    createSpellingTab();

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



    langs << "Français" << "English" << "Italiano" << "Deutsch" << "Português (Brasil)" << "Español (España)"<< "Pусский";
    langCodes << "fr_FR" << "en_US" << "it_IT" << "de_DE" << "pt_BR" << "sp_SP" << "ru_RU";
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



void SettingsDialog::createThemeTab()
{
    // custom colors :


    prev_applyCustomColors = settings.value("Settings/applyCustomColors", true).toBool();


    settings.beginGroup("MainWindow");

    prev_sheetColor= settings.value("textZoneBackColor", "#ffffff").toString();
    prev_sheetTextColor = settings.value("textZoneTextColor", "#000000").toString();
    prev_backColor = settings.value("textTabBackColor", "#ffffff").toString();
    prev_treeBackColor = settings.value("projectTreeBackColor", "#dee4ea").toString();
    prev_treeTextColor = settings.value("projectTreeTextColor", "#000000").toString();
    prev_notesColor = settings.value("noteZoneBackColor", "#fff7d7").toString();
    prev_notesTextColor = settings.value("noteZoneTextColor", "#000000").toString();
    settings.endGroup();

    ui->sheetColorButton->setStyleSheet("background-color: " + prev_sheetColor );
    ui->sheetTextColorButton->setStyleSheet("background-color: "  + prev_sheetTextColor);
    ui->backColorButton->setStyleSheet("background-color: " + prev_backColor);
    ui->treeBackColorButton->setStyleSheet("background-color: " + prev_treeBackColor);
    ui->treeTextColorButton->setStyleSheet("background-color: " + prev_treeTextColor);
    ui->notesColorButton->setStyleSheet("background-color: " + prev_notesColor);
    ui->notesTextColorButton->setStyleSheet("background-color: " + prev_notesTextColor);


    connect(ui->sheetColorButton, SIGNAL(clicked()), this, SLOT(setColorDialog()));
    connect(ui->sheetTextColorButton, SIGNAL(clicked()), this, SLOT(setColorDialog()));
    connect(ui->backColorButton, SIGNAL(clicked()), this, SLOT(setColorDialog()));
    connect(ui->treeBackColorButton, SIGNAL(clicked()), this, SLOT(setColorDialog()));
    connect(ui->treeTextColorButton, SIGNAL(clicked()), this, SLOT(setColorDialog()));
    connect(ui->notesColorButton, SIGNAL(clicked()), this, SLOT(setColorDialog()));
    connect(ui->notesTextColorButton, SIGNAL(clicked()), this, SLOT(setColorDialog()));



    // theme list :


    prev_themePath = settings.value("Themes/theme", "").toString();
    this->resetThemeListWidget();



}

void SettingsDialog::setColorDialog()
{
    QString objName;
    QToolButton *button = qobject_cast<QToolButton *>(sender());
    if (button){
        objName = button->objectName();
    }else
        return;

    QColor color = QColorDialog::getColor(Qt::white, this, button->text(), QColorDialog::DontUseNativeDialog	);
    button->setStyleSheet("background-color: " + color.name() );

    //    QString string;
    //    QString r = string.setNum(backgroundColor.red(),10);
    //    QString g = string.setNum(backgroundColor.green(), 10);
    //    QString b = string.setNum(backgroundColor.blue(),10);

    setStyleSheet();
}
void SettingsDialog::setStyleSheet(){


    sheetColor= ui->sheetColorButton->styleSheet().remove("background-color: ");
    sheetTextColor = ui->sheetTextColorButton->styleSheet().remove("background-color: ");
    backColor = ui->backColorButton->styleSheet().remove("background-color: ");
    treeBackColor = ui->treeBackColorButton->styleSheet().remove("background-color: ");
    treeTextColor = ui->treeTextColorButton->styleSheet().remove("background-color: ");
    notesColor = ui->notesColorButton->styleSheet().remove("background-color: ");
    notesTextColor = ui->notesTextColorButton->styleSheet().remove("background-color: ");

    //    if(!ui->customColorsBox->isChecked()){

    //        sheetColor= prev_sheetColor;
    //        sheetTextColor = prev_sheetTextColor;
    //        backColor = prev_backColor;
    //        treeBackColor = prev_treeBackColor;
    //        treeTextColor = prev_treeTextColor;
    //        notesColor = prev_notesColor;
    //        notesTextColor = prev_notesTextColor;


    //    }



    settings.beginGroup("MainWindow");

    settings.setValue("textZoneBackColor", sheetColor);
    settings.setValue("textZoneTextColor", sheetTextColor);
    settings.setValue("textTabBackColor", backColor);
    settings.setValue("projectTreeBackColor", treeBackColor);
    settings.setValue("projectTreeTextColor",treeTextColor );
    settings.setValue("noteZoneBackColor", notesColor);
    settings.setValue("noteZoneTextColor", notesTextColor);


    settings.endGroup();


    emit applyStyleSheetSignal();

}


void SettingsDialog::on_customColorsBox_clicked(bool checked)
{

    settings.setValue("Settings/applyCustomColors", checked);
    setStyleSheet();

}

void SettingsDialog::on_resetColorsButton_clicked()
{
    ui->sheetColorButton->setStyleSheet("background-color: #ffffff");
    ui->sheetTextColorButton->setStyleSheet("background-color: #000000");
    ui->backColorButton->setStyleSheet("background-color: #ffffff");
    ui->treeBackColorButton->setStyleSheet("background-color: #dee4ea");
    ui->treeTextColorButton->setStyleSheet("background-color: #000000");
    ui->notesColorButton->setStyleSheet("background-color: #fff7d7");
    ui->notesTextColorButton->setStyleSheet("background-color: #000000");

    setStyleSheet();


}


//--------------------------------------------------------------

void SettingsDialog::resetThemeListWidget()
{
    disconnect(ui->themeListWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(themeListWidget_itemChanged(QListWidgetItem*)));

    // filling listWidget
ui->themeListWidget->clear();

    QHash<QString, QString> themesHash = Themes::themesList();


    QHash<QString, QString>::const_iterator i = themesHash.constBegin();
    while (i != themesHash.constEnd()) {


        QListWidgetItem *item = new QListWidgetItem(i.value(),ui->themeListWidget);
  QString path = i.key();
        item->setData(36, i.key());
        item->setCheckState(Qt::Unchecked);
        item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        ++i;


    }

    // check the currently used theme :

    QString currentThemePath = hub->themes()->currentTheme()->path();

    if(ui->themeListWidget->count() != 0)
        for(int i = 0; i < ui->themeListWidget->count(); ++i){

            QListWidgetItem *selectedItem = ui->themeListWidget->item(i);
            if(selectedItem->data(36).toString() == currentThemePath)
                selectedItem->setCheckState(Qt::Checked);
            else
                selectedItem->setCheckState(Qt::Unchecked);

        }

    connect(ui->themeListWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(themeListWidget_itemChanged(QListWidgetItem*)), Qt::UniqueConnection);
}


void SettingsDialog::themeListWidget_itemChanged(QListWidgetItem *item)
{
    Qt::CheckState state = static_cast<Qt::CheckState>(item->data(Qt::CheckStateRole).toInt());
    if(state == Qt::Checked) {
        hub->themes()->loadTheme(item->data(36).toString());
        settings.setValue("Themes/theme", item->data(36).toString());
    }
    else if(state == Qt::Unchecked) {
        hub->themes()->unloadTheme();
        settings.setValue("Themes/theme", "");
    }

    resetThemeListWidget();


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

void SettingsDialog::createSpellingTab()
{

    QHash<QString, QString> hash =  SpellChecker::dictsList();
    QHash<QString, QString>::iterator i = hash.begin();
    while (i != hash.end()) {
        QString dict = i.value();

        ui->spellCheckerComboBox->addItem(dict);


        ++i;
    }

    connect(ui->spellCheckerComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(spellCheckerComboBox_currentTextChanged()));





    // filling ui->dictInstallTipLabel :

    QString downloadLink = "http://extensions.services.openoffice.org/en/search?f[0]=field_project_tags%3A157";

#ifdef Q_OS_LINUX


    ui->dictInstallTipLabel->setText(tr(
                                         "<p>To install additional dictionaries, research \"hunspell\" "
                                         "in your package manager.</p>"
                                         "<p>On Ubuntu you can type in a console :</p>"
                                         "<p>sudo apt-get install hunspell*</p>"
                                         "<br>"
                                         "<p>Moreover, to install additional dictionaries, paste hunspell dictionaries in the \"/home/[user]/.plume-creator/dicts/\"  "
                                         "of the install directory.</p>"
                                         "<p>Hunspell dictionaries are in the form of pairs of *.aff and *.dic files.</p>"
                                         "<p>You can find these files unzipping dictionaries extensions from OpenOffice or LibreOffice. They are "
                                         "*.oxt files and you can find them here : </p>") +

                                     "<address><a href=" + downloadLink + ">" + downloadLink + "</a></address></center>"


                                         );


#endif
#ifdef Q_OS_WIN32
    ui->dictInstallTipLabel->setText(tr(
                                         "<p>To install additional dictionaries, paste hunspell dictionaries in the \"dicts\" folder "
                                         "of the install directory.</p>"
                                         "<p>Hunspell dictionaries are in the form of pairs of *.aff and *.dic files.</p>"
                                         "<p>You can find these files unzipping dictionaries extensions from OpenOffice or LibreOffice. They are "
                                         "*.oxt files and you can find them here : </p>") +

                                     "<address><a href=" + downloadLink + ">" + downloadLink + "</a></address></center>"


                                     );


#endif
#ifdef Q_OS_MAC
    ui->dictInstallTipLabel->setText(tr(
                                         "<p>To install additional dictionaries, research \"hunspell\" "
                                         "in your paquage manager.</p>"

                                         ));




#endif



    // filling listWidget

    QStringList userDictList = hub->userDict();
    QStringList attendTreeNamesList = hub->attendTree_namesList();


    foreach(QString word, userDictList){
        if(attendTreeNamesList.contains(word))
            continue;
        QListWidgetItem *item = new QListWidgetItem(word,ui->wordListWidget);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
    }


    connect(ui->wordListWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(itemDataChanged(QListWidgetItem*)));
}


void SettingsDialog::dictsChanged()
{


    QString dict = ui->spellCheckerComboBox->currentText();
    if(dict.isEmpty())
        return;



    dict =  SpellChecker::dictsList().key(dict);


    emit spellDictsChangedSignal(dict);



}

//---------------------------------------------------------------------------------

void SettingsDialog::spellCheckerComboBox_currentTextChanged()
{
    spellLangIsModified = true;
    //    qDebug() << "spellLangIsModified = true";
}
//---------------------------------------------------------------------------------

void SettingsDialog::on_includeNamesFromTheMiseEnSceneCheckBox_clicked()
{
    includeNamesFromTheMiseEnSceneIsModified = true;
}

//---------------------------------------------------------------------------------

void SettingsDialog::on_importWordsButton_clicked()
{
    ImportUserDictDialog *importuserdictdialog = new ImportUserDictDialog(this);

    QStringList importedList;
    if(importuserdictdialog->exec())
        importedList = importuserdictdialog->importedList();
    else
        return;


    // current word list:
    QStringList itemsNames;
    QList<QListWidgetItem *> itemsList = ui->wordListWidget->findItems("", Qt::MatchContains);
    foreach(QListWidgetItem *item , itemsList)
        itemsNames << item->text();


    itemsNames.append(importedList);
    ui->wordListWidget->clear();

    itemsNames.removeDuplicates();

    foreach(QString word, itemsNames){
        QListWidgetItem *item = new QListWidgetItem(word,ui->wordListWidget);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
    }
}


//---------------------------------------------------------------------------------

void SettingsDialog::on_renameWordButton_clicked()
{
    if(ui->wordListWidget->selectedItems().isEmpty())
        return;

    QListWidgetItem *item = ui->wordListWidget->selectedItems().first();
    ui->wordListWidget->editItem(item);

}
//---------------------------------------------------------------------------------

void SettingsDialog::on_removeWordButton_clicked()
{
    if(!ui->wordListWidget->selectedItems().isEmpty())
        ui->wordListWidget->takeItem(ui->wordListWidget->row(ui->wordListWidget->selectedItems().first()));
}
//---------------------------------------------------------------------------------

void SettingsDialog::on_addWordButton_clicked()
{
    QListWidgetItem *item = new QListWidgetItem(ui->wordListWidget);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);

    ui->wordListWidget->setItemSelected(item, true);
    ui->wordListWidget->scrollToItem(item);
    ui->wordListWidget->editItem(item);
}

void SettingsDialog::itemDataChanged(QListWidgetItem *item)
{
    QList<QListWidgetItem *> itemsList = ui->wordListWidget->findItems(item->text(), Qt::MatchCaseSensitive | Qt::MatchExactly);

    if(itemsList.size() > 1)
        ui->wordListWidget->removeItemWidget(itemsList.first());
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


    ui->useCommasInNumbersCheckBox->setChecked(settings.value("Settings/numberSymbolIsComma", false).toBool());

    // text tab :

    settings.beginGroup( "Settings" );





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

    // theme tab :

    ui->customColorsBox->setChecked(settings.value("Settings/applyCustomColors", true).toBool());


    // style tab :


    if(hub->isProjectOpened()){

        ui->styleListWidget->addItems(textStyles->namesList());
        ui->styleListWidget->setCurrentRow(0);
    }


    // spelling tab :
    QString spellLang = settings.value("SpellChecking/lang", "").toString();
#if QT_VERSION < 0x050000
    ui->spellCheckerComboBox->setCurrentIndex(ui->spellCheckerComboBox->findText(spellLang));
#endif
#if QT_VERSION >= 0x050000
    ui->spellCheckerComboBox->setCurrentText(spellLang);
#endif
    if(spellLang == "" && ui->spellCheckerComboBox->count() != 0){
        ui->spellCheckerComboBox->setCurrentIndex(0);
        spellLangIsModified = true;
    }
    ui->includeNamesFromTheMiseEnSceneCheckBox->setChecked(settings.value("SpellChecking/includeNamesFromTheMiseEnScene", true).toBool());




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
    settings.setValue("numberSymbolIsComma", ui->useCommasInNumbersCheckBox->isChecked());
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


    // theme tab :
    settings.setValue("Settings/applyCustomColors", ui->customColorsBox->isChecked());

    // style tab :

    if(hub->isProjectOpened()){
        textStyles->saveStyles();
        if(styleInfoModified){
            emit changeAllDocsTextStylesSignal();
        }
    }


    // spelling tab :


    settings.setValue("SpellChecking/lang", ui->spellCheckerComboBox->currentText());
    if(spellLangIsModified)
        this->dictsChanged();

    settings.setValue("SpellChecking/includeNamesFromTheMiseEnScene", ui->includeNamesFromTheMiseEnSceneCheckBox->isChecked());
    if(includeNamesFromTheMiseEnSceneIsModified)
        this->dictsChanged();

    QStringList userDict;
    QList<QListWidgetItem *> itemsList = ui->wordListWidget->findItems("", Qt::MatchContains);
    foreach(QListWidgetItem *item , itemsList)
        userDict.append(item->text().toUtf8());
    hub->setUserDict(userDict);



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

    // theme tab :

    on_customColorsBox_clicked(prev_applyCustomColors);

    settings.beginGroup("MainWindow");

    settings.setValue("textZoneBackColor", prev_sheetColor);
    settings.setValue("textZoneTextColor", prev_sheetTextColor);
    settings.setValue("textTabBackColor", prev_backColor);
    settings.setValue("projectTreeBackColor", prev_treeBackColor);
    settings.setValue("projectTreeTextColor",prev_treeTextColor );
    settings.setValue("noteZoneBackColor", prev_notesColor);
    settings.setValue("noteZoneTextColor", prev_notesTextColor);

    settings.endGroup();

    emit applyStyleSheetSignal();

    hub->themes()->loadTheme(prev_themePath);


    QDialog::reject();

}



