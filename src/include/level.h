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

#ifndef LEVEL_H
#define LEVEL_H

#include "tile.h"    //tile
#include "tileset.h" //tileset

#include <fstream> //operator<<|operator>>
#include <memory>  //tile
#include <vector>  //^

#include <gtkmm/grid.h> //inheritance

/*!
  Level data structure. Handles Tile updates.
**/
struct Level : public Gtk::Grid
{
  friend class Map; //Level is owned by Map

  Level(Tileset* tileset, int id, int width, int height);
  Level(Tileset* tileset, std::ifstream& file);

  void center();
  void deleteColumns(int colNum, int count);
  void deleteRows(int rowNum, int count);
  void fit(int borderWidth);
  void insertColumns(int colNum, int count);
  void insertRows(int rowNum, int count);
  void reset();
  void shift(int x, int y);

  //utility
  int getTileSize() const;
  void setTileSize(int levelIndex);


  //overloads
  friend std::ostream& operator<<(std::ostream& out, const Level& level);
  friend std::istream& operator>>(std::istream& in, Level& level);

private:
  int largestRoomId;                       // The largest room id of the Level
  int lastRoomId;                          // The last used room id of the Level
  int height;                              // The Level;s height
  int id;                                  // The Level's id
  std::vector<std::unique_ptr<Tile>> tile; // The Tiles in the Level
  Tileset* tileset;                        // The Tileset that the Level uses
  int width;                               // The Level's width
  
  bool clickEvent(GdkEventButton* event, int* gridId);
  void createNewTile(int i = -1);
  bool enterEvent (GdkEventCrossing* event, int* gridId);
  void getExtents(int* extents);
  void print(Cairo::RefPtr<Cairo::PdfSurface>& surface, Cairo::RefPtr<Cairo::Context>& cr);
  void propInit();
  void updateCornerBits(int gridId, bool propagate);
  void updateTile(int* gridId, guint keyMask);
};

#endif
