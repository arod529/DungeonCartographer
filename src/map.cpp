/*************************************************************************
| Dungeon Cartographer, a grid based map drawing program.                |
| Copyright (C) 2019  Alexander L. Hopper                                |
|                                                                        |
| This program is free software: you can redistribute it and/or modify   |
| it under the terms of the GNU General Public License as published by   |
| the Free Software Foundation, either version 3 of the License, or      |
| (at your option) any later version.                                    |
|                                                                        |
| This program is distributed in the hope that it will be useful,        |
| but WITHOUT ANY WARRANTY; without even the implied warranty of         |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          |
| GNU General Public License for more details.                           |
|                                                                        |
| You should have received a copy of the GNU General Public License      |
| along with this program.  If not, see <https://www.gnu.org/licenses/>. |
*************************************************************************/

#include "map.h"

#include <limits>   // operator>>
#include <iostream> // operator<<|operator>>

Map::~Map()
{
  clearMap(); //prevents some a double free err on exit; find cause later?
}

//------------------
//----- PUBLIC -----
//------------------

/*!
  Appends a Level to the Map.
**/
void Map::appendLevel()
{
  level.emplace_back(std::make_unique<Level>(tileset, level.size(), width, height));
  signal_levelCreated(level.back()->id, level.back().get());
}

/*!
  Centers the Level.

  @param[in] levelIndex The index of the level to center.
**/
void Map::centerLevel(int levelIndex)
  {level[levelIndex]->center();}

/*!
  Deletes columns from the Level

  @param[in] levelIndex The index of the Level to delete columns from.
  @param[in] colNum The index of the col at which to begin removal.
  @param[in] count The number of rows to remove.
**/
void Map::deleteColumns(int levelIndex, int colNum, int count)
  {level[levelIndex]->deleteColumns(colNum, count);}

/*!
  Deletes rows from the Level

  @param[in] levelIndex The index of the Level to delete rows from.
  @param[in] rowNum The index of the row at which to begin removal.
  @param[in] count The number of rows to remove.
**/
void Map::deleteRows(int levelIndex, int rowNum, int count)
  {level[levelIndex]->deleteRows(rowNum, count);}

/*!
  Fits a Level to its drawn portion with a given border width.

  @param[in] levelIndex the index of the Level to fit.
  @param[in] borderWidth The width of the border in number of tiles.
**/
void Map::fitLevel(int levelIndex, int borderWidth)
  {level[levelIndex]->fit(borderWidth);}

/*!
  Inserts columns into the Level.

  @param[in] levelIndex The index of the Level to insert rows into.
  @param[in] colNum The index of the column at which to begin insertion.
  @param[in] count The number of columns to insert.
**/
void Map::insertColumns(int levelIndex, int colNum, int count)
  {level[levelIndex]->insertColumns(colNum, count);}

/*!
  Inserts rows into the Level.

  @param[in] levelIndex The index of the Level to insert rows into.
  @param[in] rowNum The index of the row at which to begin insertion.
  @param[in] count The number of rows to insert.
**/
void Map::insertRows(int levelIndex, int rowNum, int count)
  {level[levelIndex]->insertRows(rowNum, count);}

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

/*!
  Creates a new map.

  @param[in] mapTileset The tileset to use for the Map.
  @param[in] mapSize The default size of Levels in the Map.
**/
void Map::newMap(Tileset* mapTileset, int mapWidth, int mapHeight)
{
  //clear any existing levels
  clearMap();

  //set properties
  width = mapWidth;
  height = mapHeight;
  tileset = mapTileset;
  filepath = "";

  //append a new level
  appendLevel();
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
  Shifts the Level the given direction and magnitude.

  @param[in] levelIndex The index of the Level to shift.
  @param[in] x The direction and magnitude of the horizontal shift.
  @param[in] y The direction and magnitude of the vertical shift.
**/
void Map::shiftLevel(int levelIndex, int x, int y)
{
  level[levelIndex]->shift(x, y);
}

//-------------------
//----- Utility -----
//-------------------
std::string Map::getFilePath() const
  {return filepath;}

int Map::getLevelHeight(int levelIndex) const
  {return level[levelIndex]->height;}

int Map::getLevelWidth(int levelIndex) const
  {return level[levelIndex]->width;}

int Map::getTileSize(int levelIndex) const
  {return level[levelIndex]->getTileSize();}

void Map::setTileSize(int levelIndex, int tileSize)
  {level[levelIndex]->setTileSize(tileSize);}

//-------------------
//----- PRIVATE -----
//-------------------

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

//---------------------
//----- Overloads -----
//---------------------
std::ostream& operator<<(std::ostream& out, const Map& map)
{
  out << "Map["
      << map.width << ','
      << map.height << ','
      << map.tileset->filepath << ']' << '\n';
  out.flush();
  return out;
}

std::istream& operator>>(std::istream& in, Map& map)
{
  static const std::streamsize MAX = std::numeric_limits<std::streamsize>::max();

  in.ignore(MAX, '[');
  in >> map.width;
  in.get(); //discard comma
  in >> map.height;
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
