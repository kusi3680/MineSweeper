#ifndef SWEEPER_H
#define SWEEPER_H

#include <QObject>
#include <QPoint>

#include "FlagState.h"

class SweeperPrivate;

class Sweeper : public QObject
{
    Q_OBJECT
    const int side; // number of boxes per side
    const int mines; // number of mines in game

    SweeperPrivate *privateMembers; //has all the game info,

    public:
        Sweeper(int side, int mines, QObject *parent = 0); // constructor
        int getSide() const; // getter for side
        void click(const QPoint &point);

        void toggleFlag(const QPoint &point); 


        Flag getFlag(const QPoint &point)const;


        bool isOpended(const QPoint &point)const;
        bool isMined(const QPoint &point) const;
        bool isExploded(const QPoint &point) const;

        int getNeighborMines(const QPoint &point) const;

        int getEstimatedFlags() const;
        int getTimeSeconds() const;
    signals:
        bool bombed();
        void win();

    public slots:
        void restartSlot();

};

#endif // SWEEPER_H
