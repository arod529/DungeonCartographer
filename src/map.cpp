#include "map.h"

#include <fstream>
#include <limits>

/*!
  Initializes a map with defaults from Settings. Creates a starting Level.

  @param[in] settings The program settings
**/
Map::Map(Settings* settings, std::string mapFile)
: size{settings->mapSize}
, tileset{&settings->tileset}
{
  if(mapFile != "")
  {
    openFile(mapFile);
  }
  else //create a default level
  {
    level.emplace_back(tileset, 0, size);  
  }
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

bool Map::openFile(std::string fPath)
{
  std::ifstream file(fPath);

  //check file is open
  if(!file.is_open())
  {
    fprintf(stderr, "ERROR: Dungeon Cartographer @ Map::openFile: The file could not be opened: %s\n", fPath.c_str());
    return false;
  }

  //read map
  file >> *this;

  //read level (there must be at least one)
  do
  {
    level.emplace_back(Level(tileset));
    file >> level.back();
  } while(!file.eof());

  return true;
}

std::ostream& operator<<(std::ostream& out, const Map& map)
{
  out << "Map["
       << map.size << ','
       << map.tileset->filepath << ']' << '\n';
  out.flush();
  return out;
}

std::istream& operator>>(std::istream& in, Map& map)
{
  static const std::streamsize MAX = std::numeric_limits<std::streamsize>::max();

  in.ignore(MAX, '[');
  in >> map.size;
  in.get(); //discard comma

  std::string tilesetFile = "";
  std::getline(in, tilesetFile, ']');
  if(!map.tileset->isTileset(tilesetFile))
    {map.tileset = new Tileset(tilesetFile);}

  in.ignore(MAX, '\n'); //getline doesn't eat this newline

  //leave stream in good state by discarding empty lines
  while(in.peek() == '\n')
    {in.get();}

  return in;
}
