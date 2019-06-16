#ifndef MAP_H
#define MAP_H

#include "tilesettile.h"
#include "level.h"
#include "settings.h"

#include <string>
#include <unordered_map>
#include <vector>

/*!
  Map data structure. Tileset, and grid size for new maps default to the the global
  values in Settings.

  \bug make level private
  \bug tileset should be pointer
  \bug incomplete definition
  \bug add map file
  \bug should be in own header with Level, Tile, and Tileset?
**/
class Map
{
  public:
    Map(Settings*);            //default map
    Map(std::string mapFile);  //map from file
    ~Map();                    //close a map

    //utility
    void loadTileset(std::string tileSetFile);

    //accessors
    int getSize();

    //mutators
    void setTileset(std::string tileSetFile);
    void setSize(int s);

    std::vector<Level> level;    //array of levels for a map

  private:
    int size; //default level size
    std::unordered_map<uint, TilesetTile> tileset; //default level tileset
};

#endif
