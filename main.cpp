#include "common.h"
#include "Image.h"
#include "Player.h"
#include "Map.h"
#include "Maze.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <unistd.h>
#include <math.h> 

struct InputState
{
  bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
  GLfloat lastX = 400, lastY = 300; //исходное положение мыши
  bool firstMouse = true;
  bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
  bool capturedMouseJustNow = false;
} static Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat passedTime = 0.0f;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
  case GLFW_KEY_1:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  case GLFW_KEY_2:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
	default:
		if (action == GLFW_PRESS)
      Input.keys[key] = true;
		else if (action == GLFW_RELEASE)
      Input.keys[key] = false;
	}
}

int processPlayerMovement(Player &player, bool is_key, int **prop)
{
  if (Input.keys[GLFW_KEY_W])
    return player.ProcessInput(MovementDir::UP, is_key, prop);
  else if (Input.keys[GLFW_KEY_S])
    return player.ProcessInput(MovementDir::DOWN, is_key, prop);
  if (Input.keys[GLFW_KEY_A])
    return player.ProcessInput(MovementDir::LEFT, is_key, prop);
  else if (Input.keys[GLFW_KEY_D])
    return player.ProcessInput(MovementDir::RIGHT, is_key, prop);

  return 0;
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    Input.captureMouse = !Input.captureMouse;

  if (Input.captureMouse)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Input.capturedMouseJustNow = true;
  }
  else
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
  if (Input.firstMouse)
  {
    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
    Input.firstMouse = false;
  }

  GLfloat xoffset = float(xpos) - Input.lastX;
  GLfloat yoffset = Input.lastY - float(ypos);

  Input.lastX = float(xpos);
  Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
  // ...
}


int initGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  std::cout << "Controls: "<< std::endl;
  std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
  std::cout << "W, A, S, D - movement  "<< std::endl;
  std::cout << "press ESC to exit" << std::endl;

	return 0;
}

void end_game(GLFWwindow* window, Image &screenBuffer, Image &img)
{
  for(int y = 0; y < MAP_HEIGHT; ++y)
    for(int x = 0; x <= MAP_WIDTH; ++x)
      screenBuffer.PutPixel(x, y, img.GetPixel(x, MAP_HEIGHT - y - 1));

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
  glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;

  glfwSwapBuffers(window);
  sleep(3);
}

void tone(Image &screenBuffer, int tone, bool is_up)
{
  float alpha = 1;

  for(int y = 0; y < MAP_HEIGHT; ++y)
      for(int x = 0; x <= MAP_WIDTH; ++x)
      {
        Pixel screen_pixel = screenBuffer.GetPixel(x, y);
        screen_pixel.a = alpha;
        if(is_up)
        {
          screen_pixel.r *= tone;
          screen_pixel.g *= tone;
          screen_pixel.b *= tone;
        }else
        {
          screen_pixel.r /= tone;
          screen_pixel.g /= tone;
          screen_pixel.b /= tone;
        }
        screenBuffer.PutPixel(x, y, screen_pixel);
      }
}

