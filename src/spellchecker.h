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

    SpellChecker();
    ~SpellChecker();
    void setDict(const QString &dictionaryPath, const QString &userDictionary);

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
  void userDictSignal(QString userDictString);

private:
    void put_word(const QString &word);
    Hunspell *_hunspell;
    QString _userDictionary;
    bool m_isActive;
    QString userDictString;
    QStringList userDictStringList;

    QString encodingFix;
};

#endif // SPELLCHECKER_H
