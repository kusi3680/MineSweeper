#include "SweeperGameField.h"

#include <time.h>
#include <assert.h>
#include <algorithm>

// initial state
FieldCell::FieldCell() :
    mined(false), neiMined(0), flag(Flag::NO), opened(false), bombed(false)
{
}

// Construct
SweeperGameField::SweeperGameField(int side, int mines, const QPoint &freeCell,
                                 QObject *parent) :
    QObject(parent),
    randomEngine(time(NULL)),
    side(side),
    mines(mines),
    flags(0),
    gameContinues(true)
{
    map.resize(side * side);

    placeMines(mines, freeCell);
}

// Delete
SweeperGameField::~SweeperGameField()
{
}

// place mines randomly, all helper functions defined below
void SweeperGameField::placeMines(const int mines, const QPoint &freeCell)
{
    for(int i = 0; i < mines; i++){
        QPoint point;

        do{
            point = randomPoint();
        }while((point == freeCell) ||
               (getCell(point).mined) ||
               areNeighbors(point, freeCell));

        placeMine(point);
    }
}

// places mine at specified point
void SweeperGameField::placeMine(const QPoint &point)
{
    FieldCell &cell = getCell(point);
    cell.mined = true;
    markNeighbors(point);
}

void SweeperGameField::markNeighbors(const QPoint &point)
{
    for(int i = 0; i < 3; i++){
        markNeighbor(QPoint(point.x() - 1 + i, point.y() + 1));
        markNeighbor(QPoint(point.x() - 1 + i, point.y() - 1));
    }
    markNeighbor(QPoint(point.x() - 1, point.y()));
    markNeighbor(QPoint(point.x() + 1, point.y()));
}

void SweeperGameField::markNeighbor(const QPoint &point)
{
    if(exists(point)){
        (getCell(point).neiMined)++;
    }
}

bool SweeperGameField::exists(const QPoint &point) const
{
    bool xOk = (point.x() >= 0) && (point.x() < side);
    bool yOk = (point.y() >= 0) && (point.y() < side);

    return xOk && yOk;
}

QPoint SweeperGameField::randomPoint() const
{
    int randomNumber = randomEngine() % (side * side);
    int row = randomNumber / side;
    int col = randomNumber % side;

    return QPoint(col, row);
}


FieldCell &SweeperGameField::getCell(const QPoint &point)
{
    return map[point.y() * side + point.x()];
}

// same with const
const FieldCell &SweeperGameField::getCell(const QPoint &point) const
{
    return map[point.y() * side + point.x()];
}

bool SweeperGameField::allFreeOpened()const
{
    return std::none_of(map.begin(), map.end(),
                        [](auto tile){return (!tile.opened) && (!tile.mined);});
}

void SweeperGameField::openNotMinedNeighbors(const QPoint &point)
{
    QQueue<QPoint> queue;
    QList<QPoint> openedList;

    enqueueNeighbors(point, queue, openedList);
    while(!queue.empty()){
        QPoint point= queue.takeFirst();
        FieldCell &cell = getCell(point);

        cell.opened = true;
        openedList.append(point);
        if(cell.neiMined == 0){
            enqueueNeighbors(point, queue, openedList);
        }
    }
}

void SweeperGameField::enqueueNeighbors(const QPoint &point,
                                       QQueue<QPoint> &queue,
                                       QList<QPoint> &openedList)
{
    for(int i = 0; i < 3; i++){
        enqueueNeighbor(QPoint(point.x() - 1 + i, point.y() + 1), queue, openedList);
        enqueueNeighbor(QPoint(point.x() - 1 + i, point.y() - 1), queue, openedList);
    }
    enqueueNeighbor(QPoint(point.x() - 1, point.y()), queue, openedList);
    enqueueNeighbor(QPoint(point.x() + 1, point.y()), queue, openedList);
}

void SweeperGameField::enqueueNeighbor(const QPoint &point,
                                      QQueue<QPoint> &queue,
                                      QList<QPoint> &openedList)
{
    if(exists(point) && (!openedList.contains(point)) &&
            (!getCell(point).mined)){
        queue.enqueue(point);
    }
}

void SweeperGameField::openAll()
{
    for(auto &tile : map){
        tile.opened = true;
    }
}

void SweeperGameField::flagAllMines()
{
    for(auto &tile : map){
        if(tile.mined && (tile.flag == Flag::NO)){
            toggleFlag(tile);
        }
    }
}

void SweeperGameField::toggleFlag(const QPoint &point)
{
    toggleFlag(getCell(point));
}

void SweeperGameField::toggleFlag(FieldCell &cell)
{
    if(gameContinues){
        switch (cell.flag) {
        case Flag::NO: cell.flag = Flag::MINE;
            flags++;
            break;
        case Flag::MINE: cell.flag = Flag::DOUBT;
            flags--;
            break;
        case Flag::DOUBT: cell.flag = Flag::NO;
            break;
        default: assert(false);
            break;
        }
    }
}

void SweeperGameField::click(const QPoint &point)
{
    if(gameContinues){
        FieldCell &cell = getCell(point);
        if(!cell.opened){
            cell.opened = true;

            if(cell.mined){
                cell.bombed = true;
                openAll();
                gameContinues = false;
                emit bombed();
            }else{
                // Open neighbors only if clicked to free space
                if(getNeiMines(point) == 0){
                    openNotMinedNeighbors(point);
                }
                if(allFreeOpened()){
                    flagAllMines();
                    gameContinues = false;
                    emit win();
                }
            }
        }
    }
}

Flag SweeperGameField::getFlag(const QPoint &point) const
{
    return getCell(point).flag;
}

bool SweeperGameField::isOpended(const QPoint &point) const
{
    return getCell(point).opened;
}

bool SweeperGameField::isMined(const QPoint &point) const
{
    const FieldCell &cell = getCell(point);
    if(!cell.opened){
        return false;
    }else{
        return cell.mined;
    }
}

bool SweeperGameField::isExploded(const QPoint &point) const
{
    const FieldCell &cell = getCell(point);
    if(!cell.opened){
        return false;
    }else{
        return cell.bombed;
    }
}

int SweeperGameField::getNeiMines(const QPoint &point) const
{
    const FieldCell &cell = getCell(point);
    if(!cell.opened){
        return 0;
    }else{
        return cell.neiMined;
    }
}

int SweeperGameField::getEstimatedFlags()const
{
    return mines - flags;
}

bool SweeperGameField::areNeighbors(const QPoint &point1, const QPoint &point2)const
{
    return (abs(point1.x() - point2.x()) <= 1) &&
            (abs(point1.y() - point2.y()) <= 1);
}
