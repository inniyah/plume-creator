#include "textstyles.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   

TextStyles::TextStyles(QObject *parent) :
    QObject(parent),
    defaultStyle(0),
    previousZoom(0),
    zoom(0)
{

}

void TextStyles::setProjectInfoFile()
{

    domDocument = hub->infoTreeDomDoc();



QDomElement root = domDocument.documentElement();

    QDomElement prjInfoElem = root.firstChildElement("prj");
    //    if(extFilePath != prjInfoElem.attribute("workPath")){


    QDir dir(hub->projectWorkPath());

    prjInfoElem.setAttribute("workPath", dir.absolutePath());
    dir.cdUp();
    prjInfoElem.setAttribute("path", dir.absolutePath());

    if(domDocument.elementsByTagName("styles").size() == 0){
        QDomElement elem = domDocument.createElement("styles");
        root.appendChild(elem);
    }
    styleElem = root.firstChildElement("styles");

    if(domDocument.elementsByTagName("baseStyles").size() == 0){
        QDomElement elem = domDocument.createElement("baseStyles");
        root.appendChild(elem);
    }
    baseStyleElem = root.firstChildElement("baseStyles");



    readSettings();
}


QStringList TextStyles::namesList()
{
    if(!nameList.isEmpty())
        return nameList;
    else{
        readSettings();  // 2 default styles are created if the list is empty
        return nameList;
    }

}


int TextStyles::fontSizeAt(int index, QString mode)
{
    int m_zoom;

    if(mode == "none")
        m_zoom = 0;
    else if(mode == "zoom")
        m_zoom  = zoom;
    else
        m_zoom = 0;

    if(!fontSizeList.isEmpty())
        return fontSizeList[index] + m_zoom;
    else return 12;
}

void TextStyles::setFontSizeAt(int index, int size, QString mode)
{
    int m_zoom;

    if(mode == "none")
        m_zoom = 0;
    else if(mode == "zoom")
        m_zoom  = zoom;
    else
        m_zoom = 0;

    fontSizeList[index] = size + m_zoom;
}


void TextStyles::addDefaultStyle(QString name)
{
    nameList.append(name);
    fontFamilyList.append("Liberation Serif");
    fontSizeList.append(12);
    blockFirstLineIndentList.append(20);
    blockBottomMarginList.append(10);
    blockLeftMarginList.append(0);
    blockAlignmentList.append(0);
    fontItalicList.append(false);
    fontBoldList.append(false);
    fontUnderlineList.append(false);
    fontStrikeOutList.append(false);
}

void TextStyles::removeStyleAt(int index){

    if(index == 99999)
        index = currentStyleIndex;

    nameList.removeAt(index);
    fontFamilyList.removeAt(index);
    fontSizeList.removeAt(index);
    blockFirstLineIndentList.removeAt(index);
    blockBottomMarginList.removeAt(index);
    blockLeftMarginList.removeAt(index);
    blockAlignmentList.removeAt(index);
    fontItalicList.removeAt(index);
    fontBoldList.removeAt(index);
    fontUnderlineList.removeAt(index);
    fontStrikeOutList.removeAt(index);

    if(defaultStyle == index)
        defaultStyle -= 1;
    if(defaultStyle < 0)
        defaultStyle = 0;

}
//-----------------------------------------------------------------------
void TextStyles::renameStyle(int index, QString newName)
{
    nameList.replace(index, newName);
}

//-----------------------------------------------------------------------
Qt::Alignment TextStyles::blockAlignmentTrueNameAt(int index)
{
    int align = this->blockAlignmentAt(index);

    if(align == 0)
        return Qt::AlignLeft;
    else if(align == 1)
        return Qt::AlignHCenter;
    else if(align == 2)
        return Qt::AlignRight;
    else if(align == 3)
        return Qt::AlignJustify;

return Qt::AlignLeft;
}

