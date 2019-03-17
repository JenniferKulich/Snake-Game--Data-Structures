#include "event.h"

KeyboardEvent::KeyboardEvent(unsigned char k, int x, int y) : key(k), x(x), y(y) {}

void KeyboardEvent::doAction(Game *game)
{
   if (key == ESCAPE_KEY)
   {
      delete game;
      glutLeaveMainLoop();
   }
}

void DisplayEvent::doAction(Game *game)
{
   if (game->isGameOver())
   {
      glutLeaveMainLoop();
   }
   else
      game->draw();
}

ReshapeEvent::ReshapeEvent(int w, int h) : w(w), h(h){}

void TickTockEvent::doAction(Game *game)
{
   game->makeMove();
   glutPostRedisplay();
}
