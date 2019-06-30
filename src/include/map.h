#ifndef MAP_H
#define MAP_H

#include "constants.h" //uint
#include "level.h"     //level
#include "tileset.h"   //tileset

#include <fstream> //operator<<|operator>>
#include <memory>  //level
#include <string>  //saveToFile()|openFile()|filepath
#include <vector>  //level

#include <sigc++/sigc++.h> //signals

/*!
  Map data structure. Handles map file manipulation, and Level interface with the UI.
**/
class Map
{
  public:
    ~Map();

    void appendLevel();
    void centerLevel(int levelIndex);
    bool loadFile(std::string filepath);
    void newMap(Tileset* mapTileset, int mapSize);
    void print();
    void resetLevel(int levelIndex);
    bool saveToFile(std::string filepath);
    void shiftLevel(int levelIndex, int x, int y);
    
    //utility
    std::string getFilePath() const;
    int getLevelSize(int levelIndex) const;
    int getTileSize(int levelIndex) const;
    void setTileSize(int levelIndex, int tileSize);

    //signals
    sigc::signal<void, uint, Level*> signal_levelCreated; // A level has been created.
    sigc::signal<void> signal_mapCleared; // The map has been cleared. No levels exist.

    //overloads
    friend std::ostream& operator<<(std::ostream& out, const Map& map);
    friend std::istream& operator>>(std::istream& in, Map& map);

  private:
    int size;                                  // default level size
    Tileset* tileset;                          // The tile set that the map uses
    std::string filepath = "";                 // The file path associated with this map
    std::vector<std::unique_ptr<Level>> level; // The levels in the map
    
    void clearMap();
};

#endif
