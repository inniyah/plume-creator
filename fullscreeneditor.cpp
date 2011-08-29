#include <QtGui>

#include "fulltextzone.h"
#include "fullscreeneditor.h"
#include "digitalclock.h"
#include "timer.h"

FullscreenEditor::FullscreenEditor(QTextDocument *doc, QWidget *parent) :
    QWidget(parent)
{

    FullTextZone *fullTextEdit = new FullTextZone(doc, this);

    QHBoxLayout *textLayout = new QHBoxLayout;

    textLayout->addStretch();
    textLayout->addWidget(fullTextEdit);
    textLayout->addStretch();




    DigitalClock *clock = new DigitalClock(this);
//    Timer *timer = new Timer(this);
    timerLabel = new QLabel;
    wordCountLabel = new QLabel;


    QPushButton *exitFullscreenButton = new QPushButton(tr("Exit"), this);
    connect(exitFullscreenButton, SIGNAL(clicked()),this,SLOT(close()));





        QHBoxLayout *statsLayout = new QHBoxLayout();
        statsLayout->addWidget(wordCountLabel);
  //      statsLayout->addWidget(timer);
                statsLayout->addWidget(timerLabel);
        statsLayout->addWidget(clock);
        statsLayout->addWidget(exitFullscreenButton);



        QVBoxLayout *mainLayout = new QVBoxLayout();

        mainLayout->addLayout(textLayout);
mainLayout->addLayout(statsLayout);


        setLayout(mainLayout);


        applyConfig();

        showFullScreen();

        // get screen width for fullscreen text width option :
        fullTextEdit->setXMax(QApplication::desktop()->screenGeometry().width() - 50);
}

void FullscreenEditor::setWordCount(int num)
{
    QString text;
    wordCountLabel->setText(text.setNum(num,10));
}

void FullscreenEditor::setTimer(QString text)
{
    timerLabel->setText(text);
}





void FullscreenEditor::closeEvent(QCloseEvent* event)
{
    emit closeSignal();
    event->accept();


}


















//---------------------------------------------------------------------------
//----------Apply Config---------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------


void FullscreenEditor::applyConfig()
{

}
