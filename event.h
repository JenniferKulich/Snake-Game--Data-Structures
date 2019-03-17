#ifndef __EVENT_H
#define __EVENT_H
#include <iostream>
#include "player.h"
#include "game.h"

#define ESCAPE_KEY 27

class Event
{
public:
   void virtual doAction(Game *) = 0;
   virtual ~Event(){};
};

class DisplayEvent : public Event
{
public:
   void doAction(Game *);
};

class ReshapeEvent : public Event
{
   int w, h;
public:
   ReshapeEvent(int, int);
   void doAction(Game *) {}
};

class TickTockEvent : public Event
{
public:
   void doAction(Game *);
};

class KeyboardEvent : public Event
{
   unsigned char key;
   int x, y;
public:
   KeyboardEvent(unsigned char, int, int);
   void doAction(Game *);
};

#endif
