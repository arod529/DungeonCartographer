#include "tilesettile.h"
#include "tile.h"

#include <fstream>
#include <limits>

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
