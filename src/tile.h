#ifndef TILE_H
#define TILE_H

#include "tilesettile.h"
#include "level.h"

#include <gtkmm/drawingarea.h>

//forward declaration
class Level;

/*!
  Tile data structure. Contains the grid id, the containing Level, and Tileset
  of the tile. Internal functions update the tiles, queue a draw, and examine the
  adjacent tiles for existence and index.
**/
class Tile : public Gtk::DrawingArea
{
public:
  TilesetTile* tilesetTile; //tileset that defines the tile characteristics

  Tile(int gridId, Level* tileLvl, TilesetTile* tilesetTile);
  void updateTile();

  //overloads
  friend std::ostream& operator<<(std::ostream& out, const Tile& tile);
  friend std::istream& operator>>(std::istream& in, Tile& tile);

private:
  int gridId; //id of the tile's grid
  bool locked = false; //protection against multiple recursion
  Level* tileLvl; //pointer to tile owning level

  void getTileExists(bool* tileExists);
  void getAdjacentIndex(int* adjacentIndex);
  void updateCornerBits(bool propagate);
  void queDraw();

  //signals
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  bool on_button_press_event(GdkEventButton* btn) override;
};

#endif
