#pragma once

#include "ImageTab.h"
#include "Core/Window.h"
#include "BatchEditWindow.h"

#include <memory>
#include <vector>

class Application
{
private:
	bool m_isRunning = true;

	int m_SelectedTab = 0;
	BatchEditWindow m_BatchEditWindow;

	std::unique_ptr<Window> m_Window;
	std::vector<std::unique_ptr<ImageTab>> m_ImageTabs;

public:
	Application();
	~Application();

	void Run();

	void Update();
	void Render();

	const Window& GetWindow() const { return *m_Window; }
};
