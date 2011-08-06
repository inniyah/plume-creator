#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui>

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

private:
QSettings settings;
QSpinBox *autosaveTimeSpin;
int autosaveTime;


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
    QSpinBox *textSpin;
int textSpinValue;

};
#endif // SETTINGSDIALOG_H
