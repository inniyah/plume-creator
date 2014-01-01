#ifndef MINIMAP_H
#define MINIMAP_H

#include <QWidget>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif
#include <QtGui>

namespace Ui {
class Minimap;
}
class Minimap : public QWidget
{
    Q_OBJECT
public:
    explicit Minimap(QWidget *parent = 0);
    ~Minimap();

    void setWidgetInProxy(QWidget *widget);
    void resizeContent();
signals:

public slots:
void setTextThumbnail(QPixmap thumbnail);

private:
   Ui::Minimap *ui;
//   QGraphicsProxyWidget *proxy;
   QGraphicsPixmapItem *thumbItem;
   QGraphicsScene scene;
};

#endif // MINIMAP_H
