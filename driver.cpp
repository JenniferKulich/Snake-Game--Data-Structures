#include <iostream>
#include <chrono>
#include <vector>
#include "game.h"
#include "player.h"

using namespace std;
using namespace std::chrono;

int main()
{
   vector<duration<double>> turnTimes;
   time_point<system_clock> start, end;

   Player *player = new Player();
   Playfield *playfield = new Playfield();
   Game *game = new Game(player, playfield);

   while (!game->isGameOver())
   {
      start = system_clock::now();
      game->makeMove();
      end = system_clock::now();
      turnTimes.push_back(end-start);
   }
   cout << "Final Score: " << game->getScore() << endl;

   double sum = 0.0;
   for(duration<double> d : turnTimes)
      sum += d.count();

   cout << "Average turn time: " << sum/turnTimes.size() << endl;
   return 0;
}
