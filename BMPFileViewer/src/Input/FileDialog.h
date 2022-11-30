#pragma once

#include <filesystem>

class FileDialog
{
public:
	struct FileDialogFilter
	{
		std::wstring_view Name;
		std::wstring_view Filter;
	};

	FileDialog() = delete;

	static std::filesystem::path OpenFileDialog(const std::vector<FileDialogFilter>& filters, void* nativeWindow);
	static std::filesystem::path OpenFolderDialog(void* nativeWindow);
	static std::filesystem::path SaveFileDialog(const std::vector<FileDialogFilter>& filters, void* nativeWindow);
};

