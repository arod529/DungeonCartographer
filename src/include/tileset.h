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

  //overloads
  friend std::istream& operator>>(std::istream& in, TilesetTile& tilesettile);

private:
  uint id;                          // The id representing the image
  std::string name;                 // The display name of the image
  std::string filePath;             // The path to the image
  Glib::RefPtr<Gdk::Pixbuf> pixbuf; // A pixbuf created from the image
};

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
