#ifndef MAP_H
#define MAP_H

#include "constants.h"
#include "settings.h"
#include "ui.h"
#include "tileset.h"

#include <fstream>
#include <limits>
#include <string>
#include <vector>

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/menuitem.h>
#include <gtkmm/notebook.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/widget.h>

struct Tile : public Gtk::DrawingArea
{
  friend class Map;

  Tile(const Tileset* tileset, uint tilesetTileId, int gridId, int gridSize);

  //overloads
  friend std::ostream& operator<<(std::ostream& out, const Tile& tile);
  friend std::istream& operator>>(std::istream& in, Tile& tile);

private:
  int gridId;             // The tile's grid locaton
  int gridSize;           // The size of the grid the tile is on
  uint tilesetTileId;     // The id of the tilesetTile(image) this tile displays
  const Tileset* tileset; // The Tileset this tile uses
};

struct Level : public Gtk::Grid
{
  friend class Map;

  Level(Tileset* tileset, int id, int size);

  //overloads
  friend std::ostream& operator<<(std::ostream& out, const Level& level);
  friend std::istream& operator>>(std::istream& in, Level& level);

private:
  int id;                 // The levels id
  int size;               // The levels size
  Tileset* tileset;       // The tileset that the tiles use
  std::vector<Tile> tile; // The tiles in the level
};

/*!
  Map data structure. Tileset, and grid size for new maps default to the the global
  values in Settings.

  \bug make level private
  \bug incomplete definition
  \bug add map file
  \bug tileset should be pointer? have global in seetings?
**/
class Map
{
  public:
    Map(Settings* settings, UI* ui);
    void signalInit();

    //utility
    bool saveToFile(std::string filepath);
    bool openFile(std::string filepath);

    //event handlers
    void save();
    void saveAs();
    void addLevel(Gtk::Widget* page, uint pageNum);

    //overloads
    friend std::ostream& operator<<(std::ostream& out, const Map& map);
    friend std::istream& operator>>(std::istream& in, Map& map);

  private:
    UI* ui;                    // UI for managing interface
    int size;                  // default level size
    Tileset* tileset;          // The tile set that the map uses
    std::string filepath = ""; // The file path associated with this map
    std::vector<Level> level;  // The levels in the map
};

#endif
