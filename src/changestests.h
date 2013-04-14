#ifndef CHANGESTESTS_H
#define CHANGESTESTS_H

#include <QObject>
#include <QDomDocument>
#include <QHash>
#include <QFile>
#include <QTextDocument>
#include "maintextdocument.h"

class ChangesTests : public QObject
{
    Q_OBJECT
public:
    static bool test(int oldInt, int newInt);
    static bool test(QString oldString, QString newString);
    static bool test(QHash<QTextDocument *, QFile *> oldHash, QHash<QTextDocument *, QFile *> newHash);
    static bool test(QHash<QTextDocument *, int> oldHash, QHash<QTextDocument *, int> newHash);
    static bool test(QHash<MainTextDocument *, QFile *> oldHash, QHash<MainTextDocument *, QFile *> newHash);
    static bool test(QHash<MainTextDocument *, int> oldHash, QHash<MainTextDocument *, int> newHash);
signals:
    
public slots:
    
};

#endif // CHANGESTESTS_H
