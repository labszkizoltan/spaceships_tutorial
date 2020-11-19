
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

#include "acceleration_functions.h"

// Is called whenever a key is pressed/released via GLFW
// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
// {
// 	std::cout << key << std::endl;
// 	if (key == GLFW_KEY_N && action == GLFW_PRESS)
// 	{
// 		glfwGetWindowUserPointer();
// 	}
// }


// GLFWAPI GLFWscrollfun glfwSetScrollCallback(GLFWwindow* window, GLFWscrollfun callback);
// void function_name(GLFWwindow* window, double xoffset, double yoffset)
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Player* playerPtr = (Player*)glfwGetWindowUserPointer(window);
//	playerPtr->m_Observer.zoom_level *= playerPtr->m_Observer.zoom_level > 500.0f ? 1.0f : (1.0f+0.1*(float)yoffset);
	if (yoffset > 0) { playerPtr->m_Observer.ZoomIn(1.1f); }
	else if (yoffset < 0) { playerPtr->m_Observer.ZoomOut(1.1f); }
}


int main()
{
	MyWindow appWindow(windowWidth, windowHeight, "Portals");
	appWindow.SetMouseScrollCallback(mouse_scroll_callback);

	Scene myScene("assets/scene_definitions/03_SceneDefinition_test_orbits.txt");
	myScene.SetAspectRatio((float)windowWidth / (float)windowHeight);

	Observer observer;

	static int body_index = 0;
	float body_switch_timer = 0.0f;
	float shoot_timer = 0.0f;

	Player player;
	player.SetBodyPtr(myScene.GetBodyPtr(0));
	player.Synchronize();

	appWindow.SetUserPointer(&player);

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

		if (appWindow.IsKeyPressed(GLFW_KEY_SPACE) && shoot_timer > 0.5f) { myScene.OnShoot(player.m_BodyPtr); myScene.OnShoot(player.m_BodyPtr + 8 * sizeof(Body)); shoot_timer = 0.0f; }
		if (appWindow.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1) && shoot_timer > 0.5f) { myScene.OnShoot(player.m_BodyPtr); shoot_timer = 0.0f; }
//		if (appWindow.IsKeyPressed(GLFW_KEY_SPACE) && shoot_timer > 0.5f) { myScene.OnShoot(player.m_BodyPtr); shoot_timer = 0.0f; }
		shoot_timer += timestep;

		// Update the scene
//		myScene.UpdateWithCollision(timestep, SimpleAcceleration); // working as well
//		myScene.UpdateWithCollision(timestep, GravityAcceleration); // working as well
		myScene.UpdateWithCollision(timestep, SimplifiedGravity); // working as well

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


