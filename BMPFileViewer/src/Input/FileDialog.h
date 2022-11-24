#pragma once

#include <filesystem>

class FileDialog
{
public:
	FileDialog() = delete;

	static std::filesystem::path OpenFileDialog(std::wstring_view filter, void* nativeWindow);
	static std::filesystem::path OpenFolderDialog(void* nativeWindow);
	static std::filesystem::path SaveFileDialog(std::wstring_view filter, std::wstring_view defaultExt, void* nativeWindow);
};

