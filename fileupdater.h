#ifndef FILEUPDATER_H
#define FILEUPDATER_H

#include <QObject>
#include <QtGui>
#include <QDomDocument>

class FileUpdater : public QWidget
{
    Q_OBJECT
public:
    explicit FileUpdater(QWidget *parent = 0);
    bool checkAttendanceFile(QString projectFileName);

signals:
    
public slots:

private slots:
    void updateAttendanceFile();

private:
    QFile *attFile;
    QDomDocument domDocument;
    QDomElement root;
    QString devicePath;
    QString attendVersion;
};

#endif // FILEUPDATER_H
