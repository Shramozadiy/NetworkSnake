#pragma once

#include <vector>
#include <mutex>

class Data
{
public:

	// Типы данных класса

	struct Point
	{
		int x = -1000;
		int y = -1000;
	};

	struct Snake
	{
		std::vector <Data::Point> body;
		int previousDirection = 0;
		int nextDirection = 0;
	};


	struct Space
	{
		int x;
		int y;
		int width;
		int height;
	};

	/////////////////////////////////////////////////////


	Data(int x, int y, int width, int height);

	Data(const Data& obj);



	// Функции управления данными

	std::vector <Snake>& getSnakes();

	void setSnakes(std::vector <Snake> newSnakes);

	std::vector <Point> getFood();

	void setFood(std::vector <Point> newFood);

	Space getGameSpace();


	void setNextDirection(int id, int nextDir);

	int getNextDirection(int id);

	int getGamePoints(int id);

	void setGamePoints(int points, int id);

	// Данные
private:

	std::mutex _lock;

	std::vector <Snake> snakes;
	std::vector <Point> food;

	 Space gameSpace;

	 int gamePoints[2] = {};
};