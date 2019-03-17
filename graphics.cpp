#include "graphics.h"
#include <iostream>

void DrawRectangle(float x1, float y1, float x2, float y2, Color c)
{
    glColor3fv( Colors[c] );
    glBegin( GL_LINE_LOOP );
        glVertex2f( x1, y1 );
        glVertex2f( x2, y1 );
        glVertex2f( x2, y2 );
        glVertex2f( x1, y2 );
    glEnd();
}

void DrawFilledRectangle(float x1, float y1, float x2, float y2, Color c)
{
    glColor3fv( Colors[c] );
    glBegin( GL_POLYGON );
        glVertex2f( x1, y1 );
        glVertex2f( x2, y1 );
        glVertex2f( x2, y2 );
        glVertex2f( x1, y2 );
    glEnd();
    if (c != GREEN) 
       DrawRectangle(x1, y1, x2, y2, BLACK);
    else
       DrawRectangle(x1, y1, x2, y2, GREEN);
    glFlush();
}

void DrawFilledRectangle(std::pair<int, int> loc, int dX, int dY, Color c)
{
   int x1 = loc.first * dX;
   int x2 = x1 + dX;
   int y1 = loc.second * dY;
   int y2 = y1 + dY;

   DrawFilledRectangle(x1, y1, x2, y2, c);
}
