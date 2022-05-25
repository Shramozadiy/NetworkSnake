#include "network.h"




int iActiveServer = 0;
int iActiveClient = 0;
std::string ip4Server;

void wait(int FPS)
{
	static int count = 0;
	static chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
	chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();

	long long time_lost = chrono::duration_cast<chrono::milliseconds>(end - start).count();

	long long time_need = long long(count * 1000.0 / FPS);

	if (time_lost < time_need)
	{
		std::this_thread::sleep_for(chrono::milliseconds(time_need) - (end - start));
	}

	count++;
}

void closeServer()
{
	iActiveServer = 0;
}
void closeClient()
{
	iActiveClient = 0;
}

int isActiveServer()
{
	return iActiveServer;
}
int isActiveClient()
{
	return iActiveClient;
}

std::string getIp4Server()
{
	return ip4Server;
}

void createServer(void)
{
	iActiveServer = 1;

	short packet_size = 0;												// The size of sending / receiving packet in bytes


	boost::asio::io_context io_context;

	udp::socket socket(io_context, udp::endpoint(udp::v4(), 8000));

	thread game = thread([]()
		{
			while (iActiveServer)
			{
				wait(20);
				Game::getInstance().refresh();
			}
		});

	// Цикл передачи и приема сообщений
	while (iActiveServer)
	{
		Sleep(10);

		vector <Data::Snake> snakes;
		vector <Data::Point> food;
		snakes = Game::getInstance().data.getSnakes();
		food = Game::getInstance().data.getFood();

		udp::endpoint remote_endpoint;

		{
			boost::array <int, 1> msg;
			int sizeMessage = socket.receive_from(boost::asio::buffer(msg), remote_endpoint);
			//snakes[1].nextDirection = msg[0];
			Game::getInstance().data.setNextDirection(1, msg[0]);
			//GameSpace::getInstance().setMovementDirection(msg[0], 1);
		}


		{
			boost::system::error_code ignored_error;

			boost::array <int, 2> msg1 = { Game::getInstance().data.getGamePoints(0), Game::getInstance().data.getGamePoints(1) };
			socket.send_to(boost::asio::buffer(msg1), remote_endpoint, 0, ignored_error);

			for (auto& i : snakes)
			{
				socket.send_to(boost::asio::buffer(i.body), remote_endpoint, 0, ignored_error);
			}

			socket.send_to(boost::asio::buffer(food), remote_endpoint, 0, ignored_error);

		}

	}

	game.join();

	ui::Menu::getInstance().enable();

}


// Client part for Server-Client chat. Developed by Mr_Dezz


void readText(std::string& text)
{

	long long number = ReadText::getInstance().getNumber();

	int i = 0;
	char symbol = 0;


	while (1)
	{
		if (ReadText::getInstance().getNumber() > number)
		{
			symbol = (char)ReadText::getInstance().get(number);
			/*if (symbol == '\r')
				break;*/
			regex r("[0-9\.]");

			if (symbol == '\b')
			{
				if (text.size() != 0)
					text.pop_back();
				number++;
			}
			else if (regex_match(string(1, symbol), r))
			{
				text = text + symbol;
				number++;
				ui::Menu::getInstance().buttons[2].addText((char*)text.c_str(), 0.05);
			}
			else
			{
				number++;
			}
		}

	}

	//return text;
}



void connectToServer(HWND hWnd)
{
	iActiveClient = 1;



	try
	{

		boost::asio::io_context io_context;

		udp::resolver resolver(io_context);
		udp::endpoint receiver_endpoint =
			*resolver.resolve(udp::v4(), ip4Server, "8000").begin();

		udp::socket socket(io_context);


		socket.open(udp::v4());


		// Цикл передачи и приема сообщений
		while (iActiveClient)
		{

			vector <Data::Snake> snakes;
			vector <Data::Point> food;
			snakes = Game::getInstance().data.getSnakes();
			food = Game::getInstance().data.getFood();

			{
				boost::array <int, 1> msg = { snakes[1].nextDirection };
				socket.send_to(boost::asio::buffer(msg), receiver_endpoint);
			}


			{

				udp::endpoint sender_endpoint;
				boost::system::error_code ignored_error;

				boost::array <int, 2> msg1;

				socket.receive_from(boost::asio::buffer(msg1), sender_endpoint);

				Game::getInstance().data.setGamePoints(msg1[0], 0);
				Game::getInstance().data.setGamePoints(msg1[1], 1);

				for (auto& i : snakes)
				{
					struct Point { int x, y; };
					boost::array <Point, 20 > msg;

					size_t sizeMessage = socket.receive_from(boost::asio::buffer(msg), sender_endpoint);

					int count = sizeMessage / sizeof(Point);

					i.body.resize(count, { 0, 0 }); // Изменяем размер змейки

					for (int j = 0; j < count; j++)
					{
						i.body[j].x = msg[j].x;
						i.body[j].y = msg[j].y;
					}
				}

				struct Point { int x, y; };
				boost::array <Point, 20 > msg;

				size_t sizeMessage = socket.receive_from(boost::asio::buffer(msg), sender_endpoint);

				int count = sizeMessage / sizeof(Point);

				food.resize(count, { 0, 0 }); // Изменяем размер змейки

				for (int j = 0; j < count; j++)
				{
					food[j].x = msg[j].x;
					food[j].y = msg[j].y;
				}
			}


			snakes[1].nextDirection = Game::getInstance().data.getNextDirection(1);
			Game::getInstance().data.setSnakes(snakes);
			Game::getInstance().data.setFood(food);

		}
	}
	catch (std::exception& e)
	{

	}

	iActiveClient = 0;
	ui::Menu::getInstance().enable();
}