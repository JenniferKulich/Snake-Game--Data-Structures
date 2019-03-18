#include "util.h"

void keyboard(unsigned char key, int x, int y)
{
   utilityCentral(new KeyboardEvent(key, x, y));
}

void display()
{
// glClear(GL_COLOR_BUFFER_BIT);
   utilityCentral(new DisplayEvent); 
// glutSwapBuffers();
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
   static Playfield *playfield = new Playfield(false); // true means add obj
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
