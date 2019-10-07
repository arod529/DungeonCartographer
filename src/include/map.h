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

#ifndef MAP_H
#define MAP_H

#include "constants.h" //uint
#include "level.h"     //level
#include "tileset.h"   //tileset

#include <fstream> //operator<<|operator>>
#include <memory>  //level
#include <string>  //saveToFile()|openFile()|filepath
#include <vector>  //level

#include <sigc++/sigc++.h> //signals

/*!
  Map data structure. Handles map file manipulation, and Level interface with the UI.
**/
class Map
{
  public:
    ~Map();

    void appendLevel();
    void centerLevel(int levelIndex);
    void deleteColumns(int levelIndex, int colNum, int count);
    void deleteRows(int levelIndex, int rowNum, int count);
    void fitLevel(int levelIndex, int borderWidth);
    void insertColumns(int levelIndex, int colNum, int count);
    void insertRows(int levelIndex, int rowNum, int count);
    bool loadFile(std::string filepath);
    void newMap(Tileset* mapTileset, int mapWidth, int mapHeight);
    void print();
    void resetLevel(int levelIndex);
    bool saveToFile(std::string filepath);
    void shiftLevel(int levelIndex, int x, int y);
    
    //utility
    std::string getFilePath() const;
    int getLevelHeight(int levelIndex) const;
    int getLevelWidth(int levelIndex) const;
    int getTileSize(int levelIndex) const;
    void setTileSize(int levelIndex, int tileSize);

    //signals
    sigc::signal<void, uint, Level*> signal_levelCreated; // A level has been created.
    sigc::signal<void> signal_mapCleared; // The map has been cleared. No levels exist.

    //overloads
    friend std::ostream& operator<<(std::ostream& out, const Map& map);
    friend std::istream& operator>>(std::istream& in, Map& map);

  private:
    int width;                                 //default level size
    int height;                                //default level size
    Tileset* tileset;                          //The tile set that the map uses
    std::string filepath = "";                 //The file path associated with this map
    std::vector<std::unique_ptr<Level>> level; //The levels in the map
    
    void clearMap();
};

#endif
