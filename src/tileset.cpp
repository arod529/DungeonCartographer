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

#include "tileset.h"

//-------------------
//----- Tileset -----
//-------------------

/*!
  Initializes the Tileset by loading a file.

  @param[in] fpath The path to the file to load.
**/
Tileset::Tileset(std::string fpath)
: filepath{fpath}
{
  if(fpath != "") loadFile(filepath);
}

/*!
  Load a tileset from a file

  @param[in] fpath File path for the tile set to load.

  \bug doesn't check if file exists
**/
bool Tileset::loadFile(std::string fpath)
{
  //open file
  std::ifstream file(fpath);

  //Make sure file can be opened
  if(!file.is_open())
  {
    fprintf(stderr, "ERROR: Dungeon Cartographer @ Tileset::loadFile: The file could not be opened: %s\n", fpath);
    return false;
  }

  auto pos = file.tellg();
  uint tilesetId = 0;

  //get name
  std::getline(file, name);

  //get tiles
  do
  {
    pos = file.tellg();      // get current streambuf position
    file >> tilesetId;       // get id
    file.seekg(pos);         // return to previous position
    file >> tile[tilesetId]; // add TilesetTile to tileset
  } while(!file.eof());

  //close file
  file.close();

  filepath = fpath;

  return true;
}

/*!
  Reloads the Tileset from the current file.
**/
bool Tileset::reload()
  {return loadFile(filepath);}

/*!
  Determines if the Tileset is using the file.

  @param[in] fpath The path of the file to compare to.

  @return Whether the Tileset is using the file.
**/
bool Tileset::isTileset(std::string fpath)
  {return (fpath == filepath);}

