#ifndef ZIPCHECKER_H
#define ZIPCHECKER_H

#include <QObject>
#include <QStringList>
#include <QDir>
#include <QHash>

#include "utils.h"

class ZipChecker : public QObject
{
    Q_OBJECT
public:
    explicit ZipChecker(QObject *parent = 0);
    QStringList list();
    void addFile(QString type, int idNumber);
    void removeFile(QString type, int idNumber);
    void clearList();
    static bool compareAndClean(QString tempDirectory, QStringList fileList);

signals:
    
public slots:

private:
    QStringList baseList;
    QStringList textsList;
};

#endif // ZIPCHECKER_H
