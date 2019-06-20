#include "tilesettile.h"
#include "tile.h"

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
// TilesetTile::TilesetTile(uint id, std::string name, std::string filePath, Glib::RefPtr<Gdk::Pixbuf> pixbuf)
// : id(id), name(name), filePath(filePath), pixbuf(pixbuf) {}

/*!
  Free the memmory used by the pixbuf
**/
// TilesetTile::~TilesetTile()
//   {g_object_unref(pixbuf);}

uint TilesetTile::getId() const
  {return id;}

std::string TilesetTile::getName() const
  {return name;}

std::string TilesetTile::getFilePath() const
  {return (std::string)filePath;}

/*!
  Initialize a TilesetTile from a file stream.

  Creates a new pixbuf.
**/
std::istream& operator>>(std::istream& in, TilesetTile& tilesettile)
{
  static const std::streamsize MAX = std::numeric_limits<std::streamsize>::max();

  in >> tilesettile.id; //get id
  in.ignore(MAX, ','); //eat comma
  std::getline(in, tilesettile.name, ','); //get name
  std::getline(in, tilesettile.filePath); //get file path

  tilesettile.pixbuf = Gdk::Pixbuf::create_from_file(tilesettile.filePath);

  //leave stream in good state by discarding empty lines
  while(in.peek() == '\n')
    {in.get();}

  return in;
}
