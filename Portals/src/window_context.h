#pragma once

//#include <iostream>

// GLAD
//#include <glad/glad.h>

// GLFW
#include <glfw3.h>


class MyWindow
{
public:
	MyWindow(int width, int height, std::string name = "DefaultName");

	void SetKeyCallback(GLFWkeyfun callback);
	void SetMouseButtonCallback(GLFWmousebuttonfun callback);
	void SetCursorPositionCallback(GLFWcursorposfun callback);

	bool IsKeyPressed(int keycode);
	bool IsMouseButtonPressed(int button);
	std::pair<float, float> GetMousePosition();
//	float GetMouseX();
//	float GetMouseY();

	void HandleUserInputs();

	GLFWwindow* GetWindow() { return m_Window; }

private:
	GLFWwindow* m_Window;

};



