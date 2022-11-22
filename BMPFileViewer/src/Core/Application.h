#pragma once

#include "ImageTab.h"
#include "Core/Window.h"

#include <memory>
#include <vector>

class Application
{
private:
	bool m_isRunning = true;

	int m_SelectedTab = 0;

	std::unique_ptr<Window> m_Window;
	std::vector<std::unique_ptr<ImageTab>> m_ImageTabs;

public:
	Application();
	~Application();

	void Run();

	void Update();
	void Render();
};
