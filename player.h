#ifndef __PLAYER_H
#define __PLAYER_H
#include <list>
#include <queue>
#include <cstdlib>
#include <ctime>
#include "playfield.h"
#include "BFSPath.h"
#include "DFSPath.h"

ValidMove ManhattanMove(const int *grid);
void newMove(const int *grid, ValidMove &move, ValidMove origionalMove, int count, int headIndex);

class Player
{
public:
   Player ();
   ValidMove makeMove(const Playfield *);
   static int food;
   static int moveCount;
private:
};

#endif
