#ifndef MAINTEXTDOCUMENT_H
#define MAINTEXTDOCUMENT_H


#include <QObject>
#include <QTextDocument>
#include <QDebug>

#include "wordcountengine.h"
#include "texthighlighter.h"

class MainTextDocument : public QTextDocument
{
    Q_OBJECT
public:
    explicit MainTextDocument(QObject *parent = 0);


    int idNumber() const;
    void setIdNumber(int number);
    QString docType() const;
    void setDocType(QString type);
    int cursorPos() const;

    int wordCount(){return wordCountEngine->wordCount();}
    TextHighlighter* textHighlighter();

signals:
    void wordCountChanged(QString type, int id, int count);

public slots:
    void setCursorPos(int pos);

private slots:
    void wordCountChangedSlot(int count);

private:
    WordCountEngine *wordCountEngine;
    TextHighlighter *highlighter;
    int itemId, m_cursorPos;
    QString m_docType;

};

#endif // MAINTEXTDOCUMENT_H
