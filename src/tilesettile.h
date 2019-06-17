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
  TilesetTile(uint id = 0, std::string name = "", std::string filePath = "", GdkPixbuf* pixbuf = NULL);
  ~TilesetTile();

  //accessors
  uint getId() const;
  std::string getName() const;
  std::string getFilePath() const;

  //events
  static bool event_drawTile(GtkWidget* drawingArea, cairo_t* cr);

  //overloads
  friend std::istream& operator>>(std::istream& in, TilesetTile& tilesettile);

private:
  uint id;
  std::string name;
  std::string filePath;
  GdkPixbuf* pixbuf;
};

#endif
