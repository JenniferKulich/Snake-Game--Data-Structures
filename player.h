#ifndef __PLAYER_H
#define __PLAYER_H
#include <list>
#include <cstdlib>
#include <ctime>
#include "playfield.h"
#include "BFSPath.h"
#include "DFSPath.h"

int randomMove(const int *grid, int headSpot);
ValidMove ManhattanMove(const int *grid);
void newMove(const int *grid, ValidMove &move, ValidMove trialMove, int count, int headIndex);

class Player
{
public:
   Player ();
   ValidMove makeMove(const Playfield *);

};

#endif
