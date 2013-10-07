#include "sizehandle.h"

SizeHandle::SizeHandle(QWidget *parent) :
    QWidget(parent),isPressed(false)
{

this->setMouseTracking(true);
}

void SizeHandle::leaveEvent(QEvent *event)
{

//    this->setStyleSheet( "QWidget {}");


    QApplication::restoreOverrideCursor();
    isPressed = false;
    event->accept();

}

//-------------------------------------------------------------------


void SizeHandle::enterEvent(QEvent *event)
{

//    this->setStyleSheet("QWidget {background: grey}");

    QApplication::setOverrideCursor(QCursor(Qt::SizeHorCursor));

    event->accept();

}
//-------------------------------------------------------------------

void SizeHandle::paintEvent(QPaintEvent *event)
{

        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }


//-------------------------------------------------------------------

void SizeHandle::mousePressEvent(QMouseEvent *event)
{
    mousePos = event->globalPos();

    isPressed = true;
}

void SizeHandle::mouseReleaseEvent(QMouseEvent *event)
{

    isPressed = false;
    event->accept();
}

//-------------------------------------------------------------------

//-------------------------------------------------------------------

void SizeHandle::mouseMoveEvent(QMouseEvent *event)
{
    if(!isPressed)
        return;

    if(m_side.testFlag(SizeHandle::Left)){


        emit modifySizeSignal(mousePos.x() - event->globalPos().x() - event->pos().x());

    }
    else if(m_side.testFlag(SizeHandle::Right)){



        emit modifySizeSignal(-(mousePos.x() - event->globalPos().x() - event->pos().x()));

    }   else
        qWarning() << "SizeHandle::mouseMoveEvent     no side";






    mousePos = event->globalPos();




}

//-------------------------------------------------------------------

SizeHandle::Sides SizeHandle::side() const
{
    return m_side;
}

//-------------------------------------------------------------------

void SizeHandle::setSide(const SizeHandle::Sides &side)
{
    m_side = side;
}

//-------------------------------------------------------------------

//----------------------------------------------------------------------------------------

