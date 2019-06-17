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
  \bug incomplete definition
  \bug add map file
  \bug tileset should be pointer? have global in seetings?
**/
class Map
{
  public:
    std::vector<Level> level;    //array of levels for a map

    Map(Settings*);            //default map
    Map(std::string mapFile);  //map from file
    ~Map();                    //close a map

    //utility
    void loadTileset(std::string tileSetFile);

  private:
    int size; //default level size
    std::unordered_map<uint, TilesetTile> tileset; //default level tileset
};

#endif