void TextStyles::changeDocStyles(QTextDocument *doc, QString mode)
{
    int m_zoom;

    if(mode == "none")
        m_zoom = 0;
    else if(mode == "applyZoom" || mode == "modifyZoom")
        m_zoom  = zoom;
    else if(mode == "removeZoom")
        m_zoom  = 0;
    else
        m_zoom = 0;

    //    qDebug() << "m_zoom : " << QString::number(m_zoom);

    for(int j = 0; j < doc->blockCount(); ++j){

        QTextBlock textBlock = doc->findBlockByNumber(j);
        int styleIndex;
        QTextCursor curs(doc);

        if(j == 0){// solve a bug : first block has system default formatting, which is visually false.
            curs.setPosition(0);
            //                    if(curs.blockNumber() == 1){
            //                        curs.setPosition(0);
            //                        curs.insertText(" ");
            //                    }
            if(mode == "applyZoom")
                styleIndex = this->compareStylesWithText(curs.charFormat(), curs.blockFormat(), "none");
            else if(mode == "removeZoom")
                styleIndex = this->compareStylesWithText(curs.charFormat(), curs.blockFormat(), "zoom");
            else if(mode == "modifyZoom")
                styleIndex = this->compareStylesWithText(curs.charFormat(), curs.blockFormat(), "modifyZoom");
            else
                styleIndex = this->compareBaseStylesWithTextToUpdateIt(curs.charFormat(), curs.blockFormat());
        }


        else{

            curs.setPosition(textBlock.position());

            if(mode == "applyZoom")
                styleIndex = this->compareStylesWithText(curs.charFormat(), curs.blockFormat(), "none");
            else if(mode == "removeZoom")
                styleIndex = this->compareStylesWithText(curs.charFormat(), curs.blockFormat(), "zoom");
            else if(mode == "modifyZoom")
                styleIndex = this->compareStylesWithText(curs.charFormat(), curs.blockFormat(), "modifyZoom");
            else
                styleIndex = this->compareBaseStylesWithTextToUpdateIt(curs.charFormat(), curs.blockFormat());
        }
        //                qDebug() << "block bmargin : " << QString::number(textBlock.blockFormat().bottomMargin());
        //                qDebug() << "style bmargin : " << QString::number(this->blockBottomMarginAt(0));
        //                qDebug() << "block indent : " << QString::number(textBlock.blockFormat().indent());
        //                qDebug() << "style indent : " << QString::number(this->blockFirstLineIndentAt(0));
        //                qDebug() << "block lmargin : " << QString::number(textBlock.blockFormat().leftMargin());
        //                qDebug() << "style lmargin : " << QString::number(this->blockLeftMarginAt(0));
        //                qDebug() << "block align : " << QString::number(textBlock.blockFormat().alignment());
        //                qDebug() << "style align : " << QString::number(this->blockAlignmentTrueNameAt(0));
        //                qDebug() << "block size : " << QString::number(textBlock.charFormat().fontPointSize());
        //                qDebug() << "style size : " << QString::number(this->fontSizeAt(0));
        //                qDebug() << "block family : " << textBlock.charFormat().fontFamily();
        //                qDebug() << "style family : " << this->fontFamilyAt(0);

        //                qDebug() << "block number : " << QString::number(j);
        if(styleIndex != 99999){

            //                    qDebug() << "j++++++ fontSizeAt  : " << QString::number(this->fontSizeAt(styleIndex));

            QTextBlockFormat blockFormat;
            blockFormat.setBottomMargin(this->blockBottomMarginAt(styleIndex));
            blockFormat.setTextIndent(this->blockFirstLineIndentAt(styleIndex));
            blockFormat.setLeftMargin(this->blockLeftMarginAt(styleIndex));
            blockFormat.setAlignment(this->blockAlignmentTrueNameAt(styleIndex));
            blockFormat.setTopMargin(0);
            blockFormat.setRightMargin(0);
            QTextCharFormat charFormat;
            charFormat.setFontPointSize(this->fontSizeAt(styleIndex) + m_zoom); // zoom !
            charFormat.setFontFamily(this->fontFamilyAt(styleIndex));

            QTextCursor tCursor(doc);
            tCursor.setPosition(textBlock.position());

            // select all of the selected block :

            QTextCursor tStartCursor(doc);
            tStartCursor.setPosition(tCursor.selectionStart());
            tStartCursor.movePosition(QTextCursor::StartOfBlock);
            int startFirstBlock = tStartCursor.position();
            //                    qDebug() << "j---------- : " << QString::number(j) << "startBlock : "<< QString::number(startFirstBlock);

            QTextCursor tEndCursor(doc);
            tEndCursor.setPosition(tCursor.selectionEnd());
            tEndCursor.movePosition(QTextCursor::EndOfBlock);
            int endLastBlock = tEndCursor.position();
            //                    qDebug() << "j---------- : " << QString::number(j) << "endBlock : "<< QString::number(startFirstBlock);

            tCursor.setPosition(startFirstBlock);
            tCursor.setPosition(endLastBlock, QTextCursor::KeepAnchor);


            // merge :
            tCursor.mergeBlockFormat(blockFormat);
            tCursor.mergeCharFormat(charFormat);
        }



    }
}

