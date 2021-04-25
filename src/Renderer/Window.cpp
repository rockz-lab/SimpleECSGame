#include "Window.h"

#include <cassert>

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stdout, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

Window::Window(int width, int height, const std::string& title)
{
	glewExperimental = GL_TRUE;
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#ifdef DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE)

#endif // DEBUG

	m_height = height;
	m_width = width;
	
	m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	assert(m_window != nullptr, "window creation failed!");
	glfwMakeContextCurrent(m_window);

	GLint GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult)
	{
		printf("ERROR: %s", glewGetErrorString(GlewInitResult));
		glfwTerminate();
			exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);
	

	if (_DEBUG)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);
	}

}


void Window::Update()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void Window::Clear()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Window::operator bool()
{
	if (!glfwWindowShouldClose(m_window))
	{
		Clear();
		return true;
	}
	else
		return false;
}
