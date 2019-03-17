#ifndef UTIL_H
#define UTIL_H
#include <GL/freeglut.h>
#include <sstream>
#include "event.h"

/// Keyboard handler
void keyboard(unsigned char, int, int);
/// Display handler
void display(void);
/// Handler for reshape events
void reshape(const int w, const int h);
/// Handler for clock events
void ticktock(int);
/// other function prototypes
/// Function to do the initialization of the openGL event handlers
void initOpenGL(int msec);
void utilityCentral(Event*);
#endif
