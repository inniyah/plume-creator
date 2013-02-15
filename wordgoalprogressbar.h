#ifndef WORDGOALPROGRESSBAR_H
#define WORDGOALPROGRESSBAR_H

#include <QProgressBar>


namespace Ui {
class WordGoalProgressBar;
}

class WordGoalProgressBar : public QWidget
{
    Q_OBJECT
    
public:
    explicit WordGoalProgressBar(QWidget *parent = 0);
    ~WordGoalProgressBar();
    
    void setGoal(int goal);
    int goal();
    void setBase(int base);
    void setValue(int value);
    int value();

protected:
    void contextMenuEvent(QContextMenuEvent *event);

public slots:
    void changeProgressBy(int progress);

private slots:
    void setGoalDialog();
void createActions();

private:
    Ui::WordGoalProgressBar *ui;
QAction *resetAct, *setGoalAct;

};

#endif // WORDGOALPROGRESSBAR_H
