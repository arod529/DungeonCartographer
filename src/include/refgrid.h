#ifndef REFGRID_H
#define REFGRID_H

#include <gtkmm/box.h>

class RefGrid : public Gtk::Box
{
public:
  RefGrid(int xTileNum, int yTileNum);

  bool isActive() const;
  void setActive(bool a);
  void setRGB(double r, double g, double b);
  void setSize(int xTileNum, int yTileNum);

private:
  bool active{false};
  double lWidth{1};       //grid line width
  double rgb[3]{0, 0, 1}; //grid color
  int width{25};          //number of grid spaces wide
  int height{25};         //number of grid spaces heigh

  //signal overload
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};

#endif