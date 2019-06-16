#ifndef TILESET_H
#define TILESET_H

#include <gtk/gtk.h>
#include <string>

/*!
  Tileset data structure. Contains the id, name, file path, and pixbuf of the
  tile image. An array of tileset structs defines a complete tileset.

  \bug variables should be private, and read only aside from initialization.
**/
struct Tileset
{
public:
  Tileset();
  Tileset(uint _id, std::string _name, std::string _filePath, GdkPixbuf* _pixbuf);
  ~Tileset();

  uint getId() const;
  std::string getName() const;
  std::string getFilePath() const;

  GdkPixbuf* pixbuf;
private:
  uint id;
  std::string  name;
  std::string filePath;
};

#endif
