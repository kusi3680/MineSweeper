#ifndef FLAGSTATE_H
#define FLAGSTATE_H

/*
 *  no flag, no neighbors or a mine or doubt (?) or count of neighboring mines
 */

enum class Flag
{
    NO,
    MINE,
    DOUBT,

    COUNT
};

#endif // FLAGSTATE_H
