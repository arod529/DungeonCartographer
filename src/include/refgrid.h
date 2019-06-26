#ifndef REFGRID_H
#define REFGRID_H

#include <gtkmm/box.h>

class RefGrid : public Gtk::Box
{
public:
  RefGrid();
  // RefGrid(const RefGrid& rg);
  // ~RefGrid();

private:
  double lWidth{1}; //grid line width
  double rgb[3]{0, 0, 1}; //grid color
  int gridSize{0};
  int tileNum{25}; //number of tiles

  //signal overload
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};

#endif