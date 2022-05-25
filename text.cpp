
#include "text.h"


Text& Text::getInstance()
{
	static Text  instance;
	return instance;
}

void Text::initText(char const* fileName)
{
	loadTexture(fileName, &textTextureId);
	glBindTexture(GL_TEXTURE_2D, textTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	for (int i = 0; i < 256; i++)
	{
		charWidthArray_[i] = 1.0;
	}

	setCharSize(fileName, 3);
}

void Text::setCharSize(char const* fileName, int checkByte)
{
	int width, height, cnt;
	unsigned char* data = stbi_load(fileName, &width, &height, &cnt, 0);

	int pixPerChar = width / 16;
	for (int k = 0; k < 256; k++)
	{
		int x = (k % 16) * pixPerChar;
		int y = (k / 16) * pixPerChar;
		int i;
		int ind;
		unsigned char alpha = 0;
		for (i = x + pixPerChar - 1; i > x; i--)
		{
			for (int j = y + pixPerChar - 1; j > y; j--)
			{
				alpha = data[(j * width + i) * cnt + checkByte];
				if (alpha > 0) break;
			}
			if (alpha > 0) break;
		}

		i += pixPerChar / 10.0;
		if (i > x + pixPerChar - 1) i = x + pixPerChar - 1;
		if (k == 32) i = (x + pixPerChar / 2.0);

		charWidthArray_[k] = (i - x) / (float)pixPerChar;
	}



}

float Text::getLenText(const char* text)
{
	float len = 0;
	while (*text)
	{
		unsigned char c = *text;
		len += charWidthArray_[c];
		text++;
	}
	return len;
}




void Text::display(const char* text)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, textTextureId);
	glPushMatrix();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	float rectCoord[] = { 0,0, 1,0, 1,1, 0,1 };
	float rectTex[] = { 0,1, 1,1, 1,0, 0,0 };

	glVertexPointer(2, GL_FLOAT, 0, rectCoord);
	glTexCoordPointer(2, GL_FLOAT, 0, rectTex);

	while (*text)
	{
		static float charSize = 1 / 16.0;
		unsigned char c = *text;
		float cWidth = charWidthArray_[c];
		int x = c & 0b1111;
		int y = c >> 4;
		struct { float left, right, top, bottom; } rct;
		rct.left = x * charSize;
		rct.right = rct.left + charSize * cWidth;
		rct.top = y * charSize;
		rct.bottom = rct.top + charSize;

		rectTex[0] = rectTex[6] = rct.left;
		rectTex[2] = rectTex[4] = rct.right;
		rectTex[1] = rectTex[3] = rct.bottom;
		rectTex[5] = rectTex[7] = rct.top;
		rectCoord[2] = rectCoord[4] = cWidth;

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		text++;
		glTranslatef(cWidth, 0, 0);
	}

	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}



