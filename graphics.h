#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <GL/freeglut.h>
#include <utility>

enum Color {BLACK=0, WHITE, GREEN, RED, BLUE, ORANGE, LAST_COLOR};

const float Colors[][3] = {
   {0.0, 0.0, 0.0},  // BLACK
   {1.0, 1.0, 1.0},  // WHITE
   {0.0, 1.0, 0.0},  // GREEN
   {1.0, 0.0, 0.0},  // RED
   {0.0, 0.0, 1.0},  // BLUE
   {1.0, 0.55, 0.0},  // ORANGE
};

void DrawFilledRectangle(float, float, float, float, Color b);
void DrawFilledRectangle(std::pair<int, int>, int, int, Color c);
#endif
