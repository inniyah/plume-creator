#include "editmenu.h"
#include "ui_editmenu.h"

#include <QtGui>

EditMenu::EditMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditMenu)
{
    ui->setupUi(this);


}

EditMenu::~EditMenu()
{
    delete ui;
}
//----------------------------------------------------------------------------------------

void EditMenu::createContent()
{
    ui->styleListWidget->clear();
    ui->styleListWidget->addItems(textStyles->namesList());

    connect(ui->fontComboBox, SIGNAL(currentFontChanged(QFont)), this, SIGNAL(textFontChangedSignal(QFont)));
    connect(ui->fontSizeSpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(textHeightChangedSignal(int)));

    connect(ui->styleListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(styleSelectedSlot(QListWidgetItem*)));


    connect(ui->textWidthSlider, SIGNAL(valueChanged(int)), this, SIGNAL(textWidthSliderValueChanged(int)));

    connect(ui->zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(ui->zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));

}

//----------------------------------------------------------------------------------------

void EditMenu::hideWidgetsByName(QStringList widgetToHideList)
{
    while(!widgetToHideList.isEmpty()){
        QString objName = widgetToHideList.takeFirst();

        QWidget *widget = this->findChild<QWidget *>(objName);
        widget->hide();
    }
}


//----------------------------------------------------------------------------------------
void EditMenu::charFormatChangedSlot(QTextCharFormat format)
{
    disconnect(ui->fontComboBox, SIGNAL(currentFontChanged(QFont)), this, SIGNAL(textFontChangedSignal(QFont)));
    disconnect(ui->fontSizeSpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(textHeightChangedSignal(int)));


    ui->fontComboBox->setCurrentFont(format.fontFamily());
    ui->fontSizeSpinBox->setValue(format.fontPointSize());


    connect(ui->fontComboBox, SIGNAL(currentFontChanged(QFont)), this, SIGNAL(textFontChangedSignal(QFont)));
    connect(ui->fontSizeSpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(textHeightChangedSignal(int)));

}

//----------------------------------------------------------------------------------------


void EditMenu::tabChangedSlot(QTextCharFormat newTabFormat)
{




    charFormatChangedSlot(newTabFormat);
}

//----------------------------------------------------------------------------------------
void EditMenu::styleSelectedSlot(QListWidgetItem *item)
{

    emit styleSelectedSignal(ui->styleListWidget->row(item));
}

//----------------------------------------------------------------------------------------
void EditMenu::setStyleSelectionSlot(int selection)
{

    if(selection == 99999){
        for (int i = 0; i < textStyles->namesList().size(); ++i) {
            ui->styleListWidget->item(i)->setSelected(false);
        }
        return;
    }


    disconnect(ui->styleListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(styleSelectedSlot(QListWidgetItem*)));

            ui->styleListWidget->item(selection)->setSelected(true);

    connect(ui->styleListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(styleSelectedSlot(QListWidgetItem*)));
}






void EditMenu::setTextWidthMax(int max)
{
    ui->textWidthSlider->setRange(200, max);
}
//----------------------------------------------------------------------------------------


void EditMenu::setTextWidthSliderValue(int sliderValue)
{
    ui->textWidthSlider->setValue(sliderValue);

}
//----------------------------------------------------------------------------------------
int EditMenu::textWidthSliderValue()
{
    return ui->textWidthSlider->value();

}






//------------------------------------------------------------------------------------


void EditMenu::zoomIn()
{
    emit zoomInSignal();
}

//------------------------------------------------------------------------------------

void EditMenu::zoomOut()
{
    emit zoomOutSignal();

}





//----------------------------------------------------------------------------------------
//----------------------------------------------------------------


void EditMenu::applyConfig()
{

    ui->styleListWidget->clear();
    ui->styleListWidget->addItems(textStyles->namesList());



}

