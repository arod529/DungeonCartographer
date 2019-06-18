#ifndef UI_H
#define UI_H

#include "event.h"
#include "settings.h"
#include "level.h"
#include "map.h"

#include <cairo/cairo.h>
#include <gtk/gtk.h>

#include <stdexcept>
#include <string>
#include <vector>

struct UI
{
	public:
		UI(Settings*, Map*);
		~UI();

		void drawLevel(Map* _map, int i);

	private:
		//------------------------------
		//----- Program Properties -----
		//------------------------------
		Settings* settings;
		Map* map;


		//--------------------------
		//----- Gtk Properties -----
		//--------------------------
		GtkBuilder*     builder;
		GtkCssProvider* css;

		GObject* window;

		GObject* save;
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
