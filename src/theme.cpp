#include "theme.h"

Theme::Theme(QObject *parent) :
    QObject(parent)
{
    m_name = "empty theme";
    m_tempPath = "";
    m_path = "";
    m_qss = "";
}

QString Theme::name() const
{
    return m_name;
}

void Theme::setName(const QString &name)
{
    m_name = name;
}


QString Theme::qss() const
{
    return m_qss;
}

void Theme::setQss(const QString &qss)
{
    m_qss = qss;
}

QString Theme::tempPath() const
{
    return m_tempPath;
}

void Theme::setTempPath(const QString &tempPath)
{
    m_tempPath = tempPath;
}

QString Theme::picsPath() const
{
    return m_tempPath + "/pics/";
}


QString Theme::path() const
{
    return m_path;
}

void Theme::setPath(const QString &path)
{
    m_path = path;
}

//QHash<QString, QString> Theme::picMatches() const
//{
//    return m_picMatches;
//}


//void Theme::setPicMatches(const QHash<QString, QString> &picMatches)
//{
//    m_picMatches = picMatches;
//}

