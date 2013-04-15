#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>

#include "utils.h"

/*!
   Delete a directory along with all of its contents.

   \param dirName Path of directory to remove.
   \return true on success; false on error.
*/
bool Utils::removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }

    return result;
}

QString Utils::spaceInNumber(const QString numberString, const QString symbol)
{
    QString originalString = numberString;
    QString finalString;
    QStringList list;

    while(!originalString.isEmpty()){
    list.append(originalString.right(3));
    originalString.chop(3);
    }

    while(!list.isEmpty()){
        finalString.append(list.takeLast());
        finalString.append(symbol);
    }
    finalString.chop(symbol.size());

    return finalString;
}
