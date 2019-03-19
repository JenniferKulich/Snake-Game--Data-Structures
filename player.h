#ifndef __PLAYER_H
#define __PLAYER_H
#include <list>
#include "playfield.h"

class Player
{
public:
   Player ();
   ValidMove makeMove(const Playfield *);
private:
  ValidMove rowsEven(int headX, int headY);
  ValidMove rowsOdd(int headX, int headY, int foodX, int foodY);


};

#endif