//-----------------------------------------------------------------------
int TextStyles::compareStylesWithText(QTextCharFormat charFormat, QTextBlockFormat blockFormat, QString mode)
{

    int m_zoom;

    if(mode == "none")
        m_zoom = 0;
    else if(mode == "zoom")
        m_zoom  = zoom;
    else if(mode == "modifyZoom")
        m_zoom  = previousZoom;
    else
        m_zoom = 0;



    //    readSettings();

    QString family = charFormat.fontFamily();
    int size = charFormat.fontPointSize();

    int indent = blockFormat.textIndent();
    int leftMargin = blockFormat.leftMargin();
    int bottomMargin = blockFormat.bottomMargin();
    Qt::Alignment alignment = blockFormat.alignment();
    int align;
    if(alignment == Qt::AlignLeft)
        align = 0;
    else if(alignment == Qt::AlignHCenter)
        align = 1;
    else if(alignment == Qt::AlignRight)
        align = 2;
    else if(alignment == Qt::AlignJustify)
        align = 3;


    for (int i = 0; i < fontFamilyList.size(); ++i) {

        if(fontFamilyList.at(i) == family && fontSizeList.at(i) == size - m_zoom
                && blockFirstLineIndentList.at(i) == indent && blockLeftMarginList.at(i) == leftMargin
                && blockBottomMarginList.at(i) == bottomMargin && blockAlignmentList.at(i) == align)
            return i;
    }

    return 99999; //99999 is number if no matching style is found




}
//-----------------------------------------------------------------------
int TextStyles::compareBaseStylesWithTextToUpdateIt(QTextCharFormat charFormat, QTextBlockFormat blockFormat)
{

    QString family = charFormat.fontFamily();
    int size = charFormat.fontPointSize();

    int indent = blockFormat.textIndent();
    int leftMargin = blockFormat.leftMargin();
    int bottomMargin = blockFormat.bottomMargin();
    Qt::Alignment alignment = blockFormat.alignment();
    int align;
    if(alignment == Qt::AlignLeft)
        align = 0;
    else if(alignment == Qt::AlignHCenter)
        align = 1;
    else if(alignment == Qt::AlignRight)
        align = 2;
    else if(alignment == Qt::AlignJustify)
        align = 3;


    for (int i = 0; i < baseFontFamilyList.size(); ++i) {

        if(baseFontFamilyList.at(i) == family && baseFontSizeList.at(i) == size
                && baseBlockFirstLineIndentList.at(i) == indent && baseBlockLeftMarginList.at(i) == leftMargin
                && baseBlockBottomMarginList.at(i) == bottomMargin && baseBlockAlignmentList.at(i) == align && i < fontSizeList.size())

            return i;

    }

    return 99999; //99999 is number if no matching style is found




}
//-----------------------------------------------------------------------

