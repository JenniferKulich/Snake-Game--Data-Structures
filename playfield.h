/********************************************************************//**
 * @file
 ***********************************************************************/
#ifndef PLAYFIELD_H
#define PLAYFIELD_H
#include <utility>
#include <queue>
#include <GL/freeglut.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include "graphics.h"

enum ValidMove {NONE, LEFT, RIGHT, UP, DOWN};

const std::string MoveNames[] = {"None", "Left", "Right", "Up", "Down"};

#define PLAYFIELD_WIDTH 17
#define PLAYFIELD_HEIGHT 15
#define CLEAR_VALUE 0
#define TAIL_VALUE 1
#define HEAD_VALUE 2
#define FOOD_VALUE 4
#define OutOfBounds(x) ((x.first < 0 || x.first == PLAYFIELD_WIDTH ||\
                         x.second < 0|| x.second == PLAYFIELD_HEIGHT))
                    
/********************************************************************//**
 * @class Playfield
 **********************************************************************/
class Playfield
{
   std::pair<int, int> head;
   std::pair<int, int> food;
   std::queue<std::pair<int, int>> tail;
   unsigned tailLength;
   int cellWidth;
   int cellHeight;
   int *grid;
   bool hasObstacles;
public:
   Playfield(bool placeObst = false);
   ~Playfield();
  
   void draw();
   bool moveHead(ValidMove); 
   void updatePlayfield();
   std::pair<int, int> translateHead(ValidMove);
   std::pair<int, int> placeNewFood();
   const int* getGrid() const;
   int getScore();
   void placeObstacles();
};
#endif
