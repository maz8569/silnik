#include "Rendering/WindowManager.h"

GameEngine::WindowManager::WindowManager()
{
	//createWindow();
}

GameEngine::WindowManager::~WindowManager()
{
	closeWindow();
}

int GameEngine::WindowManager::createWindow()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //blockResize();

    // glfw window creation
    // --------------------
    
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, name.c_str(), NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "Create Window\n";

    return 0;
}

void GameEngine::WindowManager::updateWindow()
{
    glfwPollEvents();
    glfwSwapBuffers(window);
}

void GameEngine::WindowManager::closeWindow()
{
    glfwTerminate();

}

void GameEngine::WindowManager::blockCursor()
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void GameEngine::WindowManager::freeCursor()
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void GameEngine::WindowManager::freeResize()
{
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
}

void GameEngine::WindowManager::blockResize()
{
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}
