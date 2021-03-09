#ifndef MAIN_MAP_H
#define MAIN_MAP_H

#include <string>
#include <iostream>
#include <fstream>
#include "Image.h"
#include "Player.h"
#include <GLFW/glfw3.h>

constexpr GLsizei MAP_WIDTH = 512, MAP_HEIGHT = 512, UI_WIDTH = 0;
constexpr GLsizei WINDOW_WIDTH = MAP_WIDTH + UI_WIDTH;
constexpr GLsizei WINDOW_HEIGHT = MAP_HEIGHT;

struct Map
{
  explicit Map(const std::string &a_path, bool* doors);

  int Width()    const { return width; }
  int Height()   const { return height; }
  char* Data()        { return  data; }
  int** Prop()        { return  prop; }
  bool get_key() {return is_key; }

  void Draw(Image &screen);
  Point Make_prop();
  void Draw_tile(Image &screen, int x, int y, Image &img);
  void set_key(bool key);
  
  Map& operator=(const Map& other);
  Map(const Map &other);

  ~Map()
  { 
    delete [] data;

    for(int i = 0; i < MAP_HEIGHT; ++i)
        delete [] prop[i];
    delete [] prop;
    delete [] doors;
  }

private:
  int width = -1;
  int height = -1;
  char *data = nullptr;
  int **prop = nullptr;

  int frame = 0;
  int door_frame = 0;
  int framerate = 8;
  bool is_key = 0;
  bool* doors = nullptr;
};

#endif //MAIN_MAP_H