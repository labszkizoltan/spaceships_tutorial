

#include <iostream>
#include <algorithm> // needed for std::max

// GLAD
#include <glad/glad.h>

// GLFW
#include <glfw3.h>

#include "window_context.h"


MyWindow::MyWindow(int width, int height, std::string name)
{
	m_Width = (float)width;
	m_Height = (float)height;

	std::cout << "Starting GLFW context, OpenGL 4.5" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_SAMPLES, 4);
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

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_MULTISAMPLE);

	glEnable(GL_PROGRAM_POINT_SIZE); // needed for the marker points
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

// GLFWAPI GLFWscrollfun glfwSetScrollCallback(GLFWwindow* window, GLFWscrollfun callback);
// void function_name(GLFWwindow* window, double xoffset, double yoffset)
void MyWindow::SetMouseScrollCallback(GLFWcursorposfun callback)
{
	glfwSetScrollCallback(m_Window, callback);
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

std::pair<float, float> MyWindow::GetMousePositionFromCenter()
{
	double xpos, ypos;
	glfwGetCursorPos(m_Window, &xpos, &ypos);
	return std::pair<float, float>((float)xpos-m_Width/2.0f, (float)ypos-m_Height/2.0f);
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
	if (IsKeyPressed(GLFW_KEY_M)) { obs.MoveRight(timestep * obsMoveSpeed); } // does the same as D, so I can make a video with  my phone when moving and rotating at the same time
	if (IsKeyPressed(GLFW_KEY_A)) { obs.MoveLeft(timestep * obsMoveSpeed); }

	if (IsKeyPressed(GLFW_KEY_E)) { obs.TurnClockwise(timestep * obsTurnRate); }
	if (IsKeyPressed(GLFW_KEY_Q)) { obs.TurnAntiClockwise(timestep * obsTurnRate); }
	if (IsKeyPressed(GLFW_KEY_LEFT)) { obs.TurnLeft(timestep * obsTurnRate); }
	if (IsKeyPressed(GLFW_KEY_RIGHT)) { obs.TurnRight(timestep * obsTurnRate); }
	if (IsKeyPressed(GLFW_KEY_UP)) { obs.TurnUp(timestep * obsTurnRate); }
	if (IsKeyPressed(GLFW_KEY_DOWN)) { obs.TurnDown(timestep * obsTurnRate); }

	if (IsKeyPressed(GLFW_KEY_P)) { obs.ZoomIn(1.05f); }
	if (IsKeyPressed(GLFW_KEY_O)) { obs.ZoomOut(1.05f); }
}

void MyWindow::HandlePlayerInputs(Player& player, Timestep timestep)
{
	glfwPollEvents();
	static float bodyMoveSpeed = 50.0f, accelerationRate = 10.0f;
	float obsTurnRate = 2.5f / player.m_Observer.zoom_level;

	if (IsKeyPressed(GLFW_KEY_I)) { player.m_BodyPtr->MoveForward(timestep * bodyMoveSpeed); }
	if (IsKeyPressed(GLFW_KEY_K)) { player.m_BodyPtr->MoveBackward(timestep * bodyMoveSpeed); }
//	if (IsKeyPressed(GLFW_KEY_R)) { player.m_BodyPtr->MoveUp(timestep * bodyMoveSpeed); }
//	if (IsKeyPressed(GLFW_KEY_F)) { player.m_BodyPtr->MoveDown(timestep * bodyMoveSpeed); }
	if (IsKeyPressed(GLFW_KEY_L)) { player.m_BodyPtr->MoveRight(timestep * bodyMoveSpeed); }
	if (IsKeyPressed(GLFW_KEY_J)) { player.m_BodyPtr->MoveLeft(timestep * bodyMoveSpeed); }

	if (IsKeyPressed(GLFW_KEY_W)) { player.m_BodyPtr->AccelerateDir( timestep * player.m_BodyPtr->orientation.f3); }
	if (IsKeyPressed(GLFW_KEY_S)) { player.m_BodyPtr->AccelerateDir(-timestep * player.m_BodyPtr->orientation.f3); }
	if (IsKeyPressed(GLFW_KEY_A)) { player.m_BodyPtr->AccelerateDir(-timestep * player.m_BodyPtr->orientation.f1); }
	if (IsKeyPressed(GLFW_KEY_D)) { player.m_BodyPtr->AccelerateDir( timestep * player.m_BodyPtr->orientation.f1); }
	if (IsKeyPressed(GLFW_KEY_R)) { player.m_BodyPtr->AccelerateDir( timestep * player.m_BodyPtr->orientation.f2); }
	if (IsKeyPressed(GLFW_KEY_F)) { player.m_BodyPtr->AccelerateDir(-timestep * player.m_BodyPtr->orientation.f2); }

	

	if (IsKeyPressed(GLFW_KEY_B)) { player.m_BodyPtr->Stop(); }

	if (IsKeyPressed(GLFW_KEY_KP_ADD)) { bodyMoveSpeed *= 1.05; }
	if (IsKeyPressed(GLFW_KEY_KP_SUBTRACT)) { bodyMoveSpeed /= 1.05; }

	if (IsKeyPressed(GLFW_KEY_E)) { player.m_BodyPtr->TurnClockwise(timestep * obsTurnRate); }
	if (IsKeyPressed(GLFW_KEY_Q)) { player.m_BodyPtr->TurnAntiClockwise(timestep * obsTurnRate); }
	if (IsKeyPressed(GLFW_KEY_LEFT)) { player.m_BodyPtr->TurnLeft(timestep * obsTurnRate); }
	if (IsKeyPressed(GLFW_KEY_RIGHT)) { player.m_BodyPtr->TurnRight(timestep * obsTurnRate); }
	if (IsKeyPressed(GLFW_KEY_UP)) { player.m_BodyPtr->TurnUp(timestep * obsTurnRate); }
	if (IsKeyPressed(GLFW_KEY_DOWN)) { player.m_BodyPtr->TurnDown(timestep * obsTurnRate); }

	if (IsKeyPressed(GLFW_KEY_P)) { player.m_Observer.ZoomIn(1.05f); }
	if (IsKeyPressed(GLFW_KEY_O)) { player.m_Observer.ZoomOut(1.05f); }

	static std::pair<float, float> mousePos;
	static float radiusFromCenter, r_min = 200.0f;

	mousePos = GetMousePositionFromCenter();
	radiusFromCenter = sqrt(mousePos.first*mousePos.first + mousePos.second*mousePos.second);
//	player.m_BodyPtr->Turn(Vec3D(-mousePos.second, mousePos.first, 0.0f), 0.0001f * std::max(0.0f, radiusFromCenter - r_min) / player.m_Observer.zoom_level);
	player.m_BodyPtr->Turn(mousePos.first*player.m_BodyPtr->orientation.f2+mousePos.second*player.m_BodyPtr->orientation.f1, 0.0001f * std::max(0.0f, radiusFromCenter - r_min) / player.m_Observer.zoom_level);
}

void MyWindow::SetUserPointer(void * userPtr)
{
	glfwSetWindowUserPointer(m_Window, userPtr);
}

void * MyWindow::GetUserPointer(void * userPtr)
{
	return glfwGetWindowUserPointer(m_Window);
}







