
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
#include "renderer/scene.h"

#include "utils/shape_creator.h"

#include "glm/gtc/matrix_transform.hpp"

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


// The MAIN function, from here we start the application and run the game loop
int main()
{
	MyWindow appWindow(800, 600, "Portals");
	appWindow.SetKeyCallback(key_callback);

	uint32_t vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	float cube_vertices[] = {
		// position			colour
		-0.5f, -0.5f, -0.5f,	 0.7f, 0.1f, 0.3f,
		-0.5f, -0.5f,  0.5f,	 0.1f, 0.7f, 0.3f,
		-0.5f,  0.5f, -0.5f,	 0.0f, 0.7f, 0.3f,
		-0.5f,  0.5f,  0.5f,	 0.2f, 0.8f, 0.3f,
		 0.5f, -0.5f, -0.5f,	 0.2f, 0.9f, 0.3f,
		 0.5f, -0.5f,  0.5f,	 0.2f, 0.7f, 0.1f,
		 0.5f,  0.5f, -0.5f,	 0.2f, 0.7f, 0.2f,
		 0.5f,  0.5f,  0.5f,	 0.2f, 0.7f, 0.3f
	};
	OpenGLVertexBuffer cube_buffer(cube_vertices, sizeof(cube_vertices));
	cube_buffer.SetLayout({
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float3, "aColor"}
	});
	
	uint32_t cube_indices[] = {
		0,1,2, 1,2,3, 4,5,6, 5,6,7,
		0,1,4, 1,4,5, 2,3,6, 3,6,7,
		0,2,4, 2,4,6, 1,3,5, 3,5,7
	};
	OpenGLIndexBuffer cube_index_buffer(cube_indices, sizeof(cube_indices) / sizeof(uint32_t));

	Shader myShader(ParseShader("src/renderer/shader_sources/vertex_shader.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader.glsl"));
	myShader.Bind();
	myShader.SearchAndAddUniform("body_translation");
	myShader.SearchAndAddUniform("body_orientation");
	myShader.SearchAndAddUniform("body_scale");
	myShader.SearchAndAddUniform("observer_translation");
	myShader.SearchAndAddUniform("observer_orientation");
	myShader.SearchAndAddUniform("zoom_level");

	myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 0.0f));
	myShader.UploadUniformMat3  ("body_orientation", glm::mat3(1.0f));
	myShader.UploadUniformFloat ("body_scale", 1.0f);
	myShader.UploadUniformFloat3("observer_translation", glm::vec3(0.0f, 0.0f, 0.0f));
	myShader.UploadUniformMat3  ("observer_orientation", glm::mat3(1.0f));
	myShader.UploadUniformFloat ("zoom_level", 1.0f);

	glm::vec3 obs_loc(0.0f);
	float zoom_level = 1.0f;
	glm::mat3 obs_orientation(1.0f);

	Scene myScene(cube_buffer, cube_index_buffer);

	// Game loop
	while (!glfwWindowShouldClose(appWindow.GetWindow()))
	{
		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		static float r = 0.2f, g = 0.0f, b = 0.7f;

		if (appWindow.IsKeyPressed(GLFW_KEY_R)) { r += 0.01f; r = r > 1.0f ? 0.0f : r; }
		if (appWindow.IsKeyPressed(GLFW_KEY_G)) { g += 0.01f; g = g > 1.0f ? 0.0f : g; }
		if (appWindow.IsKeyPressed(GLFW_KEY_B)) { b += 0.01f; b = b > 1.0f ? 0.0f : b; }

		if (appWindow.IsKeyPressed(GLFW_KEY_W)) { obs_loc.z += 0.05f; myShader.UploadUniformFloat3("observer_translation", obs_loc); }
		if (appWindow.IsKeyPressed(GLFW_KEY_S)) { obs_loc.z -= 0.05f; myShader.UploadUniformFloat3("observer_translation", obs_loc); }
		if (appWindow.IsKeyPressed(GLFW_KEY_RIGHT)) { obs_loc.y += 0.05f; myShader.UploadUniformFloat3("observer_translation", obs_loc); }
		if (appWindow.IsKeyPressed(GLFW_KEY_LEFT)) { obs_loc.y -= 0.05f; myShader.UploadUniformFloat3("observer_translation", obs_loc); }
		if (appWindow.IsKeyPressed(GLFW_KEY_UP)) { obs_loc.x += 0.05f; myShader.UploadUniformFloat3("observer_translation", obs_loc); }
		if (appWindow.IsKeyPressed(GLFW_KEY_DOWN)) { obs_loc.x -= 0.05f; myShader.UploadUniformFloat3("observer_translation", obs_loc); }

		if (appWindow.IsKeyPressed(GLFW_KEY_P)) { zoom_level *= 1.05f; myShader.UploadUniformFloat("zoom_level", zoom_level); }
		if (appWindow.IsKeyPressed(GLFW_KEY_O)) { zoom_level /= 1.05f; myShader.UploadUniformFloat("zoom_level", zoom_level); }

		// Render
		// Clear the colorbuffer
		glClearColor(r, g, b, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 0.0f));
		myShader.UploadUniformMat3("body_orientation", glm::mat3(1.0f));
		myShader.UploadUniformFloat("body_scale", 1.0f);
		myScene.Draw();

		myShader.UploadUniformFloat3("body_translation", glm::vec3(3.0f, 0.0f, 0.0f));
		myShader.UploadUniformMat3("body_orientation", glm::mat3(1.0f));
		myShader.UploadUniformFloat("body_scale", 1.5f);
		myScene.Draw();

		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 3.0f, 0.0f));
		myShader.UploadUniformMat3("body_orientation", glm::mat3(1.0f));
		myShader.UploadUniformFloat("body_scale", 2.0f);
		myScene.Draw();

//		glBindVertexArray(vertexArray);
//		glDrawElements(GL_TRIANGLES, sizeof(cube_indices)/sizeof(uint32_t), GL_UNSIGNED_INT, nullptr);


		// Swap the screen buffers
		glfwSwapBuffers(appWindow.GetWindow());
	}

	// Terminates GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}




