#ifndef TILE_H
#define TILE_H

#include "tilesettile.h"
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
  TilesetTile* tilesetTile; //tileset that defines the tile characteristics

  Tile(int _gridId, Level* _tileLvl, TilesetTile* _tilesetTile);
  void updateTile();

  //overloads
  friend std::ostream& operator<<(std::ostream& _out, const Tile& _tile);
  friend std::istream& operator>>(std::istream& _in, Tile& _tile);

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
