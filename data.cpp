
#include "data.h"




Data::Data(int x, int y, int width, int height)
{
	gameSpace = { x, y, width, height };
}

Data::Data(const Data& obj)
{
	std::lock_guard<std::mutex> lg(_lock);
	gameSpace = obj.gameSpace;
	snakes = obj.snakes;
	food = obj.food;
}



// Функции управления данными

std::vector <Data::Snake>& Data::getSnakes()
{
	std::lock_guard<std::mutex> lg(_lock);
	return snakes;
}

void Data::setSnakes(std::vector <Snake> newSnakes)
{
	std::lock_guard<std::mutex> lg(_lock);
	snakes = newSnakes;
}

std::vector <Data::Point> Data::getFood()
{
	std::lock_guard<std::mutex> lg(_lock);
	return food;
}

void Data::setFood(std::vector <Point> newFood)
{
	std::lock_guard<std::mutex> lg(_lock);
	food = newFood;
}

Data::Space Data::getGameSpace()
{
	std::lock_guard<std::mutex> lg(_lock);
	return gameSpace;
}


void Data::setNextDirection(int id, int nextDir)
{
	std::lock_guard<std::mutex> lg(_lock);
	snakes[id].nextDirection = nextDir;
}

int Data::getNextDirection(int id)
{
	std::lock_guard<std::mutex> lg(_lock);
	return snakes[id].nextDirection;
}

int Data::getGamePoints(int id)
{
	std::lock_guard<std::mutex> lg(_lock);
	return gamePoints[id];
}

void Data::setGamePoints(int points, int id)
{
	std::lock_guard<std::mutex> lg(_lock);
	gamePoints[id] = points;
}