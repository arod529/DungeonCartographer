#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

class Settings
{
	public:
		const std::string defaultTilesetFile = "./tiles/defaultSet";
		const char* uiFile = (char*)"./ui/dungeonCartographer.ui";
		const char* cssFile = (char*)"./ui/dungeonCartographer.css";

		Settings();

		//utility
		bool writeDefaultTilesetFile();
		bool writeDefaultSettingsFile();
		bool loadSettingsFile(std::string _fileName);

		//accessors
		int getMapSize() const;
		int getZoomSpeed() const;
		int getScrollSpeed() const;
		bool getZoomFollowsMouse() const;

		//mutators
		void setMapSize(int _mapSize);
		void setZoomSpeed(int _zoomSpeed);
		void setScrollSpeed(int _scrollSpeed);
		void setZoomFollowsMouse(bool _zoomFollowsMouse);

		//overloads
		friend std::ostream& operator<<(std::ostream& _out, const Settings& _settings);
		friend std::istream& operator>>(std::istream& _in, Settings& _settings);

	private:
		const std::string defaultSettingsFile = "./ui/config";

		int mapSize;
		int zoomSpeed;
		int scrollSpeed;

		bool zoomFollowsMouse;

		int tabCount;
};

#endif
