#include "SweeperPrivate.h"

#include <assert.h>

// constructor
SweeperPrivate::SweeperPrivate(int side, int mines, QObject *parent) :
    QObject(parent),
    side(side),
    mines(mines),
    time(0),
    gameField(nullptr),
    startTime(QDateTime::currentDateTime()),
    timer(nullptr)
{
}

int SweeperPrivate::getSide() const
{
    return side;
}

int SweeperPrivate::getEstimatedFlags()const
{
    if(gameField == nullptr){
        return mines;
    }else{
        return gameField->getEstimatedFlags();
    }
}

// updates timer on top
void SweeperPrivate::createFieldAndTimer(const QPoint &point)
{
    assert(timer == nullptr);
    assert(gameField == nullptr);
    gameField = new SweeperGameField(side, mines, point, this);
    connect(gameField, SIGNAL(bombed()), SLOT(bombedSlot()));
    connect(gameField, SIGNAL(win()), SLOT(winSlot()));

    timer = new QTimer(this);
    timer->start(100);
    connect(timer, SIGNAL(timeout()), SLOT(update()));
}

void SweeperPrivate::click(const QPoint &point)
{
    if(gameField == nullptr){
        createFieldAndTimer(point);
    }
    gameField->click(point);
}

void SweeperPrivate::toggleFlag(const QPoint &point)
{
    if(gameField == nullptr){
    }else{
        gameField->toggleFlag(point);
    }
}

Flag SweeperPrivate::getFlag(const QPoint &point) const
{
    if(gameField == nullptr){
        return Flag::NO;
    }else{
        return gameField->getFlag(point);
    }
}

bool SweeperPrivate::isOpended(const QPoint &point) const
{
    if(gameField == nullptr){
        return false;
    }else{
        return gameField->isOpended(point);
    }
}

bool SweeperPrivate::isMined(const QPoint &point) const
{
    if(gameField == nullptr){
        return false;
    }else{
        return gameField->isMined(point);
    }
}

bool SweeperPrivate::isExploded(const QPoint &point) const
{
    if(gameField == nullptr){
        return false;
    }else{
        return gameField->isExploded(point);
    }
}

int SweeperPrivate::getNeiMines(const QPoint &point) const
{
    if(gameField == nullptr){
        return 0;
    }else{
        return gameField->getNeiMines(point);
    }
}

// obsolete?
int SweeperPrivate::getTimeSeconds() const
{
    return time;
}

 // SLOTS:

void SweeperPrivate::update()
{
    time = startTime.secsTo(QDateTime::currentDateTime());
}

void SweeperPrivate::bombedSlot()
{
    assert(timer != nullptr);
    timer->stop();
    emit bombed();
}

void SweeperPrivate::winSlot()
{
    assert(timer != nullptr);
    timer->stop();
    emit win();
}
