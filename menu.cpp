#include "menu.h"


namespace ui
{

	Menu& Menu::getInstance()
	{
		static Menu  instance;
		return instance;
	}

	bool Menu::isActive()
	{
		return iActive_;
	}

	void Menu::enable()
	{
		iActive_ = 1;
	}

	void Menu::disable()
	{
		iActive_ = 0;
	}

}