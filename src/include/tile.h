#ifndef TILE_H
#define TILE_H

#include "tileset.h"   //tileset
#include "constants.h" //uint

#include <fstream> //operator<<|operator>>

#include <glibmm/refptr.h>     //on_draw()
#include <gtkmm/drawingarea.h> //inheritance

/*!
  Tile data structure. Handles draw updates, provides information functions about the
  surrounding Tiles.
**/
struct Tile : public Gtk::DrawingArea
{
  friend class Level; //Tile is owned by Level

  Tile(Tileset* tileset, uint tileId, int gridId, int gridSize);

  //utility
  void getTileExists(bool* tileExists);
  void getAdjacentIndex(int* adjacentIndex);

  //overloads
  friend std::ostream& operator<<(std::ostream& out, const Tile& tile);
  friend std::istream& operator>>(std::istream& in, Tile& tile);

private:
  int gridId;          //The tile's grid location
  int gridSize;        //The size of the grid the tile is on
  uint tileId;         //The id of the tileset's tile(image) this tile displays
  Tileset* tileset;    //The Tileset this tile uses
  bool locked = false; //protection against multiple recursion

  //signal overload
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};

#endif
