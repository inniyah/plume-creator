#include "sounds.h"

Sounds::Sounds(QObject *parent) :
    QObject(parent)

{
    effect.setSource(QUrl::fromLocalFile("qrc:/sounds/keyany.wav"));
    effect.setLoopCount(QSoundEffect::Infinite);
    effect.setVolume(1.0f);
}
void Sounds::startSoundKeyAny(){

    qDebug() << effect.status();
            effect.play();
}
