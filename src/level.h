#ifndef LEVEL_H
#define LEVEL_H

#include "tilesettile.h"
#include "tileset.h"
#include "tile.h"

#include <gtkmm/drawingarea.h>

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
    Tileset* tileset;
    std::vector<Tile> tile; //tile for map
    std::vector<Gtk::DrawingArea*> drawingArea; //drawing areas for map

    Level(Tileset* tileset, int id = 0, int size = 0);
    ~Level();

    //accessors
    int getSize() const;

    //overloads
    friend std::ostream& operator<<(std::ostream& out, const Level& level);
    friend std::istream& operator>>(std::istream& in, Level& level);

  private:
    int id; // the level id
    int size; //default level size
};

#endif
