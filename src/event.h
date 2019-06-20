#include "settings.h"
#include "level.h"

#include <glibmm/refptr.h>

#include <gtkmm/layout.h>
#include <gtkmm/grid.h>
#include <gtkmm/drawingarea.h>

/*!
  Structure to provide events with access to multiple and/or
  dynamic data, whilst only requiring one argument and
  a single signal connect. Each index contains data
  specific to the equivalent tab.
**/
struct EventData
{
  static int currTab;  //the currently selected tab

  //arrays of pointers to data
  Settings* settings;
  Level* level[1];

  GtkWidget* layout[1];
  GtkWidget* grid[1];
  GtkWidget* drawingArea0[1];

  void addTabData(int);
  void removeTabData(int);
};

/*!
  Provides static wrappers for gtk events.
**/
class Event
{
public:
  //UI modification
  static void btn_zoomIn(GtkButton* btn, void* eventData);
  static void btn_zoomOut(GtkButton* btn, void* eventData);
  static void btn_zoomFit(GtkButton* btn, void* eventData);
  static void zoomSpeed(GtkRange* range, Settings* settings);
  static void scrollSpeed(GtkRange* range, Settings* settings);
  static void canvasScroll(GtkWidget* layout, GdkEvent* event, void* eventData);

  //Map modification
  static void save(GtkButton* btn, void* map);
  static void tileClick(GtkWidget* drawingArea, GdkEvent* event, void* tile);
  static bool drawTile(Cairo::RefPtr<Cairo::Context> cr, Gtk::DrawingArea* da, Tile* tile);
  static void centerLevel(Gtk::Layout* layout, Gtk::Grid* grid);
private:
  static void zoomToFit(Level* level, GtkWidget* layout, GtkWidget* drawingArea0);
  static void zoom(Settings* settings, Level* level, GtkWidget* layout, GtkWidget* grid, GtkWidget* drawingArea0, double scrollDir);
};
