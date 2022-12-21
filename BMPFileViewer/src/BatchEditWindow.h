#pragma once
#include "Filter/FilterStack.h"

#include <filesystem>

class Application;

class BatchEditWindow
{
private:
	const Application& m_Application;

	bool m_IsOpen = false;

	FilterStack m_FilterStack;
	std::filesystem::path m_FolderPath;
	std::filesystem::path m_OutputFolderPath;

	std::vector<std::filesystem::path> m_Images;

public:
	explicit BatchEditWindow(const Application& application);
	~BatchEditWindow() = default;

	void Open() { m_IsOpen = true; }
	void Render();
	void Reset();
};
