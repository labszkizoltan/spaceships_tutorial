
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

	Shape3D myShape = CreateSphere(10);
//	Shape3D myShape = CreateCube(3);
//	Shape3D myShape = CreatePlane(10, 10);
//	myShape.rotate(Rotation(0.6, {1,0,0}));
//	myShape.normalize();

	std::vector<Vec3D> shape_with_color;
	shape_with_color.resize(2 * myShape.vertices.size());
	for (int i = 0; i < 2*myShape.vertices.size(); i += 2)
	{
		shape_with_color[i] = myShape.vertices[i/2];
//		shape_with_color[i + 1] = Vec3D(0.1f, 0.7f, 0.3f);
		shape_with_color[i + 1] = myShape.vertices[i / 2]; // set the colours to be the same as the coordinates, this is useful for debugging
	}

	float cube_vertices[] = {
		// position			colour
		-0.5f, -0.5f, -0.5f,	 0.2f, 0.2f, 0.2f,
		 0.5f, -0.5f, -0.5f,	 0.8f, 0.2f, 0.2f,
		-0.5f, -0.5f,  0.5f,	 0.2f, 0.2f, 0.8f,
		 0.5f, -0.5f,  0.5f,	 0.8f, 0.2f, 0.8f,
		-0.5f,  0.5f, -0.5f,	 0.2f, 0.8f, 0.2f,
		 0.5f,  0.5f, -0.5f,	 0.8f, 0.8f, 0.2f,
		-0.5f,  0.5f,  0.5f,	 0.2f, 0.8f, 0.8f,
		 0.5f,  0.5f,  0.5f,	 0.8f, 0.8f, 0.8f
	};
//	OpenGLVertexBuffer cube_buffer(cube_vertices, sizeof(cube_vertices));
	OpenGLVertexBuffer cube_buffer((float*)&shape_with_color[0], shape_with_color.size() * sizeof(Vec3D)); // despite the name, this sould be a sphere
	cube_buffer.SetLayout({
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float3, "aColor"}
	});
	
	uint32_t cube_indices[] = {
		0,1,2, 1,3,2, 4,5,6, 5,7,6,
		2,0,6, 0,4,6, 3,1,7, 1,5,7,
		2,3,7, 3,7,6, 0,1,4, 1,5,4
	};
//	OpenGLIndexBuffer cube_index_buffer(cube_indices, sizeof(cube_indices) / sizeof(uint32_t));
	OpenGLIndexBuffer cube_index_buffer((uint32_t*)&myShape.indices[0], myShape.indices.size()); // despite the name, this sould be a sphere

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

	Scene myScene(cube_buffer, cube_index_buffer);

	// Game loop
	while (!glfwWindowShouldClose(appWindow.GetWindow()))
	{
		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		
		if (appWindow.IsKeyPressed(GLFW_KEY_W)) { myScene.m_Observer.MoveForward(0.1f); }
		if (appWindow.IsKeyPressed(GLFW_KEY_S)) { myScene.m_Observer.MoveBackward(0.1f); }
		if (appWindow.IsKeyPressed(GLFW_KEY_R)) { myScene.m_Observer.MoveRight(0.1f); }
		if (appWindow.IsKeyPressed(GLFW_KEY_F)) { myScene.m_Observer.MoveLeft(0.1f); }
		if (appWindow.IsKeyPressed(GLFW_KEY_D)) { myScene.m_Observer.MoveUp(0.1f); }
		if (appWindow.IsKeyPressed(GLFW_KEY_A)) { myScene.m_Observer.MoveDown(0.1f); }

		if (appWindow.IsKeyPressed(GLFW_KEY_E))		{ myScene.m_Observer.TurnAntiClockwise(0.01f); }
		if (appWindow.IsKeyPressed(GLFW_KEY_Q))		{ myScene.m_Observer.TurnClockwise(0.01f); }
		if (appWindow.IsKeyPressed(GLFW_KEY_RIGHT))	{ myScene.m_Observer.TurnUp(0.01f); }
		if (appWindow.IsKeyPressed(GLFW_KEY_LEFT))	{ myScene.m_Observer.TurnDown(0.01f); }
		if (appWindow.IsKeyPressed(GLFW_KEY_DOWN))	{ myScene.m_Observer.TurnLeft(0.01f); }
		if (appWindow.IsKeyPressed(GLFW_KEY_UP))	{ myScene.m_Observer.TurnRight(0.01f); }

		if (appWindow.IsKeyPressed(GLFW_KEY_P)) { myScene.m_Observer.ZoomIn(1.05f); }
		if (appWindow.IsKeyPressed(GLFW_KEY_O)) { myScene.m_Observer.ZoomOut(1.05f); }

		myScene.SetObserver(myShader);

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.05f, 0.3f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 0.0f));
		myShader.UploadUniformMat3("body_orientation", glm::mat3(1.0f));
		myShader.UploadUniformFloat("body_scale", 0.5f);
		myScene.Draw();
		
		static float angle = 0.0f;
		angle += 0.02f;
		Mat_3D mat = Rotation(angle, { 1,1,0 });
		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 2.0f));
		myShader.UploadUniformMat3("body_orientation", mat.Glm());
		myShader.UploadUniformFloat("body_scale", 1.0f);
		myScene.Draw();
		
		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 5.0f));
		myShader.UploadUniformMat3("body_orientation", glm::mat3(1.0f));
		myShader.UploadUniformFloat("body_scale", 1.5f);
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




