/***************************************************************************
 *   Copyright (C) 2012 by Cyril Jacquet                                   *
 *   cyril.jacquet@plume-creator.eu                                                 *
 *                                                                         *
 *  This file is part of Plume Creator.                                    *
 *                                                                         *
 *  Plume Creator is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  Plume Creator is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with Plume Creator.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/
#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QDomDocument>
#include "JlCompress.h"
#include "fileupdater.h"

class Utils
{
public:
    static bool removeDir(const QString &dirName);
    static QString spaceInNumber(const QString numberString = "",const QString symbol = " ");

    static int addProjectToSettingsArray(QString fileName);
    static bool modifyProjectModifiedDateInSettingsArray(int arrayNumber, QString date);
    static int findProjectInSettingArray(QString fileName);
    static bool isProjectExistingInSettingArray(QString fileName);
    static int adaptSettingArrayToProject(QString fileName);
    static void sortSettingsArray();

    static bool isProjectFromOldSystem(QString file);
    static QString updateProjectIfOldSystem(QString file);
    static QString projectRealName(QString fileName);

    static QString parseHtmlText(QString htmlText);
};

#endif // UTILS_H
