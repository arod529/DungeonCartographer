#include "tileset.h"

/*!
  Default empty initializers.
**/
Tileset::Tileset()
{
  id = 0;
  name = "";
  filePath = "";
  pixbuf = NULL;
}

/*!
  Assignment initializer for tileset .

  @param[in] _id The id of the tile struct; a binary number that represents the
                  walls and corner bits that the tile represents. \n
                  |se|sw|nw|ne|s|w|n|e| \n
                  bit[0-3] = wall bits \n
                  bit[4-7] = corner bits
  @param[in] _name The display name of the tile.
  @param[in] _filePath Path to the svg file of the tile.
  @param[in] _pixbuf The pixbuf for the tile.
**/
Tileset::Tileset(uint _id, std::string _name, std::string _filePath, GdkPixbuf* _pixbuf)
{
  id = _id;
  name = _name;
  filePath = _filePath;
  pixbuf = _pixbuf;
}

Tileset::~Tileset()
{

}

uint Tileset::getId() const
  {return id;}

std::string Tileset::getName() const
  {return name;}

std::string Tileset::getFilePath() const
  {return (std::string)filePath;}
