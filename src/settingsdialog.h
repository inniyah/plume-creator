/***************************************************************************
 *   Copyright (C) 2012 by Cyril Jacquet                                   *
 *   cyril.jacquet@plume-creator.eu                                                 *
 *                                                                         *
 *  This file is part of Plume Creator.                                    *
 *                                                                         *
 *  Plume Creator is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  Plume Creator is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with Plume Creator.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QDialog>
#include <QSettings>


#include <QNetworkProxy>

#include "hub.h"
#include "textstyles.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    void createContent();
    void setCurrentTab(int index);

protected:
    void accept();
    void reject();

signals:
    // general tab :
    void autosaveTimeSignal(int autosaveTime);
    void setDisplayModeSignal(QString mode, bool isToolBarInStatusBar);

    // text tab :

    // style tab :
    void changeAllDocsTextStylesSignal();

    // spell tab :
    void spellDictsChangedSignal(const QString &dictionaryPath);

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void setTextStyles(TextStyles *styles){textStyles = styles;}


private slots:
    void createGeneralTab();
    void createTextTab();
    void createStyleTab();
    void createSpellingTab();

    void readSettings();

    // general tab :

    void langChanged();
    void programStyleChanged();
    void displayModeChanged(int dispModeIndex);
    void portableModeChanged(bool mode);
    void toolBarInToolBarChanged(bool isToolBarInStatusBar);

    // text tab :
    void setStyleInfos(int index);

    // style tab :
    void infoModified();
    void addStyle();
    void removeStyle();
    void renameStyle();

    //spelling tab :
    void dictsChanged();

    void spellCheckerComboBox_currentTextChanged();

    void on_importWordsButton_clicked();

    void on_renameWordButton_clicked();

    void on_removeWordButton_clicked();

    void on_addWordButton_clicked();

void itemDataChanged(QListWidgetItem *item);
private:
    Hub *hub;
    Ui::SettingsDialog *ui;

    QSettings settings;

    // general tab :

    QStringList langs;
    QStringList langCodes;

    QStringList styles;
    QStringList styleCodes;

    QStringList displayModes;
    QStringList displayModeCodes;

    int autosaveTime;

    QString displayMode, prev_displayMode;
    bool previousIsPortable;
bool prevIsToolBarInStatusBar;
    // text tab :

    int textWidthValue, previous_textWidthValue;


    QFont synFont;
    int synSpinValue;
    int synIndentValue, synMarginValue;

    QFont noteFont;
    int noteSpinValue;
    int noteIndentValue, noteMarginValue;

    // style tab :

    TextStyles *textStyles;
    bool styleInfoModified;
    int currentStyleIndex;

    // spelling Tab :
bool spellLangIsModified;
QStringList userDictStringList;

};

#endif // SETTINGSDIALOG_H
