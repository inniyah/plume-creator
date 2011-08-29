#include <QtGui>


#include "statsbox.h"
#include "digitalclock.h"
#include "timer.h"

StatsBox::StatsBox(QWidget *parent) :
    QFrame(parent)
{

    QGroupBox *clockBox = new QGroupBox(tr("Time"));
    DigitalClock *clock = new DigitalClock(this);
    QVBoxLayout *clockLayout = new QVBoxLayout;
    clockLayout->addWidget(clock);
    clockLayout->setContentsMargins(3,3,3,3);
    clockBox->setLayout(clockLayout);

    QGroupBox *timerBox = new QGroupBox(tr("Timer"));
    Timer *timer = new Timer(this);
    QVBoxLayout *timerLayout = new QVBoxLayout;
    timerLayout->addWidget(timer);
    timerLayout->setContentsMargins(3,3,3,3);
    timerBox->setLayout(timerLayout);



    QGroupBox *wordCountBox = new QGroupBox(tr("Word Count"));
    wordCountLabel = new QLabel;
    QVBoxLayout *wordCountLayout = new QVBoxLayout;
    wordCountLayout->addWidget(wordCountLabel);
    wordCountLayout->setContentsMargins(3,3,3,3);
    wordCountBox->setLayout(wordCountLayout);

    QGridLayout *base = new QGridLayout;
    base->addWidget(clockBox, 0, 0);;
    base->addWidget(timerBox,1,0);
    base->addWidget(wordCountBox,2,0);
    setLayout(base);


    //for fullscreen editor :
    connect(timer,SIGNAL(timerSignal(QString)), this, SIGNAL(timerSignal(QString)));
}


void StatsBox::setWordCount(int num)
{
    QString text;
    wordCountLabel->setText(text.setNum(num,10));
}
