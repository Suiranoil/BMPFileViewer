#include "BatchEditWindow.h"

#include "Core/Application.h"
#include "Input/FileDialog.h"

#include <imgui.h>

BatchEditWindow::BatchEditWindow(const Application& application)
	: m_Application(application)
{
}

void BatchEditWindow::Render()
{
	if (!m_IsOpen)
		return;

	ImGui::SetNextWindowSize(ImVec2(m_Application.GetWindow().GetWidth() / 2.0f, m_Application.GetWindow().GetHeight() / 2.0f), ImGuiCond_Appearing);
	ImGui::Begin("Batch Edit", &m_IsOpen);

	ImGui::BeginChild("Images", ImVec2(ImGui::GetWindowWidth() * 0.5f, 0), true);

	for (const auto& image : m_Images)
	{
		ImGui::TextUnformatted(image.filename().string().c_str());
	}

	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("Settings", ImVec2(0, 0), true);

	ImGui::TextUnformatted("Input Folder");
	ImGui::InputText("##Folder", m_FolderPath.string().data(), m_FolderPath.string().size(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("...##InputFolderButton"))
	{
		const auto path = FileDialog::OpenFolderDialog(m_Application.GetWindow().GetNativeWindow());
		if (!path.empty())
		{
			m_FolderPath = path;
			m_Images.clear();
			for (const auto& entry : std::filesystem::directory_iterator(m_FolderPath))
			{
				if (entry.is_regular_file())
				{
					if (entry.path().extension() == ".bmp")
					{
						m_Images.push_back(entry.path());
					}
				}
			}
		}
	}

	if (ImGui::Button("Load preset"))
	{
		const auto path = FileDialog::OpenFileDialog({{L"TOML Files (*.toml)", L"*.toml"}}, m_Application.GetWindow().GetNativeWindow());

		if (!path.empty())
		{
			toml::parse_result result = toml::parse_file(path.string());

			if (result)
				m_FilterStack.Deserialize(result);
		}
	}

	ImGui::TextUnformatted("Output Folder");
	ImGui::InputText("##OutputFolder", m_OutputFolderPath.string().data(), m_OutputFolderPath.string().size(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("...##OutputFolderButton"))
	{
		const auto path = FileDialog::OpenFolderDialog(m_Application.GetWindow().GetNativeWindow());
		if (!path.empty())
			m_OutputFolderPath = path;
	}

	if (ImGui::Button("Process"))
	{
		for (const auto& image : m_Images)
		{
			Image img(image.string());
			m_FilterStack.Apply(img);
			img.Save((m_OutputFolderPath / image.filename()).string());
		}
	}

	ImGui::EndChild();

	ImGui::End();
}

void BatchEditWindow::Reset()
{
	m_FolderPath.clear();
	m_OutputFolderPath.clear();
	m_Images.clear();
	m_FilterStack.ClearFilters();
}
