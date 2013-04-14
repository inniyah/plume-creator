#ifndef WORDCOUNTENGINE_H
#define WORDCOUNTENGINE_H

#include <QObject>
#include <QtGui>

class WordCountEngine : public QObject
{
    Q_OBJECT
public:
    explicit WordCountEngine(QObject *parent = 0, QTextDocument *doc = 0);
    int wordCount(){return m_wordCount;}

signals:
    void wordCountChanged(int);

public slots:

private slots:
    void update();
    void updateCursorPosition(QTextCursor textCursor);
private:
    int m_wordCount, preBlockCount, preBlockNum, blockNum, wordCountWithoutBlock;
    QTextDocument *m_doc;
};

#endif // WORDCOUNTENGINE_H
