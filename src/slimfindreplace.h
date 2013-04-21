#ifndef SLIMFINDREPLACE_H
#define SLIMFINDREPLACE_H

#include <QWidget>

#include "maintextdocument.h"
#include "hub.h"

namespace Ui {
class SlimFindReplace;
}

class SlimFindReplace : public QWidget
{
    Q_OBJECT
    
public:
    explicit SlimFindReplace(QWidget *parent = 0);
    ~SlimFindReplace();
    void setDocument(MainTextDocument *doc);
    void setTextEdit(QTextEdit *txtEdit);

protected:


public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void closeSlot();

private slots:

    void on_findLineEdit_textChanged(const QString &arg1);

    void on_replaceLineEdit_textChanged(const QString &arg1);

    void on_closeButton_clicked();

    void on_previousButton_clicked();

    void on_replaceAndNextButton_clicked();

    void on_nextButton_clicked();


    void on_caseSensitiveCheckBox_toggled(bool checked);

private:
    void applyConfig();
    void saveConfig();
    QList<int> findTextPos(QString text);

    Ui::SlimFindReplace *ui;
    Hub *hub;
    MainTextDocument *document;
    QTextEdit *textEdit;
    QString findText;
    QString replaceText;
    QList<int> positionsList;
    bool caseSensitive;
    QTextDocument::FindFlags findFlags;
    bool allowRestartFromBeginning, allowRestartFromEnd;
    int oldPosition;

};

#endif // SLIMFINDREPLACE_H
