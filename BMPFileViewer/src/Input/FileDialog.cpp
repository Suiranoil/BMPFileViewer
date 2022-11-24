#include "FileDialog.h"

#include <Windows.h>
#include <ShObjIdl.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

std::filesystem::path FileDialog::OpenFileDialog(std::wstring_view filter, void* nativeWindow)
{
	IFileDialog* dialog;
	HRESULT result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog));

	if (result == S_OK)
	{
		DWORD flags;
		dialog->GetOptions(&flags);
		dialog->SetOptions(flags | FOS_FORCEFILESYSTEM | FOS_NOCHANGEDIR | FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST);

		IShellItem* item;
		result = SHCreateItemFromParsingName(filter.data(), NULL, IID_PPV_ARGS(&item));

		if (result == S_OK)
		{
			dialog->SetFileTypes(1, (COMDLG_FILTERSPEC*)&item);
			item->Release();
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

std::filesystem::path FileDialog::SaveFileDialog(std::wstring_view filter, std::wstring_view defaultExt, void* nativeWindow)
{
	IFileDialog* dialog;
	HRESULT result = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog));

	if (result == S_OK)
	{
		DWORD flags;
		dialog->GetOptions(&flags);
		dialog->SetOptions(flags | FOS_FORCEFILESYSTEM | FOS_NOCHANGEDIR | FOS_PATHMUSTEXIST | FOS_OVERWRITEPROMPT);

		IShellItem* item;
		result = SHCreateItemFromParsingName(filter.data(), NULL, IID_PPV_ARGS(&item));

		if (result == S_OK)
		{
			dialog->SetFileTypes(1, (COMDLG_FILTERSPEC*)&item);
			item->Release();
		}

		dialog->SetDefaultExtension(defaultExt.data());

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
