LIBNAME = libPlayer.so
LIB_SRC = player.cpp
DRIVER_SRC = driver.cpp game.cpp playfield.cpp

SNAKE_SRC = main.cpp event.cpp game.cpp graphics.cpp playfield.cpp util.cpp

LIB_OBJS = $(LIB_SRC:.cpp=.o)
DRIVER_OBJS = $(DRIVER_SRC:.cpp=.o)
SNAKE_OBJS = $(SNAKE_SRC:.cpp=.o)

INCLUDE_FILES = game.h graphics.h player.h event.h playfield.h util.h

#GNU C/C++ Compiler
GCC = g++

# Compiler flags
CXXFLAGS = -fPIC -Wall -O3 -std=c++11
LIBS = -lglut -lGL -lGLU -lm -lpthread

.PHONY: clean tar

# Targets include all, clean, debug, tar

all : snake

snake: $(LIBNAME) $(SNAKE_OBJS)
	$(GCC) -o $@ $(LIBNAME) $(SNAKE_OBJS) $(LIBS) -Wl,-rpath=.

driver: $(LIBNAME) $(DRIVER_OBJS)
	$(GCC) -o $@ $(LIBNAME) $(DRIVER_OBJS) -Wl,-rpath=.

$(LIBNAME): $(LIB_OBJS)
	$(GCC) -o $@ $^ -shared

snake: CXXFLAGS += -DGRAPHICS

tar: $(LIB_SRC) $(DRIVER_SRC) $(SNAKE_SRC) $(INCLUDE_FILES)
	tar cvf Snake.tar $^ Makefile

clean:
	rm -rf *.o *.d core driver snake *.so Snake.tar

-include $(LIB_SRC:.cpp=.d)

-include $(DRIVER_SRC:.cpp=.d)

-include $(SNAKE_SRC:.cpp=.d)

%.d: %.cpp
	@set -e; /bin/rm -rf $@;$(GCC) -MM $< $(CXXFLAGS) > $@
