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

  Level(Tileset* tileset, int id, int width, int height);
  Level(Tileset* tileset, std::ifstream& file);

  void center();
  // void insertRow();
  void reset();
  void shift(int x, int y);
  bool updateTile(GdkEventButton* btn, int gridId);

  //utility
  int getTileSize() const;
  void setTileSize(int levelIndex);


  //overloads
  friend std::ostream& operator<<(std::ostream& out, const Level& level);
  friend std::istream& operator>>(std::istream& in, Level& level);

private:
  int id;                                  // The Level's id
  int width, height;                       // The Level's size
  Tileset* tileset;                        // The Tileset that the Level uses
  std::vector<std::unique_ptr<Tile>> tile; // The Tiles in the Level
  
  void createNewTile();
  void getExtents(int* extents);
  void print(Cairo::RefPtr<Cairo::PdfSurface>& surface, Cairo::RefPtr<Cairo::Context>& cr);
  void propInit();
  void updateCornerBits(int gridId, bool propagate);
};

#endif
