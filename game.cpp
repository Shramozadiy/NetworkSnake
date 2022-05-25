#include "game.h"

Game& Game::getInstance()
{
	static Game  instance;
	return instance;
}


void Game::addSnake(int size, int x, int y)
{
	Data::Snake newSnake;

	for (int i = 0; i < size; i++)
	{
		newSnake.body.push_back({ -100, -100 });
	}

	newSnake.body[0].x = x;
	newSnake.body[0].y = y;

	std::vector <Data::Snake> snakes;
	snakes = data.getSnakes();
	snakes.push_back(newSnake);
	data.setSnakes(snakes);

}


bool Game::isInBodyAllSnakes(int x, int y)
{
	std::vector <Data::Snake> snakes;
	snakes = data.getSnakes();

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

void Game::addFood()
{
	std::vector <Data::Point> food;
	food = data.getFood();

	while (1)
	{
		int x = rand() % data.getGameSpace().width;
		int y = rand() % data.getGameSpace().height;

		if (isInBodyAllSnakes(x, y) == 0 && isFood(x, y) == 0)
		{
			food.push_back({ x,y });
			data.setFood(food);
			return;
		}
	}
}

bool Game::isFood(int x, int y)
{
	std::vector <Data::Point> food;
	food = data.getFood();

	for (auto i = food.begin(); i < food.end(); i++)
	{
		if (i->x == x && i->y == y)
		{
			return 1;
		}
	}
	return 0;
}

void Game::deleteFood(int x, int y)
{
	std::vector <Data::Point> food;
	food = data.getFood();

	for (int i = 0; i < food.size(); i++)
	{
		if (food[i].x == x && food[i].y == y)
		{
			food.erase(food.begin() + i);
		}
	}

	data.setFood(food);
}


void Game::refresh()
{

	std::vector <Data::Snake> snakes;
	snakes = data.getSnakes();

	Data::Space gameSpace;
	gameSpace = data.getGameSpace();
	int num = 0;
	for (auto& i : snakes)
	{
		

		auto head = i.body[0];

		switch (i.nextDirection)
		{
		case 0:
			if (i.previousDirection != 2)
			{
				head.x = head.x - 1;
				i.previousDirection = i.nextDirection;
			}
			else
				head.x = head.x + 1;
			break;
		case 1:
			if (i.previousDirection != 3)
			{
				head.y = head.y + 1;
				i.previousDirection = i.nextDirection;
			}
			else
				head.y = head.y - 1;
			break;
		case 2:
			if (i.previousDirection != 0)
			{
				head.x = head.x + 1;
				i.previousDirection = i.nextDirection;
			}
			else
				head.x = head.x - 1;
			break;
		case 3:
			if (i.previousDirection != 1)
			{
				head.y = head.y - 1;
				i.previousDirection = i.nextDirection;
			}
			else
				head.y = head.y + 1;
			break;
		default:

			break;
		}
		


		head.x = head.x < 0 ? gameSpace.width - 1 : head.x;
		head.y = head.y < 0 ? gameSpace.height - 1 : head.y;

		head.x = head.x > gameSpace.width - 1 ? 0 : head.x;
		head.y = head.y > gameSpace.height - 1 ? 0 : head.y;

		i.body.insert(i.body.begin(), head);
		i.body.pop_back();

		data.setSnakes(snakes);

		// Съела ли змейка еду
		if (isFood(i.body[0].x, i.body[0].y))
		{
			deleteFood(i.body[0].x, i.body[0].y);
			addFood();
			i.body.push_back({ -100,-100 });
			data.setGamePoints(data.getGamePoints(num) + 1, num);
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
		num++;
	}

	data.setSnakes(snakes);
}

void Game::display()
{
	std::vector <Data::Point> food;
	std::vector <Data::Snake> snakes;
	food = data.getFood();
	snakes = data.getSnakes();

	float width = data.getGameSpace().width;
	float height = data.getGameSpace().height;

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
		glColor3f(1, 0, 1);
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
			glColor3f(1, 1, 1);
			glVertex2d(j.x + 10, j.y + 10);
			glVertex2d(j.x + 1 + 10, j.y + 10);
			glVertex2d(j.x + 1 + 10, j.y + 1 + 10);
			glVertex2d(j.x + 10, j.y + 1 + 10);
			glEnd();
		}
	}

	glDisable(GL_POINT_SMOOTH);

	//data.setFood(food);
	//data.setSnakes(snakes);

}


void Game::setMovement(int move, int id)
{
	std::vector <Data::Snake> snakes;
	snakes = data.getSnakes();
	snakes[id].nextDirection = move;
	data.setSnakes(snakes);
}

