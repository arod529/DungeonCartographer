#include "map.h"

#include <gtk/gtk.h>
#include <fstream>
#include <limits>

/*!
  Initializes a map with defaults from Settings. Creates a starting Level.

  @param[in] settings The program settings
**/
Map::Map(Settings* settings)
: size(settings->mapSize)
, tilesetFile(settings->defaultTilesetFile)
{
  //set default tileset
  loadTileset(settings->defaultTilesetFile);

  //create a level
  level.emplace_back(0, size, tilesetFile, &tileset);
}

/*!
  Load a tileset from a file

  @param[in] filepath File path for the tile set to load.

  \bug doesn't check if file exists
**/
void Map::loadTileset(std::string filepath)
{
  //open file
  std::ifstream file(filepath);

  auto pos = file.tellg();
  uint tilesetId = 0;

  //read file
  do
  {
    pos = file.tellg(); //get current streambuf position
    file >> tilesetId; //get id
    file.seekg(pos); //return to previous position
    file >> tileset[tilesetId]; //add TilesetTile to tileset
  } while(!file.eof());

  //close file
  file.close();
}

/*!
  Writes a map to file.

  @param[in] _filename The path to the file.
**/
bool Map::saveToFile(std::string filepath)
{
  std::ofstream file(filepath);

  //write map to file
  file << *this;

  //append levels to file
  for(int i = 0; i < level.size(); i++)
  {
    file << level[i];
    for(int j = 0; j < level[i].tile.size(); j++)
    {
      file << level[i].tile[j];
    }
  }

  file.close();
  return true;
}

std::ostream& operator<<(std::ostream& out, const Map& map)
{
  out << "Map["
       << map.size << ','
       << map.tilesetFile << ']' << '\n';
  out.flush();
  return out;
}

std::istream& operator>>(std::istream& in, Map& map)
{
  static const std::streamsize MAX = std::numeric_limits<std::streamsize>::max();

  in.ignore(MAX, '[');
  in >> map.size;
  in.get(); //discard comma
  std::getline(in, map.tilesetFile, ']');
  in.ignore(MAX, '\n'); //getline doesn't eat this newline

  //leave stream in good state by discarding empty lines
  while(in.peek() == '\n')
    {in.get();}

  return in;
}
