#include "Application.h"
#include "Input/FileDialog.h"

#include "Fonts/RobotoRegular.h"

#include <glad/glad.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

Application::Application()
{
	m_Window = std::make_unique<Window>("BMP File Viewer", 800, 600);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)m_Window->GetNativeWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");

	ImGui::GetIO().IniFilename = nullptr;
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
		GetRobotoRegularData(),
		GetRobotoRegularSize(),
		16.0f);
}

Application::~Application()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Application::Run()
{
	while (m_isRunning)
	{
		Update();
		Render();
	}
}

void Application::Update()
{
	Window::PollEvents();

	if (m_Window->ShouldClose())
		m_isRunning = false;

	if (m_Window->GetWidth() == 0 || m_Window->GetHeight() == 0)
		return;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImVec2 menuBarSize;
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open"))
			{
				const auto path = FileDialog::OpenFileDialog({{L"BMP Files (*.bmp)", L"*.bmp"}}, m_Window->GetNativeWindow());

				if (!path.empty())
				{
					bool isAlreadyOpen = false;
					for (const auto& tab : m_ImageTabs)
					{
						if (tab->GetName() == path.filename())
						{
							isAlreadyOpen = true;
							break;
						}
					}

					if (!isAlreadyOpen)
						m_ImageTabs.emplace_back(std::make_unique<ImageTab>(path.string()));
				}
			}
			if (ImGui::MenuItem("Save As..."))
			{
				const auto path = FileDialog::SaveFileDialog({{L"BMP Files (*.bmp)", L"*.bmp"}}, m_Window->GetNativeWindow());

				if (!m_ImageTabs.empty())
				{
					if (!path.empty())
						m_ImageTabs[m_SelectedTab]->SaveImage(path.string());
				}
			}
			if (ImGui::MenuItem("Exit"))
			{
				m_Window->SetShouldClose(true);
			}
				
			ImGui::EndMenu();
		}

		menuBarSize = ImGui::GetWindowSize();
		ImGui::EndMainMenuBar();
	}

	ImGui::SetNextWindowPos(ImVec2(0, menuBarSize.y));
	ImGui::SetNextWindowSize(ImVec2((float)m_Window->GetWidth(), (float)m_Window->GetHeight() - menuBarSize.y));
	ImGui::Begin("##BMPFileViewer", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);

	int tabToClose = -1;
	if (ImGui::BeginTabBar("##ImageTabs", ImGuiTabBarFlags_AutoSelectNewTabs))
	{
		for (size_t i = 0; i < m_ImageTabs.size(); i++)
		{
			bool isOpen = true;
			if (ImGui::BeginTabItem(m_ImageTabs[i]->GetName().c_str(), &isOpen))
			{
				m_SelectedTab = i;
				m_ImageTabs[i]->ImGuiRender();
				ImGui::EndTabItem();
			}

			if (!isOpen)
				tabToClose = i;
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
	
	ImGui::Render();

	if (tabToClose != -1)
	{
		m_ImageTabs.erase(m_ImageTabs.begin() + tabToClose);
	}
}

void Application::Render()
{
	glViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	m_Window->SwapBuffers();
}
