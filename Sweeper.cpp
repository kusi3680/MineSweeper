#include "Sweeper.h"

#include "SweeperPrivate.h"

/*
 *
 * Next Step: make Sweeper a child of sweeper private
 *
 * */


// cunstructor
Sweeper::Sweeper(int side, int mines, QObject *parent) :

    QObject(parent),
    side(side),
    mines(mines),
    privateMembers(new SweeperPrivate(side, mines, this))
{
    connect(privateMembers, SIGNAL(bombed()), SIGNAL(bombed()));
    connect(privateMembers, SIGNAL(win()), SIGNAL(win()));
}

int Sweeper::getSide() const
{
    return privateMembers->getSide();
}

int Sweeper::getEstimatedFlags() const
{
    return privateMembers->getEstimatedFlags();
}

int Sweeper::getTimeSeconds()const
{
    return privateMembers->getTimeSeconds();
}

void Sweeper::click(const QPoint &point)
{
    privateMembers->click(point);
}

/**
 * Sets flag to specified cell.
 * @param point - Cell index.
 */
void Sweeper::toggleFlag(const QPoint &point)
{
    privateMembers->toggleFlag(point);
}

/**
 *  true if flag seted to this cell.
 */
Flag Sweeper::getFlag(const QPoint &point) const
{
    return privateMembers->getFlag(point);
}

/**
 *  Returns true if cell was opened.
 */
bool Sweeper::isOpended(const QPoint &point) const
{
    return privateMembers->isOpended(point);
}

bool Sweeper::isMined(const QPoint &point) const
{
    return privateMembers->isMined(point);
}

bool Sweeper::isExploded(const QPoint &point) const
{
    return privateMembers->isExploded(point);
}

int Sweeper::getNeighborMines(const QPoint &point)const
{
    return privateMembers->getNeiMines(point);
}

// new game
void Sweeper::restartSlot()
{
    delete privateMembers;
    privateMembers = new SweeperPrivate(side, mines, this);
    connect(privateMembers, SIGNAL(bombed()), SIGNAL(bombed()));
    connect(privateMembers, SIGNAL(win()), SIGNAL(win()));
}
