#ifndef __PLAYER_H
#define __PLAYER_H
#include <list>
#include <queue>
#include <cstdlib>
#include <ctime>
#include "playfield.h"
#include "BFSPath.h"
#include "DFSPath.h"


class Player
{
  bool startingBFS = true;
  bool searchingFood = false /*true*/;
  bool toBottomRight = false;
  bool toTopRight = false;
  bool toTopLeft = false;
  bool toBottomLeft = false;
  bool foodSecondTop = false;
  int foodEaten = 0;

public:
   Player ();
   ValidMove makeMove(const Playfield *);
   static int food;
   static int moveCount;

private:
  ValidMove ManhattanMove(const int *grid);
  void newMove(const int *grid, ValidMove &move, ValidMove origionalMove, int count, int headIndex);

  ValidMove moveRightSide(const int *grid, int headSpot, bool &contin);
  ValidMove moveTopRight(const int *grid, int headSpot, bool &contin);
  ValidMove moveTopLeft(const int *grid, int headSpot, bool &contin);
  ValidMove moveBottomLeft(const int *grid, int headSpot, bool &contin);
  ValidMove ManhattanChecker(const int *grid, int headSpot);

  int newTopRightCorner(const int *grid);
  int newTopLeftCorner(const int *grid);
  int toRightSide(const int *grid, int playerIndex);
  int newBottomLeftCorner(const int *grid);


};

#endif
