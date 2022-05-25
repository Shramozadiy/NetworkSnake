#pragma once



#include <Windows.h>
#include <gl/GL.h>

#include "texture.h"

class Text
{
private:
	Text() {}
	Text(const Text&);
	Text& operator=(Text&);

	GLuint textTextureId = 0;
	float charWidthArray_[256] = {};
public:

	static Text& getInstance();

	void initText(char const* fileName);

	void setCharSize(char const* fileName, int checkByte);

	float getLenText(const char* text);

	void display(const char* text);

};




