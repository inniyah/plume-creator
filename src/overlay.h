#ifndef OVERLAY_H
#define OVERLAY_H

#include <QWidget>

class Overlay : public QWidget
{
    Q_OBJECT
public:
    explicit Overlay(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void editZoneMoved(QRect editZoneRect);

private:
  int m_x, m_y, m_w, m_h;
QRect m_editZoneRect, m_leftHoverZone, m_rightHoverZone;
bool firstTime1;
};

#endif // OVERLAY_H
