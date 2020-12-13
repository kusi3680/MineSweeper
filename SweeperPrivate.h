#ifndef SWEEPERPRIVATE_H
#define SWEEPERPRIVATE_H

#include <QObject>
#include <QDateTime>
#include <QTimer>

#include "SweeperGameField.h"

class SweeperPrivate : public QObject
{
    Q_OBJECT
    public: // all these methods are used in sweeper. need to make it a child of this
        SweeperPrivate(int side, int mines, QObject *parent = 0);
        int getSide()const;
        int getEstimatedFlags() const;

        void click(const QPoint &point);
        void toggleFlag(const QPoint &point);
        Flag getFlag(const QPoint &point)const;
        bool isOpended(const QPoint &point)const;
        bool isMined(const QPoint &point)const;
        bool isExploded(const QPoint &point)const;
        int getNeiMines(const QPoint &point)const; //Nei = neighbor
        int getTimeSeconds()const;

    signals:
        void bombed(); // when clicked on bomb
        void win(); // when all correctly flagged

    private slots:
        void update();
        void bombedSlot();
        void winSlot();

    private:
        const int side; // side of mines, change to change box size
        const int mines; // chnage to change difficulty
        int time; // time taken, NEXT STEP: CALCULATE SCORE
        SweeperGameField *gameField; // map
        QDateTime startTime;
        QTimer *timer;
        void createFieldAndTimer(const QPoint &point); //RANDOMLY CREATES MAP
};

#endif // SWEEPERPRIVATE_H
