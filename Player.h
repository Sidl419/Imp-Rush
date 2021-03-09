#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include <iostream>

struct Point
{
  int x;
  int y;
};

enum class MovementDir
{
  UP,
  DOWN,
  LEFT,
  RIGHT,
  IDLE
};

struct Player
{
  explicit Player(Point pos = {.x = 10, .y = 10}) :
                 coords(pos), old_coords(coords) {};

  void Move(Point pos)
  {
    coords.x = pos.x; 
    coords.y = pos.y;
    old_coords.x = coords.x;
    old_coords.y = coords.y;
  }

  bool Moved() const;
  int ProcessInput(MovementDir dir, bool is_key, int **prop);
  void Draw(Image &screen, MovementDir dir);
  Point GetCoords() const { return coords; }

private:
  Point coords {.x = 10, .y = 10};
  Point old_coords {.x = 10, .y = 10};
  Pixel color {.r = 111, .g = 111, .b = 111, .a = 111};
  int move_speed = 1;
  int frame = 0;
  bool is_idle = 1;
  int framerate = 8;
};

#endif //MAIN_PLAYER_H
