

#include <iostream>

// GLAD
#include <glad/glad.h>

// GLFW
#include <glfw3.h>

#include "window_context.h"


MyWindow::MyWindow(int width, int height, std::string name)
{

	std::cout << "Starting GLFW context, OpenGL 4.5" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	m_Window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	glfwMakeContextCurrent(m_Window);
	if (m_Window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return;
	}

	// Define the viewport dimensions
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


// GLFWAPI GLFWkeyfun glfwSetKeyCallback(GLFWwindow* window, GLFWkeyfun callback);
// typedef void(*GLFWkeyfun)(GLFWwindow*, int, int, int, int);
void MyWindow::SetKeyCallback(GLFWkeyfun callback)
{
	glfwSetKeyCallback(m_Window, callback);
}

// // Is called whenever a key is pressed/released via GLFW
// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
// {
// 	std::cout << key << std::endl;
// 	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
// 		glfwSetWindowShouldClose(window, GL_TRUE);
// }




// GLFWAPI GLFWmousebuttonfun glfwSetMouseButtonCallback(GLFWwindow* window, GLFWmousebuttonfun callback);
// typedef void(*GLFWmousebuttonfun)(GLFWwindow*, int, int, int);
void MyWindow::SetMouseButtonCallback(GLFWmousebuttonfun callback)
{
	glfwSetMouseButtonCallback(m_Window, callback);
}


// GLFWAPI GLFWcursorposfun glfwSetCursorPosCallback(GLFWwindow* window, GLFWcursorposfun callback);
// typedef void(*GLFWcursorposfun)(GLFWwindow*, double, double);
void MyWindow::SetCursorPositionCallback(GLFWcursorposfun callback)
{
	glfwSetCursorPosCallback(m_Window, callback);
}

bool MyWindow::IsKeyPressed(int keycode)
{
	int state = glfwGetKey(m_Window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool MyWindow::IsMouseButtonPressed(int button)
{
	int state = glfwGetMouseButton(m_Window, button);
	return state == GLFW_PRESS;
}

std::pair<float, float> MyWindow::GetMousePosition()
{
	double xpos, ypos;
	glfwGetCursorPos(m_Window, &xpos, &ypos);
	return std::pair<float, float>((float)xpos, (float)ypos);
}

// 
// float MyWindow::GetMouseX()
// {
// 	std::pair<float, float> [x, y] = GetMousePosition();
// 	return x;
// }
// 
// inline float MyWindow::GetMouseY()
// {
// 	std::pair<float, float> [x, y] = GetMousePosition();
// 	return y;
// }
// 

void MyWindow::HandleUserInputs(Observer& obs, Timestep timestep)
{
	// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
	glfwPollEvents();

	float obsTurnRate = 2.5f/obs.zoom_level, obsMoveSpeed = 15.0f;

	if (IsKeyPressed(GLFW_KEY_W)) { obs.MoveForward(timestep * obsMoveSpeed); }
	if (IsKeyPressed(GLFW_KEY_S)) { obs.MoveBackward(timestep * obsMoveSpeed); }
	if (IsKeyPressed(GLFW_KEY_R)) { obs.MoveUp(timestep * obsMoveSpeed); }
	if (IsKeyPressed(GLFW_KEY_F)) { obs.MoveDown(timestep * obsMoveSpeed); }
	if (IsKeyPressed(GLFW_KEY_D)) { obs.MoveRight(timestep * obsMoveSpeed); }
	if (IsKeyPressed(GLFW_KEY_A)) { obs.MoveLeft(timestep * obsMoveSpeed); }

	if (IsKeyPressed(GLFW_KEY_E)) { obs.TurnClockwise(timestep * obsTurnRate); }
	if (IsKeyPressed(GLFW_KEY_Q)) { obs.TurnAntiClockwise(timestep * obsTurnRate); }
	if (IsKeyPressed(GLFW_KEY_RIGHT)) { obs.TurnLeft(timestep * obsTurnRate); }
	if (IsKeyPressed(GLFW_KEY_LEFT)) { obs.TurnRight(timestep * obsTurnRate); }
	if (IsKeyPressed(GLFW_KEY_DOWN)) { obs.TurnUp(timestep * obsTurnRate); }
	if (IsKeyPressed(GLFW_KEY_UP)) { obs.TurnDown(timestep * obsTurnRate); }

	if (IsKeyPressed(GLFW_KEY_P)) { obs.ZoomIn(1.05f); }
	if (IsKeyPressed(GLFW_KEY_O)) { obs.ZoomOut(1.05f); }

}







