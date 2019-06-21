#ifndef LEVEL_H
#define LEVEL_H

#include "tilesettile.h"
#include "tileset.h"
#include "tile.h"
#include "ui.h"
#include "settings.h"

#include <gtkmm/drawingarea.h>
#include <gtkmm/grid.h>

#include <vector>

//forward declaration
class UI;
class Tile;

/*!
  Level data structure. Tileset and grid size for new levels default to the global
  values in Map().
**/
class Level : public Gtk::Grid
{
  public:
    Tileset* tileset;
    std::vector<Tile> tile; //tile for map

    Level(UI* ui, Tileset* tileset, int id = 0, int size = 0);
    void metaInit(UI* ui);

    //accessors
    int getSize() const;

    //event handlers
    void zoom(int currTab, int scrollDir, int zoomSpeed);

    //overloads
    friend std::ostream& operator<<(std::ostream& out, const Level& level);
    friend std::istream& operator>>(std::istream& in, Level& level);

  private:
    int id; // the level id
    int size; //default level size
};

#endif
