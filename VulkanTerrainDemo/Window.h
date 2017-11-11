#pragma once

#include "Shared.h"

class Window
{
public:
	Window();
	~Window();

	void close();

	bool update();


private:

	void initWindow();

	void closeWindow();

	void updateWindow();

	void initSurface();

	bool keepWindow = true;
};

