#include "level.h"

#include "constants.h"

#include <fstream>
#include <limits>

/*!
  Initializes a new blank Level.

  @param[in] _size The size of the level.
  @param[in] _tileset The Tileset to use for the level
**/
Level::Level(int _size, std::unordered_map<uint, TilesetTile>* _tileset)
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

std::ostream& operator<<(std::ostream& _out, const Level& _level)
{
  _out << "Level["
       << _level.id << ','
       << _level.size << ','
       << _level.tilesetFile << ']' << '\n';
  _out.flush();
  return _out;
}

std::istream& operator>>(std::istream& _in, Level& _level)
{
  static const std::streamsize MAX = std::numeric_limits<std::streamsize>::max();

  _in.ignore(MAX, '[');
  _in >> _level.id;
  _in.get(); //discard comma
  _in >> _level.size;
  _in.get(); //discard comma
  std::getline(_in, _level.tilesetFile, ']');
  _in.ignore(MAX, '\n'); //getline doesn't eat this newline

  //leave stream in good state by discarding empty lines
  while(_in.peek() == '\n')
    {_in.get();}

  return _in;
}
