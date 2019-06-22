#include "map.h"

//------------------------
//----- INITIALIZERS -----
//------------------------
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
}

Level::Level(Tileset* tileset, int id, int size)
: tileset{tileset}, id{id}, size{size}
{
  //set grid properties
  set_row_homogeneous(true); set_column_homogeneous(true);
  set_row_spacing(0); set_column_spacing(0);
  get_style_context()->add_class("mainWindow");

  //Fill level with background tiles
  for(int i = 0; i < size*size; i++)
  {
    tile.emplace_back(tileset, BACKGROUND, i, size);
    attach(tile[i], i%size, i/size, 1, 1);
  }
};

Tile::Tile(const Tileset* tileset, uint tilesetTileId, int gridId, int gridSize)
: tileset{tileset}, tilesetTileId{tilesetTileId}, gridId{gridId}, gridSize{gridSize}
{
  //set drawing area properties
  set_can_focus(true);
  set_size_request(25,25);

  //mouse click mask
  add_events(Gdk::EventMask::BUTTON_PRESS_MASK);

  //make them visible
  show();
};

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
    level.emplace_back(tileset, pageNum, size);  

    //make a builder for the tab
    auto tabBuilder = Gtk::Builder::create_from_file(ui->uiTab);

    //get the notebook
    int currTab = ui->getCurrTab();

    //create layout
    ui->layout.emplace_back();
    tabBuilder->get_widget("layout", ui->layout.back());  

    //add level to layout
    ui->layout.back()->add(level[pageNum]);

    //create tab
    Gtk::Box* tab;
    tabBuilder->get_widget("tab", tab);
    //set default tab label
    Gtk::Label* tabLabel;
    tabBuilder->get_widget("tabLabel", tabLabel);
    tabLabel->set_label("Level " + std::to_string(currTab+1));

    //add layout and tab to notebook; insert inplace of new tab
    ui->notebook->insert_page(*ui->layout.back(), *tab, currTab);
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

std::ostream& operator<<(std::ostream& out, const Level& level)
{
  out << "Level["
       << level.id << ','
       << level.size << ','
       << level.tileset->filepath << ']' << '\n';
  out.flush();
  return out;
}

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

/*!
  Writes a Tile to a file stream.
**/
std::ostream& operator<<(std::ostream& out, const Tile& tile)
{
  if(tile.gridId%tile.gridSize != 0)
  {
    out.seekp(-1, std::ios::cur);
    out << '|';
  }

  out << tile.tilesetTileId << '\n';
  out.flush();
  return out;
}

/*!
  Reads a Tile from a file stream.

  The Tile must have a valid tileLvl, and therefore cannot be initialized with this.
**/
std::istream& operator>>(std::istream& in, Tile& tile)
{
  in >> tile.tilesetTileId;
  if(in.peek() == '|') in.get(); //scrap separator

  //leave stream in good state by discarding empty lines
  while(in.peek() == '\n')
    {in.get();}

  return in;
}