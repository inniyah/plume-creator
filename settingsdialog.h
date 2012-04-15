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

signals:

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

public slots:
    void accept();

private slots:
    void readSettings();
    void langChanged();
private:
    QSettings settings;

    QComboBox *langComboBox;
    QStringList langs;
    QStringList langCodes;

    QSpinBox *autosaveTimeSpin;
    int autosaveTime;
    QCheckBox *checkUpdateAtStartupCheckBox;
QCheckBox *preventDoubleSpaceCheckBox;

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
