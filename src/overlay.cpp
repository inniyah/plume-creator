#include "overlay.h"
#include <QPainter>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif
#include <QtGui>

Overlay::Overlay(QWidget *parent) :
    QWidget(parent)
{
    setPalette(Qt::transparent);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    this->setMouseTracking(true);


    QSettings settings;
    firstTime1 = settings.value("Settings/TextArea/firstTime1", true).toBool();
}

void Overlay::paintEvent(QPaintEvent *event)
{
    //    QPainter painter(this);
    //      painter.setRenderHint(QPainter::Antialiasing);
    //      painter.setPen(QPen(Qt::blue));
    //      painter.drawRect(m_editZoneRect);


    if(firstTime1){
        QPainter hoverPainter(this);
        hoverPainter.setRenderHint(QPainter::Antialiasing);
        hoverPainter.setPen(QPen(Qt::red));
        hoverPainter.drawRect(m_leftHoverZone);
        //        hoverPainter.drawRect(m_rightHoverZone);

        hoverPainter.drawText(QPoint(0, 15 + m_leftHoverZone.height()), tr("Move the mouse here to show the tool bar."));

        QSettings settings;
        settings.setValue("Settings/TextArea/firstTime1", false);
    }
}


void Overlay::editZoneMoved(QRect editZoneRect)
{

    QPoint point = this->mapFromGlobal(QPoint(editZoneRect.x(), editZoneRect.y()));
    m_editZoneRect.setX(point.x());
    m_editZoneRect.setY(point.y());
    m_editZoneRect.setWidth(editZoneRect.width());
    m_editZoneRect.setHeight(editZoneRect.height());


    // hover zones :

    int hoverZoneSize =  m_editZoneRect.height() / 15;

    QPoint leftPoint(m_editZoneRect.x(), m_editZoneRect.y());
    m_leftHoverZone = QRect(leftPoint.x(), leftPoint.y(), - hoverZoneSize, hoverZoneSize);

    QPoint rightPoint(m_editZoneRect.x() + m_editZoneRect.width(), m_editZoneRect.y());
    m_rightHoverZone = QRect(rightPoint.x(), rightPoint.y(), hoverZoneSize, hoverZoneSize);


}

