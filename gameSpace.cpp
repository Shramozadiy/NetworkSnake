#include "gameSpace.h"

GameSpace& GameSpace::getInstance()
{
	static GameSpace  instance;
	return instance;
}


void GameSpace::addSnake(int size, int x, int y)
{
	Snake newSnake;

	for (int i = 0; i < size; i++)
	{
		newSnake.body.push_back({ -100, -100 });
	}

	newSnake.body[0].x = x;
	newSnake.body[0].y = y;

	snakes.push_back(newSnake);
	
}


bool GameSpace::isInBodyAllSnakes(int x, int y)
{

	for (auto i : snakes)
	{
		for (auto j : i.body)
		{
			if (j.x == x && j.y == y)
				return 1;
		}
	}

	return 0;
}

void GameSpace::addFood()
{

	while (1)
	{
		int x = rand() % WIDTH_GAME_SPACE;
		int y = rand() % HEIGHT_GAME_SPACE;

		if (isInBodyAllSnakes(x, y) == 0)
		{
			food.push_back({ x,y });
			return;
		}
	}
}

bool GameSpace::isFood(int x, int y)
{
	for (auto i = food.begin(); i < food.end(); i++)
	{
		if (i->x == x && i->y == y)
		{
			return 1;
		}
	}
	return 0;
}

void GameSpace::deleteFood(int x, int y)
{
	for (int i = 0; i < food.size(); i++)
	{
		if (food[i].x == x && food[i].y == y)
		{
			food.erase(food.begin() + i);
		}
	}
}


void GameSpace::refresh()
{
	for (auto& i : snakes)
	{

		// Передвижение змейки
		i.move();

		i.body[0].x;
		i.body[0].y;


		i.body[0].x = i.body[0].x < 0 ? WIDTH_GAME_SPACE - 1 : i.body[0].x;
		i.body[0].y = i.body[0].y < 0 ? HEIGHT_GAME_SPACE - 1 : i.body[0].y;

		i.body[0].x = i.body[0].x > WIDTH_GAME_SPACE - 1 ? 0 : i.body[0].x;
		i.body[0].y = i.body[0].y > HEIGHT_GAME_SPACE - 1 ? 0 : i.body[0].y;

		i.move(i.body[0].x, i.body[0].y);

		// Съела ли змейка еду
		if (isFood(i.body[0].x, i.body[0].y))
		{
			deleteFood(i.body[0].x, i.body[0].y);
			addFood();
			i.body.push_back({ -100,-100 });
		}

		// Врезалась ли змейка
		bool iInSnakeBody = 0;
		for (auto& j : snakes)
		{
			if (&i == &j)
				continue;

			for (auto k : j.body)
			{
				if (k.x == i.body[0].x && k.y == i.body[0].y)
					iInSnakeBody = 1;
			}
		}

		for (int j = 1; j < i.body.size(); j++)
		{
			if (i.body[j].x == i.body[0].x && i.body[j].y == i.body[0].y)
				iInSnakeBody = 1;
		}

		if (iInSnakeBody == 1)
		{
			exit(0);
		}



	}
}

void GameSpace::display()
{
	float width = WIDTH_GAME_SPACE;
	float height = HEIGHT_GAME_SPACE;

	glPointSize(5);
	glEnable(GL_POINT_SMOOTH);

	glBegin(GL_LINE_LOOP);

	// Отрисовка границ игрового пространства
	glVertex2d(10, 10);
	glVertex2d(10 + width, 10);
	glVertex2d(10 + width, 10 + height);
	glVertex2d(10, 10 + height);

	glEnd();

	// Отрисовка еды
	for (auto i : food)
	{
		glBegin(GL_LINE_LOOP);
		glVertex2d(i.x + 10, i.y + 10);
		glVertex2d(i.x + 1 + 10, i.y + 10);
		glVertex2d(i.x + 1 + 10, i.y + 1 + 10);
		glVertex2d(i.x + 10, i.y + 1 + 10);
		glEnd();
	}

	for (auto i : snakes)
	{
		for (auto j : i.body)
		{
			glBegin(GL_LINE_LOOP);
			glVertex2d(j.x + 10, j.y + 10);
			glVertex2d(j.x + 1 + 10, j.y + 10);
			glVertex2d(j.x + 1 + 10, j.y + 1 + 10);
			glVertex2d(j.x + 10, j.y + 1 + 10);
			glEnd();
		}
	}

	glDisable(GL_POINT_SMOOTH);


}


void GameSpace::setMovementDirection(int movement, int id)
{
	switch (snakes[id].previousMovementDirection)
	{
	case 0:
		if (movement != 2)
			snakes[id].nextMovementDirection = movement;
		break;
	case 1:
		if (movement != 3)
			snakes[id].nextMovementDirection = movement;
		break;
	case 2:
		if (movement != 0)
			snakes[id].nextMovementDirection = movement;
		break;
	case 3:
		if (movement != 1)
			snakes[id].nextMovementDirection = movement;
		break;
	default:
		break;
	}
	
}

void GameSpace::setMovement(int move, int id)
{
	setMovementDirection(move, id);
	snakes[id].movement = move;
}

int GameSpace::getMovementDirection(int id)
{
	return snakes[id].movement;
}