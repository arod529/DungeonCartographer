#include "map.h"

#include <fstream>
#include <limits>

#include <gtkmm/menuitem.h>
#include <gtkmm/notebook.h>
#include <gtkmm/toolbutton.h>

/*!
  Initializes a map with defaults from Settings. Creates a starting Level.

  @param[in] settings The program settings
**/
Map::Map(Settings* settings, UI* ui)
: size{settings->mapSize}
, tileset{&settings->tileset}
, ui{ui}
{
  signalInit();
  addLevel(NULL, 0);
  addLevel(NULL, 1);
  addLevel(NULL, 2);
  addLevel(NULL, 3);
}

void Map::signalInit()
{
  Gtk::ToolButton* btn;
  ui->getWidget<Gtk::ToolButton>("saveBtn", btn);
  btn->signal_clicked().connect(sigc::mem_fun(*this, &Map::save));
  ui->getWidget<Gtk::ToolButton>("saveAsBtn", btn);
  btn->signal_clicked().connect(sigc::mem_fun(*this, &Map::saveAs));

  Gtk::MenuItem* menu;
  ui->getWidget<Gtk::MenuItem>("saveMenu", menu);
  menu->signal_activate().connect(sigc::mem_fun(*this, &Map::save));
  ui->getWidget<Gtk::MenuItem>("saveAsMenu", menu);
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
    file << level[i];
    for(int j = 0; j < level[i].tile.size(); j++)
    {
      file << level[i].tile[j];
    }
  }

  file.close();
  return true;
}

bool Map::openFile(std::string fPath)
{
  std::ifstream file(fPath);

  //check file is open
  if(!file.is_open())
  {
    fprintf(stderr, "ERROR: Dungeon Cartographer @ Map::openFile: The file could not be opened: %s\n", fPath.c_str());
    return false;
  }

  //read map
  file >> *this;

  //read level (there must be at least one)
  do
  {
    level.emplace_back(ui, tileset);
    file >> level.back();
  } while(!file.eof());

  return true;
}

//--------------------------
//----- Event Handlers -----
//--------------------------
void Map::save()
{
  if(filepath == "") //there is no associated file
    saveAs();
  else //save the file
    saveToFile(filepath);
}

void Map::saveAs()
{
  //request save as dialogue
  std::string fpath{ui->saveAs()};

  if(fpath != "")
  {
    filepath = fpath;
    saveToFile(filepath);
  }
}

void Map::addLevel(Gtk::Widget* page, uint pageNum)
{
  if(pageNum == level.size()) //the page is the new tab page
  {
    level.emplace_back(ui, tileset, level.size(), size);
    ui->addLevel(&level.back());
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
