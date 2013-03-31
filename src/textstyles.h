/***************************************************************************
 *   Copyright (C) 2011 by Cyril Jacquet                                   *
 *   terreville@gmail.com                                                 *
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
#ifndef TEXTSTYLES_H
#define TEXTSTYLES_H

#include <QtGui>

#include <QDomElement>

#include "hub.h"

class TextStyles : public QObject
{
public:
    TextStyles(QObject *parent = 0);
    void setProjectInfoFile();
    void addDefaultStyle(QString name = "New Style");
    void removeStyleAt(int index = 99999);
    void renameStyle(int index, QString newName);
    void changeDocStyles(QTextDocument *doc, QString mode = "none");
    int compareStylesWithText(QTextCharFormat charFormat, QTextBlockFormat blockFormat, QString mode = "none"); //99999 is number if no matching style is found
    int compareBaseStylesWithTextToUpdateIt(QTextCharFormat charFormat, QTextBlockFormat blockFormat);//99999 is number if no matching style is found
    void loadStyles(){this->readSettings();}
    void loadBaseStyles(){this->readBaseSettings();}
    void saveStyles();
    void saveBaseStyles();

public slots:

    void setHub(Hub *varHub){hub = varHub;}
    QStringList namesList();

    int zoomModifier(){return zoom;}
    void setZoomModifier(int zoomModifier){zoom = zoomModifier;}
    int previousZoomModifier(){return previousZoom;}
    void setPreviousZoomModifier(int zoomModifier){previousZoom = zoomModifier;}

    QString nameAt(int index){if(!nameList.isEmpty())return nameList[index]; else return "";}
    QString fontFamilyAt(int index){if(!fontFamilyList.isEmpty())return fontFamilyList[index]; else return "Liberation Serif";}
    int fontSizeAt(int index, QString mode = "none");
    int blockFirstLineIndentAt(int index){if(!blockFirstLineIndentList.isEmpty())return blockFirstLineIndentList[index]; else return 20;}
    int blockBottomMarginAt(int index){if(!blockBottomMarginList.isEmpty())return blockBottomMarginList[index]; else return 10;}
    int blockLeftMarginAt(int index){if(!blockLeftMarginList.isEmpty())return blockLeftMarginList[index]; else return 0;}
    int blockAlignmentAt(int index){if(!blockAlignmentList.isEmpty())return blockAlignmentList[index]; else return 0;}
    bool fontItalicAt(int index){if(!fontItalicList.isEmpty())return fontItalicList[index]; else return false;}
    bool fontBoldAt(int index){if(!fontBoldList.isEmpty())return fontBoldList[index]; else return false;}
    bool fontUnderlineAt(int index){if(!fontUnderlineList.isEmpty())return fontUnderlineList[index]; else return false;}
    bool fontStrikeOutAt(int index){if(!fontStrikeOutList.isEmpty())return fontStrikeOutList[index]; else return false;}
    int defaultStyleIndex(){return defaultStyle;}

    void setNameAt(int index, QString name){nameList[index] = name;}
    void setFontFamilyAt(int index, QString family){fontFamilyList[index] = family;}
    void setFontSizeAt(int index, int size, QString mode = "none");
    void setBlockFirstLineIndentAt(int index, int firstlineIndent){blockFirstLineIndentList[index] = firstlineIndent;}
    void setBlockBottomMarginAt(int index, int bottomMargin){blockBottomMarginList[index] = bottomMargin;}
    void setBlockLeftMarginAt(int index, int leftMargin){blockLeftMarginList[index] = leftMargin;}
    void setBlockAlignmentAt(int index, int alignment){blockAlignmentList[index] = alignment;}
    void setFontItalicAt(int index, bool italic){fontItalicList[index] = italic;}
    void setFontBoldAt(int index, bool bold){fontBoldList[index] = bold;}
    void setFontUnderlineAt(int index, bool underline){fontUnderlineList[index] = underline;}
    void setFontStrikeOutAt(int index, bool strikeOut){fontStrikeOutList[index] = strikeOut;}
    void setDefaultStyle(int index){defaultStyle = index;}

    Qt::Alignment blockAlignmentTrueNameAt(int index);
    bool isDefaultStyle(int index){if(defaultStyle == index) return true; else return false;}

private slots:
    void readSettings();
    void readBaseSettings();
    bool stringToBool(QString value);
    QString boolToString(bool value);
    void writeDomDoc();

private:
    Hub *hub;
    QSettings settings;

    int currentStyleIndex;

    int previousZoom;
    int zoom;

    QList<QString>  nameList;
    QList<QString>  fontFamilyList;
    QList<int>  fontSizeList;
    QList<int>  blockFirstLineIndentList;
    QList<int>  blockBottomMarginList;
    QList<int>  blockLeftMarginList;
    QList<int>  blockAlignmentList;
    QList<bool>  fontItalicList;
    QList<bool>  fontBoldList;
    QList<bool>  fontUnderlineList;
    QList<bool>  fontStrikeOutList;
    int defaultStyle;




    QList<QString>  baseNameList;
    QList<QString>  baseFontFamilyList;
    QList<int>  baseFontSizeList;
    QList<int>  baseBlockFirstLineIndentList;
    QList<int>  baseBlockBottomMarginList;
    QList<int>  baseBlockLeftMarginList;
    QList<int>  baseBlockAlignmentList;
    QList<bool>  baseFontItalicList;
    QList<bool>  baseFontBoldList;
    QList<bool>  baseFontUnderlineList;
    QList<bool>  baseFontStrikeOutList;
    int baseDefaultStyle;

    QFile *prjinfoFile;
    QDomDocument domDocument;

    QDomElement styleElem, baseStyleElem;
};

#endif // TEXTSTYLES_H
