
CC      = gcc
EXE	= 
CFLAGS  = -g -O2 -DDEBUGGABLE -D_THREAD_SAFE -I/usr/local/include/SDL2 -I/usr/X11R6/include  -I/usr/X11/include -DHAVE_OPENGL -g
LIBS	=  -lSDL2_test -L/usr/local/lib -lSDL2 

TARGETS = mvem$(EXE)

all: $(TARGETS)

mvem$(EXE): main.c hardware.c core11.c debugsc11.c system.c hwinterface.c # debugsc80.c  core80.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

