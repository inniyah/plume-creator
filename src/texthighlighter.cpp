#include "texthighlighter.h"

TextHighlighter::TextHighlighter(QTextDocument *parentDoc) :
    QSyntaxHighlighter(parentDoc)
{
}
void TextHighlighter::highlightBlock(const QString &text)
{
    if(textToHighLight.isEmpty())
        return;

    QTextCharFormat foundTextFormat;
    foundTextFormat.setBackground(Qt::yellow);


    setCurrentBlockState(0);
    int position = 0;
    while(position >= 0){

        setFormat(text.indexOf(textToHighLight, position, sensitivity), textToHighLight.size(), foundTextFormat);
        position = text.indexOf(textToHighLight, position + textToHighLight.size(), sensitivity) ;
    }
    setCurrentBlockState(1);

}

void TextHighlighter::setTextToHighlight(QString string)
{
    textToHighLight = string;
    this->rehighlight();
}
void TextHighlighter::setCaseSensitivity(bool isCaseSensitive)
{
    if(isCaseSensitive){
    sensitivity = Qt::CaseSensitive;
  }
    else{

        sensitivity = Qt::CaseInsensitive;
    }
}
