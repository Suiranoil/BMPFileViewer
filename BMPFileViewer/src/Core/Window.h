#pragma once

#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Window
{
private:
	static int s_WindowCount;

	GLFWwindow* m_Window;
	int m_Width, m_Height;

	std::string m_Title;

public:
	Window(std::string_view title, int width, int height);
	Window(const Window&) = delete;
	~Window();

	Window& operator=(const Window&) = delete;

	static void PollEvents();
	static void WaitEvents();

	void SwapBuffers() const;

	[[nodiscard]] bool ShouldClose() const { return glfwWindowShouldClose(m_Window); }
	void SetShouldClose(bool value) { glfwSetWindowShouldClose(m_Window, value); }

	[[nodiscard]] int GetWidth() const { return m_Width; }
	[[nodiscard]] int GetHeight() const { return m_Height; }
	[[nodiscard]] void* GetNativeWindow() const { return m_Window; }
private:
	static void WindowResizeCallback(GLFWwindow* window, int width, int height);
};
