#include "map.h"

#include <gtk/gtk.h>
#include <fstream>
#include <limits>

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
bool Map::saveToFile(std::string _filepath)
{
  std::ofstream file(_filepath.c_str());

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

std::ostream& operator<<(std::ostream& _out, const Map& _map)
{
  _out << "Map["
       << _map.size << ','
       << _map.tilesetFile << ']' << '\n';
  _out.flush();
  return _out;
}

std::istream& operator>>(std::istream& _in, Map& _map)
{
  static const std::streamsize MAX = std::numeric_limits<std::streamsize>::max();

  _in.ignore(MAX, '[');
  _in >> _map.size;
  _in.get(); //discard comma
  std::getline(_in, _map.tilesetFile, ']');
  _in.ignore(MAX, '\n'); //getline doesn't eat this newline

  //leave stream in good state by discarding empty lines
  while(_in.peek() == '\n')
    {_in.get();}

  return _in;
}
