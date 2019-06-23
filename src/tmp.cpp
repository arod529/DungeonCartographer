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