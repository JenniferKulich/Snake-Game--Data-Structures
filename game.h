#ifndef __GAME_H
#define __GAME_H
#include <map>
#include <sstream>
#include <GL/freeglut.h>
#include "graphics.h"
#include "player.h"

class Game
{
   Player * player;
   Playfield *playfield;
   bool gameOver;
   int  score;
   int  turnCount;

public:
   Game(Player *, Playfield *);
   ~Game();

   bool isGameOver();
   void gameIsOver();
   int  getScore();
   void draw();
   bool makeMove();

private:
};
#endif
