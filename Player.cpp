#include "Player.h"


bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

int Player::ProcessInput(MovementDir dir, bool is_key, int **prop)
{
  int move_dist = move_speed * 1;
  int res = 0;
  bool temp;

  if(prop[coords.x + tileSize / 2][coords.y + tileSize / 2] == 2)
    return 1;
  if(prop[coords.x + tileSize / 2][coords.y + tileSize / 2] == 5)
    return 2;
  if(prop[coords.x + tileSize / 2][coords.y + tileSize / 2] == 3)
    res = 3;
  if((prop[coords.x + tileSize / 2][coords.y + tileSize / 2] >= 10) && is_key)
    res = prop[coords.x + tileSize / 2][coords.y + tileSize / 2];
  
  switch(dir)
  {
    case MovementDir::UP:
      temp = (prop[coords.x + tileSize / 2][coords.y + move_dist + tileSize] >= 10) && !is_key;
      if((prop[coords.x + tileSize / 2][coords.y + move_dist + tileSize] != 1) && !temp)
      {
        old_coords.y = coords.y;
        coords.y += move_dist;
      }
      break;
    case MovementDir::DOWN:
      temp = (prop[coords.x + tileSize / 2][coords.y - move_dist] >= 10) && !is_key;
      if((prop[coords.x + tileSize / 2][coords.y - move_dist] != 1) && !temp)
      {
        old_coords.y = coords.y;
        coords.y -= move_dist;
      }
      break;
    case MovementDir::LEFT:
      temp = (prop[coords.x - move_dist][coords.y + tileSize / 2] >= 10) && !is_key;
      if((prop[coords.x - move_dist][coords.y + tileSize / 2] != 1) && !temp)
      {
        old_coords.x = coords.x;
        coords.x -= move_dist;
      }
      break;
    case MovementDir::RIGHT:
      temp = (prop[coords.x + move_dist + tileSize][coords.y + tileSize / 2] >= 10) && !is_key;
      if((prop[coords.x + move_dist + tileSize][coords.y + tileSize / 2] != 1) && !temp)
      {
        old_coords.x = coords.x;
        coords.x += move_dist;
      }
      break;
    default:
      break;
  }

  return res;
}

void Player::Draw(Image &screen, MovementDir dir)
{
  int move_dist = move_speed * 1;

  std::string path;

  switch(dir){
    case MovementDir::UP:
    case MovementDir::DOWN:
    case MovementDir::LEFT:
    case MovementDir::RIGHT:
      if(is_idle){ frame = 0; is_idle = 0; }
      path = "./resources/big_textures/imp_run_anim_f" + std::to_string(frame / 2);
      path += ".png";
      break;
    case MovementDir::IDLE:
      if(not is_idle){ frame = 0; is_idle = 1; }
      path = "./resources/big_textures/imp_idle_anim_f" + std::to_string(frame / 2);
      path += ".png";
      break;
    default:
      break;
  }
  frame = (frame + 1) % framerate;

  Image img(path);
  for(int y = coords.y + 1; y <= coords.y + tileSize; ++y)
  {
    for(int x = coords.x; x <= coords.x + tileSize; ++x)
    {
      Pixel player_pixel = img.GetPixel(x - coords.x, tileSize - y + coords.y);
      player_pixel.a = 0.5;
      if(player_pixel.r != 0 && player_pixel.g != 0 && player_pixel.b != 0)
        screen.PutPixel(x, y, player_pixel);
    }
  }
}