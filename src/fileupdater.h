#ifndef FILEUPDATER_H
#define FILEUPDATER_H

#include <QObject>
#include <QtGui>
#include <QDomDocument>


class FileUpdater : public QWidget
{
    Q_OBJECT
public:
    explicit FileUpdater(QWidget *parent = 0);
    bool checkAttendanceFile(QString projectFileName);
    bool checkTreeFile(QString projectFileName);
    bool checkInfoFile(QString projectFileName);
    static bool isZip(QString projectFileName);

    QString getAttendVersion(){return attendVersion;}
    QString getTreeVersion(){return treeVersion;}
    QString getInfoVersion(){return infoVersion;}

signals:
    
public slots:

private slots:
    void updateAttendanceFile();
    void updateTreeFile();
    void updateInfoFile();

private:
    QFile *attFile;
    QFile *treeFile;
    QFile *infoFile;
    QDomDocument attendDomDocument;
    QDomElement attendRoot;
    QDomDocument treeDomDocument;
    QDomElement treeRoot;
    QDomDocument infoDomDocument;
    QDomElement infoRoot;
    QString devicePath;
    QString attendVersion;
    QString treeVersion;
    QString infoVersion;


};

#endif // FILEUPDATER_H
