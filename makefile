OBJD=./obj
SRCD=./src

IDIR=./headers

DEBUG=-g
CFLAGS=`pkg-config gtk+-3.0 --cflags --libs` -std=c++11 $(DEBUG)
LFLAGS=$(CFLAGS)

CC=g++ $(CFLAGS) -c
LC=g++ $(LFLAGS) -o

.PHONY: clean

all: dungeonCartographer

#--------
#Programs
#--------
dungeonCartographer: settings.o ui.o dungeonCartographer.o
	$(LC) dungeonCartographer $(OBJD)/dungeonCartographer.o $(OBJD)/ui.o $(OBJD)/settings.o

#------------
#Object Files
#------------
dungeonCartographer.o: $(SRCD)/dungeonCartographer.cpp $(SRCD)/ui.h $(SRCD)/settings.h
	$(CC) -o $(OBJD)/dungeonCartographer.o $(SRCD)/dungeonCartographer.cpp

settings.o: $(SRCD)/settings.cpp $(SRCD)/settings.h
	$(CC) -o $(OBJD)/settings.o $(SRCD)/settings.cpp

ui.o: $(SRCD)/ui.cpp $(SRCD)/ui.h $(SRCD)/settings.h
	$(CC) -o $(OBJD)/ui.o $(SRCD)/ui.cpp

#-------
#Utility
#-------
clean:
	rm $(OBJD)/*.o
	rm dungeonCartographer