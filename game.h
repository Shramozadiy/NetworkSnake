#pragma once



#include <Windows.h>
#include <gl/GL.h>
#include <iostream>
#include <vector>

//#include "snake.h"
#include "data.h"


class Game
{
	Game() {}
	Game(const Game&);
	Game& operator=(Game&);


public:

	Data data = Data(0,0,40,30);

	static Game& getInstance();

	void addSnake(int size, int x, int y);

	void addFood();

	bool isFood(int x, int y);

	void deleteFood(int x, int y);

	void refresh();

	void display();

	bool isInBodyAllSnakes(int x, int y);

	void setMovement(int move, int id);

};