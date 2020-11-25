
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
#include "renderer/helper_visuals.h"

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

void SetTimeSpeed(MyWindow& appWindow, float& timeSpeed)
{
	if (appWindow.IsKeyPressed(GLFW_KEY_0)) { timeSpeed=0.0f; }
	if (appWindow.IsKeyPressed(GLFW_KEY_1)) { timeSpeed=1.0f; }
	if (appWindow.IsKeyPressed(GLFW_KEY_2)) { timeSpeed=2.0f; }
	if (appWindow.IsKeyPressed(GLFW_KEY_3)) { timeSpeed=8.0f; }
}

int main()
{
	MyWindow appWindow(windowWidth, windowHeight, "Portals");
	glfwSetWindowPos(appWindow.GetWindow(), 50, 100);
	appWindow.SetMouseScrollCallback(mouse_scroll_callback);

//	Scene myScene("assets/scene_definitions/03_SceneDefinition_test_orbits.txt");
	Scene myScene("assets/scene_definitions/04_SceneDefinition_meteors.txt");

	myScene.SetAspectRatio((float)windowWidth / (float)windowHeight);
	
	static int body_index = 0;
	float body_switch_timer = 0.0f;
	float shoot_timer = 0.0f;

	Highlighter highlighter(ParseShader("src/renderer/shader_sources/vertex_shader_highlighter.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader_highlighter.glsl"));
	highlighter.SetAspectRatio((float)windowWidth / (float)windowHeight);

	Player player;
	player.SetBodyPtr(myScene.GetBodyPtr(0));
	player.Synchronize();

	appWindow.SetUserPointer(&player);

	float time = (float)glfwGetTime();
	float timeSpeed = 1.0f;
	Timestep timestep = 0.0f; // timestep can be initialized like this, because its constructor takes in only one float, implicit cast is possible
	float lastFrameTime = 0.0f;
	
	// Game loop
	while (!glfwWindowShouldClose(appWindow.GetWindow()))
	{
		lastFrameTime = (float)glfwGetTime();

		// Set the speed of the simulation, note that the quality of the update will be worse, as the timestep will be bigger
		SetTimeSpeed(appWindow, timeSpeed);

		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		appWindow.HandlePlayerInputs(player, timeSpeed*timestep);
		player.Synchronize();

		if (appWindow.IsKeyPressed(GLFW_KEY_M) && body_switch_timer > 1.0f) { body_index++; player.SetBodyPtr(myScene.GetBodyPtr(body_index)); body_switch_timer = 0.0f; }
		if (appWindow.IsKeyPressed(GLFW_KEY_N) && body_switch_timer > 1.0f) { body_index--; player.SetBodyPtr(myScene.GetBodyPtr(body_index)); body_switch_timer = 0.0f; }
		body_switch_timer += timeSpeed*timestep;

		if (appWindow.IsKeyPressed(GLFW_KEY_SPACE) && shoot_timer > 0.5f) { myScene.OnShoot(player.m_BodyPtr); myScene.OnShoot(player.m_BodyPtr + 2); shoot_timer = 0.0f; }
		if (appWindow.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1) && shoot_timer > 0.5f) { myScene.OnShoot(player.m_BodyPtr); shoot_timer = 0.0f; }
//		if (appWindow.IsKeyPressed(GLFW_KEY_SPACE) && shoot_timer > 0.5f) { myScene.OnShoot(player.m_BodyPtr); shoot_timer = 0.0f; }
		shoot_timer += timeSpeed*timestep;


		// Update the scene
//		myScene.UpdateWithCollision(timeSpeed*timestep, SimpleAcceleration); // working as well
//		myScene.UpdateWithCollision(timeSpeed*timestep, GravityAcceleration); // working as well
		myScene.UpdateWithCollision(timeSpeed*timestep, SimplifiedGravity); // working as well

		// Draw the scene into the default framebuffer
//		myScene.Draw(player.m_Observer);
		myScene.Draw(player);

		highlighter.DrawAllMarkers(myScene, player);
//		for(int i=0; i<10; i++)
//			highlighter.DrawMarker(myScene.GetBodyPtr(i)->location, Vec3D(1, 0, 0));
		
		// Swap the screen buffers
		glfwSwapBuffers(appWindow.GetWindow());


		timestep = (float)glfwGetTime() - lastFrameTime;
	}

	// Terminates GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}


