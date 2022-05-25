#include "button.h"

namespace ui
{

	Button::Button(float x, float y, float width, float height)
	{
		coordinates_.x = x;
		coordinates_.y = y;

		width_ = width;
		height_ = height;
	}

	void Button::display()
	{
		glPointSize(7);
		glEnable(GL_POINT_SMOOTH);

		glBegin(GL_LINE_LOOP);

		glVertex2d(coordinates_.x, coordinates_.y);
		glVertex2d(coordinates_.x, coordinates_.y + height_);
		glVertex2d(coordinates_.x + width_, coordinates_.y + height_);
		glVertex2d(coordinates_.x + width_, coordinates_.y);
		glEnd();

		glDisable(GL_POINT_SMOOTH);




		//float xText = (vertexesButton[0].x + vertexesButton[1].x) / 2.0;
		//float yText = (vertexesButton[0].y + vertexesButton[1].y) / 2.0;

		float xText = coordinates_.x + width_ / 2.0;
		float yText = coordinates_.y + height_ / 2.0;

		

		float widthText = Text::getInstance().getLenText(text_);
		float heightText = 1;
		float countSimbols = strlen(text_);

		glPushMatrix();
		glTranslatef(xText, yText, 0);
		glScalef(sizeText_, sizeText_, 0);
		glTranslatef(-widthText / 2.0, -heightText / 2, 0);
		Text::getInstance().display(text_);
		glPopMatrix();
	}

	bool Button::isInButton(float x, float y)
	{
		if (x > coordinates_.x && x < coordinates_.x + width_ &&
			y > coordinates_.y && y < coordinates_.y + height_)
			return 1;
		else
			return 0;
	}



	void Button::addText(const char* text, float sizeText)
	{
		text_ = text;
		sizeText_ = sizeText;
	}


}