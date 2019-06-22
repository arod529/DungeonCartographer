OBJD=./obj
SRCD=./src

IDIR=./headers

DEBUG=-g
CFLAGS=`pkg-config --cflags --libs gtkmm-3.0` -std=c++17 $(DEBUG)
LFLAGS=$(CFLAGS)

CC=g++ $(CFLAGS) -c
LC=g++ $(LFLAGS) -o

OBJS=$(OBJD)/dungeonCartographer.o $(OBJD)/settings.o $(OBJD)/tileset.o $(OBJD)/ui.o $(OBJD)/map.o

.PHONY: clean

all: dungeonCartographer
settings.o: $(OBJD)/settings.o
tileset.o: $(OBJD)/tileset.o
ui.o: $(OBJD)/ui.o
map.o: $(OBJD)/map.o


#--------
#Programs
#--------
dungeonCartographer: $(OBJS)
	$(LC) dungeonCartographer $(OBJD)/dungeonCartographer.o $(OBJD)/settings.o $(OBJD)/ui.o \
				$(OBJD)/tileset.o $(OBJD)/map.o

#------------
#Object Files
#------------
$(OBJD)/dungeonCartographer.o: $(SRCD)/dungeonCartographer.cpp $(SRCD)/settings.h
	$(CC) -o $(OBJD)/dungeonCartographer.o $(SRCD)/dungeonCartographer.cpp

$(OBJD)/settings.o: $(SRCD)/settings.cpp $(SRCD)/settings.h $(SRCD)/tileset.h
	$(CC) -o $(OBJD)/settings.o $(SRCD)/settings.cpp

$(OBJD)/ui.o: $(SRCD)/ui.cpp $(SRCD)/ui.cpp $(SRCD)/ui.h $(SRCD)/settings.h
	$(CC) -o $(OBJD)/ui.o $(SRCD)/ui.cpp

$(OBJD)/tileset.o: $(SRCD)/tileset.cpp $(SRCD)/tileset.h $(SRCD)/constants.h
	$(CC) -o $(OBJD)/tileset.o $(SRCD)/tileset.cpp

$(OBJD)/map.o: $(SRCD)/map.cpp $(SRCD)/map.h $(SRCD)/settings.h $(SRCD)/ui.h $(SRCD)/tileset.h $(SRCD)/constants.h
	$(CC) -o $(OBJD)/map.o $(SRCD)/map.cpp



#-------
#Utility
#-------
clean:
	rm $(OBJD)/*.o
	rm dungeonCartographer

run:
	./dungeonCartographer
