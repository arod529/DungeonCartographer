#ifndef LEVEL_H
#define LEVEL_H

#include "tileset.h"
#include "tile.h"

#include <gtk/gtk.h>
#include <unordered_map>
#include <vector>

//forward declaration
class Tile;

/*!
  Level data structure. Tileset and grid size for new levels default to the global
  values in Map().

  \bug tileset should be pointer
**/
class Level
{
  public:
    std::unordered_map<uint, Tileset>* tileset;                      //default level tileset
    std::vector<Tile> tile;               //tile for map
    std::vector<GtkWidget*>  drawingArea;   //drawing areas for map

    Level(int _size, std::unordered_map<uint, Tileset>* _tileset);

    //accessors
    int getSize() const;

  private:
    int size; //default level size
};

#endif
