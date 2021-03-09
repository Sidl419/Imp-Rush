#include "Map.h"

Map::Map(const std::string &a_path, bool* exits)
{
    doors = new bool[4];
    for(int i = 0; i < 4; i++)
        doors[i] = exits[i];
    
    std::ifstream fin;
    fin.open(a_path);

    width = MAP_WIDTH / tileSize;
    height = MAP_HEIGHT / tileSize;

    data = new char[height * width];
    int i = 0, d_count = 0;

    std::string line;
    while(std::getline(fin, line))
        for(int j = 0; j < line.length(); j++)
        {
            if(line[j] == 'x')
            {
                if(doors[d_count])
                    data[i] = 'x';
                else
                    data[i] = '#';
                d_count++;
            }else
                data[i] = line[j];
            i++;
        }

    fin.close();
}

void Map::Draw(Image &screen)
{
    std::string path;

    Image floor("./resources/big_textures/floor.png");
    Image hole("./resources/big_textures/hole.png");

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            switch(data[i * width + j])
            {
                case '.':
                    path = "./resources/big_textures/floor.png";
                    break;
                case '#':
                    path = "./resources/big_textures/wall.png";
                    break;
                case ' ':
                    path = "./resources/big_textures/hole.png";
                    break;
                case 'K':
                    if(is_key)
                    {
                        path = "./resources/big_textures/floor.png";
                    }else
                    {
                        Draw_tile(screen, i, j, floor);
                        path = "./resources/big_textures/key_" + std::to_string(frame / 2);
                        path += ".png";
                        frame = (frame + 1) % framerate;
                    }
                    break;
                case 'x':
                    Draw_tile(screen, i, j, hole);
                    if(is_key)
                    {
                        path = "./resources/big_textures/door_" + std::to_string(3 - door_frame);
                        path += ".png";
                        if(door_frame > 0)
                            door_frame--;
                    }
                    else
                        path = "./resources/big_textures/door_0.png";
                    break;
                case 'Q':
                    path = "./resources/big_textures/exit.png";
                    break;
                default:
                    break;
            }

            Image img(path);
            Draw_tile(screen, i, j, img);
        }
    }
}

void Map::Draw_tile(Image &screen, int i, int j, Image &img)
{
    int start_x = j * tileSize;
    int end_y = (height - 1 - i + 1) * tileSize;
    for(int y = (height - 1 - i) * tileSize + 1; y <= end_y; ++y)
    {
        for(int x = start_x; x <= (j + 1) * tileSize; ++x)
        {
            Pixel terrain_pixel = img.GetPixel(x - start_x, end_y - y);
            terrain_pixel.a = 1;
            if(terrain_pixel.r != 0 && terrain_pixel.g != 0 && terrain_pixel.b != 0)
                screen.PutPixel(x, y, terrain_pixel);
        }
    }
}

Point Map::Make_prop()
{
    width = MAP_WIDTH / tileSize;
    height = MAP_HEIGHT / tileSize;

    int val;
    Point player;

    prop = new int*[MAP_HEIGHT];
    for(int i = 0; i < MAP_HEIGHT; i++)
        prop[i] = new int[MAP_WIDTH];

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            switch(data[i * width + j])
            {
                case '.':
                    val = 0;
                    break;
                case '#':
                    val = 1;
                    break;
                case ' ':
                    val = 2;
                    break;
                case 'K':
                    val = 3;
                    break;
                case 'x':
                    val = 1;
                    if(i == 0)
                        val = 50;
                    if(j == 0)
                        val = 60;
                    if(j == width - 1)
                        val = 70;
                    if(i == height - 1)
                        val = 80;
                    break;
                case 'Q':
                    val = 5;
                    break;
                case '@':
                    val = 0;
                    player.x = j * tileSize + tileSize / 2;
                    player.y = (height - 1 - i) * tileSize + tileSize / 2;
                    break;
                default:
                    val = -1;
                    break;
            }

            int start_x = j * tileSize;
            int end_y = (height - 1 - i + 1) * tileSize;
            
            for(int y = (height - 1 - i) * tileSize + 1; y <= end_y; ++y)
                for(int x = start_x; x < (j + 1) * tileSize; ++x)
                {
                    prop[x][y] = val;
                    //std::cout << x << ' ' << y << std::endl;
                }
        }
    }

    return player;
}

Map& Map::operator=(const Map& other)
{
    if (this == &other)
        return *this;
 
    delete [] data;
    for(int i = 0; i < MAP_HEIGHT; ++i)
        delete [] prop[i];
    delete [] prop;

    data = nullptr;
    prop = nullptr;
    
    width = other.width;
    height = other.height;
    frame = other.frame;
    door_frame = other.door_frame;
    framerate = other.framerate;
    is_key = other.is_key;

    data = new char[height * width];
    prop = nullptr;

 
    for(int i = 0; i < height * width; i++)
        data[i] = other.data[i];

    return *this;
}

Map::Map(const Map &other)
{   
    width = other.width;
    height = other.height;
    frame = other.frame;
    door_frame = other.door_frame;
    framerate = other.framerate;
    is_key = other.is_key;

    data = new char[other.height * other.width];
    prop = nullptr;

    for(int i = 0; i < height * width; i++)
        data[i] = other.data[i];
}

void Map::set_key(bool key) 
{ 
    is_key = key; 
    door_frame = 2; 

    for(int i = 0; i < MAP_HEIGHT; i++)
        for(int j = 0; j < MAP_WIDTH; j++)
            if(prop[i][j] == 3)
                prop[i][j] = 0;
}