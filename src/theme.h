#ifndef THEME_H
#define THEME_H

#include <QObject>

class Theme : public QObject
{
    Q_OBJECT
public:
    explicit Theme(QObject *parent = 0);
    
    QString name() const;
    void setName(const QString &name);

    QString tempPath() const;
    void setTempPath(const QString &tempPath);

    QString qss() const;
    void setQss(const QString &qss);

    QString picsPath() const;

//    QHash<QString, QString> picMatches() const;
//    void setPicMatches(const QHash<QString, QString> &picMatches);

    QString path() const;
    void setPath(const QString &path);

signals:
    
public slots:

private:
    QString m_name, m_tempPath, m_qss, m_path;
//    QHash<QString, QString> m_picMatches; // QString objectName, QString picFile
    
};

#endif // THEME_H
