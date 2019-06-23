#include "map.h"

#include <limits> //operator>>

//signalInit()
#include <gtkmm/menuitem.h>
#include <gtkmm/notebook.h> 
#include <gtkmm/toolbutton.h>

//addLevel()
#include <gtkmm/builder.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/widget.h>
#include <gtkmm/viewport.h>

/*!
  Initializes a map with defaults from Settings. Creates a starting Level.

  @param[in] settings The program settings
  @param[in] ui The ui
**/
Map::Map(Settings* settings, UI* ui)
: size{settings->mapSize}
, tileset{&settings->tileset}
, ui{ui}
{
  sigInit();
  addLevel(NULL, 0);
}

/*!
  Initilizes signal events managed by Map
**/
void Map::sigInit()
{
  Gtk::ToolButton* btn;
  ui->getWidget<Gtk::ToolButton>("btn_save", btn);
  btn->signal_clicked().connect(sigc::mem_fun(*this, &Map::save));
  ui->getWidget<Gtk::ToolButton>("btn_saveAs", btn);
  btn->signal_clicked().connect(sigc::mem_fun(*this, &Map::saveAs));

  Gtk::MenuItem* menu;
  ui->getWidget<Gtk::MenuItem>("menu_save", menu);
  menu->signal_activate().connect(sigc::mem_fun(*this, &Map::save));
  ui->getWidget<Gtk::MenuItem>("menu_saveAs", menu);
  menu->signal_activate().connect(sigc::mem_fun(*this, &Map::saveAs));

  Gtk::Notebook* notebook;
  ui->getWidget<Gtk::Notebook>("notebook", notebook);
  notebook->signal_switch_page().connect(sigc::mem_fun(*this, &Map::addLevel));
}

//-------------------
//----- Utility -----
//-------------------

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

//--------------------------
//----- Event Handlers -----
//--------------------------

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
    filepath = fpath;
    saveToFile(filepath);
  }
}

/*!
  Creates a new Level and adds it to the UI.

  @param[in] page unused; gtk_signal requirement
  @param[in] pageNum The page to add the level to.
**/
void Map::addLevel(Gtk::Widget* page, uint pageNum)
{
  if(pageNum == level.size()) //the page is the new tab page
  {
    level.emplace_back(std::make_unique<Level>(tileset, pageNum, size));

    //make a builder for the tab
    auto tabBuilder = Gtk::Builder::create_from_file(ui->uiTab);

    //get scrolled window and viewport
    Gtk::ScrolledWindow* scrolledWindow;
    tabBuilder->get_widget("scrolledWindow", scrolledWindow);
    Gtk::Viewport* viewport;
    tabBuilder->get_widget("viewport", viewport);

    //add level viewport
    viewport->add(*level[pageNum]);

    //get the current tab
    int currTab = ui->getCurrTab();

    //create tab
    Gtk::Label* tabLabel;
    tabBuilder->get_widget("tabLabel", tabLabel);
    tabLabel->set_label("Level " + std::to_string(currTab+1));

    //add scrolled window and tab to notebook; insert inplace of new tab
    ui->notebook->insert_page(*scrolledWindow, *tabLabel, currTab);
    //show all
    ui->notebook->show_all_children();
    //set to active page
    ui->notebook->set_current_page(currTab);
  }
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
