#include "map.h"

#include <limits> //operator>>

//signalInit()
#include <gtkmm/menuitem.h>
#include <gtkmm/notebook.h> 
#include <gtkmm/toolbutton.h>

//addLevel()
#include <gtkmm/builder.h>
#include <gtkmm/scrolledwindow.h>

//scroll()
#include <gtkmm/adjustment.h>

#include <iostream>

/*!
  Initializes a map with defaults from Settings. Creates a starting Level.

  @param[in] settings The program settings
  @param[in] ui The ui
**/
Map::Map(Settings* settings, UI* ui)
: size{settings->mapSize}
, tileset{&settings->tileset}
, settings{settings}
, ui{ui}
{
  sigInit();
  createNewLevel();
}

Map::~Map()
{
  clearMap(); //prevents some a double free err on exit; find cause later?
}

//-------------------
//----- Utility -----
//-------------------

/*!
  Adds a Level to the UI. It must exist.
**/
void Map::addLevel(uint pageNum)
{ 
  ui->addTab(pageNum, level[pageNum].get());

  //set up scroll zoom event for tab
  auto scrolledWindow = (Gtk::ScrolledWindow*)ui->notebook->get_nth_page(pageNum);
  scrolledWindow->signal_scroll_event().connect(sigc::mem_fun(*this, &Map::scroll), false);
}

/*!
  Clears the ui of the map. Deletes the Levels.
**/
void Map::clearMap()
{
  //clear levels from ui
  ui->clearTabs();

  //clear existing levels
  level.clear();
}

bool Map::loadFile(std::string fPath)
{
  std::ifstream file(fPath);

  //check file is open
  if(!file.is_open())
  {
    fprintf(stderr, "ERROR: Dungeon Cartographer @ Map::openFile: The file could not be opened: %s\n", fPath.c_str());
    return false;
  }

  clearMap();

  //read map
  file >> *this;

  //read level (there must be at least one)
  do
  {
    level.emplace_back(std::make_unique<Level>(tileset, file));
    addLevel(level.back()->id);
  }
  while(!file.eof());

  file.close();

  return true;
}

/*!
  Writes a map to file.

  @param[in] _filename The path to the file.
**/
bool Map::saveToFile(std::string filepath)
{
  std::ofstream file(filepath);

  //write map to file
  file << *this;

  //append levels to file
  for(int i = 0; i < level.size(); i++)
  {
    file << *level[i];
    //append tiles to file
    for(int j = 0; j < level[i]->tile.size(); j++)
    {
      file << *level[i]->tile[j];
    }
  }

  file.close();
  return true;
}

/*!
  Initializes signal events managed by Map
**/
void Map::sigInit()
{
  Gtk::ToolButton* btn;
  ui->getWidget<Gtk::ToolButton>("btn_new", btn);
  btn->signal_clicked().connect(sigc::mem_fun(*this, &Map::newMap));

  ui->getWidget<Gtk::ToolButton>("btn_open", btn);
  btn->signal_clicked().connect(sigc::mem_fun(*this, &Map::open));

  ui->getWidget<Gtk::ToolButton>("btn_save", btn);
  btn->signal_clicked().connect(sigc::mem_fun(*this, &Map::save));
  ui->getWidget<Gtk::ToolButton>("btn_saveAs", btn);
  btn->signal_clicked().connect(sigc::mem_fun(*this, &Map::saveAs));

  ui->getWidget<Gtk::ToolButton>("btn_zoomIn", btn);
  btn->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &Map::zoom), -1));
  ui->getWidget<Gtk::ToolButton>("btn_zoomOut", btn);
  btn->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &Map::zoom), 1));
  ui->getWidget<Gtk::ToolButton>("btn_zoomFit", btn);
  btn->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &Map::zoom), 0));


  Gtk::MenuItem* menu;
  ui->getWidget<Gtk::MenuItem>("menu_new", menu);
  menu->signal_activate().connect(sigc::mem_fun(*this, &Map::newMap));

  ui->getWidget<Gtk::MenuItem>("menu_open", menu);
  menu->signal_activate().connect(sigc::mem_fun(*this, &Map::open));

  ui->getWidget<Gtk::MenuItem>("menu_save", menu);
  menu->signal_activate().connect(sigc::mem_fun(*this, &Map::save));
  ui->getWidget<Gtk::MenuItem>("menu_saveAs", menu);
  menu->signal_activate().connect(sigc::mem_fun(*this, &Map::saveAs));

  ui->notebook->signal_switch_page().connect(sigc::mem_fun(*this, &Map::createNewLevel));
}

