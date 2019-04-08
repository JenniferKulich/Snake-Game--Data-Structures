/********************************************************************//**
 * @file
 ***********************************************************************/
#include "playfield.h"
#include <iostream>
#include <iomanip>

Playfield::Playfield(bool obst) : tailLength(0), cellWidth(0), cellHeight(0),
   hasObstacles(obst)
{
   // Seed pseudo-random generator
   srand(time(NULL));

   // Start head of snake in the middle of the playfield
   head = std::pair<int, int>(rand() % PLAYFIELD_WIDTH, rand() % PLAYFIELD_HEIGHT);
   food = std::pair<int, int>(rand() % PLAYFIELD_WIDTH, rand() % PLAYFIELD_HEIGHT);
#ifdef GRAPHICS
   cellWidth = glutGet(GLUT_WINDOW_WIDTH) / PLAYFIELD_WIDTH;
   cellHeight = glutGet(GLUT_WINDOW_HEIGHT) / PLAYFIELD_HEIGHT;
#else
   cellWidth = 1;
   cellHeight = 1;
#endif

   int count;
   grid = new int[PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT];

   do
   {
      std::memset(grid, CLEAR_VALUE, sizeof(int)*PLAYFIELD_WIDTH*PLAYFIELD_HEIGHT);
      placeObstacles();
      SnakeGraph graph(grid, PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT);
      Biconnected bicon(&graph);
      CC cc(&graph);
      count = cc.count() + bicon.articulationNodes().size(); 
   } while (count > 1);

   grid[head.first + head.second * PLAYFIELD_WIDTH] = HEAD_VALUE;
   grid[food.first + food.second * PLAYFIELD_WIDTH] = FOOD_VALUE;
   updatePlayfield();
   drawObstacles();
}

Playfield::~Playfield()
{
   delete[] grid;
}

void Playfield::drawObstacles()
{
   for (int cell = 0 ; cell < PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT ; cell++)
   {
      if (grid[cell] == TAIL_VALUE)
      {
#ifdef GRAPHICS
         std::pair<int, int> obstacle(cell % PLAYFIELD_WIDTH, cell / PLAYFIELD_WIDTH);
         DrawFilledRectangle(obstacle, cellWidth, cellHeight, BLUE);
#endif
      }
   }
}

void Playfield::placeObstacles()
{
   if (!hasObstacles) return;
   int obstacleCount = 0.05 * PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT;

   while (obstacleCount--)
   {
      int placement;
      do
      {
         placement = rand() % (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT);
      } while (grid[placement] != CLEAR_VALUE);
      grid[placement] = TAIL_VALUE;
   }
}
 
const int *Playfield::getGrid() const { return grid ; }

void Playfield::updatePlayfield()
{
#ifdef GRAPHICS
   DrawFilledRectangle(head, cellWidth, cellHeight, BLACK);
   DrawFilledRectangle(food, cellWidth, cellHeight, RED);

   if (tail.size() > 0)
      DrawFilledRectangle(tail.back(), cellWidth, cellHeight, WHITE);
#endif

   while (tailLength < tail.size())
   {
      std::pair<int, int> segment = tail.front() ; tail.pop();
      grid[segment.first + segment.second * PLAYFIELD_WIDTH] = CLEAR_VALUE;
#ifdef GRAPHICS
      DrawFilledRectangle(segment, cellWidth, cellHeight, GREEN);
#endif
   }
}

std::pair<int, int> Playfield::translateHead(ValidMove move)
{
   std::pair<int, int> newLocation = head;

   switch (move)
   {
      case UP: newLocation.second++; break;
      case DOWN: newLocation.second--; break;
      case LEFT: newLocation.first--; break;
      case RIGHT: newLocation.first++; break;
      default: break;
   }
   return newLocation;
}

std::pair<int, int> Playfield::placeNewFood()
{
   int x, y;
   do 
   {
      x = rand() % PLAYFIELD_WIDTH;
      y = rand() % PLAYFIELD_HEIGHT;
   } while (grid[y * PLAYFIELD_WIDTH + x] != CLEAR_VALUE);
   grid[x + y * PLAYFIELD_WIDTH] = FOOD_VALUE;
   return std::pair<int, int>(x, y);
}

int Playfield::getScore() { return tailLength+1 ; }

bool Playfield::moveHead(ValidMove move)
{
   if (move == NONE) 
      return false;

   std::pair<int, int> newHeadPosition = translateHead(move);

   // Hit the border
   if (OutOfBounds(newHeadPosition)) 
      return false;

   // Hit its tail
   if (grid[newHeadPosition.first + newHeadPosition.second * PLAYFIELD_WIDTH] ==
       TAIL_VALUE)
   {
#ifdef GRAPHICS
      DrawFilledRectangle(newHeadPosition, cellWidth, cellHeight, ORANGE);
      std::this_thread::sleep_for (std::chrono::seconds(5));
#endif
      return false;
   }

   if (newHeadPosition == food)
   {
      tailLength++;
      food = placeNewFood();
   }

   tail.push(head);
   grid[head.first + head.second * PLAYFIELD_WIDTH] = TAIL_VALUE;
   head = newHeadPosition;
   grid[head.first + head.second * PLAYFIELD_WIDTH] = HEAD_VALUE;
   return true;
}
