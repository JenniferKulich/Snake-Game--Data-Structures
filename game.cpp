#include "game.h"

Game::Game(Player *p, Playfield *pf) : player(p), playfield(pf), gameOver(false), score(0), turnCount(0)
{
   struct timespec ts;
   clock_gettime(CLOCK_MONOTONIC, &ts);
   srand((time_t)ts.tv_nsec);
}

Game::~Game()
{
   delete player;
   delete playfield;
}

bool Game::makeMove()
{
   bool retVal = false;
   
   ValidMove playerMove = player->makeMove(playfield);
   retVal = playfield->moveHead(playerMove);

   if (!retVal) gameIsOver();

   turnCount++;
   playfield->updatePlayfield();
   return !isGameOver();
}

void Game::gameIsOver() { gameOver = true; }

bool Game::isGameOver() { return gameOver; }

int Game::getScore() { return playfield->getScore() ; }

void Game::draw()
{ }
