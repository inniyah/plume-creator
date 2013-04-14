#include <QtGui>


#include "statsbox.h"
#include "wordcountthread.h"
#include "digitalclock.h"
#include "timer.h"
//
StatsBox::StatsBox(QWidget *parent) :
    QFrame(parent)
{

    QGroupBox *clockBox = new QGroupBox(tr("Time"));
    DigitalClock *clock = new DigitalClock(this);
    clock->setHub(hub);
    QVBoxLayout *clockLayout = new QVBoxLayout;
    clockLayout->addWidget(clock);
    clockLayout->setContentsMargins(3,3,3,3);
    clockBox->setLayout(clockLayout);

    QGroupBox *timerBox = new QGroupBox(tr("Timer"));
    Timer *timer = new Timer(this);
    timer->setHub(hub);
    QVBoxLayout *timerLayout = new QVBoxLayout;
    timerLayout->addWidget(timer);
    timerLayout->setContentsMargins(3,3,3,3);
    timerBox->setLayout(timerLayout);



    QGroupBox *wordCountBox = new QGroupBox(tr("Word Count"));
    wordCountLabel = new QLabel;
    projectWordCountButton = new QPushButton(tr("All"));
    connect(projectWordCountButton,SIGNAL(clicked()),this, SLOT(displayProjectWordCount()));
    QHBoxLayout *wordCountLayout = new QHBoxLayout;
    wordCountLayout->addWidget(wordCountLabel);
    wordCountLayout->addWidget(projectWordCountButton);
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

//---------------------------------------------------------------------------------------------------------
void StatsBox::setWordCount(int num)
{
    QString text;
    wordCountLabel->setText(text.setNum(num,10));
}
//---------------------------------------------------------------------------------------------------------
void StatsBox::displayProjectWordCount()
{
    QWidget *countWindow = new QWidget(0, (Qt::Popup));
    QLabel *currentProjectLabel = new QLabel(tr("Project : "));
    QLabel *currentBookLabel = new QLabel(tr("Current book : "));
    QLabel *currentChapterLabel = new QLabel(tr("current chapter : "));
    currentProjectCountLabel = new QLabel(tr("calculating..."));
    currentBookCountLabel = new QLabel(tr("calculating..."));
    currentChapterCountLabel = new QLabel(tr("calculating..."));
    QLabel *currentProjectPageCountLabel = new QLabel();
    QLabel *currentBookPageCountLabel = new QLabel();
    QLabel *currentChapterPageCountLabel = new QLabel();

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(currentProjectLabel,0,0, Qt::AlignRight);
    gridLayout->addWidget(currentProjectCountLabel,0,1, Qt::AlignLeft);
    gridLayout->addWidget(currentProjectPageCountLabel,0,2, Qt::AlignRight);
    gridLayout->addWidget(currentBookLabel,1,0, Qt::AlignRight);
    gridLayout->addWidget(currentBookCountLabel,1,1, Qt::AlignLeft);
    gridLayout->addWidget(currentBookPageCountLabel,1,2, Qt::AlignRight);
    gridLayout->addWidget(currentChapterLabel,2,0, Qt::AlignRight);
    gridLayout->addWidget(currentChapterCountLabel,2,1, Qt::AlignLeft);
    gridLayout->addWidget(currentChapterPageCountLabel,2,2, Qt::AlignRight);
    countWindow->setLayout(gridLayout);

    countWindow->move(projectWordCountButton->mapToGlobal(QPoint(projectWordCountButton->width(),0)));

countWindow->show();



// launch wordcount thread :

WordCountThread *prjWordCountThread = new WordCountThread();
prjWordCountThread->setHub(hub);

connect(this, SIGNAL(docsForProjectWordCountSignal(QHash<MainTextDocument*,QFile*>)), prjWordCountThread, SLOT(setDocs(QHash<MainTextDocument*,QFile*>)), Qt::UniqueConnection);
connect(this, SIGNAL(domForProjectWordCountSignal(QDomDocument)), prjWordCountThread, SLOT(setDom(QDomDocument)), Qt::UniqueConnection);
connect(this, SIGNAL(setCurrentNumberSignal(int)), prjWordCountThread, SLOT(setCurrentNumber(int)), Qt::UniqueConnection);
connect(prjWordCountThread, SIGNAL(chapterWordCountSignal(int)),this, SLOT(setChapterWordCount(int)), Qt::UniqueConnection);
connect(prjWordCountThread, SIGNAL(bookWordCountSignal(int)),this, SLOT(setBookWordCount(int)), Qt::UniqueConnection);
connect(prjWordCountThread, SIGNAL(projectWordCountSignal(int)),this, SLOT(setProjectWordCount(int)), Qt::UniqueConnection);

emit fetchDomAndDocsSignal();
emit fetchCurrentNumber();


prjWordCountThread->start();

}
//---------------------------------------------------------------------------------------------------------
void StatsBox::setChapterWordCount(int count)
{
    QString string;
    currentChapterCountLabel->setText(string.setNum(count));
}


//---------------------------------------------------------------------------------------------------------
void StatsBox::setBookWordCount(int count)
{
    QString string;
    currentBookCountLabel->setText(string.setNum(count));

}

//---------------------------------------------------------------------------------------------------------
void StatsBox::setProjectWordCount(int count)
{
    QString string;
    currentProjectCountLabel->setText(string.setNum(count));

}
