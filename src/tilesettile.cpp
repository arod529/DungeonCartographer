#include "tilesettile.h"
#include "tile.h"

#include <gtk/gtk.h>

#include <fstream>
#include <limits>

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
TilesetTile::TilesetTile(uint id, std::string name, std::string filePath, GdkPixbuf* _pixbuf)
: id(id), name(name), filePath(filePath), pixbuf(pixbuf) {}

/*!
  Free the memmory used by the pixbuf
**/
TilesetTile::~TilesetTile()
  {g_object_unref(pixbuf);}

uint TilesetTile::getId() const
  {return id;}

std::string TilesetTile::getName() const
  {return name;}

std::string TilesetTile::getFilePath() const
  {return (std::string)filePath;}

bool TilesetTile::drawTile(GtkWidget* drawingArea, cairo_t* cr)
{
  //get drawing area size
  int s = gtk_widget_get_allocated_width(drawingArea);

  //get current assigned pixbuf and scale
  GdkPixbuf* tmpPixbuf = gdk_pixbuf_scale_simple(pixbuf,s,s,GDK_INTERP_NEAREST);

  //draw the image
  gdk_cairo_set_source_pixbuf(cr,tmpPixbuf,0,0);
  cairo_paint(cr);

  //free mem
  g_object_unref(tmpPixbuf);

  return false;
}

/*!
  Initialize a TilesetTile from a file stream.

  Creates a new pixbuf and writes any resulting errors to stderr.
**/
std::istream& operator>>(std::istream& in, TilesetTile& tilesettile)
{
  static const std::streamsize MAX = std::numeric_limits<std::streamsize>::max();

  in >> tilesettile.id; //get id
  in.ignore(MAX, ','); //eat comma
  std::getline(in, tilesettile.name, ','); //get name
  std::getline(in, tilesettile.filePath); //get file path

  GError *err = NULL;
  tilesettile.pixbuf = gdk_pixbuf_new_from_file(tilesettile.filePath.c_str(), &err);
  if(err != NULL)
  {
    fprintf(stderr, "ERROR: Dungeon Cartographer @ TilesetTile>>: pixbuf error for file: %s\n", tilesettile.filePath.c_str());
    fprintf(stderr, "%s\n", err->message);
  }

  //leave stream in good state by discarding empty lines
  while(in.peek() == '\n')
    {in.get();}

  return in;
}
