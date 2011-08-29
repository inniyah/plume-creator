#include <QtGui>


#include "timer.h"

Timer::Timer(QWidget *parent) :
    QWidget(parent)
{
    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    timerSettingFrame = new QFrame;
    timerRunningFrame = new QFrame;
    hourEdit = new QSpinBox;
    minEdit = new QSpinBox;
    secEdit = new QSpinBox;
    timeLabel = new QLabel;


    QToolButton *startButton = new QToolButton(this);
    startButton->setText(tr("Start")),
            connect(startButton, SIGNAL(released()), this, SLOT(startTime()));

    QToolButton *resetButton = new QToolButton(this);
    resetButton->setText(tr("Reset"));
    connect(resetButton, SIGNAL(pressed()), this, SLOT(resetTime()));

    QToolButton *pauseButton = new QToolButton(this);
    pauseButton->setText(tr("Pause"));
    pauseButton->setCheckable(true);
    connect(pauseButton, SIGNAL(clicked(bool)), this, SLOT(pauseTime(bool)));


    resetButton->setFixedWidth(50);
    startButton->setFixedWidth(50);
    pauseButton->setFixedWidth(50);

    hourEdit->setFixedWidth(50);
    hourEdit->setRange(0,24);
    hourEdit->setAccelerated(true);
    hourEdit->setSuffix(tr(" h"));
    minEdit->setFixedWidth(50);
    minEdit->setRange(0,60);
    minEdit->setAccelerated(true);
    minEdit->setSuffix(tr(" min"));
    secEdit->setFixedWidth(50);
    secEdit->setRange(0,60);
    secEdit->setAccelerated(true);
    secEdit->setSuffix(tr(" s"));

    secEdit->hide();


    connect(minEdit,SIGNAL(valueChanged(int)),this,SLOT(minSpinFormat(int)));
    connect(secEdit,SIGNAL(valueChanged(int)),this,SLOT(secSpinFormat(int)));

    timeLabel->setMinimumWidth(50);



    QGridLayout *settingLayout = new QGridLayout;
    settingLayout->addWidget(hourEdit,0,0);
    settingLayout->addWidget(minEdit,0,1);
    settingLayout->addWidget(secEdit,0,2);
    settingLayout->addWidget(startButton,1,0,1,3,Qt::AlignHCenter);
    settingLayout->setSpacing(2);
    settingLayout->setContentsMargins(3,3,3,3);
    timerSettingFrame->setLayout(settingLayout);




    QGridLayout *runningLayout = new QGridLayout;
    runningLayout->addWidget(timeLabel,0,0,1,2,Qt::AlignHCenter);
    runningLayout->addWidget(resetButton,1,0);
    runningLayout->addWidget(pauseButton,1,1);
    runningLayout->setSpacing(2);
    runningLayout->setContentsMargins(3,3,3,3);
    timerRunningFrame->setLayout(runningLayout);









    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(timerSettingFrame);
    layout->addWidget(timerRunningFrame);
    layout->setContentsMargins(3,3,3,3);
    setLayout(layout);



    timerRunningFrame->hide();
    timerSettingFrame->show();

}

//-------------------------------------------------------------------------

void Timer::secSpinFormat(int sec)
{

    if(sec == 60){
        secEdit->setValue(0);
        minEdit->setValue( minEdit->value() + 1);
    }
}

//-------------------------------------------------------------------------

void Timer::minSpinFormat(int min)
{
    if(min == 60){
        minEdit->setValue(0);
        hourEdit->setValue( hourEdit->value() + 1);
    }
}

//-------------------------------------------------------------------------

void Timer::startTime()
{


    //  timeValue = hourEdit->value() * 3600 + minEdit->value() * 60 + secEdit->value();
    h = hourEdit->value();
    m =  minEdit->value();
    s = secEdit->value();

    //    if(timeValue == 0)
    if(h == 0 && m == 0 && s == 0)
        return;

    //    QString debug;
    //    qDebug() << "timeValue :" << debug.setNum(timeValue);


    timerSettingFrame->hide();
    timerRunningFrame->show();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);

    showTime();




}

void Timer::resetTime()
{
    timer->stop();

    timerSettingFrame->show();
    timerRunningFrame->hide();
}

void Timer::pauseTime(bool paused)
{
    if(paused)
        timer->stop();
    else
        timer->start(1000);

}


void Timer::showTime()
{
    //    int h/* = floor(timeValue / 3600)*/;

    //    int m/* = floor((timeValue / 3600 - h) * 60)*/;

    //    int s/* = floor((((timeValue / 3600 - h) * 60) - m) * 60)*/;
    if(h == 0 && m == 0 && s == 1){
        s = 0;
        QMessageBox msgBox;
        msgBox.setText("Time is up !");
        msgBox.exec();

        timer->stop();

        timerSettingFrame->show();
        timerRunningFrame->hide();
    }


    s = s - 1;

    if(s == -1){
        s = 59;
        m = m - 1;
    }
    if(m == -1){
        m = 59;
        h = h - 1;
    }

    QString a;
    QString b;
    QString c;

    timeLabel->setText(a.setNum(h,10) + tr(" h ") + b.setNum(m,10) + tr(" min ") + c.setNum(s,10) + tr(" s "));

    QString debug;
    qDebug() << "timeValue in QTime :" << debug.setNum(h,10)
             << ":"<<debug.setNum(m,10)<<":"<<debug.setNum(s,10);
    //    QTime time;
    //    time.setHMS(h,m,s);
    //    QString text = time.toString("h:mm:ss");

    //    qDebug() << "time :" << text << "----" << time.isValid();

    //    if ((time.second() % 2) == 0)
    //        text[1] = ' ';
    //    text[3] = ' ';
    //   timeLabel->setText(text);

    //    timeValue = timeValue - 1;




    //    if(timeValue == 0){
    //        timer->stop();


//for fullscreen editor :
    emit timerSignal(timeLabel->text());


}
