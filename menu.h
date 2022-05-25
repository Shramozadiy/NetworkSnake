#pragma once

#include <vector>

#include "button.h"

namespace ui
{
	class Menu
	{
		Menu() {}
		Menu(const Menu&);
		Menu& operator=(Menu&);

		bool iActive_ = 0;

	public:

		std::vector <Button> buttons;

		static Menu& getInstance();

		bool isActive();

		void enable();

		void disable();

	};


}