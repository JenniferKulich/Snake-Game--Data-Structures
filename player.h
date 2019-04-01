#ifndef __PLAYER_H
#define __PLAYER_H
#include <list>
#include "playfield.h"
#include "BFSPath.h"


class Player
{
public:
   Player ();
   ValidMove makeMove(const Playfield *);


};

#endif
