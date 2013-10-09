#ifndef EDITTOOLBAR_H
#define EDITTOOLBAR_H

#include <QToolBar>

namespace Ui {
class EditToolBar;
}

class EditToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit EditToolBar(QWidget *parent = 0);
    ~EditToolBar();
    void applyConfig();

protected:
    void mouseMoveEvent(QMouseEvent *event);

    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void timerEvent(QTimerEvent *);
signals:
    void actionSignal(QString,bool);

public slots:

    void openToolBar();

    void editZoneMoved(QRect editZoneRect);
    void cursorStateSlot(QString actionName, bool value);
private slots:
    void closeToolBar();
    void createActions();
    void populateToolBar(QStringList actionsStringList);
    void actionSlot();
    void actionSlot(bool value);
    void addToTimerQueue();
    void stopTimer();
    void pinToolBar(bool value);
private:
    Ui::EditToolBar *ui;
    QToolBar *toolBar;

    QRect m_editZoneRect;
    bool isOpened;
    int m_height;
    QRect closeZoneRect;
    bool isPressed;
    int timerStack;
    QList<int> timerIdList;
    bool timerIsActivated;
};

#endif // EDITTOOLBAR_H
