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
public:
  //accessors
  uint getId() const;
  std::string getName() const;
  std::string getFilePath() const;

  //overloads
  friend std::istream& operator>>(std::istream& in, TilesetTile& tilesettile);

private:
  uint id;
  std::string name;
  std::string filePath;
  Glib::RefPtr<Gdk::Pixbuf> pixbuf;
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
