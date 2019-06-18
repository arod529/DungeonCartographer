#include "level.h"

#include "constants.h"

#include <fstream>
#include <limits>

/*!
  Initializes a new blank Level.

  @param[in] _size The size of the level.
  @param[in] _tileset The Tileset to use for the level
**/
Level::Level(int id, int size, std::string tilesetFile, std::unordered_map<uint, TilesetTile>* tileset)
: id(id), size(size), tilesetFile(tilesetFile), tileset(tileset)
{
  //set all tiles to background tile
  for(int i = 0; i < size*size; i++)
    tile.emplace_back(i, this, &(*tileset)[BACKGROUND]);

  //initialize drawing areas for map
  for(int i = 0; i < size*size; i++)
  {
    drawingArea.emplace_back(new Gtk::DrawingArea());
  }

  for(int i = 0; i < size*size; i++)
    drawingArea[i]->set_can_focus(true);
}

Level::~Level()
{
  for(int i = 0; i < drawingArea.size(); i++)
    delete drawingArea[i];
}

/*!
  Returns the size of the level.

  @return The size of the Level.
**/
int Level::getSize() const
  {return size;}

std::ostream& operator<<(std::ostream& out, const Level& level)
{
  out << "Level["
       << level.id << ','
       << level.size << ','
       << level.tilesetFile << ']' << '\n';
  out.flush();
  return out;
}

std::istream& operator>>(std::istream& in, Level& level)
{
  static const std::streamsize MAX = std::numeric_limits<std::streamsize>::max();

  in.ignore(MAX, '[');
  in >> level.id;
  in.get(); //discard comma
  in >> level.size;
  in.get(); //discard comma
  std::getline(in, level.tilesetFile, ']');
  in.ignore(MAX, '\n'); //getline doesn't eat this newline

  //leave stream in good state by discarding empty lines
  while(in.peek() == '\n')
    {in.get();}

  return in;
}