//--------------------------
//----- Event Handlers -----
//--------------------------

/*!
  Creates a new Level from the Map defaults

  @param[in] page unused; gtk_signal requirement
  @param[in] pageNum The page to add the level to.
**/
void Map::createNewLevel(Gtk::Widget* page, uint pageNum)
{
  //triggered by tab change && (is only page || tab not newtab)
  if(page != NULL && (pageNum == 0 || ((Gtk::Label*)ui->notebook->get_tab_label(*page))->get_text() != "+"))
    {return;}
  else //manual call or new Level tab
  {
    //Make a level with defaults from map, add it to ui
    level.emplace_back(std::make_unique<Level>(tileset, level.size(), size));
    addLevel(level.back()->id);
  }
}

/*!
  Creates a new map from default global settings.
**/
void Map::newMap()
{
  clearMap();

  //reload defaults from settings
  size = settings->mapSize;
  tileset = &settings->tileset;
  filepath = "";

  createNewLevel();
}

/*!
  Saves to the file associated with the Map.
  If there is no file associated, displays save as dialog.
**/
void Map::save()
{
  if(filepath == "") //there is no associated file
    saveAs();
  else //save the file
    saveToFile(filepath);
}

/*!
  Displays a save as dialog, and subsequently saves the file if a name is chosen.
**/
void Map::saveAs()
{
  //request save as dialog
  std::string fpath{ui->saveAs()};

  if(fpath != "")
  {
    if(saveToFile(filepath));
      {filepath = fpath;}
  }
}

/*!
  Calls zooms or scrolls the Level on mouse wheel input based on modifer keys.

  @param[in] scroll_event The event information.

  @return The signal has been fully handled.
**/
bool Map::scroll(GdkEventScroll* scroll_event)
{
  //call zoom event if ctrl key is held
  if(scroll_event->state == Gdk::ModifierType::CONTROL_MASK)
  {
    zoom(scroll_event->delta_y);
  }
  else
  {
    if(scroll_event->state == Gdk::ModifierType::SHIFT_MASK)
      ui->scroll(scroll_event->delta_y, 0);
    else
      ui->scroll(0, scroll_event->delta_y);
  }
  return true; //don't pass on event
}

/*!
  Displays a open file dialog, and subsequently loads the file if a name is chosen.
**/
void Map::open()
{
  //request save as dialog
  std::string fPath{ui->openFile()};

  if(fPath != "")
  {
    if(loadFile(fPath))
      {filepath = fPath;}
  }
}

/*!
  Zooms the Level.
**/
void Map::zoom(int scrollDir)
{
  ui->zoom(scrollDir, size, level[ui->notebook->get_current_page()]->tile[0].get());
}

//---------------------
//----- Overloads -----
//---------------------
std::ostream& operator<<(std::ostream& out, const Map& map)
{
  out << "Map["
      << map.size << ','
      << map.tileset->filepath << ']' << '\n';
  out.flush();
  return out;
}

std::istream& operator>>(std::istream& in, Map& map)
{
  static const std::streamsize MAX = std::numeric_limits<std::streamsize>::max();

  in.ignore(MAX, '[');
  in >> map.size;
  in.get(); //discard comma

  std::string tilesetFile = "";
  std::getline(in, tilesetFile, ']');
  if(!map.tileset->isTileset(tilesetFile))
    {map.tileset = new Tileset(tilesetFile);}

  in.ignore(MAX, '\n'); //getline doesn't eat this newline

  //leave stream in good state by discarding empty lines
  while(in.peek() == '\n')
    {in.get();}

  return in;
}
