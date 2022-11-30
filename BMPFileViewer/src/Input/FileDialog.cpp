#include "FileDialog.h"

#include <Windows.h>
#include <ShObjIdl.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

std::vector<COMDLG_FILTERSPEC> FileDialogFilterToCOMDLG_FILTERSPEC(const std::vector<FileDialog::FileDialogFilter>& filters)
{
	std::vector<COMDLG_FILTERSPEC> result;
	result.reserve(filters.size());

	for (const auto& [name, filter] : filters)
	{
		COMDLG_FILTERSPEC spec;
		spec.pszName = name.data();
		spec.pszSpec = filter.data();
		result.push_back(spec);
	}

	return result;
}

std::filesystem::path FileDialog::OpenFileDialog(const std::vector<FileDialogFilter>& filters, void* nativeWindow)
{
	IFileDialog* dialog;
	HRESULT result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog));

	if (result == S_OK)
	{
		DWORD flags;
		dialog->GetOptions(&flags);
		dialog->SetOptions(flags | FOS_FORCEFILESYSTEM | FOS_NOCHANGEDIR | FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST);

		if (!filters.empty())
		{
			dialog->SetFileTypes(filters.size(), FileDialogFilterToCOMDLG_FILTERSPEC(filters).data());
		}

		result = dialog->Show(glfwGetWin32Window((GLFWwindow*)nativeWindow));

		if (result == S_OK)
		{
			IShellItem* item;
			result = dialog->GetResult(&item);

			if (result == S_OK)
			{
				PWSTR path;
				item->GetDisplayName(SIGDN_FILESYSPATH, &path);

				std::filesystem::path filePath = path;
				CoTaskMemFree(path);
				item->Release();

				return filePath;
			}
		}

		dialog->Release();
	}

	return std::filesystem::path();
}

std::filesystem::path FileDialog::OpenFolderDialog(void* nativeWindow)
{
	IFileDialog* dialog;
	HRESULT result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog));

	if (result == S_OK)
	{
		DWORD flags;
		dialog->GetOptions(&flags);
		dialog->SetOptions(flags | FOS_FORCEFILESYSTEM | FOS_NOCHANGEDIR | FOS_PATHMUSTEXIST | FOS_PICKFOLDERS);

		result = dialog->Show(glfwGetWin32Window((GLFWwindow*)nativeWindow));

		if (result == S_OK)
		{
			IShellItem* item;
			result = dialog->GetResult(&item);

			if (result == S_OK)
			{
				PWSTR path;
				item->GetDisplayName(SIGDN_FILESYSPATH, &path);

				std::filesystem::path filePath = path;
				CoTaskMemFree(path);
				item->Release();

				return filePath;
			}
		}

		dialog->Release();
	}

	return std::filesystem::path();
}

std::filesystem::path FileDialog::SaveFileDialog(const std::vector<FileDialogFilter>& filters, void* nativeWindow)
{
	IFileDialog* dialog;
	HRESULT result = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog));

	if (result == S_OK)
	{
		DWORD flags;
		dialog->GetOptions(&flags);
		dialog->SetOptions(flags | FOS_FORCEFILESYSTEM | FOS_NOCHANGEDIR | FOS_PATHMUSTEXIST | FOS_OVERWRITEPROMPT);


		if (!filters.empty())
		{
			dialog->SetFileTypes(filters.size(), FileDialogFilterToCOMDLG_FILTERSPEC(filters).data());

			std::wstring_view defaultExtension = filters[0].Filter;
			defaultExtension.remove_prefix(defaultExtension.find(L'.'));
			dialog->SetDefaultExtension(defaultExtension.data());
		}

		result = dialog->Show(glfwGetWin32Window((GLFWwindow*)nativeWindow));

		if (result == S_OK)
		{
			IShellItem* item;
			result = dialog->GetResult(&item);

			if (result == S_OK)
			{
				PWSTR path;
				item->GetDisplayName(SIGDN_FILESYSPATH, &path);

				std::filesystem::path filePath = path;
				CoTaskMemFree(path);
				item->Release();

				return filePath;
			}
		}

		dialog->Release();
	}

	return std::filesystem::path();
}
