#ifndef SETTINGS_H
#define SETTINGS_H

class Settings
{
	public:
		char* uiFile = (char*)"./ui/dungeonCartographer.ui";
		char* cssFile = (char*)"./ui/dungeonCartographer.css";
		char* settingsFile = (char*)"./ui/config";

		Settings();

		//utility
		void writeDefaultTilesetFile();
		void writeDefaultSettingsFile();

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

	private:
		int mapSize;
		int zoomSpeed;
		int scrollSpeed;

		bool zoomFollowsMouse;

		int tabCount;
};

#endif
