#ifndef TEXTCELL_H
#define TEXTCELL_H

#include <QWidget>
#include "maintextdocument.h"

namespace Ui {
class TextCell;
}

class TextCell : public QWidget
{
    Q_OBJECT

public:
    explicit TextCell(QWidget *parent = 0);
    ~TextCell();
    void setTextDocument(MainTextDocument *doc);


    QString title() const;
    void setTitle(const QString &title);

    int id() const;
    void setId(int id);

    QString type() const;
    void setType(const QString &type);

signals:
    void openSheet(int);

private slots:
    void openSheet();

private:
    Ui::TextCell *ui;
    QString m_title;
    QString m_type;
    int m_id;
};

#endif // TEXTCELL_H
