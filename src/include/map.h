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
  Map data structure. Tileset, and grid size for new maps default to the the global
  values in Settings.

  \bug make level private
  \bug incomplete definition
  \bug add map file
  \bug tileset should be pointer? have global in seetings?
**/
class Map
{
  public:
    Map(Settings* settings, UI* ui);
    void sigInit();

    //utility
    bool saveToFile(std::string filepath);
    bool openFile(std::string filepath);

    //event handlers
    void save();
    void saveAs();
    void addLevel(Gtk::Widget* page, uint pageNum);

    //overloads
    friend std::ostream& operator<<(std::ostream& out, const Map& map);
    friend std::istream& operator>>(std::istream& in, Map& map);

  private:
    UI* ui;                                    // UI for managing interface
    int size;                                  // default level size
    Tileset* tileset;                          // The tile set that the map uses
    std::string filepath = "";                 // The file path associated with this map
    std::vector<std::unique_ptr<Level>> level; // The levels in the map
};

#endif
