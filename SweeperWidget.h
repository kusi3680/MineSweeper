#ifndef SWEEPERWIDGET_H
#define SWEEPERWIDGET_H

#include <QMainWindow>
#include <QLCDNumber>
#include <QLabel>

#include "Sweeper.h"

class SweeperWidget : public QMainWindow
{
    Q_OBJECT

    Sweeper * sweeper;
    long timeSeconds;
    QLCDNumber *flagsEstimationLcd;
    QLCDNumber *timerLcd;
    QLabel *smileLabel;

    public:
        explicit SweeperWidget(Sweeper *sweeper, QWidget *parent = 0);

    signals:

    public slots:

    private slots:
        void bombed();
        void win();

        void update();
        void restartSlot();
};

#endif // SWEEPERWIDGET_H