void TextStyles::readBaseSettings()
{

    baseNameList.clear();
    baseFontFamilyList.clear();
    baseFontSizeList.clear();
    baseBlockFirstLineIndentList.clear();
    baseBlockBottomMarginList.clear();
    baseBlockLeftMarginList.clear();
    baseBlockAlignmentList.clear();
    baseFontItalicList.clear();
    baseFontBoldList.clear();
    baseFontUnderlineList.clear();
    baseFontStrikeOutList.clear();

    QDomNodeList baseNodesList = baseStyleElem.elementsByTagName("baseStyle");
    int size = baseNodesList.size();


    for (int i = 0; i < size; ++i) {
        QDomElement child = baseNodesList.at(i).toElement();

        baseNameList.append(child.attribute("name",tr("New Style")));
        baseFontFamilyList.append(child.attribute("fontFamily","Liberation Serif"));
        baseFontSizeList.append(child.attribute("fontSize","12").toInt());
        baseBlockFirstLineIndentList.append(child.attribute("blockFirstLineIndent", "20").toInt());
        baseBlockBottomMarginList.append(child.attribute("blockBottomMargin", "10").toInt());
        baseBlockLeftMarginList.append(child.attribute("blockLeftMargin", "0").toInt());
        baseBlockAlignmentList.append(child.attribute("blockAlignment", "0").toInt());
        baseFontItalicList.append(stringToBool(child.attribute("fontItalic", "false")));
        baseFontBoldList.append(stringToBool(child.attribute("fontBold")));
        baseFontUnderlineList.append(stringToBool(child.attribute("fontUnderline")));
        baseFontStrikeOutList.append(stringToBool(child.attribute("fontStrikeOut")));

    }

    baseStyleElem.attribute("baseDefaultStyleIndex", "0");

}


//-----------------------------------------------------------------------

void TextStyles::readSettings()
{


    nameList.clear();
    fontFamilyList.clear();
    fontSizeList.clear();
    blockFirstLineIndentList.clear();
    blockBottomMarginList.clear();
    blockLeftMarginList.clear();
    blockAlignmentList.clear();
    fontItalicList.clear();
    fontBoldList.clear();
    fontUnderlineList.clear();
    fontStrikeOutList.clear();

    QDomNodeList nodesList = styleElem.elementsByTagName("style");
    int size = nodesList.size();

    if(size == 0){
        //writing default styles : normal and poetry

        QDomElement elem = domDocument.createElement("style");
        QDomElement child = styleElem.appendChild(elem).toElement();


        child.setAttribute("name", tr("Normal"));
        child.setAttribute("fontFamily","Liberation Serif");
        child.setAttribute("fontSize","12");
        child.setAttribute("blockFirstLineIndent","20");
        child.setAttribute("blockBottomMargin","10");
        child.setAttribute("blockLeftMargin","0");
        child.setAttribute("blockAlignment","0");
        child.setAttribute("fontItalic","false");
        child.setAttribute("fontBold","false");
        child.setAttribute("fontUnderline","false");
        child.setAttribute("fontStrikeOut","false");

        elem = domDocument.createElement("style");
        child = styleElem.appendChild(elem).toElement();

        child.setAttribute("name", tr("Poetry"));
        child.setAttribute("fontFamily","Liberation Serif");
        child.setAttribute("fontSize","12");
        child.setAttribute("blockFirstLineIndent","0");
        child.setAttribute("blockBottomMargin","0");
        child.setAttribute("blockLeftMargin","50");
        child.setAttribute("blockAlignment","0");
        child.setAttribute("fontItalic","false");
        child.setAttribute("fontBold","false");
        child.setAttribute("fontUnderline","false");
        child.setAttribute("fontStrikeOut","false");


        styleElem.setAttribute("defaultStyleIndex", "0");

        nodesList = styleElem.elementsByTagName("style");
        size = nodesList.size();



    }



    for (int j = 0; j < size ; ++j) {
        QDomElement child = nodesList.at(j).toElement();


        nameList.append(child.attribute("name",tr("New Style")));
        fontFamilyList.append(child.attribute("fontFamily","Liberation Serif"));
        fontSizeList.append(child.attribute("fontSize","12").toInt());
        blockFirstLineIndentList.append(child.attribute("blockFirstLineIndent", "20").toInt());
        blockBottomMarginList.append(child.attribute("blockBottomMargin", "10").toInt());
        blockLeftMarginList.append(child.attribute("blockLeftMargin", "0").toInt());
        blockAlignmentList.append(child.attribute("blockAlignment", "0").toInt());
        fontItalicList.append(stringToBool(child.attribute("fontItalic")));
        fontBoldList.append(stringToBool(child.attribute("fontBold")));
        fontUnderlineList.append(stringToBool(child.attribute("fontUnderline")));
        fontStrikeOutList.append(stringToBool(child.attribute("fontStrikeOut")));
    }
    defaultStyle = styleElem.attribute("defaultStyleIndex", "0").toInt();



}


