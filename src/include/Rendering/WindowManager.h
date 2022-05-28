#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

// manages window
namespace GameEngine {
	class WindowManager
	{
	public:

		unsigned int SCR_WIDTH = 1920;
		unsigned int SCR_HEIGHT = 980;
		GLFWwindow* window = nullptr;

		//name of the game
		std::string name = "Package Rush";

		WindowManager();
		~WindowManager();

		int createWindow();
		void updateWindow();
		void closeWindow();
		void blockCursor();
		void freeCursor();

		void freeResize();
		void blockResize();
	};
}

