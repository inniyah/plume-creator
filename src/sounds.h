#ifndef SOUNDS_H
#define SOUNDS_H

#include <QObject>
#include <QDebug>
#include <QSoundEffect>
#include <QFile>

class Sounds : public QObject
{
    Q_OBJECT
public:
    explicit Sounds(QObject *parent = 0);
    
signals:
    
public slots:
    void startSoundKeyAny();
private:
    QSoundEffect effect;

};

#endif // SOUNDS_H
