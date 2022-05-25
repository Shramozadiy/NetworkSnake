#pragma once

#include <Windows.h>
#include <gl/GL.h>
#include "text.h"

namespace ui
{

	class Button
	{
		struct { float x, y; } coordinates_;
		float width_, height_;

		const char* text_ = 0;
		float sizeText_ = 0;

	public:

		Button(float x, float y, float width, float height);

		void display();

		bool isInButton(float x, float y);

		void addText(const char* text, float sizeText);
	};

}