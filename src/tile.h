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

  \bug gridId and tileLvl should be private and read only after initialization.
  \bug locked should be private.
  \bug getTileExists(), getAdjacentIndex() updateCornerBits(), queDraw() should be private
**/
class Tile
{
public:
  int      gridId;       //id of the tile's grid
  Level*   tileLvl;     //pointer to tile owning level
  Tileset* tileTileset;  //tileset that defines the tile characteristics

  bool locked = false; //protection against multiple recursion

  Tile(int _gridId, Level* _tileLvl, Tileset* _tileTileset);
  void updateTile();

  void getTileExists(bool* _tileExists);
  void getAdjacentIndex(int* _adjacentIndex);
  void updateCornerBits(bool _propagate);
  void queDraw();
};

#endif
