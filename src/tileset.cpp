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

#include <ios>

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
  Gets the pixbuf pointer of a given tile id.

  @param[in] i The id of the tile
  @return The pixbuf
**/
Glib::RefPtr<Gdk::Pixbuf> Tileset::getPixbuf(int i)
{
  return tile[i].pixbuf;
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
  uint16 tilesetId = 0;

  //get name
  std::getline(file, name);

  file >> std::hex;
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

  //write to file (id,name,filePath\n) for each tile
  file << std::hex;
  file << "Default Dungeon Cartographer Tileset" << '\n'
  
  //East wall variants
  << E         << ',' <<                     "E Wall" << ',' <<                   "./tiles/eWall.svg" << '\n'
  << E_NW      << ',' <<           "E Wall NW Corner" << ',' <<          "./tiles/eWall_nwCorner.svg" << '\n'
  << E_SW      << ',' <<           "E Wall SW Corner" << ',' <<          "./tiles/eWall_swCorner.svg" << '\n'
  << E_NW_SW   << ',' <<        "E Wall NW-SW Corner" << ',' <<       "./tiles/eWall_nw-swCorner.svg" << '\n'
  << E_E       << ',' <<              "E Wall E Door" << ',' <<             "./tiles/eWall_eDoor.svg" << '\n'
  << E_NW_E    << ',' <<    "E Wall NW Corner E Door" << ',' <<    "./tiles/eWall_nwCorner_eDoor.svg" << '\n'
  << E_SW_E    << ',' <<    "E Wall SW Corner E Door" << ',' <<    "./tiles/eWall_swCorner_eDoor.svg" << '\n'
  << E_NW_SW_E << ',' << "E Wall NW-SW Corner E Door" << ',' << "./tiles/eWall_nw-swCorner_eDoor.svg" << '\n'
  
  //North wall variants
  << N         << ',' <<                     "N Wall" << ',' <<                   "./tiles/nWall.svg" << '\n'
  << N_SW      << ',' <<           "N Wall SE Corner" << ',' <<          "./tiles/nWall_seCorner.svg" << '\n'
  << N_SE      << ',' <<           "N Wall SW Corner" << ',' <<          "./tiles/nWall_swCorner.svg" << '\n'
  << N_SW_SE   << ',' <<        "N Wall SE-SW Corner" << ',' <<       "./tiles/nWall_se-swCorner.svg" << '\n'
  << N_N       << ',' <<              "N Wall N Door" << ',' <<             "./tiles/nWall_nDoor.svg" << '\n'
  << N_SW_N    << ',' <<    "N Wall SW Corner N Door" << ',' <<    "./tiles/nWall_swCorner_nDoor.svg" << '\n'
  << N_SE_N    << ',' <<    "N Wall SE Corner N Door" << ',' <<    "./tiles/nWall_seCorner_nDoor.svg" << '\n'
  << N_SW_SE_N << ',' << "N Wall SE-SW Corner N Door" << ',' << "./tiles/nWall_se-swCorner_nDoor.svg" << '\n'
  
  //West wall variants
  << W         << ',' <<                     "W Wall" << ',' <<                   "./tiles/wWall.svg" << '\n'
  << W_NE      << ',' <<           "W Wall NE Corner" << ',' <<          "./tiles/wWall_neCorner.svg" << '\n'
  << W_SE      << ',' <<           "W Wall SE Corner" << ',' <<          "./tiles/wWall_seCorner.svg" << '\n'
  << W_NE_SE   << ',' <<        "W Wall NE-SE Corner" << ',' <<       "./tiles/wWall_ne-seCorner.svg" << '\n'
  << W_W       << ',' <<              "W Wall W Door" << ',' <<             "./tiles/wWall_wDoor.svg" << '\n'
  << W_NE_W    << ',' <<    "W Wall NE Corner W Door" << ',' <<    "./tiles/wWall_neCorner_wDoor.svg" << '\n'
  << W_SE_W    << ',' <<    "W Wall SE Corner W Door" << ',' <<    "./tiles/wWall_seCorner_wDoor.svg" << '\n'
  << W_NE_SE_W << ',' << "W Wall NE-SE Corner W Door" << ',' << "./tiles/wWall_ne-seCorner_wDoor.svg" << '\n'

  //South wall varaints
  << S         << ',' <<                     "S Wall" << ',' <<                   "./tiles/sWall.svg" << '\n'
  << S_NE      << ',' <<           "S Wall NE Corner" << ',' <<          "./tiles/sWall_neCorner.svg" << '\n'
  << S_NW      << ',' <<           "S Wall NW Corner" << ',' <<          "./tiles/sWall_nwCorner.svg" << '\n'
  << S_NE_NW   << ',' <<        "S Wall NE-NW Corner" << ',' <<       "./tiles/sWall_ne-nwCorner.svg" << '\n'
  << S_S       << ',' <<              "S Wall S Door" << ',' <<             "./tiles/sWall_sDoor.svg" << '\n'
  << S_NE_S    << ',' <<    "S Wall NE Corner S Door" << ',' <<    "./tiles/sWall_neCorner_sDoor.svg" << '\n'
  << S_NW_S    << ',' <<    "S Wall NW Corner S Door" << ',' <<    "./tiles/sWall_nwCorner_sDoor.svg" << '\n'
  << S_NE_NW_S << ',' << "S Wall NE-NW Corner S Door" << ',' << "./tiles/sWall_ne-nwCorner_sDoor.svg" << '\n'


  //North-East wall variants
  << NE       << ',' <<                   "NE Wall" << ',' <<                 "./tiles/neWall.svg" << '\n'
  << NE_E     << ',' <<            "NE Wall E Door" << ',' <<           "./tiles/neWall_eDoor.svg" << '\n'
  << NE_N     << ',' <<            "NE Wall N Door" << ',' <<           "./tiles/neWall_nDoor.svg" << '\n'
  << NE_NE    << ',' <<           "NE Wall NE Door" << ',' <<          "./tiles/neWall_neDoor.svg" << '\n'
  << NE_SW    << ',' <<         "NE Wall SW Corner" << ',' <<        "./tiles/neWall_swCorner.svg" << '\n'
  << NE_SW_E  << ',' <<  "NE Wall SW Corner E Door" << ',' <<  "./tiles/neWall_swCorner_eDoor.svg" << '\n'
  << NE_SW_N  << ',' <<  "NE Wall SW Corner N Door" << ',' <<  "./tiles/neWall_swCorner_nDoor.svg" << '\n'
  << NE_SW_NE << ',' << "NE Wall SW Corner NE Door" << ',' << "./tiles/neWall_swCorner_neDoor.svg" << '\n'

  //South-East wall variants
  << SE       << ',' <<                   "SE Wall" << ',' <<                 "./tiles/seWall.svg" << '\n'
  << SE_E     << ',' <<            "SE Wall E Door" << ',' <<           "./tiles/seWall_eDoor.svg" << '\n'
  << SE_S     << ',' <<            "SE Wall S Door" << ',' <<           "./tiles/seWall_sDoor.svg" << '\n'
  << SE_SE    << ',' <<           "SE Wall SE Door" << ',' <<          "./tiles/seWall_seDoor.svg" << '\n'
  << SE_NW    << ',' <<         "SE Wall NW Corner" << ',' <<        "./tiles/seWall_nwCorner.svg" << '\n'
  << SE_NW_E  << ',' <<  "SE Wall NW Corner E Door" << ',' <<  "./tiles/seWall_nwCorner_eDoor.svg" << '\n'
  << SE_NW_S  << ',' <<  "SE Wall NW Corner S Door" << ',' <<  "./tiles/seWall_nwCorner_sDoor.svg" << '\n'
  << SE_NW_SE << ',' << "SE Wall NW Corner SE Door" << ',' << "./tiles/seWall_nwCorner_seDoor.svg" << '\n'

  //North-West wall variants
  << NW       << ',' <<                   "NW Wall" << ',' <<                 "./tiles/nwWall.svg" << '\n'
  << NW_N     << ',' <<            "NW Wall N Door" << ',' <<           "./tiles/nwWall_nDoor.svg" << '\n'
  << NW_W     << ',' <<            "NW Wall W Door" << ',' <<           "./tiles/nwWall_wDoor.svg" << '\n'
  << NW_NW    << ',' <<           "NW Wall NW Door" << ',' <<          "./tiles/nwWall_nwDoor.svg" << '\n'
  << NW_SE    << ',' <<         "NW Wall SE Corner" << ',' <<        "./tiles/nwWall_seCorner.svg" << '\n'
  << NW_SE_N  << ',' <<  "NW Wall SE Corner N Door" << ',' <<  "./tiles/nwWall_seCorner_nDoor.svg" << '\n'
  << NW_SE_W  << ',' <<  "NW Wall SE Corner W Door" << ',' <<  "./tiles/nwWall_seCorner_wDoor.svg" << '\n'
  << NW_SE_NW << ',' << "NW Wall SE Corner NW Door" << ',' << "./tiles/nwWall_seCorner_nwDoor.svg" << '\n'

  //South-West wall variants
  << SW       << ',' <<                   "SW Wall" << ',' <<                 "./tiles/swWall.svg" << '\n'
  << SW_W     << ',' <<            "SW Wall W Wall" << ',' <<           "./tiles/swWall_wDoor.svg" << '\n'
  << SW_S     << ',' <<            "SW Wall S Wall" << ',' <<           "./tiles/swWall_sDoor.svg" << '\n'
  << SW_SW    << ',' <<           "SW Wall SW Wall" << ',' <<          "./tiles/swWall_swDoor.svg" << '\n'
  << SW_NE    << ',' <<         "SW Wall NE Corner" << ',' <<        "./tiles/swWall_neCorner.svg" << '\n'
  << SW_NE_W  << ',' <<  "SW Wall NE Corner W Door" << ',' <<  "./tiles/swWall_neCorner_wDoor.svg" << '\n'
  << SW_NE_S  << ',' <<  "SW Wall NE Corner S Door" << ',' <<  "./tiles/swWall_neCorner_sDoor.svg" << '\n'
  << SW_NE_SW << ',' << "SW Wall NE Corner SW Door" << ',' << "./tiles/swWall_neCorner_swDoor.svg" << '\n'

  //West-East wall variants
  << EW    << ',' <<         "EW Wall" << ',' <<        "./tiles/ewWall.svg" << '\n'
  << EW_E  << ',' <<  "EW Wall E Door" << ',' <<  "./tiles/ewWall_eDoor.svg" << '\n'
  << EW_W  << ',' <<  "EW Wall W Door" << ',' <<  "./tiles/ewWall_wDoor.svg" << '\n'
  << EW_EW << ',' << "EW Wall EW Door" << ',' << "./tiles/ewWall_ewDoor.svg" << '\n'

  //North-South wall variants
  << NS    << ',' <<         "NS Wall" << ',' <<        "./tiles/nsWall.svg" << '\n'
  << NS_N  << ',' <<  "NS Wall N Door" << ',' <<  "./tiles/nsWall_nDoor.svg" << '\n'
  << NS_S  << ',' <<  "NS Wall S Door" << ',' <<  "./tiles/nsWall_sDoor.svg" << '\n'
  << NS_NS << ',' << "NS Wall NS Door" << ',' << "./tiles/nsWall_nsDoor.svg" << '\n'

  //North-East-West wall variants
  << NEW     << ',' <<          "NEW Wall" << ',' <<         "./tiles/newWall.svg" << '\n'
  << NEW_E   << ',' <<   "NEW Wall E Door" << ',' <<   "./tiles/newWall_eDoor.svg" << '\n'
  << NEW_N   << ',' <<   "NEW Wall N Door" << ',' <<   "./tiles/newWall_nDoor.svg" << '\n'
  << NEW_W   << ',' <<   "NEW Wall W Door" << ',' <<   "./tiles/newWall_wDoor.svg" << '\n'
  << NEW_NE  << ',' <<  "NEW Wall NE Door" << ',' <<  "./tiles/newWall_neDoor.svg" << '\n'
  << NEW_EW  << ',' <<  "NEW Wall EW Door" << ',' <<  "./tiles/newWall_ewDoor.svg" << '\n'
  << NEW_NW  << ',' <<  "NEW Wall NW Door" << ',' <<  "./tiles/newWall_nwDoor.svg" << '\n'
  << NEW_NEW << ',' << "NEW Wall NEW Door" << ',' << "./tiles/newWall_newDoor.svg" << '\n'

  //North-South-East wall variants
  << NSE     << ',' <<          "NSE Wall" << ',' <<         "./tiles/nseWall.svg" << '\n'
  << NSE_E   << ',' <<   "NSE Wall E Door" << ',' <<   "./tiles/nseWall_eDoor.svg" << '\n'
  << NSE_N   << ',' <<   "NSE Wall N Door" << ',' <<   "./tiles/nseWall_nDoor.svg" << '\n'
  << NSE_S   << ',' <<   "NSE Wall S Door" << ',' <<   "./tiles/nseWall_sDoor.svg" << '\n'
  << NSE_NE  << ',' <<  "NSE Wall NE Door" << ',' <<  "./tiles/nseWall_neDoor.svg" << '\n'
  << NSE_SE  << ',' <<  "NSE Wall SE Door" << ',' <<  "./tiles/nseWall_seDoor.svg" << '\n'
  << NSE_NS  << ',' <<  "NSE Wall NS Door" << ',' <<  "./tiles/nseWall_nsDoor.svg" << '\n'
  << NSE_NSE << ',' << "NSE Wall NSE Door" << ',' << "./tiles/nseWall_nseDoor.svg" << '\n'

  //South-East-West wall variants
  << SEW     << ',' <<          "SEW Wall" << ',' <<         "./tiles/sewWall.svg" << '\n'
  << SEW_E   << ',' <<   "SEW Wall E Door" << ',' <<   "./tiles/sewWall_eDoor.svg" << '\n'
  << SEW_W   << ',' <<   "SEW Wall W Door" << ',' <<   "./tiles/sewWall_wDoor.svg" << '\n'
  << SEW_S   << ',' <<   "SEW Wall S Door" << ',' <<   "./tiles/sewWall_sDoor.svg" << '\n'
  << SEW_EW  << ',' <<  "SEW Wall EW Door" << ',' <<  "./tiles/sewWall_ewDoor.svg" << '\n'
  << SEW_SE  << ',' <<  "SEW Wall SE Door" << ',' <<  "./tiles/sewWall_seDoor.svg" << '\n'
  << SEW_SW  << ',' <<  "SEW Wall SW Door" << ',' <<  "./tiles/sewWall_swDoor.svg" << '\n'
  << SEW_SEW << ',' << "SEW Wall SEW Door" << ',' << "./tiles/sewWall_sewDoor.svg" << '\n'

  //North-South-West wall variants
  << NSW     << ',' <<          "NSW Wall" << ',' <<         "./tiles/nswWall.svg" << '\n'
  << NSW_N   << ',' <<   "NSW Wall N Door" << ',' <<   "./tiles/nswWall_nDoor.svg" << '\n'
  << NSW_W   << ',' <<   "NSW Wall W Door" << ',' <<   "./tiles/nswWall_wDoor.svg" << '\n'
  << NSW_S   << ',' <<   "NSW Wall S Door" << ',' <<   "./tiles/nswWall_sDoor.svg" << '\n'
  << NSW_NW  << ',' <<  "NSW Wall NW Door" << ',' <<  "./tiles/nswWall_nwDoor.svg" << '\n'
  << NSW_NS  << ',' <<  "NSW Wall NS Door" << ',' <<  "./tiles/nswWall_nsDoor.svg" << '\n'
  << NSW_SW  << ',' <<  "NSW Wall SW Door" << ',' <<  "./tiles/nswWall_swDoor.svg" << '\n'
  << NSW_NSW << ',' << "NSW Wall NSW Door" << ',' << "./tiles/nswWall_nswDoor.svg" << '\n'

  //North-South-East-West wall variants
  << NSEW      << ',' <<           "NSEW Wall" << ',' <<          "./tiles/nsewWall.svg" << '\n'
  << NSEW_E    << ',' <<    "NSEW Wall E Door" << ',' <<    "./tiles/nsewWall_eDoor.svg" << '\n'
  << NSEW_N    << ',' <<    "NSEW Wall N Door" << ',' <<    "./tiles/nsewWall_nDoor.svg" << '\n'
  << NSEW_W    << ',' <<    "NSEW Wall W Door" << ',' <<    "./tiles/nsewWall_wDoor.svg" << '\n'
  << NSEW_S    << ',' <<    "NSEW Wall S Door" << ',' <<    "./tiles/nsewWall_sDoor.svg" << '\n'
  << NSEW_NE   << ',' <<   "NSEW Wall NE Door" << ',' <<   "./tiles/nsewWall_neDoor.svg" << '\n'
  << NSEW_EW   << ',' <<   "NSEW Wall EW Door" << ',' <<   "./tiles/nsewWall_ewDoor.svg" << '\n'
  << NSEW_SE   << ',' <<   "NSEW Wall SE Door" << ',' <<   "./tiles/nsewWall_seDoor.svg" << '\n'
  << NSEW_NW   << ',' <<   "NSEW Wall NW Door" << ',' <<   "./tiles/nsewWall_nwDoor.svg" << '\n'
  << NSEW_NS   << ',' <<   "NSEW Wall NS Door" << ',' <<   "./tiles/nsewWall_nsDoor.svg" << '\n'
  << NSEW_SW   << ',' <<   "NSEW Wall SW Door" << ',' <<   "./tiles/nsewWall_swDoor.svg" << '\n'
  << NSEW_NEW  << ',' <<  "NSEW Wall NEW Door" << ',' <<  "./tiles/nsewWall_newDoor.svg" << '\n'
  << NSEW_NSE  << ',' <<  "NSEW Wall NSE Door" << ',' <<  "./tiles/nsewWall_nseDoor.svg" << '\n'
  << NSEW_NSW  << ',' <<  "NSEW Wall NSW Door" << ',' <<  "./tiles/nsewWall_nswDoor.svg" << '\n'
  << NSEW_SEW  << ',' <<  "NSEW Wall SEW Door" << ',' <<  "./tiles/nsewWall_sewDoor.svg" << '\n'
  << NSEW_NSEW << ',' << "NSEW Wall NSEW Door" << ',' << "./tiles/nsewWall_nsewDoor.svg" << '\n'

  //No wall variants
  << O             << ',' <<                    "Open" << ',' <<                   "./tiles/open.svg" << '\n'
  << O_NE          << ',' <<          "Open NE Corner" << ',' <<          "./tiles/open_neCorner.svg" << '\n'
  << O_NW          << ',' <<          "Open NW Corner" << ',' <<          "./tiles/open_nwCorner.svg" << '\n'
  << O_NE_NW       << ',' <<       "Open NE-NW Corner" << ',' <<       "./tiles/open_ne-nwCorner.svg" << '\n'
  << O_SW          << ',' <<          "Open SW Corner" << ',' <<          "./tiles/open_swCorner.svg" << '\n'
  << O_NE_SW       << ',' <<       "Open NE-SW Corner" << ',' <<       "./tiles/open_ne-swCorner.svg" << '\n'
  << O_NW_SW       << ',' <<       "Open NW-SW Corner" << ',' <<       "./tiles/open_nw-swCorner.svg" << '\n'
  << O_NE_NW_SW    << ',' <<    "Open NE-NW-SW Corner" << ',' <<    "./tiles/open_ne-nw-swCorner.svg" << '\n'
  << O_SE          << ',' <<          "Open SE Corner" << ',' <<          "./tiles/open_seCorner.svg" << '\n'
  << O_NE_SE       << ',' <<       "Open NE-SE Corner" << ',' <<       "./tiles/open_ne-seCorner.svg" << '\n'
  << O_NW_SE       << ',' <<       "Open NW-SE Corner" << ',' <<       "./tiles/open_nw-seCorner.svg" << '\n'
  << O_NE_NW_SE    << ',' <<    "Open NE-NW-SE Corner" << ',' <<    "./tiles/open_ne-nw-seCorner.svg" << '\n'
  << O_SW_SE       << ',' <<       "Open SW-SE Corner" << ',' <<       "./tiles/open_sw-seCorner.svg" << '\n'
  << O_NE_SW_SE    << ',' <<    "Open NE-SW-SE Corner" << ',' <<    "./tiles/open_ne-sw-seCorner.svg" << '\n'
  << O_NW_SW_SE    << ',' <<    "Open NW-SW-SE Corner" << ',' <<    "./tiles/open_nw-sw-seCorner.svg" << '\n'
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