#ifndef SWEEPERFIELDWIDGET_H
#define SWEEPERFIELDWIDGET_H

#include <QWidget>

#include "Sweeper.h"

class SweeperFieldWidget : public QWidget
{
    Q_OBJECT

    Sweeper *sweeper;

    float plotToWidgetScale;
    float widgetToPlotScale;

    int horizontalBorder;
    int verticalBorder;

    const int cellWidth;

    int margin;

    const int plotWidth;

    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent * event);
    void mouseReleaseEvent(QMouseEvent *e);

    void drawMine(QPainter &p, int x, int y);
    void drawExplosion(QPainter &p, int x, int y);
    void drawMineFlag(QPainter &p, int x, int y);
    void drawDoubtFlag(QPainter &p, int x, int y);

    QColor getCellColor(const QPoint &point);

public:
    SweeperFieldWidget(Sweeper *sweeper, QWidget *parent = 0);

signals:

public slots:

};

#endif // SWEEPERFIELDWIDGET_H
