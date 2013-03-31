#ifndef CHANGESTESTS_H
#define CHANGESTESTS_H

#include <QObject>
#include <QDomDocument>
#include <QHash>
#include <QFile>
#include <QTextDocument>

class ChangesTests : public QObject
{
    Q_OBJECT
public:
    static bool test(int oldInt, int newInt);
    static bool test(QString oldString, QString newString);
    static bool test(QHash<QTextDocument *, QFile *> oldHash, QHash<QTextDocument *, QFile *> newHash);
    static bool test(QHash<QTextDocument *, int> oldHash, QHash<QTextDocument *, int> newHash);
signals:
    
public slots:
    
};

#endif // CHANGESTESTS_H
