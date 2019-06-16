#include "tilesettile.h"

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

}

uint TilesetTile::getId() const
  {return id;}

std::string TilesetTile::getName() const
  {return name;}

std::string TilesetTile::getFilePath() const
  {return (std::string)filePath;}
