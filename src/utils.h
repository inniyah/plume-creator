#ifndef UTILS_H
#define UTILS_H

#include <QString>

class Utils
{
public:
    static bool removeDir(const QString &dirName);
    static QString spaceInNumber(const QString numberString = "",const QString symbol = " ");
};

#endif // UTILS_H
