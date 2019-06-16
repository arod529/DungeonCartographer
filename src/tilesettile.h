#ifndef TILESETTILE_H
#define TILESETTILE_H

#include <gtk/gtk.h>
#include <string>

/*!
  TilesetTileTile data structure. Contains the id, name, file path, and pixbuf of the
  tile image. An array of TilesetTile structs defines a complete TilesetTile.

  \bug variables should be private, and read only aside from initialization.
**/
struct TilesetTile
{
public:
  TilesetTile();
  TilesetTile(uint _id, std::string _name, std::string _filePath, GdkPixbuf* _pixbuf);
  ~TilesetTile();

  uint getId() const;
  std::string getName() const;
  std::string getFilePath() const;

  GdkPixbuf* pixbuf;
private:
  uint id;
  std::string  name;
  std::string filePath;
};

#endif
