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

int  newBottomRightCorner(const int *grid);
int newTopRightCorner(const int *grid);
int newTopLeftCorner(const int *grid);
int toRightSide(const int *grid, int playerIndex);
int newBottomLeftCorner(const int *grid);

class Player
{
  bool searchingFood = true;
  bool toBottomRight = false;
  bool toTopRight = false;
  bool toTopLeft = false;
  bool toBottomLeft = false;
public:
   Player ();
   ValidMove makeMove(const Playfield *);
   static int food;
   static int moveCount;
   ValidMove moveBottomRight(const int *grid, int headSpot, int foodSpot, bool &contin);
   ValidMove moveTopRight(const int *grid, int headSpot, int foodSpot, bool &contin);
   ValidMove moveTopLeft(const int *grid, int headSpot, int foodSpot, bool &contin);
   ValidMove ManhattanChecker(const int *grid, int headSpot, int foodSpot);


private:
};

#endif
