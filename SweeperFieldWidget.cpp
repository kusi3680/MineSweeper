#include "SweeperFieldWidget.h"

#include <QPainter>
#include <QTimer>
#include <QMouseEvent>

#include <algorithm>
#include <assert.h>

SweeperFieldWidget::SweeperFieldWidget(Sweeper *sweeper, QWidget *parent) :
    QWidget(parent),
    sweeper(sweeper),
    plotToWidgetScale(0),
    widgetToPlotScale(0),
    horizontalBorder(0),
    verticalBorder(0),
    cellWidth(64),
    margin(2),
    plotWidth(cellWidth * sweeper->getSide() + margin * 2)
{
    QTimer *timer = new QTimer(this);
    timer->start(100);
    connect(timer, SIGNAL(timeout()), SLOT(update()));

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(500, 500);
}

void SweeperFieldWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter p(this);
    p.scale(plotToWidgetScale, plotToWidgetScale);
    p.translate(horizontalBorder, verticalBorder);
    QPen pen(Qt::black);
    pen.setWidth(plotWidth / 1000);
    p.setPen(pen);
    QFont f;
    f.setPixelSize(plotWidth / 100 * 4);
    p.setFont(f);

    for(int row = 0; row < sweeper->getSide(); row++){
        for(int col = 0; col < sweeper->getSide(); col++){
            QPoint point(col, row);

            p.setBrush(getCellColor(point));

            int x = cellWidth * col;
            int y = cellWidth * row;
            p.drawRect(x, y, cellWidth, cellWidth);

            if(sweeper->isOpended(point)){
                if(sweeper->isMined(point)){
                    if(sweeper->isExploded(point)){
                        drawExplosion(p, x, y);
                    }else{
                        drawMine(p, x, y);
                    }
                }else{
                    if(sweeper->getNeighborMines(point) > 0){
                        p.drawText(QRect(x, y, cellWidth, cellWidth),
                                   QString::number(sweeper->getNeighborMines(point)),
                                   QTextOption(Qt::AlignCenter));
                    }
                }
            }
            switch (sweeper->getFlag(point)) {
            case Flag::NO: // Nothing to do
                break;
            case Flag::MINE:drawMineFlag(p, x, y);
                break;
            case Flag::DOUBT:drawDoubtFlag(p, x, y);
                break;
            default: assert(false);
                break;
            }
        }
    }
}

QColor SweeperFieldWidget::getCellColor(const QPoint &point)
{
    if(sweeper->getFlag(point) != Flag::NO){
        return Qt::yellow;
    }else if(sweeper->isOpended(point)){
            return Qt::transparent;
    }else{
        return Qt::lightGray;
    }
}

void SweeperFieldWidget::resizeEvent(QResizeEvent * event)
{
    int h = event->size().height();
    int w = event->size().width();

    int side = std::min(h, w);

    widgetToPlotScale = float(plotWidth) / side;
    plotToWidgetScale = float(side) / plotWidth;
    horizontalBorder = (w / plotToWidgetScale -
                        cellWidth * sweeper->getSide())/ 2;
    verticalBorder = (h / plotToWidgetScale -
                      cellWidth * sweeper->getSide())/ 2;
}

void SweeperFieldWidget::mouseReleaseEvent(QMouseEvent *e)
{
    int xOnPlot = e->pos().x() * widgetToPlotScale - horizontalBorder;
    int yOnPlot = e->pos().y() * widgetToPlotScale - verticalBorder;

    int col = xOnPlot / cellWidth;
    int row = yOnPlot / cellWidth;

    bool onField = (col >= 0) && (col < sweeper->getSide()) &&
            (row >= 0) && (row < sweeper->getSide());

    if(onField){
        QPoint cell(col, row);

        if(e->button() == Qt::LeftButton){
            sweeper->click(cell);
        }else if(e->button() == Qt::RightButton){
            sweeper->toggleFlag(cell);
        }
    }
}

void SweeperFieldWidget::drawExplosion(QPainter &p, int x, int y)
{
    p.drawPixmap(QRect(x, y, cellWidth, cellWidth),
                 QPixmap(":/icons/icons/boom.png"));
}

void SweeperFieldWidget::drawMine(QPainter &p, int x, int y)
{
    p.drawPixmap(QRect(x, y, cellWidth, cellWidth),
                 QPixmap(":/icons/icons/mine.png"));
}

void SweeperFieldWidget::drawMineFlag(QPainter &p, int x, int y)
{
    p.drawPixmap(QRect(x, y, cellWidth, cellWidth),
                 QPixmap(":/icons/icons/red-flag.png"));
}

void SweeperFieldWidget::drawDoubtFlag(QPainter &p, int x, int y)
{
    p.drawPixmap(QRect(x, y, cellWidth, cellWidth),
                 QPixmap(":/icons/icons/doubt-flag.png"));
}
