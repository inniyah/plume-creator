#ifndef TEXTHIGHLIGHTER_H
#define TEXTHIGHLIGHTER_H

#include <QSyntaxHighlighter>


class TextHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    TextHighlighter(QTextDocument *parentDoc = 0);
    void setTextToHighlight(QString string);
    void setCaseSensitivity(bool isCaseSensitive);

protected:
    void highlightBlock(const QString &text);
signals:

public slots:
    
private:
    QString textToHighLight;
    Qt::CaseSensitivity sensitivity;
};

#endif // TEXTHIGHLIGHTER_H
