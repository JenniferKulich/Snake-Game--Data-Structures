#include "util.h"

void keyboard(unsigned char key, int x, int y)
{
   utilityCentral(new KeyboardEvent(key, x, y));
}

void display()
{
   utilityCentral(new DisplayEvent); 
}

void reshape(int w, int h)
{
   glLoadIdentity();           // initialize transformation matrix
   gluOrtho2D(0.0,w,0.0,h);
   glViewport(0,0,w,h);        // adjust viewport to new window
   glutPostRedisplay();
   utilityCentral(new ReshapeEvent(w, h));
}

void ticktock(int value)
{
   utilityCentral(new TickTockEvent);
   glutTimerFunc(value, ticktock, value);
}

void updateScore(Game *game)
{
   std::stringstream ss;
   ss << "Score = " << game->getScore();
   glutSetWindowTitle(ss.str().c_str());
}

void utilityCentral(Event *event)
{
   static Player *player = new Player();
   // Passing a true parameter to the Playfield constructor will include
   // obstacles on the playfield.  Passing nothing or false prevents
   // obstacles from being added
   static Playfield *playfield = new Playfield(true);
   static Game *game = new Game(player, playfield);

   if (game->isGameOver()) 
   {
      std::cout << "GAME OVER!" << std::endl ; 
      std::cout << "Final Score: " << game->getScore() << std::endl;
      delete game;
      glutLeaveMainLoop();
   }
   else
   {
      event->doAction(game);
      updateScore(game);
   }
   delete event;
}
