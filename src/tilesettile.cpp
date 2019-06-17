#include "tilesettile.h"
#include "tile.h"

#include <gtk/gtk.h>

#include <fstream>
#include <limits>

/*!
  Default empty initializers.
**/
TilesetTile::TilesetTile()
{
  id = 0;
  name = "";
  filePath = "";
  pixbuf = NULL;
}

/*!
  Assignment initializer for TilesetTile .

  @param[in] _id The id of the tile struct; a binary number that represents the
                  walls and corner bits that the tile represents. \n
                  |se|sw|nw|ne|s|w|n|e| \n
                  bit[0-3] = wall bits \n
                  bit[4-7] = corner bits
  @param[in] _name The display name of the tile.
  @param[in] _filePath Path to the svg file of the tile.
  @param[in] _pixbuf The pixbuf for the tile.
**/
TilesetTile::TilesetTile(uint _id, std::string _name, std::string _filePath, GdkPixbuf* _pixbuf)
{
  id = _id;
  name = _name;
  filePath = _filePath;
  pixbuf = _pixbuf;
}

TilesetTile::~TilesetTile()
{
  //free memory of pixbuf
  g_object_unref(pixbuf);
}

uint TilesetTile::getId() const
  {return id;}

std::string TilesetTile::getName() const
  {return name;}

std::string TilesetTile::getFilePath() const
  {return (std::string)filePath;}

bool TilesetTile::event_drawTile(GtkWidget* drawingArea, cairo_t* cr)
{
  //get drawing area size
  int s = gtk_widget_get_allocated_width(drawingArea);
  //get current assigned pixbuf and scale
  Tile* _tile = (Tile*)g_object_get_data(G_OBJECT(drawingArea),"tile");
  GdkPixbuf* tmp = gdk_pixbuf_scale_simple(_tile->tilesetTile->pixbuf,s,s,GDK_INTERP_NEAREST);
  //draw the image
  gdk_cairo_set_source_pixbuf(cr,tmp,0,0);
  cairo_paint(cr);

  //free mem
  g_object_unref(tmp);

  return false;
}

std::istream& operator>>(std::istream& _in, TilesetTile& _tilesettile)
{
  static const std::streamsize MAX = std::numeric_limits<std::streamsize>::max();

  _in >> _tilesettile.id; //get id
  _in.ignore(MAX, ','); //eat comma
  std::getline(_in, _tilesettile.name, ','); //get name
  std::getline(_in, _tilesettile.filePath); //get file path
  while(_in.peek() == '\n') //discard empty lines
    {_in.get();}

  GError *err = NULL;
  _tilesettile.pixbuf = gdk_pixbuf_new_from_file(_tilesettile.filePath.c_str(), &err);
  if(err != NULL)
  {
    fprintf(stderr, "ERROR: Dungeon Cartographer @ TilesetTile>>: pixbuf error for file: %s\n", _tilesettile.filePath.c_str());
    fprintf(stderr, "%s\n", err->message);
  }

  return _in;
}
