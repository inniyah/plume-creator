#ifndef FULLSCREENEDITOR_H
#define FULLSCREENEDITOR_H

#include <QTextDocument>

#include <QWidget>

class FullscreenEditor : public QWidget
{
    Q_OBJECT
public:
    explicit FullscreenEditor(QWidget *parent = 0, QTextDocument *doc = 0);

signals:

public slots:

};

#endif // FULLSCREENEDITOR_H
