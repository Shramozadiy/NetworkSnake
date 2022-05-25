#pragma once


#include <Windows.h>
#include <gl/GL.h>
#include <iostream>
#include <vector>

//#include "snake.h"

#define WIDTH_GAME_SPACE 80
#define HEIGHT_GAME_SPACE 50

class GameSpace
{
	GameSpace() {}
	GameSpace(const GameSpace&);
	GameSpace& operator=(GameSpace&);


public:

	struct Point
	{
		int x;
		int y;
	};

	struct Snake
	{
		std::vector <Point> body;

		int nextMovementDirection = 0; // 0 - влево, 1 - вверх, 2 - вправо, 3 - вниз
		int previousMovementDirection = 0; // 0 - влево, 1 - вверх, 2 - вправо, 3 - вниз
		int movement = 0; // 0 - влево, 1 - вверх, 2 - вправо, 3 - вниз

		void move()
		{
			auto i = body.begin();

			Point head;

			switch (nextMovementDirection)
			{
			case 0:
				head = { i->x - 1, i->y };
				break;
			case 1:
				head = { i->x, i->y + 1 };
				break;
			case 2:
				head = { i->x + 1, i->y };
				break;
			case 3:
				head = { i->x, i->y - 1 };
				break;
			default:
				head = { i->x , i->y };
				break;
			}

			body.insert(i, head);
			body.pop_back();

			previousMovementDirection = nextMovementDirection;
		}

		void move(int x, int y)
		{
			auto i = body.begin();
			i->x = x;
			i->y = y;
		}
	};

	std::vector <Snake> snakes;
	std::vector <Point> food;

	static GameSpace& getInstance();

	void addSnake(int size, int x, int y);

	void addFood();

	bool isFood(int x, int y);

	void deleteFood (int x,  int y);

	void refresh();

	void display();

	void setMovementDirection(int movement, int id);

	int getMovementDirection(int id);

	bool isInBodyAllSnakes(int x, int y);

	void setMovement(int move, int id);
};