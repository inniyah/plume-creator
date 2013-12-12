#ifndef SPELLCHECKER_H
#define SPELLCHECKER_H

#include <QString>
#include <QObject>
#include <QHash>
#include <QTextStream>
#include <QStringList>


class Hunspell;

class SpellChecker : public QObject
{
    Q_OBJECT
public:

    SpellChecker(QObject *parent);
    ~SpellChecker();
    void setDict(const QString &dictionaryPath, const QStringList &userDictionary, const QStringList &attendTree_names);

    bool spell(const QString &word);
    QStringList suggest(const QString &word);
    void ignoreWord(const QString &word);
    void addToUserWordlist(const QString &word);

    bool isActive(){return m_isActive;}
    void activate();
    void deactivate();

    static QStringList dictsPaths();
    static QHash<QString, QString> dictsList();

    bool isInUserWordlist(QString &word);
    void removeFromUserWordlist(const QString &word);

    // fix bug when hunspell gives me latin1 encoded results on several Linux systems :
    QString testHunspellForEncoding();

signals:
  void userDictSignal(QStringList userDict);

private:
    void put_word(const QString &word);
    Hunspell *_hunspell;
    bool m_isActive, hunspellLaunched;
    QStringList userDict;


    QString encodingFix;
};

#endif // SPELLCHECKER_H
