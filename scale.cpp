#include "scale.h"


void scale(HWND hWnd, int width, int height)
{
	RECT rect;
	GetClientRect(hWnd, &rect); //  Получаем координаты клиентской области окна

	float widthWindow = (float)(rect.right - rect.left); // Ширина окна
	float heightWindow = (float)(rect.bottom - rect.top); // Высота окна

	float scale_w = 1;
	float scale_h = 1;

	if (width / widthWindow > height / heightWindow)
	{
		scale_w = width / 2.0;
		scale_h = scale_w * (heightWindow / widthWindow);
	}
	else
	{
		scale_h = height / 2.0;
		scale_w = scale_h * (widthWindow / heightWindow);
	}

	glLoadIdentity();
	glScalef(1 / scale_w, 1 / scale_h, 1.0);
	glTranslatef(-width / 2.0, -height / 2.0, 0.0);
}