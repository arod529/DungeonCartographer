#ifndef TILESETTILE_H
#define TILESETTILE_H

#include <gtk/gtk.h>
#include <string>

/*!
  TilesetTileTile data structure. Contains the id, name, file path, and pixbuf of the
  tile image. An array of TilesetTile structs defines a complete tileset.
**/
struct TilesetTile
{
public:
  TilesetTile();
  TilesetTile(uint _id, std::string _name, std::string _filePath, GdkPixbuf* _pixbuf);
  ~TilesetTile();

  //accessors
  uint getId() const;
  std::string getName() const;
  std::string getFilePath() const;

  //events
  static bool event_drawTile(GtkWidget* drawingArea, cairo_t* cr);

  //overloads
  friend std::istream& operator>>(std::istream& _in, TilesetTile& _tilesettile);

private:
  uint id;
  std::string name;
  std::string filePath;
  GdkPixbuf* pixbuf;
};

#endif
