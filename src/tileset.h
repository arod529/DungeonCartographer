#ifndef TILESET_H
#define TILESET_H

#include "tilesettile.h"
#include "constants.h"

#include <string>
#include <unordered_map>

class Tileset
{
public:
  std::string name{""};
  std::string filepath{""};
  std::unordered_map<uint, TilesetTile> tile;

  Tileset(const std::string fpath = "");

  bool loadFile(const std::string fpath);
  bool reload();
  bool isTileset(const std::string fpath);
  bool writeDefaultTilesetFile(const std::string fpath);
};

#endif
