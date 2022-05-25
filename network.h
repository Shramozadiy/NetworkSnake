#pragma once

// Server part of Server-Client chat. Developed by Mr_Dezz




//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#define _CRT_SECURE_NO_WARNINGS

#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <mutex>
#include <regex>
#include <thread>

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "menu.h"
#include "data.h"
#include "game.h"
#include "readText.h"



using boost::asio::ip::udp;

using namespace std;

void wait(int FPS);

void closeServer();

void closeClient();

int isActiveServer();

int isActiveClient();

std::string getIp4Server();

void createServer(void);


void readText(std::string& text);

void connectToServer(HWND hWnd);