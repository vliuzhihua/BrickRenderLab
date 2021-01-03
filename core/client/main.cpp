#include <Windows.h>

#include <iostream>
//#include <stdlib.h>
//#include <stdio.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "Camera.h"

#include "fstream"
#include "string"

//#include "CImg/CImg.h"
#include "FreeImage.h"
#include <fileapi.h>
//#include <WinBase.h>
//#include <cstringt.h>
#include "Shader.h"

using namespace brick;

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
	const float dis = 0.2f;
	switch (key)
	{
	case GLFW_KEY_W:
		{
		camera.Move<Camera::FORWARD>(dis);
		break;
		}
	case GLFW_KEY_S:
		{
		camera.Move<Camera::FORWARD>(-dis);
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
	case GLFW_KEY_E:
		{
		camera.Move<Camera::UP>(dis);
		break;
		}
	case GLFW_KEY_Q:
		{
		camera.Move<Camera::UP>(-dis);
		break;
		}
	}
}

double last_curx, last_cury;
bool can_ratate = false;

static void cursor_callback(GLFWwindow* window, double x, double y)
{
	//std::cout << x << " " << y << std::endl;
	const float scale = 0.005;
	if (can_ratate) {
		//camera.SetEulerRadian(camera.GetEulerRadian() + math::Vector3f{ -(float)(y - last_cury) * scale, -(float)(x - last_curx) * scale, 0.f });
		camera.Rotate( { -(float)(y - last_cury) * scale, -(float)(x - last_curx) * scale, 0.f } );
	}
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

std::string wchar2char(const wchar_t* wchar)
{
	char m_char[512];
	int len = WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}

LPCWSTR WatchDirStr = L".\\";
HANDLE DirectoryHandler;

void ProcessDirectoryWatch() {
	char notify[1024];
	memset(notify, 0, sizeof(notify));
	FILE_NOTIFY_INFORMATION* pNotification = (FILE_NOTIFY_INFORMATION*)notify;
	DWORD BytesReturned = 0;

	if (DirectoryHandler == INVALID_HANDLE_VALUE)
		return;

	while (TRUE)
	{
		ZeroMemory(pNotification, sizeof(notify));

		bool watch_state = ReadDirectoryChangesW(DirectoryHandler,
			&notify,
			sizeof(notify),
			TRUE,	//监控子目录
			FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE, //FILE_NOTIFY_CHANGE_DIR_NAME FILE_NOTIFY_CHANGE_CREATION FILE_NOTIFY_CHANGE_SIZE
			(LPDWORD)&BytesReturned,
			NULL,
			NULL);

		if (GetLastError() == ERROR_INVALID_FUNCTION)
		{
			std::cout << "文件监控，系统不支持!" << std::endl;
			break;
		}
		else if (watch_state == FALSE)
		{
			DWORD dwErr = GetLastError();
			std::cout << "文件监控，监控失败!" << std::endl;;
			break;
		}
		else if (GetLastError() == ERROR_NOTIFY_ENUM_DIR)
		{
			std::cout << "文件监控，内存溢出" << std::endl;
			continue;
		}
		else
		{
			//这里主要就是检测返回的信息，(FILE_NOTIFY_INFORMATION)
			//CString szFileName(pNotification->FileName, pNotification->FileNameLength / sizeof(wchar_t));
			if (pNotification->Action == FILE_ACTION_ADDED)
			{
				std::cout << "文件监控，新增文件!" << wchar2char(pNotification->FileName) << std::endl;
			}
			else if (pNotification->Action == FILE_ACTION_REMOVED)
			{
				std::cout << "文件监控，删除文件!" << wchar2char(pNotification->FileName) << std::endl;
			}
			else if (pNotification->Action == FILE_ACTION_MODIFIED)
			{
				std::cout << "文件监控，修改文件!" << wchar2char(pNotification->FileName) << std::endl;
			}
			else if (pNotification->Action == FILE_ACTION_RENAMED_OLD_NAME)
			{
				std::cout << "文件监控，重命名文件!" << wchar2char(pNotification->FileName) << std::endl;
			}
			else if (pNotification->Action == FILE_ACTION_RENAMED_NEW_NAME) //还没出现过这种情况
			{
				std::cout << "文件监控，重命名文件2!" << wchar2char(pNotification->FileName) << std::endl;
			}

			FShaderManager::Instance().SetNeedRecompileAll(true);
			//PostMessage通知主线程
		}
	}
}

DWORD WINAPI WatchThreadFunc(LPVOID p)
{
	ProcessDirectoryWatch();
	//printf("我是子线程， pid = %d\n", GetCurrentThreadId());   //输出子线程pid
	return 0;
}

void InitDirectoryWatch() {
	DirectoryHandler = CreateFile(WatchDirStr,
		GENERIC_READ | GENERIC_WRITE | FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
		NULL);
	if (DirectoryHandler == INVALID_HANDLE_VALUE)
	{
		DWORD dwErr = GetLastError();
		std::cout << "Watcher Init Error" << std::endl;
		return;
	}
	HANDLE hThread;
	DWORD  threadId;
	hThread = CreateThread(NULL, 0, WatchThreadFunc, 0, 0, &threadId); // 创建线程
}


int main()
{
	const int w = 1366;
	const int h = 768;
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	last_curx = w * 0.5;
	last_cury = h * 0.5;

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//cimg_library::CImg<unsigned char> Img;
	//Img.load_bmp("res\\models\\brick_rough_ue4ifa0va\\ue4ifa0va_4K_Albedo.bmp");
	//Img.load_bmp("D:\\1_BrickGE\\project_space\\res\\models\\brick_rough_ue4ifa0va\\ue4ifa0va_4K_Albedo.bmp");
	//Img.load_jpeg("D:\\1_BrickGE\\project_space\\res\\models\\brick_rough_ue4ifa0va\\ue4ifa0va_4K_Albedo.jpg");
	//Img.display();

	InitDirectoryWatch();
	FShaderManager::Instance().InitDefaultShader("shaders\\default\\default.vs", "shaders\\default\\default.ps");

	renderer.PrepareRender();

	camera.SetPosition({ 0.0f, 0.0f, -1.0f });
	
	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		
		FShaderManager::Instance().Update();
		processInput(window);

		renderer.OnWindowSizeChange(width, height);
		renderer.Render(camera);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);



}