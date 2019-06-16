#ifndef TILE_H
#define TILE_H

#include "tileset.h"
#include "level.h"

//forward declaration
class Level;

/*!
  Tile data structure. Contains the grid id, the containing Level, and Tileset
  of the tile. Internal functions update the tiles, queue a draw, and examine the
  adjacent tiles for existence and index.
**/
class Tile
{
public:
  Tileset* tileTileset; //tileset that defines the tile characteristics

  Tile(int _gridId, Level* _tileLvl, Tileset* _tileTileset);
  void updateTile();

private:
  int gridId; //id of the tile's grid
  bool locked = false; //protection against multiple recursion
  Level* tileLvl; //pointer to tile owning level

  void getTileExists(bool* _tileExists);
  void getAdjacentIndex(int* _adjacentIndex);
  void updateCornerBits(bool _propagate);
  void queDraw();
};

#endif
