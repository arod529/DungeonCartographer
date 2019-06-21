#include "level.h"

#include "constants.h"
#include "event.h"

#include <fstream>
#include <limits>

#include <gtkmm/layout.h>
#include <gtkmm/toolbutton.h>

/*!
  Initializes a new blank Level.
  
  @param[in] tset The tile set.
  @param[in] id The level's id.
  @param[in] size The size of the level.
  @param[in] ui The ui; 
**/
Level::Level(UI* ui, Tileset* tset, int id, int size)
: tileset{tset}, id{id}, size{size}
{
  metaInit(ui);

  //populate with tiles
  for(int i = 0; i < size*size; i++)
  {
    //set all tiles to background tile
    tile.emplace_back(i, this, &tileset->tile[BACKGROUND]);
    //attach to grid
    attach(tile[i], i%size, i/size, 1, 1);
  }
}

/*!
  Initilizes grid properties and connects the signal handlers.
**/
void Level::metaInit(UI* ui)
{
  //set grid properties
  set_row_homogeneous(true); set_column_homogeneous(true);
  set_row_spacing(0); set_column_spacing(0);
  
  //connect events
  Gtk::ToolButton* btn;
  ui->getWidget<Gtk::ToolButton>("zoom[0]", btn);
  btn->signal_clicked().connect(sigc::bind<int, int, int>(sigc::mem_fun(*this, &Level::zoom), ui->getCurrTab(), SCROLL_UP, ui->getZoomSpeed()));btn->signal_clicked().connect(sigc::bind<int, int, int>(sigc::mem_fun(*this, &Level::zoom), ui->getCurrTab(), SCROLL_UP, ui->getZoomSpeed()));
  ui->getWidget<Gtk::ToolButton>("zoom[1]", btn);
  btn->signal_clicked().connect(sigc::bind<int, int, int>(sigc::mem_fun(*this, &Level::zoom), ui->getCurrTab(), SCROLL_UP, ui->getZoomSpeed()));btn->signal_clicked().connect(sigc::bind<int, int, int>(sigc::mem_fun(*this, &Level::zoom), ui->getCurrTab(), SCROLL_DOWN, ui->getZoomSpeed()));
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
       << level.tileset->filepath << ']' << '\n';
  out.flush();
  return out;
}

//--------------------------
//----- Event Handlers -----
//--------------------------
/*!
  Zooms the level in and out.

  @param[in] scrollDir The direction in which to zoom.
**/
void Level::zoom(int currTab, int scrollDir, int zoomSpeed)
{
  int dx{0}, dy{0}; //delta change in position

  //grid position
  int xG = get_allocation().get_x();
  int yG = get_allocation().get_y();

  //tile size
  int s = tile[0].get_allocated_width();

  //delta size change
  int ds = zoomSpeed;

  //change grid tile size
  if(scrollDir < 0) //scroll up
  {
    tile[0].set_size_request(s+ds, s+ds);
    dx = -ds;
    dy = -ds;
  }
  else if(scrollDir > 0 && ds < s) //scroll down
  {
    tile[0].set_size_request(s-ds, s-ds);
    dx = ds;
    dy = ds;
  }

  //-----Position Adjustment-----//
  // if(_settings->getZoomFollowsMouse())
  // {
  //   //get mouse coords in canvas
  //   double xMouse,yMouse;
  //   gdk_event_get_coords(event, &xMouse, &yMouse);

  //layout size
  int xL = get_parent()->get_allocated_width();
  int yL = get_parent()->get_allocated_height();

  //   //nudge grid center towards the mouse
  //   dx -= ((xG + allocG.width/2) - xMouse)/ds;
  //   dy += ((yG + allocG.height/2) - yMouse)/ds;
  // }

  //move the grid
  dx *= size/2;
  dy *= size/2;
  ((Gtk::Layout*)get_parent())->move(*this, xG+dx, yG+dy);
}

//---------------------
//----- Overloads -----
//---------------------
std::istream& operator>>(std::istream& in, Level& level)
{
  static const std::streamsize MAX = std::numeric_limits<std::streamsize>::max();

  in.ignore(MAX, '[');
  in >> level.id;
  in.get(); //discard comma
  in >> level.size;
  in.get(); //discard comma

  std::string tilesetFile = "";
  std::getline(in, tilesetFile, ']');
  if(!level.tileset->isTileset(tilesetFile))
    {level.tileset = new Tileset(tilesetFile);}

  in.ignore(MAX, '\n'); //getline doesn't eat this newline

  //leave stream in good state by discarding empty lines
  while(in.peek() == '\n')
    {in.get();}

  return in;
}
