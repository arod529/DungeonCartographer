#include "map.h"

#include <limits>   // operator>>
#include <iostream> // operator<<|operator>>

Map::~Map()
{
  clearMap(); //prevents some a double free err on exit; find cause later?
}

/*!
  Appends a Level to the Map.
**/
void Map::appendLevel()
{
  level.emplace_back(std::make_unique<Level>(tileset, level.size(), size));
  signal_levelCreated(level.back()->id, level.back().get());
}

/*!
  Deletes all levels from the Map.
**/
void Map::clearMap()
{
  //emit map cleared signal
  signal_mapCleared();

  //clear existing levels
  level.clear();
}

/*!
  Loads a map file.

  @param[in] fPath The full path to the map file.

  @return The file loaded successfully.

  \bug no file content checks
**/
bool Map::loadFile(std::string fPath)
{
  std::ifstream file(fPath);

  //check file is open
  if(!file.is_open())
  {
    fprintf(stderr, "ERROR: Dungeon Cartographer @ Map::openFile: The file could not be opened: %s\n", fPath.c_str());
    return false;
  }

  clearMap();

  //read map
  file >> *this;

  //read level (there must be at least one)
  do
  {
    level.emplace_back(std::make_unique<Level>(tileset, file));
    signal_levelCreated(level.back()->id, level.back().get());
  }
  while(!file.eof());

  file.close();
  filepath = fPath;

  return true;
}

void Map::print()
{
  std::string fpath = "./test.pdf";
  
  //create surface
  auto surface = Cairo::PdfSurface::create(fpath, 0, 0);

  //create context
  auto cr = Cairo::Context::create(surface);

  //print levels to pdf
  for(int i = 0; i < level.size(); i++)
  {
    level[i]->print(surface, cr);
    cr->show_page();
  }
}

/*!
  Resets the Level back to default removing all modifications.
**/
void Map::resetLevel(int levelIndex)
{
  level[levelIndex]->reset();
}

/*!
  Writes a map to file.

  @param[in] fPath The path to the file.

  @return The file was written successfully.
**/
bool Map::saveToFile(std::string fPath)
{
  std::ofstream file(fPath);

  //check file is open
  if(!file.is_open())
  {
    fprintf(stderr, "ERROR: Dungeon Cartographer @ Map::saveToFile: The file could not be opened: %s\n", fPath.c_str());
    return false;
  }

  //write map to file
  file << *this;

  //append levels to file
  for(int i = 0; i < level.size(); i++)
  {
    file << *level[i];
    //append tiles to file
    for(int j = 0; j < level[i]->tile.size(); j++)
    {
      file << *level[i]->tile[j];
    }
  }

  file.close();
  filepath = fPath;

  return true;
}

/*!
  Creates a new map.

  @param[in] mapTileset The tileset to use for the Map.
  @param[in] mapSize The default size of Levels in the Map.
**/
void Map::newMap(Tileset* mapTileset, int mapSize)
{
  //clear any existing levels
  clearMap();

  //set properties
  size = mapSize;
  tileset = mapTileset;
  filepath = "";

  //append a new level
  appendLevel();
}

void Map::shiftLevel(int levelIndex, int x, int y)
{
  level[levelIndex]->shift(x, y);
}

//-------------------
//----- Utility -----
//-------------------
std::string Map::getFilePath() const
  {return filepath;}

int Map::getLevelSize(int levelIndex) const
  {return level[levelIndex]->size;}

int Map::getTileSize(int levelIndex) const
  {return level[levelIndex]->getTileSize();}

void Map::setTileSize(int levelIndex, int tileSize)
  {level[levelIndex]->setTileSize(tileSize);}

//---------------------
//----- Overloads -----
//---------------------
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
