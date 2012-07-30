#include "editmenu.h"
//
EditMenu::EditMenu(QWidget *parent) :
    QFrame(parent), xMax(0)
{




    textFontCombo = new QFontComboBox;
    connect(textFontCombo, SIGNAL(currentFontChanged(QFont)), this, SIGNAL(textFontChangedSignal(QFont)));

    textSpin = new QSpinBox;
    textSpin->setRange(6,30);
    connect(textSpin, SIGNAL(valueChanged(int)), this, SIGNAL(textHeightChangedSignal(int)));


    QWidget *stretcher = new QWidget;
    stretcher->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);


    QGridLayout *baseGridLayout = new QGridLayout;

    baseGridLayout->addWidget(textFontCombo,2,0);
    baseGridLayout->addWidget(textSpin,3,0);
    baseGridLayout->addWidget(stretcher,4,0);
    setLayout(baseGridLayout);

    connect(textFontCombo, SIGNAL(currentFontChanged(QFont)), this, SIGNAL(textFontChangedSignal(QFont)));
    connect(textSpin, SIGNAL(valueChanged(int)), this, SIGNAL(textHeightChangedSignal(int)));

    applyStyleSheet();
    applyConfig();
}

//----------------------------------------------------------------------------------------
void EditMenu::charFormatChangedSlot(QTextCharFormat format)
{
    disconnect(textFontCombo, SIGNAL(currentFontChanged(QFont)), this, SIGNAL(textFontChangedSignal(QFont)));
    disconnect(textSpin, SIGNAL(valueChanged(int)), this, SIGNAL(textHeightChangedSignal(int)));


    textSpin->setValue(format.fontPointSize());
    textFontCombo->setCurrentFont(format.fontFamily());


    connect(textFontCombo, SIGNAL(currentFontChanged(QFont)), this, SIGNAL(textFontChangedSignal(QFont)));
    connect(textSpin, SIGNAL(valueChanged(int)), this, SIGNAL(textHeightChangedSignal(int)));

}

//----------------------------------------------------------------------------------------


void EditMenu::tabChangedSlot(QTextCharFormat newTabFormat)
{


    charFormatChangedSlot(newTabFormat);
}


//----------------------------------------------------------------------------------------










//---------------------------------------------------------------------------
//----------Apply Config---------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------


void EditMenu::applyConfig()
{
    QSettings settings;
    settings.beginGroup( "Settings" );
    textSpinValue = settings.value("TextArea/textHeight", 12).toInt();
    textFont.setFamily(settings.value("TextArea/textFontFamily", "Liberation Serif").toString());
    settings.endGroup();


    textSpin->setValue(textSpinValue);
    textFontCombo->setCurrentFont(textFont);

}

//---------------------------------------------------------------------------
void EditMenu::applyStyleSheet()
{
        this->setStyleSheet(
                            "QSpinBox::up-button{subcontrol-origin: padding; subcontrol-position: left top;}"
                            "QSpinBox::down-button{subcontrol-origin: padding; subcontrol-position: left bottom;}"
                            );
}
