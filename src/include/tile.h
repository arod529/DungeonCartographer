/*************************************************************************
| Dungeon Cartographer, a grid based map drawing program.                |
| Copyright (C) 2019  Alexander L. Hopper                                |
|                                                                        |
| This program is free software: you can redistribute it and/or modify   |
| it under the terms of the GNU General Public License as published by   |
| the Free Software Foundation, either version 3 of the License, or      |
| (at your option) any later version.                                    |
|                                                                        |
| This program is distributed in the hope that it will be useful,        |
| but WITHOUT ANY WARRANTY; without even the implied warranty of         |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          |
| GNU General Public License for more details.                           |
|                                                                        |
| You should have received a copy of the GNU General Public License      |
| along with this program.  If not, see <https://www.gnu.org/licenses/>. |
*************************************************************************/

#ifndef TILE_H
#define TILE_H

#include "tileset.h"   //tileset
#include "constants.h" //uint16

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

  Tile(Tileset* tileset, uint16 tileId, int gridId, int gridWidth, int gridHeight);

  //utility
  void getTileExists(bool* tileExists);
  void getAdjacentIndex(int* adjacentIndex);


  //overloads
  friend std::ostream& operator<<(std::ostream& out, const Tile& tile);
  friend std::istream& operator>>(std::istream& in, Tile& tile);

private:
  sigc::connection clickSig; // The signal connection for the click event
  int gridId;                // The tile's grid location
  int gridWidth;             // The width of the grid the tile is on
  int gridHeight;            // The height of the grid the tile is on
  int roomId;
  uint16 tileId;               // The id of the tileset's tile(image) this tile displays
  Tileset* tileset;          // The Tileset this tile uses
  bool locked = false;       // Protection against multiple recursion

  virtual bool on_enter_notify_event(GdkEventCrossing* crossing_event);
  void print(Cairo::RefPtr<Cairo::Context>& cr);
  void reset();
  
  //signal overload
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

  //operator overload
  void operator=(const Tile& sourceTile);
};

#endif
