#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <memory>




class Window
{
public:
	Window(int width, int height, const std::string& title);

	void Update();
	void Draw();
	
	void Clear();
	operator bool();

	void EventLoopThread();
	
	void SetUserPointer(void* object)
	{
		glfwSetWindowUserPointer(m_window, object);
	}

	void SetWindowPosCallback(GLFWwindowposfun function);
private:
	int m_width;
	int m_height;

	GLFWwindow* m_window;
	
};