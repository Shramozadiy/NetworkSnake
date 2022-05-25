#pragma once

#include <Windows.h>
#include <gl/GL.h>

#include "C:/C++/stb-master/stb_image.h"




void loadTexture(char const* fileName, GLuint* idTexture);

void loadTexture(char const* fileName, GLuint* idTexture, int* width, int* height);