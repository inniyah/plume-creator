#ifndef ZIPPER_H
#define ZIPPER_H

#include <QObject>
#include <QtCore>


class Zipper : public QThread
{
    Q_OBJECT
public:
    explicit Zipper(QObject *parent = 0);
    void setJob(QString job, QString fileName);


protected:
    void run();

signals:

    void zipStarted(QString mode);
    void zipProgressed();
    void zipFinished();
    void zipError(QString error);


private:
    void extract(QString zipFileName);
    void compress(QString zipFileName);
    QString currentJob;
    QString fileName_;

};

#endif // ZIPPER_H
