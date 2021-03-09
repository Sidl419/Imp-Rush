#include "Maze.h"

Maze::Maze(const std::string &a_path)
{
    std::ifstream fin;
    fin.open(a_path);

    std::string line;

    // вычисляем размеры лабиринта
    int h = 0, t = 0;
    while(std::getline(fin, line))
        data.push_back(line);

    width = data[0].length();
    height = data.size();

    fin.close();
}

Map Maze::Next_room(int door_number)
{
    bool* doors = new bool[4];

    switch(door_number)
    {
        case 50:
            current.y -= 1;
            break;
        case 60:
            current.x -= 1;
            break;
        case 70:
            current.x += 1;
            break;
        case 80:
            current.y += 1;
            break;
        default:
            break;
    }
    char c = data[current.y][current.x];

    if(current.y == 0)
        doors[0] = 0;
    else
    {
        if(data[current.y - 1][current.x] != '.')
            doors[0] = 1;
        else
            doors[0] = 0;
    }

    if(current.x == 0)
        doors[1] = 0;
    else
    {
        if(data[current.y][current.x - 1] != '.')
            doors[1] = 1;
        else
            doors[1] = 0;
    }

    if(data[current.y + 1][current.x] != '.')
        doors[3] = 1;
    else
        doors[3] = 0;
    
    if(data[current.y][current.x + 1] != '.')
        doors[2] = 1;
    else
        doors[2] = 0;
    

    std::string s(1, c);
    std::string path = "./resources/maps/" + s;
    path += ".txt";
    Map res(path, doors);
    res.Make_prop();
    return res;
}