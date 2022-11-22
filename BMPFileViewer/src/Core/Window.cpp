#include "Window.h"

#include <stdexcept>

#include <glad/glad.h>

int Window::s_WindowCount = 0;


void Window::WindowResizeCallback(GLFWwindow* window, int width, int height)
{
	auto wnd = (Window*)glfwGetWindowUserPointer(window);

	wnd->m_Width = width;
	wnd->m_Height = height;
}

Window::Window(std::string_view title, int width, int height)
	: m_Width(width), m_Height(height), m_Title(title)
{
	if (s_WindowCount == 0)
	{
		if (!glfwInit())
		{
			throw std::runtime_error("Failed to initialize GLFW!");
		}
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
	if (!m_Window)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create window!");
	}
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetWindowSizeCallback(m_Window, WindowResizeCallback);
	glfwSwapInterval(1);

	glfwMakeContextCurrent(m_Window);

	if (s_WindowCount == 0)
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw std::runtime_error("Failed to initialize GLAD!");
		}
	}
	
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	++s_WindowCount;
}

Window::~Window()
{
	glfwDestroyWindow(m_Window);

	--s_WindowCount;
	if (s_WindowCount == 0)
	{
		glfwTerminate();
	}
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::WaitEvents()
{
	glfwWaitEvents();
}

void Window::SwapBuffers() const
{
	glfwSwapBuffers(m_Window);
}
