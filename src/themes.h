#ifndef THEMES_H
#define THEMES_H

#include <QObject>
#include <QDomDocument>
#include <QHash>

#include "theme.h"

class Themes : public QObject
{
    Q_OBJECT
public:
    explicit Themes(QObject *parent = 0);
    
   static QHash<QString,QString> themesList();

    bool loadTheme(QString themeFileOrPath);
    void unloadTheme();

    Theme *currentTheme() const;

signals:
    
public slots:

private:
    static QDomDocument readInfoXML(QString fileString);
    Theme *m_currentTheme;
};

#endif // THEMES_H
