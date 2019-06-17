#ifndef LEVEL_H
#define LEVEL_H

#include "tilesettile.h"
#include "tile.h"

#include <gtk/gtk.h>
#include <unordered_map>
#include <vector>

//forward declaration
class Tile;

/*!
  Level data structure. Tileset and grid size for new levels default to the global
  values in Map().
**/
class Level
{
  public:
    std::unordered_map<uint, TilesetTile>* tileset; //default level tileset
    std::vector<Tile> tile; //tile for map
    std::vector<GtkWidget*> drawingArea; //drawing areas for map

    Level(int _size, std::unordered_map<uint, TilesetTile>* _tileset);

    //accessors
    int getSize() const;

  private:
    int size; //default level size
};

#endif