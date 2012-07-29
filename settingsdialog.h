#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui>
//
QT_BEGIN_NAMESPACE
class GeneralSettingTab;
class TextSettingTab;
QT_END_NAMESPACE

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);

protected:
    void accept();
    void reject();
signals:
    void setDisplayModeSignal(QString mode);

public slots:

private:
    QDialogButtonBox *buttonBox;
    GeneralSettingTab *generalSettingTab;
    TextSettingTab *textSettingTab;
};


//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

class GeneralSettingTab : public QWidget
{
    Q_OBJECT
public:
    explicit GeneralSettingTab(QWidget *parent = 0);

signals:
    void autosaveTimeSignal(int autosaveTime);
    void setDisplayModeSignal(QString mode);

public slots:
    void accept();
    void reject();

private slots:
    void readSettings();
    void langChanged();
    void styleChanged();
    void displayModeChanged(int dispModeIndex);

private:
    QSettings settings;

    QComboBox *langComboBox;
    QStringList langs;
    QStringList langCodes;

    QComboBox *styleComboBox;
    QStringList styles;
    QStringList styleCodes;

    QComboBox *displayModeComboBox;
    QStringList displayModes;
    QStringList displayModeCodes;


    QSpinBox *autosaveTimeSpin;
    int autosaveTime;
    QCheckBox *checkUpdateAtStartupCheckBox, *preventDoubleSpaceCheckBox;

    QString displayMode, prev_displayMode;

};

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

class TextSettingTab : public QWidget
{
    Q_OBJECT
public:
    explicit TextSettingTab(QWidget *parent = 0);

signals:
    void textHeightSignal(int textSpinValue);
    void textFontSignal(QString textFont);

public slots:
    void accept();
    void reject();

private slots:
    void readSettings();
private:
    QSettings settings;
    QFontComboBox *textFontCombo;
    QFont textFont;
    QSpinBox *textSpin, *textIndentSpin, *textMarginSpin;
    int textSpinValue;
    QCheckBox *showScrollbarBox;
    int textIndentValue,
    textMarginValue;
    QSpinBox *widthSpin;
    int textWidthValue, previous_textWidthValue;

    QCheckBox *showFullScrollbarBox;

    QFontComboBox *synFontCombo;
    QFont synFont;
    QSpinBox *synSpin, *synIndentSpin, *synMarginSpin;
    int synSpinValue;
    QCheckBox *synShowScrollbarBox;
    int synIndentValue,
    synMarginValue;

    QFontComboBox *noteFontCombo;
    QFont noteFont;
    QSpinBox *noteSpin, *noteIndentSpin, *noteMarginSpin;
    int noteSpinValue;
    QCheckBox *noteShowScrollbarBox;
    int noteIndentValue,
    noteMarginValue;
};
#endif // SETTINGSDIALOG_H
