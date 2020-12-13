#ifndef SWEEPERGAMEFIELD_H
#define SWEEPERGAMEFIELD_H

#include <QObject>
#include <QPoint>
#include <QList>
#include <QQueue>

#include <random>
#include <vector>

#include "FlagState.h"


struct FieldCell
{
    bool mined;
    short neiMined;
    Flag flag;
    bool opened;
    bool bombed;

    FieldCell();
};

class SweeperGameField : public QObject
{
    Q_OBJECT

    mutable std::default_random_engine randomEngine;
    const int side; // Count of cells per row / col.
    int mines; // Count of mines on the field.
    int flags; // Count of flagged cells.
    bool gameContinues; // true until win or lose.
    std::vector<FieldCell> map; // map of all cells


    QPoint randomPoint()const; // random point index on field


    void placeMines(const int mines, const QPoint &freeCell); // Settles the field by mines, the first selected point must stay free
    void placeMine(const QPoint &point);
    void markNeighbors(const QPoint &point);
    void markNeighbor(const QPoint &point);
    void openNotMinedNeighbors(const QPoint &point);
    bool exists(const QPoint &point)const;

    void enqueueNeighbors(const QPoint &point,
                      QQueue<QPoint> &queue,
                      QList<QPoint> &openedList);
    void enqueueNeighbor(const QPoint &point,
                      QQueue<QPoint> &queue,
                      QList<QPoint> &openedList);

    void openAll();
    void flagAllMines();

    bool allFreeOpened() const;
    FieldCell &getCell(const QPoint &point);
    const FieldCell &getCell(const QPoint &point) const;

    void toggleFlag(FieldCell &cell);
    bool areNeighbors(const QPoint &point1, const QPoint &point2) const;

public:
    SweeperGameField(int side, int mines, const QPoint &freeCell,
                    QObject *parent = 0);
    ~SweeperGameField();

    void click(const QPoint &point);
    void toggleFlag(const QPoint &point);
    Flag getFlag(const QPoint &point)const;
    bool isOpended(const QPoint &point)const;
    bool isMined(const QPoint &point)const;
    bool isExploded(const QPoint &point)const;
    int getNeiMines(const QPoint &point)const;

    int getEstimatedFlags() const;
signals:
    void bombed();
    void win();

public slots:

};

#endif // SWEEPERGAMEFIELD_H
