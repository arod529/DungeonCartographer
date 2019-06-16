#ifndef UI_H
#define UI_H

#include "settings.h"
#include "level.h"
#include "map.h"

#include <cairo/cairo.h>
#include <gtk/gtk.h>

#include <stdexcept>
#include <string>
#include <vector>


/*!
	Structure to provide events with access to multiple and/or
	dynamic data, whilst only requiring one argument and
	a single signal connect. Each index contains data
	specific to the equivalent tab.
**/
struct EventData
{
	static int currTab;	//the currently selected tab

	//arrays of pointers to data
	Settings* settings;
	Level* level[1];

	GtkWidget* layout[1];
	GtkWidget* grid[1];
	GtkWidget* drawingArea0[1];

	void addTabData(int);
	void removeTabData(int);
};

struct UI
{
	public:
		UI(Settings*, Map*);
		~UI();

		void drawLevel(Map* _map, int i);

	private:
		GtkBuilder*     builder;
		GtkCssProvider* css;

		GObject* window;

		GObject* zoom[2];
		GObject* zoomFit;
		GObject* zoomSlider;
		GObject* scrollSlider;

		std::vector<GObject*>   layout;
		std::vector<GtkWidget*> grid;

    std::vector<GObject*> tabContent;
    std::vector<GObject*> tabLabel;
    std::vector<GObject*> tabButton;

		EventData eventData;

		void drawLevel(GtkWidget* _grid, Level* _lvl);
};

#endif
