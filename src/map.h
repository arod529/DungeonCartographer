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

    //utility
    void loadTileset(std::string tileSetFile);
    bool saveToFile(std::string filepath);

    //overloads
    friend std::ostream& operator<<(std::ostream& out, const Map& map);
    friend std::istream& operator>>(std::istream& in, Map& map);

  private:
    int size; //default level size
    std::string tilesetFile; //the file of the tileset
    std::unordered_map<uint, TilesetTile> tileset; //default level tileset
};

#endif
