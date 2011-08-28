#include "editmenubox.h"

EditMenuBox::EditMenuBox(QWidget *parent) :
    QFrame(parent), xMax(0)
{
    QToolButton *button = new QToolButton;

    textFontCombo = new QFontComboBox;
    connect(textFontCombo, SIGNAL(currentFontChanged(QFont)), this, SIGNAL(textFontChangedSignal(QFont)));

            textSpin = new QSpinBox;
    textSpin->setRange(6,30);
    connect(textSpin, SIGNAL(valueChanged(int)), this, SIGNAL(textHeightChangedSignal(int)));

    QLabel *textZoneWidthLabel = new QLabel(tr("Text Area Width :"));
    widthSlider = new QSlider(Qt::Horizontal);
QWidget *stretcher = new QWidget;
stretcher->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);


    QGridLayout *baseGridLayout = new QGridLayout;

    baseGridLayout->addWidget(button,0,0);
    baseGridLayout->addWidget(textFontCombo,2,0);
    baseGridLayout->addWidget(textSpin,3,0);
    baseGridLayout->addWidget(textZoneWidthLabel,4,0);
    baseGridLayout->addWidget(widthSlider,5,0);
baseGridLayout->addWidget(stretcher,6,0);
    setLayout(baseGridLayout);

    connect(textFontCombo, SIGNAL(currentFontChanged(QFont)), this, SIGNAL(textFontChangedSignal(QFont)));
   connect(textSpin, SIGNAL(valueChanged(int)), this, SIGNAL(textHeightChangedSignal(int)));

    applyConfig();
}

//----------------------------------------------------------------------------------------
 void EditMenuBox::charFormatChangedSlot(QTextCharFormat format)
 {
     disconnect(textFontCombo, SIGNAL(currentFontChanged(QFont)), this, SIGNAL(textFontChangedSignal(QFont)));
     disconnect(textSpin, SIGNAL(valueChanged(int)), this, SIGNAL(textHeightChangedSignal(int)));


     textSpin->setValue(format.fontPointSize());
     textFontCombo->setCurrentFont(format.fontFamily());


     connect(textFontCombo, SIGNAL(currentFontChanged(QFont)), this, SIGNAL(textFontChangedSignal(QFont)));
    connect(textSpin, SIGNAL(valueChanged(int)), this, SIGNAL(textHeightChangedSignal(int)));

 }

 //----------------------------------------------------------------------------------------

void EditMenuBox::loadSliderValue()
{
int sliderValue = settings.value("TextArea/areaWidth", 400).toInt();
    widthSlider->setSliderPosition(sliderValue);
    widthSlider->setValue(sliderValue);

QString debug;
    qDebug() << "loadSliderValue : " << debug.setNum(sliderValue);
    sliderValueChanged(sliderValue);
    connect(widthSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)), Qt::UniqueConnection);

}

//----------------------------------------------------------------------------------------

void EditMenuBox::sliderValueChanged(int value)
{
    sliderValue = value;
    settings.setValue( "TextArea/areaWidth", sliderValue);
    emit widthChangedSignal(sliderValue);
}

//----------------------------------------------------------------------------------------

void EditMenuBox::tabWitdhChangedSlot(int value)
{
    int oldXMax = 0;
    xMax = value;

    if(xMax < widthSlider->value() && widthSlider->value() < oldXMax)
        widthSlider->setValue(xMax);

    widthSlider->setRange(200, xMax);
oldXMax = xMax;
}

void EditMenuBox::tabChangedSlot(QTextCharFormat newTabFormat)
{


charFormatChangedSlot(newTabFormat);
}
//---------------------------------------------------------------------------
//----------Apply Config---------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------


void EditMenuBox::applyConfig()
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