void lantern(Image &screenBuffer, Point player)
{
  float alpha = 1;
  int p_x = player.x / tileSize + 1;
  int p_y = player.y / tileSize + 1;
  int dist, norm = 4;
  int height = MAP_HEIGHT / tileSize;
  int width = MAP_WIDTH / tileSize;

  for(int i = 0; i < height; i++)
  {
    for(int j = 0; j < width; j++)
    {
      dist = pow(p_x - j, 2) + pow(p_y - height + i, 2);
      int start_x = j * tileSize;
      int end_y = (height - 1 - i + 1) * tileSize;
      for(int y = (height - 1 - i) * tileSize + 1; y <= end_y; ++y)
      {
        for(int x = start_x + 1; x <= (j + 1) * tileSize; ++x)
        {
          Pixel screen_pixel = screenBuffer.GetPixel(x, y);
          screen_pixel.a = alpha;

          if(dist > 2)
          {
            screen_pixel.r /= norm;
            screen_pixel.g /= norm;
            screen_pixel.b /= norm;
          }
          if(dist > 10)
          {
            screen_pixel.r /= (norm / 2);
            screen_pixel.g /= (norm / 2);
            screen_pixel.b /= (norm / 2);
          }
          if(dist > 20)
          {
            screen_pixel.r /= norm;
            screen_pixel.g /= norm;
            screen_pixel.b /= norm;
          }

          screenBuffer.PutPixel(x, y, screen_pixel);
        }
      }
    }
  }
  /*
  for(int y = 0; y < MAP_HEIGHT; ++y)
    for(int x = 0; x <= MAP_WIDTH; ++x)
    {
      Pixel screen_pixel = screenBuffer.GetPixel(x, y);
      screen_pixel.a = alpha;
      dist = pow(p_x - x, 2) + pow(p_y - y, 2);

      if(dist > 5000)
      {
        screen_pixel.r /= norm;
        screen_pixel.g /= norm;
        screen_pixel.b /= norm;
      }
      if(dist > 8000)
      {
        screen_pixel.r /= norm;
        screen_pixel.g /= norm;
        screen_pixel.b /= norm;
      }
      if(dist > 10000)
      {
        screen_pixel.r /= norm;
        screen_pixel.g /= norm;
        screen_pixel.b /= norm;
      }

      screenBuffer.PutPixel(x, y, screen_pixel);
    }
    */
}

void fade(GLFWwindow* window, Image &screenBuffer, bool is_in, float time)
{
  float alpha = 1;

  for(int frame = 0; frame < 4; frame++)
  {
    tone(screenBuffer, 2, !is_in);
    usleep(time);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;

    glfwSwapBuffers(window);
  }
}

int main(int argc, char** argv)
{
	if(!glfwInit())
    return -1;

//	СОЗДАЕМ ОКНО
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Imp rush", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window); 
// ЗАКОНЧИЛИ СОЗДАВАТЬ ОКНО





	glfwSetKeyCallback        (window, OnKeyboardPressed);  
	glfwSetCursorPosCallback  (window, OnMouseMove); 
  glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
	glfwSetScrollCallback     (window, OnMouseScroll);

	if(initGL() != 0) 
		return -1;
	
  //Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

	Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS; // Первые 2 аргумента функции glViewport — это позиция нижнего левого угла окна. Третий и четвертый — это ширина и высота отрисовываемого окна в px
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;

  int game_mode;
  Maze maze("./resources/maps/Maze.txt");
  
  Map map = maze.Next_room(0);
  Point starting_pos = map.Make_prop();
	Player player{starting_pos};

  //game loop  ОТРИСОВОЧКА
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
    passedTime += deltaTime;
		lastFrame = currentFrame;
    glfwPollEvents();

    game_mode = processPlayerMovement(player, map.get_key(), map.Prop());

    if(game_mode == 1)
    {
      Image img("./resources/big_textures/game_over.png");
      end_game(window, screenBuffer, img);
      break;
    }

    if(game_mode == 2)
    {
      Image img("./resources/big_textures/you_win.png");
      end_game(window, screenBuffer, img);
      break;
    }

    if(game_mode == 3)
    {
      map.set_key(1);
    }

    if(game_mode >= 10)
    {
      fade(window, screenBuffer, 1, 50000);

      map = maze.Next_room(game_mode);
      Point starting_pos = map.Make_prop();
      player.Move(starting_pos);
      map.Draw(screenBuffer);
      player.Draw(screenBuffer, MovementDir::IDLE);
      tone(screenBuffer, 16, 0);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
      glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
      glfwSwapBuffers(window);

      fade(window, screenBuffer, 0, 50000);
    }

    if(passedTime > 0.05){
      passedTime = 0.0f;

      map.Draw(screenBuffer);
      lantern(screenBuffer, player.GetCoords());

      if (Input.keys[GLFW_KEY_W])
        player.Draw(screenBuffer,MovementDir::UP);
      else 
        if (Input.keys[GLFW_KEY_S])
          player.Draw(screenBuffer,MovementDir::DOWN);
        else
          if (Input.keys[GLFW_KEY_A])
            player.Draw(screenBuffer,MovementDir::LEFT);
          else 
            if (Input.keys[GLFW_KEY_D])
              player.Draw(screenBuffer,MovementDir::RIGHT);
            else 
              player.Draw(screenBuffer,MovementDir::IDLE);
    }

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
