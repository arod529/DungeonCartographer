OBJD=./obj
SRCD=./src

INCD=./src/include
INC=-I./src/include

DEBUG=-g
CFLAGS=`pkg-config --cflags --libs gtkmm-3.0 sigc++-2.0` -std=c++17 $(DEBUG)
LFLAGS=$(CFLAGS)

CC=g++ $(INC) $(CFLAGS) -c
LC=g++ $(LFLAGS) -o

OBJS=dungeonCartographer.o resources.o \
		 settings.o ui.o refgrid.o \
		 map.o level.o tile.o tileset.o

.PHONY: clean

all: dungeonCartographer

#--------
#Programs
#--------
dungeonCartographer: $(OBJS)
	$(LC) dungeonCartographer $(OBJD)/dungeonCartographer.o $(OBJD)/resources.o \
														$(OBJD)/settings.o $(OBJD)/ui.o $(OBJD)/refgrid.o \
														$(OBJD)/map.o $(OBJD)/level.o $(OBJD)/tile.o $(OBJD)/tileset.o

#------------
#Object Files
#------------
dungeonCartographer.o: $(SRCD)/dungeonCartographer.cpp $(INCD)/settings.h
	$(CC) -o $(OBJD)/dungeonCartographer.o $(SRCD)/dungeonCartographer.cpp

settings.o: $(SRCD)/settings.cpp $(INCD)/settings.h $(INCD)/tileset.h
	$(CC) -o $(OBJD)/settings.o $(SRCD)/settings.cpp

ui.o: $(SRCD)/ui.cpp $(SRCD)/ui.cpp $(INCD)/ui.h $(INCD)/settings.h
	$(CC) -o $(OBJD)/ui.o $(SRCD)/ui.cpp

map.o: $(SRCD)/map.cpp $(INCD)/map.h $(INCD)/settings.h $(INCD)/ui.h $(INCD)/tileset.h $(INCD)/constants.h
	$(CC) -o $(OBJD)/map.o $(SRCD)/map.cpp

level.o: $(SRCD)/level.cpp $(INCD)/level.h $(INCD)/tileset.h
	$(CC) -o $(OBJD)/level.o $(SRCD)/level.cpp

tile.o: $(SRCD)/tile.cpp $(INCD)/tile.h $(INCD)/tileset.h $(INCD)/constants.h
	$(CC) -o $(OBJD)/tile.o $(SRCD)/tile.cpp

tileset.o: $(SRCD)/tileset.cpp $(INCD)/tileset.h $(INCD)/constants.h
	$(CC) -o $(OBJD)/tileset.o $(SRCD)/tileset.cpp

refgrid.o: $(SRCD)/refgrid.cpp $(INCD)/refgrid.h
	$(CC) -o $(OBJD)/refgrid.o $(SRCD)/refgrid.cpp

resources.o: resources.c
	$(CC) -o $(OBJD)/resources.o $(SRCD)/resources.c

resources.c: resources.xml
	glib-compile-resources resources.xml --target=$(SRCD)/resources.c --generate-source --sourcedir=ui

#-------
#Utility
#-------
clean:
	rm -f $(OBJD)/*.o
	rm -f $(SRCD)/resources.c
	rm -f dungeonCartographer

run:
	./dungeonCartographer
