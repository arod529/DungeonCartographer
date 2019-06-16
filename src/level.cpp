#include "level.h"

#include "constants.h"

/*!
  Initializes a new blank Level.

  @param[in] _size The size of the level.
  @param[in] _tileset The Tileset to use for the level
**/
Level::Level(int _size, std::unordered_map<uint, Tileset>* _tileset)
{
  //set map size and tileset
  size = _size;
  tileset = _tileset;

  //set all tiles to background tile
  for(int i = 0; i < size*size; i++)
    tile.emplace_back(i, this, &(*tileset)[BACKGROUND]);

  //initialize drawing areas for map
  for(int i = 0; i < size*size; i++)
    drawingArea.emplace_back(gtk_drawing_area_new());
  for(int i = 0; i < size*size; i++)
    gtk_widget_set_can_focus (drawingArea[i],true);
}

/*!
  Returns the size of the level.

  @return The size of the Level.
**/
int Level::getSize() const
{
  return size;
}
