#include "editmenubox.h"

EditMenuBox::EditMenuBox(QWidget *parent) :
    QFrame(parent), xMax(0)
{
    QToolButton *button = new QToolButton;



    QLabel *textZoneWidthLabel = new QLabel(tr("Text Area Width :"));
    widthSlider = new QSlider(Qt::Horizontal);



    QGridLayout *baseGridLayout = new QGridLayout;

    baseGridLayout->addWidget(button,0,0);
    baseGridLayout->addWidget(textZoneWidthLabel,4,0);
    baseGridLayout->addWidget(widthSlider,5,0);

    setLayout(baseGridLayout);



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


