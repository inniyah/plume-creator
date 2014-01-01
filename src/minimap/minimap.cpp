#include "minimap.h"
#include "ui_minimap.h"

Minimap::Minimap(QWidget *parent) :
    ui(new Ui::Minimap),QWidget(parent)
{
    ui->setupUi(this);


    thumbItem = new QGraphicsPixmapItem();
//    proxy = new QGraphicsProxyWidget();
scene.addItem(thumbItem);
    ui->graphicsView->setScene(&scene);



this->hide();

}

Minimap::~Minimap()
{
    delete ui;

}

//---------------------------------------------------------------------


void Minimap::setWidgetInProxy(QWidget *widget)
{
//    proxy->setWidget(widget);
}

//---------------------------------------------------------------------

void Minimap::setTextThumbnail(QPixmap thumbnail)
{
QPixmap scaledThumbnail = thumbnail.scaledToWidth(this->width());
scene.removeItem(thumbItem);
thumbItem = scene.addPixmap(scaledThumbnail);
}

void Minimap::resizeContent()
{
    thumbItem->pixmap().scaledToWidth(this->width());
}

