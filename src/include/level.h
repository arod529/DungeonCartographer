#ifndef LEVEL_H
#define LEVEL_H

#include "tile.h"    //tile
#include "tileset.h" //tileset

#include <fstream> //operator<<|operator>>
#include <memory>  //tile
#include <vector>  //^

#include <gtkmm/grid.h> //inheritance

/*!
  Level data structure. Handles Tile updates.
**/
struct Level : public Gtk::Grid
{
  friend class Map; //Level is owned by Map

  Level(Tileset* tileset, int id, int size);
  Level(Tileset* tileset, std::ifstream& file);
  void sigInit();

  //utility
  void createNewTile();
  bool updateTile(GdkEventButton* btn, int gridId);
  void updateCornerBits(int gridId, bool propagate);

  //overloads
  friend std::ostream& operator<<(std::ostream& out, const Level& level);
  friend std::istream& operator>>(std::istream& in, Level& level);

private:
  int id;                                  // The Level's id
  int size;                                // The Level's size
  Tileset* tileset;                        // The Tileset that the Level uses
  std::vector<std::unique_ptr<Tile>> tile; // The Tiles in the Level
};

#endif
