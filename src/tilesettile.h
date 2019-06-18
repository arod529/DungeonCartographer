#ifndef TILESETTILE_H
#define TILESETTILE_H

#include <glibmm/refptr.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/drawingarea.h>

#include <string>

/*!
  TilesetTileTile data structure. Contains the id, name, file path, and pixbuf of the
  tile image. An array of TilesetTile structs defines a complete tileset.
**/
struct TilesetTile
{
public:
  // TilesetTile(uint id = 0, std::string name = "", std::string filePath = "", Glib::RefPtr<Gdk::Pixbuf> pixbuf);
  // ~TilesetTile();

  //accessors
  uint getId() const;
  std::string getName() const;
  std::string getFilePath() const;

  //util
  bool drawTile(Gtk::DrawingArea* drawingArea, Cairo::RefPtr<Cairo::Context> cr);

  //overloads
  friend std::istream& operator>>(std::istream& in, TilesetTile& tilesettile);

private:
  uint id;
  std::string name;
  std::string filePath;
  Glib::RefPtr<Gdk::Pixbuf> pixbuf;
};

#endif
