#ifndef MAINTEXTDOCUMENT_H
#define MAINTEXTDOCUMENT_H


#include <QObject>
#include <QTextDocument>
#include <QDebug>

#include "wordcountengine.h"

class MainTextDocument : public QTextDocument
{
    Q_OBJECT
public:
    explicit MainTextDocument(QObject *parent = 0);


    int idNumber() const;
    void setIdNumber(int number);
    QString docType() const;
    void setDocType(QString type);

    int wordCount(){return wordCountEngine->wordCount();}

signals:
    void wordCountChanged(QString type, int id, int count);

public slots:

private slots:
    void wordCountChangedSlot(int count);

private:
    WordCountEngine *wordCountEngine;
    int itemId;
    QString m_docType;
};

#endif // MAINTEXTDOCUMENT_H
