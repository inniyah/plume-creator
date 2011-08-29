#ifndef FULLSCREENEDITOR_H
#define FULLSCREENEDITOR_H


#include <QWidget>
#include <QLabel>
#include <QTextDocument>

class FullscreenEditor : public QWidget
{
    Q_OBJECT
public:
    explicit FullscreenEditor(QTextDocument *doc = 0, QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent* event);

signals:
    void closeSignal();

public slots:
    void setWordCount(int num);
    void setTimer(QString);
    void applyConfig();

private slots:


private:
    QLabel *wordCountLabel;
    QLabel *timerLabel;
};

#endif // FULLSCREENEDITOR_H