/*!
  Writes a default Tileset file to disk.

  @param[in] fpath The file path to write the file too.

  @return The file write success
**/
bool Tileset::writeDefaultTilesetFile(std::string fpath)
{
  //open file
  std::ofstream file(fpath);

  //Make sure file can be opened
  if(!file.is_open())
  {
    fprintf(stderr, "ERROR: Dungeon Cartographer @ writeDefaultTilesetFile: The file could not be opened: %s\n", filepath);
    return false;
  }

  //write to file (id,name,filePath\0\n) for each tile
  file << "Default Dungeon Cartographer Tileset" << '\n'
  
       << E       << ',' << "E Wall"              << ',' << "./tiles/eWall.svg"             << '\n'
       << E_NW    << ',' << "E Wall NW Corner"    << ',' << "./tiles/eWall_nwCorner.svg"    << '\n'
       << E_SW    << ',' << "E Wall SW Corner"    << ',' << "./tiles/eWall_swCorner.svg"    << '\n'
       << E_NW_SW << ',' << "E Wall NW-SW Corner" << ',' << "./tiles/eWall_nw-swCorner.svg" << '\n'

       << N       << ',' << "N Wall"              << ',' << "./tiles/nWall.svg"             << '\n'
       << N_SW    << ',' << "N Wall SE Corner"    << ',' << "./tiles/nWall_seCorner.svg"    << '\n'
       << N_SE    << ',' << "N Wall SW Corner"    << ',' << "./tiles/nWall_swCorner.svg"    << '\n'
       << N_SW_SE << ',' << "N Wall SE-SW Corner" << ',' << "./tiles/nWall_se-swCorner.svg" << '\n'

       << W       << ',' << "W Wall"              << ',' << "./tiles/wWall.svg"             << '\n'
       << W_NE    << ',' << "W Wall NE Corner"    << ',' << "./tiles/wWall_neCorner.svg"    << '\n'
       << W_SE    << ',' << "W Wall SE Corner"    << ',' << "./tiles/wWall_seCorner.svg"    << '\n'
       << W_NE_SE << ',' << "W Wall NE-SE Corner" << ',' << "./tiles/wWall_ne-seCorner.svg" << '\n'

       << S       << ',' << "S Wall"              << ',' << "./tiles/sWall.svg"             << '\n'
       << S_NE    << ',' << "S Wall NE Corner"    << ',' << "./tiles/sWall_neCorner.svg"    << '\n'
       << S_NW    << ',' << "S Wall NW Corner"    << ',' << "./tiles/sWall_nwCorner.svg"    << '\n'
       << S_NE_NW << ',' << "S Wall NE-NW Corner" << ',' << "./tiles/sWall_ne-nwCorner.svg" << '\n'

       << NE    << ',' << "NE Wall"           << ',' << "./tiles/neWall.svg"          << '\n'
       << NE_SW << ',' << "NE Wall SW Corner" << ',' << "./tiles/neWall_swCorner.svg" << '\n'
       << SE    << ',' << "SE Wall"           << ',' << "./tiles/seWall.svg"          << '\n'
       << SE_NW << ',' << "SE Wall NW Corner" << ',' << "./tiles/seWall_nwCorner.svg" << '\n'
       << NW    << ',' << "NW Wall"           << ',' << "./tiles/nwWall.svg"          << '\n'
       << NW_SE << ',' << "NW Wall SE Corner" << ',' << "./tiles/nwWall_seCorner.svg" << '\n'
       << SW    << ',' << "SW Wall"           << ',' << "./tiles/swWall.svg"          << '\n'
       << SW_NE << ',' << "SW Wall NE Corner" << ',' << "./tiles/swWall_neCorner.svg" << '\n'

       << WE << ',' << "EW Wall" << ',' << "./tiles/ewWall.svg" << '\n'
       << NS << ',' << "NS Wall" << ',' << "./tiles/nsWall.svg" << '\n'

       << NEW  << ',' << "NEW Wall"  << ',' << "./tiles/newWall.svg"  << '\n'
       << NSE  << ',' << "NSE Wall"  << ',' << "./tiles/nseWall.svg"  << '\n'
       << SEW  << ',' << "SEW Wall"  << ',' << "./tiles/sewWall.svg"  << '\n'
       << NSW  << ',' << "NSW Wall"  << ',' << "./tiles/nswWall.svg"  << '\n'
       << NSEW << ',' << "NSEW Wall" << ',' << "./tiles/nsewWall.svg" << '\n'

       << O             << ',' << "Open"                    << ',' << "./tiles/open.svg"                   << '\n'
       << O_NE          << ',' << "Open NE Corner"          << ',' << "./tiles/open_neCorner.svg"          << '\n'
       << O_NW          << ',' << "Open NW Corner"          << ',' << "./tiles/open_nwCorner.svg"          << '\n'
       << O_NE_NW       << ',' << "Open NE-NW Corner"       << ',' << "./tiles/open_ne-nwCorner.svg"       << '\n'
       << O_SW          << ',' << "Open SW Corner"          << ',' << "./tiles/open_swCorner.svg"          << '\n'
       << O_NE_SW       << ',' << "Open NE-SW Corner"       << ',' << "./tiles/open_ne-swCorner.svg"       << '\n'
       << O_NW_SW       << ',' << "Open NW-SW Corner"       << ',' << "./tiles/open_nw-swCorner.svg"       << '\n'
       << O_NE_NW_SW    << ',' << "Open NE-NW-SW Corner"    << ',' << "./tiles/open_ne-nw-swCorner.svg"    << '\n'
       << O_SE          << ',' << "Open SE Corner"          << ',' << "./tiles/open_seCorner.svg"          << '\n'
       << O_NE_SE       << ',' << "Open NE-SE Corner"       << ',' << "./tiles/open_ne-seCorner.svg"       << '\n'
       << O_NW_SE       << ',' << "Open NW-SE Corner"       << ',' << "./tiles/open_nw-seCorner.svg"       << '\n'
       << O_NE_NW_SE    << ',' << "Open NE-NW-SE Corner"    << ',' << "./tiles/open_ne-nw-seCorner.svg"    << '\n'
       << O_SW_SE       << ',' << "Open SW-SE Corner"       << ',' << "./tiles/open_sw-seCorner.svg"       << '\n'
       << O_NE_SW_SE    << ',' << "Open NE-SW-SE Corner"    << ',' << "./tiles/open_ne-sw-seCorner.svg"    << '\n'
       << O_NW_SW_SE    << ',' << "Open NW-SW-SE Corner"    << ',' << "./tiles/open_nw-sw-seCorner.svg"    << '\n'
       << O_NE_NW_SW_SE << ',' << "Open NE-NW-SW-SE Corner" << ',' << "./tiles/open_ne-nw-sw-seCorner.svg" << '\n'

       << BACKGROUND << ',' << "Background" << ',' << "./tiles/bg.svg" << '\n';

  //close file
  file.close();

  return true;
}

//---------------------
//----- Overloads -----
//---------------------
std::istream& operator>>(std::istream& in, TilesetTile& tilesettile)
{
  static const std::streamsize MAX = std::numeric_limits<std::streamsize>::max();

  in >> tilesettile.id;                    // get id
  in.ignore(MAX, ',');                     // eat comma
  std::getline(in, tilesettile.name, ','); // get name
  std::getline(in, tilesettile.filePath);  // get file path

  tilesettile.pixbuf = Gdk::Pixbuf::create_from_file(tilesettile.filePath);

  //leave stream in good state by discarding empty lines
  while(in.peek() == '\n')
    {in.get();}

  return in;
}

int TilesetTile::getSize()
{
  return pixbuf->get_width();
}