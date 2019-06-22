OBJD=./obj
SRCD=./src

IDIR=./headers

DEBUG=-g
CFLAGS=`pkg-config --cflags --libs gtkmm-3.0` -std=c++17 $(DEBUG)
LFLAGS=$(CFLAGS)

CC=g++ $(CFLAGS) -c
LC=g++ $(LFLAGS) -o

OBJS=$(OBJD)/tilesettile.o $(OBJD)/tileset.o $(OBJD)/tile.o $(OBJD)/level.o $(OBJD)/map.o \
		 $(OBJD)/settings.o $(OBJD)/ui.o $(OBJD)/dungeonCartographer.o

.PHONY: clean

all: dungeonCartographer
tilesettile.o: $(OBJD)/tilesettile.o
tileset.o: $(OBJD)/tileset.o
tile.o: $(OBJD)/tile.o
level.o: $(OBJD)/level.o
map.o: $(OBJD)/map.o
settings.o: $(OBJD)/settings.o
ui.o: $(OBJD)/ui.o


#--------
#Programs
#--------
dungeonCartographer: $(OBJS)
	$(LC) dungeonCartographer $(OBJD)/dungeonCartographer.o \
														$(OBJD)/ui.o $(OBJD)/settings.o \
														$(OBJD)/map.o $(OBJD)/level.o $(OBJD)/tile.o $(OBJD)/tileset.o $(OBJD)/tilesettile.o

#------------
#Object Files
#------------
$(OBJD)/tilesettile.o: $(SRCD)/tilesettile.cpp $(SRCD)/tilesettile.h
	$(CC) -o $(OBJD)/tilesettile.o $(SRCD)/tilesettile.cpp

$(OBJD)/tileset.o: $(SRCD)/tileset.cpp $(SRCD)/tileset.h
	$(CC) -o $(OBJD)/tileset.o $(SRCD)/tileset.cpp

$(OBJD)/tile.o: $(SRCD)/tile.cpp $(SRCD)/tile.h $(SRCD)/tilesettile.h $(SRCD)/level.h $(SRCD)/constants.h
	$(CC) -o $(OBJD)/tile.o $(SRCD)/tile.cpp

$(OBJD)/level.o: $(SRCD)/level.cpp $(SRCD)/level.h $(SRCD)/tilesettile.h $(SRCD)/tile.h $(SRCD)/constants.h
	$(CC) -o $(OBJD)/level.o $(SRCD)/level.cpp

$(OBJD)/map.o: $(SRCD)/map.cpp $(SRCD)/map.h $(SRCD)/tilesettile.h $(SRCD)/level.h $(SRCD)/settings.h
	$(CC) -o $(OBJD)/map.o $(SRCD)/map.cpp

$(OBJD)/settings.o: $(SRCD)/settings.cpp $(SRCD)/settings.h
	$(CC) -o $(OBJD)/settings.o $(SRCD)/settings.cpp

# $(OBJD)/event.o: $(SRCD)/event.cpp $(SRCD)/event.h $(SRCD)/settings.h $(SRCD)/map.h $(SRCD)/level.h
# 	$(CC) -o $(OBJD)/event.o $(SRCD)/event.cpp

$(OBJD)/ui.o: $(SRCD)/ui.cpp $(SRCD)/ui.h $(SRCD)/event.h $(SRCD)/settings.h
	$(CC) -o $(OBJD)/ui.o $(SRCD)/ui.cpp

$(OBJD)/dungeonCartographer.o: $(SRCD)/dungeonCartographer.cpp $(SRCD)/ui.h $(SRCD)/settings.h
	$(CC) -o $(OBJD)/dungeonCartographer.o $(SRCD)/dungeonCartographer.cpp


#-------
#Utility
#-------
clean:
	rm $(OBJD)/*.o
	rm dungeonCartographer

run:
	./dungeonCartographer
