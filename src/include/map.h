#ifndef MAP_H
#define MAP_H

#include "constants.h" //uint
#include "settings.h"  //settings
#include "tileset.h"   //tileset
#include "ui.h"        //ui
#include "level.h"     //level

#include <fstream> //operator<<|operator>>
#include <memory>  //level
#include <string>  //saveToFile()|openFile()|filepath
#include <vector>  //level

#include <gtkmm/widget.h> //addLevel()

/*!
  Map data structure. Handles map file manipulation, and Level interface with the UI.
**/
class Map
{
  public:
    Map(Settings* settings, UI* ui);
    ~Map();

    //overloads
    friend std::ostream& operator<<(std::ostream& out, const Map& map);
    friend std::istream& operator>>(std::istream& in, Map& map);

  private:
    UI* ui;                                    // UI for managing interface
    Settings* settings;                        // Settings for managing defaults
    int size;                                  // default level size
    Tileset* tileset;                          // The tile set that the map uses
    std::string filepath = "";                 // The file path associated with this map
    std::vector<std::unique_ptr<Level>> level; // The levels in the map

    //utility
    void addLevel(uint pageNum);
    void clearMap();
    bool loadFile(std::string filepath);
    bool saveToFile(std::string filepath);
    void sigInit();

    //event handlers
    void createNewLevel(Gtk::Widget* page = NULL, uint pageNum = 0);
    void newMap();
    void open();
    void save();
    void saveAs();
    bool scroll(GdkEventScroll* scroll_event);
    void zoom(int scrollDir);
};

#endif
