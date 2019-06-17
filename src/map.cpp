#include "map.h"

#include <gtk/gtk.h>
#include <fstream>

/*!
  Initializes a map with defaults from Settings. Creates a starting Level.

  @param[in] _settings The program settings
**/
Map::Map(Settings* _settings)
{
  //set default size
  size = _settings->getMapSize();
  tilesetFile = _settings->defaultTilesetFile;

  //set default tileset
  loadTileset(_settings->defaultTilesetFile);

  //create a level
  level.emplace_back(size, &tileset);
}

/*!
  Load a tileset from a file

  @param[in] tileSetFile File path for the tile set to load.

  \bug doesn't check if file exists
**/
void Map::loadTileset(std::string tileSetFile)
{
  //open file
  std::ifstream file(tileSetFile);

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
bool Map::saveMap(std::string _filepath)
{
  std::ofstream file(_filepath.c_str());

  //write map settings to file
  file << size << '\n'
       << tilesetFile << '\n';

  //write levels to file


  file.close();
  return true;
}
