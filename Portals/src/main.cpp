
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
#include "controls/player.h"
#include "utils/shape_creator.h"
#include "glm/gtc/matrix_transform.hpp"
#include "vendor/stb_image/stb_image.h"


const int windowWidth = 1800;
const int windowHeight = 1000;

// typedef void(*AccelerationFunction)(const std::vector<Body>& bodies, std::vector<Vec3D> accelerations);
void SimpleAcceleration(std::vector<Body>& bodies, std::vector<Vec3D>& accelerations, std::vector<Vec3D>& angular_accelerations)
{
	static float dragCoef = 0.1f;
	for (int i = 0; i < bodies.size(); i++)
	{
		accelerations[i] = CrossProduct({ 0,-0.5,0 }, bodies[i].velocity) - dragCoef * bodies[i].velocity; // +Vec3D((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
		angular_accelerations[i] = -0.1f*dragCoef*bodies[i].angularVelocity;
	}
}

// Gravity force
void GravityAcceleration(std::vector<Body>& bodies, std::vector<Vec3D>& accelerations, std::vector<Vec3D>& angular_accelerations)
{
	static float dragCoef = 0.01f;
	for (int i = 0; i < accelerations.size(); i++)
		accelerations[i] = { 0,0,0 };

	static float forceMinRange = 4.0f;
	static float forceStrenght = 0.1f;
	static float distanceSquare = 0.0f;
	static Vec3D forceDirection = { 0,0,0 };

	for (int i = 0; i < (bodies.size()-1); i++)
	{
		for (int j = i + 1; j < bodies.size(); j++)
		{
			distanceSquare = (bodies[i].location - bodies[j].location).lengthSquare();
			forceDirection = bodies[i].location - bodies[j].location;
			accelerations[i] -= distanceSquare < forceMinRange ? Vec3D{0, 0, 0} : forceStrenght * bodies[j].mass/distanceSquare * forceDirection;
			accelerations[j] += distanceSquare < forceMinRange ? Vec3D{0, 0, 0} : forceStrenght * bodies[i].mass/distanceSquare * forceDirection;
		}
	}

	for (int i = 0; i < bodies.size(); i++)
	{
		accelerations[i] -= dragCoef * bodies[i].velocity/bodies[i].mass; // +Vec3D((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
		angular_accelerations[i] = -0.1f*dragCoef*bodies[i].angularVelocity;
	}
}

void NoAcceleration(std::vector<Body>& bodies, std::vector<Vec3D>& accelerations, std::vector<Vec3D>& angular_accelerations)
{
}

// Is called whenever a key is pressed/released via GLFW
// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
// {
// 	std::cout << key << std::endl;
// 	if (key == GLFW_KEY_N && action == GLFW_PRESS)
// 	{
// 		glfwGetWindowUserPointer();
// 	}
// }

int main()
{
	MyWindow appWindow(windowWidth, windowHeight, "Portals");
//	appWindow.SetKeyCallback(key_callback);

	Scene myScene("assets/02_SceneDefinition_collision_test.txt");

	myScene.SetAspectRatio((float)windowWidth / (float)windowHeight);

	Observer observer;

	static int body_index = 0;
	float body_switch_timer = 0.0f;
	Player player;
	player.SetBodyPtr(myScene.GetBodyPtr(0));
	player.Synchronize();

	float time = (float)glfwGetTime();
	Timestep timestep = 0.0f; // timestep can be initialized like this, because its constructor takes in only one float, implicit cast is possible
	float lastFrameTime = 0.0f;
	
	// Game loop
	while (!glfwWindowShouldClose(appWindow.GetWindow()))
	{
		lastFrameTime = (float)glfwGetTime();

		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		appWindow.HandlePlayerInputs(player, timestep);
		player.Synchronize();

		if (appWindow.IsKeyPressed(GLFW_KEY_M) && body_switch_timer > 1.0f) { body_index++; player.SetBodyPtr(myScene.GetBodyPtr(body_index)); body_switch_timer = 0.0f; }
		if (appWindow.IsKeyPressed(GLFW_KEY_N) && body_switch_timer > 1.0f) { body_index--; player.SetBodyPtr(myScene.GetBodyPtr(body_index)); body_switch_timer = 0.0f; }
		body_switch_timer += timestep;

		// Update the scene
//		myScene.UpdateWithCollision(timestep, SimpleAcceleration); // working as well
		myScene.UpdateWithCollision(timestep, GravityAcceleration); // working as well
		
		// Draw the scene into the default framebuffer
//		myScene.Draw(player.m_Observer);
		myScene.Draw(player);

		// Swap the screen buffers
		glfwSwapBuffers(appWindow.GetWindow());


		timestep = (float)glfwGetTime() - lastFrameTime;
	}

	// Terminates GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}


