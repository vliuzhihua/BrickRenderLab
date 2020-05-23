#include <iostream>
#include <stdlib.h>
#include <stdio.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include "glew.h"
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "Camera.h"

Renderer renderer;
Camera camera;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	const float dis = 0.5;
	switch (key)
	{
	case GLFW_KEY_W:
		{
		camera.Move<Camera::FORWARD>(dis);
		break;
		}
	case GLFW_KEY_D:
		{
		camera.Move<Camera::RIGHT>(dis);
		break;
		}
	case GLFW_KEY_A:
		{
		camera.Move<Camera::RIGHT>(-dis);
		break;
		}
	case GLFW_KEY_S:
		{
		camera.Move<Camera::FORWARD>(-dis);
		break;
		}
	case GLFW_KEY_Q:
		{
		camera.Move<Camera::UP>(-dis);
		break;
		}
	case GLFW_KEY_E:
		{
		camera.Move<Camera::UP>(dis);
		break;
		}
	}
}

double last_curx, last_cury;
bool can_ratate = false;

static void cursor_callback(GLFWwindow* window, double x, double y)
{
	//std::cout << x << " " << y << std::endl;
	const float scale = 0.01;
	if(can_ratate)
		camera.Rotate({ (float)-(y - last_cury) * scale, -(float)(x - last_curx) * scale, 0 });
	last_curx = x;
	last_cury = y;
}

static void mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
{
	switch(button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		{
		if (action == GLFW_PRESS)
			can_ratate = true;
		else
			can_ratate = false;
		break;
		}
	}
}

int main()
{
	const int w = 640;
	const int h = 480;
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(w, h, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mousebutton_callback);
	glfwSetCursorPosCallback(window, cursor_callback);

	//glew must be initialized, ortherwise there will be some error when use some opengl function,such as glGenTexture
	glewInit();

	last_curx = w * 0.5;
	last_cury = h * 0.5;

	renderer.PrepareRender();
	
	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		renderer.OnWindowSizeChange(width, height);
		renderer.Render(camera);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}