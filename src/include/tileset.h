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

#ifndef TILESET_H
#define TILESET_H

#include "constants.h"

#include <fstream>
#include <limits>
#include <string>
#include <unordered_map>

#include <glibmm/refptr.h>
#include <gdkmm/pixbuf.h>

/*!
  TilesetTileTile data structure. Contains the id, name, file path, and pixbuf of the
  tile image. An array of TilesetTile structs defines a complete tileset.
**/
struct TilesetTile
{
  friend class Tile;
public:

  int getSize();

  //overloads
  friend std::istream& operator>>(std::istream& in, TilesetTile& tilesettile);

private:
  uint id;                          // The id representing the image
  std::string name;                 // The display name of the image
  std::string filePath;             // The path to the image
  Glib::RefPtr<Gdk::Pixbuf> pixbuf; // A pixbuf created from the image
};

/*!
  Tileset data structure. Manages tileset files.
**/
class Tileset
{
public:
  std::string name{""};
  std::string filepath{""};
  std::unordered_map<uint, TilesetTile> tile;

  Tileset(const std::string fpath = "");

  bool loadFile(const std::string fpath);
  bool reload();
  bool isTileset(const std::string fpath);
  bool writeDefaultTilesetFile(const std::string fpath);
};

#endif
