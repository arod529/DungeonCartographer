#include "refgrid.h"

RefGrid::RefGrid()
{
  set_hexpand(true);
  set_vexpand(true);
  set_sensitive(false);
  set_can_focus(false);
}

bool RefGrid::isActive() const
  {return active;}

void RefGrid::setActive(bool a)
{
  active = a;
  
  if(active)
    show();
  else
    hide();
}

bool RefGrid::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  //prevent show when inactive when parent container calls show all
  if(!active)
  {
    hide();
    return true;
  }

  int s = get_allocated_width();

  //set color
  cr->set_source_rgb(rgb[0], rgb[1], rgb[2]);
  //set line width
  cr->set_line_width(lWidth);

  double x{0.0}, y{0.0}; //origin
  double X{0.0}, Y{0.0}; //end
  double ds = s/tileNum; //width of a tile

  //draw lines
  for(int i = 1; i < tileNum; i++)
  {
    x = i*ds;
    X = x;
    y = 0;
    Y = s;
    
    //vertical
    cr->move_to(x, y);
    cr->line_to(X, Y);
    cr->stroke();

    //horizontal
    cr->move_to(y, x);
    cr->line_to(Y, X);
    cr->stroke();
  }

  return true;
}