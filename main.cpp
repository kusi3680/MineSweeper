#include <QApplication>
#include <SweeperWidget.h>
#include <QDebug>

#include "Sweeper.h"

/*
 * Kunal Sinha
 * CSCI 3010 001
 *
 * Homework 5 Final Project
 *
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*
     * Change this to change the size of the game:
     */


    // Testing prototype Configuration:
    int side = 4;
    int mines = 1;


    /*
    // easy
    int side = 8;
    int mines = 10;
    */

    /*
    // medium
    int side = 10;
    int mines = 20;
    */

    /*
    // hard
    int side = 10;
    int mines = 20;
    */


    qDebug() << "Starting with edge size :" << side << " and "<< mines << " mines.";

    Sweeper s(side, mines); // size, mines


    SweeperWidget w(&s);
    w.show();

    return a.exec();
}
