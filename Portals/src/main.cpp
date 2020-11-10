
// previous opengl tutorial files:
// D:\cpp_codes\22_OpenGL_Tutorials\01_GettingStarted\OpenGlGettingStarted


#include <iostream>

// GLAD
#include <glad/glad.h>

// GLFW
#include <glfw3.h>

#include "window_context.h"
#include "renderer/buffer.h"
#include "renderer/shader.h"
#include "renderer/coloured_mesh.h"
#include "renderer/textured_mesh.h"
#include "renderer/textured_shaded_mesh.h"
#include "renderer/skybox.h"
#include "renderer/framebuffer.h"
#include "renderer/scene.h"

#include "controls/timestep.h"
#include "utils/shape_creator.h"
#include "glm/gtc/matrix_transform.hpp"
#include "vendor/stb_image/stb_image.h"


const int windowWidth = 1280;
const int windowHeight = 720;

// typedef void(*AccelerationFunction)(const std::vector<Body>& bodies, std::vector<Vec3D> accelerations);
void SimpleAcceleration(std::vector<Body>& bodies, std::vector<Vec3D>& accelerations)
{
	for (int i = 0; i < bodies.size(); i++)
	{
//		accelerations[i] = { 0,0,0.5 };
		accelerations[i] = CrossProduct({ 0,-0.1,0 }, bodies[i].velocity);
	}
}

int main()
{
	Mat_3D m1({ 0,1,2 }, { 3,4,5 }, { 6,7,8 });
	Mat_3D m2({ 0,10,20 }, { 30,40,50 }, { 60,70,80 });
	std::cout << m1;
	std::cout << m2;
	std::cout << m1*m2;
	return 0;





	MyWindow appWindow(windowWidth, windowHeight, "Portals");
//	appWindow.SetKeyCallback(key_callback);
	
	Scene myScene("assets/01_SceneDefinition.txt");
	myScene.SetAspectRatio((float)windowWidth / (float)windowHeight);

	Observer observer;

	float time = (float)glfwGetTime();
	Timestep timestep = 0.0f; // timestep can be initialized like this, because its constructor takes in only one float, implicit cast is possible
	float lastFrameTime = 0.0f;
	
	// Game loop
	while (!glfwWindowShouldClose(appWindow.GetWindow()))
	{
		time = (float)glfwGetTime();
		timestep = time - lastFrameTime;
		lastFrameTime = time;

		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		appWindow.HandleUserInputs(observer, timestep);

		// Update the scene
//		myScene.Update(timestep); // working
		myScene.Update(timestep, SimpleAcceleration); // working as well

		// Draw the scene into the default framebuffer
		myScene.Draw(observer);

		// Swap the screen buffers
		glfwSwapBuffers(appWindow.GetWindow());
	}

	// Terminates GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}


