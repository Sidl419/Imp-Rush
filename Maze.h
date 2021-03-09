#ifndef MAIN_MAZE_H
#define MAIN_MAZE_H

#include <string>
#include <iostream>
#include <fstream>
#include "Image.h"
#include "Map.h"
#include "Player.h"
#include <GLFW/glfw3.h>
#include <vector>

struct Room
{
  int x;
  int y;
};

struct Maze
{
  explicit Maze(const std::string &a_path);

  int Width()    const { return width; }
  int Height()   const { return height; }
  std::vector<std::string> Data()        { return  data; }

  Map Next_room(int door_number);

  ~Maze(){}

private:
  int width = -1;
  int height = -1;
  std::vector<std::string> data;
  Room current {.x = 0, .y = 0 };
};

#endif //MAIN_MAZE_H