//-----------------------------------------------------------------------------------


void TextStyles::saveBaseStyles()
{
    //         save the style at start (named "baseStyles")



    QDomNodeList oldNodesList = baseStyleElem.childNodes();

    while(!oldNodesList.isEmpty()) {
        baseStyleElem.removeChild(oldNodesList.at(0));
    }

    int nameListSize = nameList.size();
    for (int j = 0; j < nameListSize ; ++j) {

        QDomElement child = domDocument.createElement("baseStyle");
        baseStyleElem.appendChild(child);

        child.setAttribute("name",nameList[j]);
        child.setAttribute("fontFamily",fontFamilyList[j]);
        child.setAttribute("fontSize",QString::number(fontSizeList[j]));
        child.setAttribute("blockFirstLineIndent",QString::number(blockFirstLineIndentList[j]));
        child.setAttribute("blockBottomMargin",QString::number(blockBottomMarginList[j]));
        child.setAttribute("blockLeftMargin",QString::number(blockLeftMarginList[j]));
        child.setAttribute("blockAlignment", QString::number(blockAlignmentList[j]));
        child.setAttribute("fontItalic",QString(fontItalicList[j]?"true":"false"));
        child.setAttribute("fontBold",QString(fontBoldList[j]?"true":"false"));
        child.setAttribute("fontUnderline",QString(fontUnderlineList[j]?"true":"false"));
        child.setAttribute("fontStrikeOut",QString(fontStrikeOutList[j]?"true":"false"));
    }
    baseStyleElem.setAttribute("baseDefaultStyleIndex", QString::number(defaultStyle));

    writeDomDoc();


}

//-----------------------------------------------------------------------------------

void TextStyles::saveStyles()
{



    QDomNodeList oldNodesList = styleElem.childNodes();


    while(!oldNodesList.isEmpty()) {
        styleElem.removeChild(oldNodesList.at(0));
    }

    int nameListSize = nameList.size();
    for (int j = 0; j < nameListSize ; ++j) {

        QDomElement child = domDocument.createElement("style");
        styleElem.appendChild(child);

        child.setAttribute("name",nameList[j]);
        child.setAttribute("fontFamily",fontFamilyList[j]);
        child.setAttribute("fontSize",QString::number(fontSizeList[j]));
        child.setAttribute("blockFirstLineIndent",QString::number(blockFirstLineIndentList[j]));
        child.setAttribute("blockBottomMargin",QString::number(blockBottomMarginList[j]));
        child.setAttribute("blockLeftMargin",QString::number(blockLeftMarginList[j]));
        child.setAttribute("blockAlignment", QString::number(blockAlignmentList[j]));
        child.setAttribute("fontItalic",QString(fontItalicList[j]?"true":"false"));
        child.setAttribute("fontBold",QString(fontBoldList[j]?"true":"false"));
        child.setAttribute("fontUnderline",QString(fontUnderlineList[j]?"true":"false"));
        child.setAttribute("fontStrikeOut",QString(fontStrikeOutList[j]?"true":"false"));
    }
    styleElem.setAttribute("defaultStyleIndex", QString::number(defaultStyle));

    writeDomDoc();



}

bool TextStyles::stringToBool(QString value)
{
    if(value == "true")
        return true;
    else
        return false;

}

QString TextStyles::boolToString(bool value)
{
    if(value == true)
        return "true";
    else
        return "false";
}

void TextStyles::writeDomDoc()
{
//    qDebug() << hub->infoTreeDomDoc().toString();
    hub->addToSaveQueue();
}
