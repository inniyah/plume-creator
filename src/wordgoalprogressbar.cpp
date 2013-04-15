#include "wordgoalprogressbar.h"
#include <QInputDialog>
#include <QMenu>
#include <QContextMenuEvent>
#include "ui_wordgoalprogressbar.h"
#include <QtGui>

WordGoalProgressBar::WordGoalProgressBar(QWidget *parent) :
    QWidget(parent), m_isWordGoalActivated(false),
    ui(new Ui::WordGoalProgressBar)
{
    ui->setupUi(this);

}
void WordGoalProgressBar::postConstructor()
{
    createActions();

    ui->progressBar->setFormat("%v");
    ui->progressBar->setMinimum(0);


    ui->progressBar->setStyleSheet("QProgressBar {"
                                   "border: 1px solid grey;"
                                   "border-radius: 2px;"
                                   "text-align: center;"
                                   "}"
                                   "QProgressBar::chunk {"
                                   "background-color: #05B8CC;"
                                   "width: 20px;"
                                   "}");

    connect(hub, SIGNAL(achievedWordGoalSignal(int)), this, SLOT(setValue(int)));
    connect(hub, SIGNAL(baseWordCountSignal(int)), this, SLOT(setBase(int)));
    connect(hub, SIGNAL(wordGoalSignal(int)), this, SLOT(setGoal(int)));
    connect(hub, SIGNAL(wordGoalIsActivatedSignal(bool)), this, SLOT(setWordGoalActivated(bool)));




}

WordGoalProgressBar::~WordGoalProgressBar()
{
    delete ui;
}

void WordGoalProgressBar::setGoalDialog()
{
    bool ok;
    int i = QInputDialog::getInt(this, tr("Set Session Target"),
                                 tr("Word count target :"), 1000, 0, 999999, 1, &ok);
    if (ok){
        hub->setWordGoalActivated(true);
        hub->setWordGoal(i);

    }
}


void WordGoalProgressBar::setGoal(int goal)
{
    ui->progressBar->setMaximum(goal);
    ui->wordGoalLabel->setText(tr("/%1 words").arg(goal));
    ui->wordGoalLabel->setVisible(m_isWordGoalActivated);

    setColors();

}

int WordGoalProgressBar::goal()
{
    return ui->progressBar->maximum();
}

void WordGoalProgressBar::setBase(int base)
{
    //    ui->progressBar->setMinimum(0);
    setColors();

}

void WordGoalProgressBar::setValue(int value)
{
    ui->progressBar->setValue(value);
    setColors();
}

int WordGoalProgressBar::value()
{
    return ui->progressBar->value();
}

void WordGoalProgressBar::changeProgressBy(int progress)
{
    if(ui->progressBar->value() + progress > ui->progressBar->maximum())
        ui->progressBar->setMaximum(ui->progressBar->value() + progress);


    ui->progressBar->setValue(ui->progressBar->value() + progress);

    //    qDebug() << "progressBy : " << QString::number(progress);

}

void WordGoalProgressBar::setColors()
{

    // set colors :

    int max = hub->wordGoal();
    int val = hub->achievedWordGoal();

    int percent = val*100/max;



    QString red = QString::number(255*(100-percent)/100);
    QString green = QString::number(255*percent/100);



    ui->progressBar->setStyleSheet("QProgressBar {"
                                   "border: 1px solid grey;"
                                   "border-radius: 2px;"
                                   "text-align: center;"
                                   "}"
                                   "QProgressBar::chunk {"
                                   "background-color: rgb("+ red +","+ green + ", 0);"
                                   "}");

}

void WordGoalProgressBar::setWordGoalActivated(bool wordGoalActivated)
{
    m_isWordGoalActivated = wordGoalActivated;

    ui->wordGoalLabel->setVisible(wordGoalActivated);
}

void WordGoalProgressBar::reset()
{
    ui->progressBar->setValue(0);
    hub->setBaseWordCount(hub->projectWordCount());
    hub->setAchievedWordGoal(0);
}
void  WordGoalProgressBar::init()
{
    hub->setBaseWordCount(hub->baseWordCount()+1);
    hub->setBaseWordCount(hub->baseWordCount()-1);
    hub->setWordGoal(hub->wordGoal()+1);
    hub->setWordGoal(hub->wordGoal()-1);
    hub->setAchievedWordGoal(hub->achievedWordGoal()+1);
    hub->setAchievedWordGoal(hub->achievedWordGoal()-1);
}


//--------- Context Menu :------------------------------------------------------------------


void WordGoalProgressBar::createActions()
{
    setGoalAct = new QAction(tr("&Set target"), this);
    setGoalAct->setStatusTip(tr("Set word count target"));
    connect(setGoalAct, SIGNAL(triggered()), this, SLOT(setGoalDialog()));

    resetAct = new QAction(QIcon(":/pics/edit-undo.png"),tr("&Reset"), this);
    resetAct->setStatusTip(tr("Reset the progress to zero"));
    connect(resetAct, SIGNAL(triggered()), this, SLOT(reset()));



}

void WordGoalProgressBar::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    menu.addAction(setGoalAct);
    menu.addAction(resetAct);

    menu.exec(event->globalPos());
}
