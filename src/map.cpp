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

  //set default tileset
  loadTileset("./tiles/defaultSet");

  //create a level
  level.emplace_back(size, &tileset);
}

/*!
  Free memory of map used by the Tileset file paths, and pixbufs.

  \bug Tileset memory management should be done by the tileset
  \bug will need to free memory of optional Tileset
**/
Map::~Map()
{
  for(auto iter = tileset.begin(); iter != tileset.end(); iter++)
  {
    auto i = iter->first;
    //delete[] tileset[i].filePath;
    g_object_unref(tileset[i].pixbuf);
  }
}

/*!
  Load a tileset from a file

  @param[in] tileSetFile File path for the tile set to load.

  \bug doesn't check if file exists
  \bug adding to tileset uses copy initializer causing ~Tileset() to be called immediately
**/
void Map::loadTileset(std::string tileSetFile)
{
  //open file
  std::ifstream file;
  file.open(tileSetFile);

  //tileset properties
  uint tilesetId = 0;
  std::string tilesetName = "";
  std::string tilesetFilePath = "";
  GdkPixbuf* tilesetPixbuf = NULL;

  std::string tmp = "";
  GError *err = NULL;

  //read file
  do
  {
    //read id
    file >> tilesetId;

    //read name
    std::getline(file, tmp, ','); //eat comma
    std::getline(file, tilesetName, ',');

    //read file path
    std::getline(file, tilesetFilePath);

    //init pixbuf from file
    tilesetPixbuf = gdk_pixbuf_new_from_file(tilesetFilePath.c_str(), &err);
    if(err != NULL) printf("%s\n", err->message);

    //add to tile set
    tileset[tilesetId] = (Tileset){tilesetId, tilesetName, tilesetFilePath, tilesetPixbuf};
  } while(!file.eof());

  //close file
  file.close();
}
