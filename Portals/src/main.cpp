
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
#include "renderer/skybox.h"

#include "controls/timestep.h"

#include "utils/shape_creator.h"

#include "glm/gtc/matrix_transform.hpp"

#include "vendor/stb_image/stb_image.h"

int main()
{
	MyWindow appWindow(1280, 720, "Portals");
//	appWindow.SetKeyCallback(key_callback);
	
//	Skybox mySky = std::move(Skybox());

	Skybox mySky;

	uint32_t vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	Shape3D myShape = CreateSphere(10);
//	myShape.write_to_file("assets/meshes/Sphere_4.txt");
//	Shape3D myShape = CreateCube(1);
//	Shape3D myShape = CreatePlane(1,1);
//	myShape.write_to_file("assets/meshes/Cube_1.txt");
	Shape3D myPlane = CreatePlane(10, 10); myPlane.center(); myPlane.scale(20);
//	myShape.rotate(Rotation(0.6, {1,0,0}));
//	myShape.normalize();

	/*
	CreateSphere(8).write_to_file("assets/meshes/Sphere_8.txt");
	CreateSphere(16).write_to_file("assets/meshes/Sphere_16.txt");
	CreateCube(1).write_to_file("assets/meshes/Cube_1.txt");
	CreateCube(2).write_to_file("assets/meshes/Cube_2.txt");
	CreateCube(4).write_to_file("assets/meshes/Cube_4.txt");
	CreateCube(8).write_to_file("assets/meshes/Cube_8.txt");
	CreateCube(16).write_to_file("assets/meshes/Cube_16.txt");
	*/

	// create the cube/sphere
	std::vector<Vec3D> shape_with_color;
	shape_with_color.resize(2 * myShape.vertices.size());
	for (int i = 0; i < 2*myShape.vertices.size(); i += 2)
	{
		shape_with_color[i] = myShape.vertices[i/2];
		shape_with_color[i + 1] = myShape.vertices[i / 2]; // set the colours to be the same as the coordinates, this is useful for debugging
	}

	OpenGLVertexBuffer cube_buffer((float*)&shape_with_color[0], shape_with_color.size() * sizeof(Vec3D)); // despite the name, this sould be a sphere
	cube_buffer.SetLayout({
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float3, "aColor"}
	});
	
	OpenGLIndexBuffer cube_index_buffer((uint32_t*)&myShape.indices[0], myShape.indices.size()); // despite the name, this sould be a sphere

	Scene myScene(cube_buffer, cube_index_buffer);

	// create the plane
	uint32_t planeVertexArray;
	glGenVertexArrays(1, &planeVertexArray);
	glBindVertexArray(planeVertexArray);

	shape_with_color.clear();
	shape_with_color.resize(2 * myPlane.vertices.size());
	for (int i = 0; i < 2 * myPlane.vertices.size(); i += 2)
	{
		shape_with_color[i] = myPlane.vertices[i / 2];
		shape_with_color[i + 1] = Vec3D{0.1f, 0.1f, 0.1f}; // set the colours to be the same as the coordinates, this is useful for debugging
	}

	OpenGLVertexBuffer plane_buffer((float*)&shape_with_color[0], shape_with_color.size() * sizeof(Vec3D)); // despite the name, this sould be a sphere
	plane_buffer.SetLayout({
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float3, "aColor"}
		});

	OpenGLIndexBuffer plane_index_buffer((uint32_t*)&myPlane.indices[0], myPlane.indices.size()); // despite the name, this sould be a sphere

	Scene planeScene(plane_buffer, plane_index_buffer);
	
	Shader myShader(ParseShader("src/renderer/shader_sources/vertex_shader.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader.glsl"));
	{
		myShader.Bind();
		myShader.SearchAndAddUniform("body_translation");
		myShader.SearchAndAddUniform("body_orientation");
		myShader.SearchAndAddUniform("body_scale");
		myShader.SearchAndAddUniform("observer_translation");
		myShader.SearchAndAddUniform("observer_orientation");
		myShader.SearchAndAddUniform("zoom_level");
		myShader.SearchAndAddUniform("alpha");

		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 0.0f));
		myShader.UploadUniformMat3("body_orientation", glm::mat3(1.0f));
		myShader.UploadUniformFloat("body_scale", 1.0f);
		myShader.UploadUniformFloat3("observer_translation", glm::vec3(0.0f, 0.0f, 0.0f));
		myShader.UploadUniformMat3("observer_orientation", glm::mat3(1.0f));
		myShader.UploadUniformFloat("zoom_level", 1.0f);
		myShader.UploadUniformFloat("alpha", 1.0f);
	}



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
		appWindow.HandleUserInputs(myScene.m_Observer, timestep);

		myScene.SetObserver(myShader);
		
		// Render
		glClearColor(0.0f, 0.05f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mySky.Draw(myScene.m_Observer);
		
		glBindVertexArray(vertexArray);
		myShader.Bind();

		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 0.0f));
		myShader.UploadUniformFloat("body_scale", 1.0f);
		myScene.Draw();
		
		myShader.UploadUniformFloat3("body_translation", glm::vec3(5.0f, 0.0f, 0.0f));
		myShader.UploadUniformFloat("body_scale", 1.0f);
		myScene.Draw();

		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 5.0f, 0.0f));
		myShader.UploadUniformFloat("body_scale", 1.0f);
		myScene.Draw();

		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 5.0f));
		myShader.UploadUniformFloat("body_scale", 2.0f);
		myScene.Draw();


		glBindVertexArray(planeVertexArray);
		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, -10.0f, -50.0f));
		planeScene.Draw();

//		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 100000.0f));
//		myShader.UploadUniformFloat("body_scale", 20000.0f);
//		myScene.Draw();

//		static int dim = 10;
//		myShader.UploadUniformMat3("body_orientation", glm::mat3(1.0f));
//		myShader.UploadUniformFloat("body_scale", 2.0f);
//		myShader.UploadUniformFloat("alpha", 1.0f);
//
//		for (int i = 0; i < dim; i++)
//		{
//			for (int j = 0; j < dim; j++)
//			{
//				for (int k = 0; k < dim; k++)
//				{
//					myShader.UploadUniformFloat3("body_translation", glm::vec3((float)i*20.0f, (float)j*20.0f, (float)k*20.0f));
//					myScene.Draw();
//				}
//			}
//		}

//		myShader.UploadUniformFloat3("body_translation", myScene.m_Observer.translation.Glm());
//		myShader.UploadUniformMat3("body_orientation", glm::mat3(1.0f));
//		myShader.UploadUniformFloat("alpha", 0.01f);
//		for (int i = 50; i > 0; i--)
//		{
//			myShader.UploadUniformFloat("body_scale", 2.0f*(float)i);
//			myScene.Draw();
//		}


		// Swap the screen buffers
		glfwSwapBuffers(appWindow.GetWindow());
	}

	// Terminates GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}






/*
// eralier version of main:


int main()
{
	MyWindow appWindow(1280, 720, "Portals");
//	appWindow.SetKeyCallback(key_callback);

//	Skybox mySky = std::move(Skybox());

	Skybox mySky;

	uint32_t vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	Shape3D myShape = CreateSphere(8);
//	myShape.write_to_file("assets/meshes/Sphere_4.txt");
//	Shape3D myShape = CreateCube(1);
//	myShape.write_to_file("assets/meshes/Cube_1.txt");
//	Shape3D myShape = CreatePlane(10, 10);
//	myShape.rotate(Rotation(0.6, {1,0,0}));
//	myShape.normalize();

//	CreateSphere(8).write_to_file("assets/meshes/Sphere_8.txt");
//	CreateSphere(16).write_to_file("assets/meshes/Sphere_16.txt");
//	CreateCube(1).write_to_file("assets/meshes/Cube_1.txt");
//	CreateCube(2).write_to_file("assets/meshes/Cube_2.txt");
//	CreateCube(4).write_to_file("assets/meshes/Cube_4.txt");
//	CreateCube(8).write_to_file("assets/meshes/Cube_8.txt");
//	CreateCube(16).write_to_file("assets/meshes/Cube_16.txt");

std::vector<Vec3D> shape_with_color;
shape_with_color.resize(2 * myShape.vertices.size());
for (int i = 0; i < 2 * myShape.vertices.size(); i += 2)
{
	shape_with_color[i] = myShape.vertices[i / 2];
	shape_with_color[i + 1] = myShape.vertices[i / 2]; // set the colours to be the same as the coordinates, this is useful for debugging
}

OpenGLVertexBuffer cube_buffer((float*)&shape_with_color[0], shape_with_color.size() * sizeof(Vec3D)); // despite the name, this sould be a sphere
cube_buffer.SetLayout({
	{ShaderDataType::Float3, "aPos"},
	{ShaderDataType::Float3, "aColor"}
	});

OpenGLIndexBuffer cube_index_buffer((uint32_t*)&myShape.indices[0], myShape.indices.size()); // despite the name, this sould be a sphere

Shader myShader(ParseShader("src/renderer/shader_sources/vertex_shader.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader.glsl"));
{
	myShader.Bind();
	myShader.SearchAndAddUniform("body_translation");
	myShader.SearchAndAddUniform("body_orientation");
	myShader.SearchAndAddUniform("body_scale");
	myShader.SearchAndAddUniform("observer_translation");
	myShader.SearchAndAddUniform("observer_orientation");
	myShader.SearchAndAddUniform("zoom_level");
	myShader.SearchAndAddUniform("alpha");

	myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 0.0f));
	myShader.UploadUniformMat3("body_orientation", glm::mat3(1.0f));
	myShader.UploadUniformFloat("body_scale", 1.0f);
	myShader.UploadUniformFloat3("observer_translation", glm::vec3(0.0f, 0.0f, 0.0f));
	myShader.UploadUniformMat3("observer_orientation", glm::mat3(1.0f));
	myShader.UploadUniformFloat("zoom_level", 1.0f);
	myShader.UploadUniformFloat("alpha", 1.0f);
}

Scene myScene(cube_buffer, cube_index_buffer);


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
	appWindow.HandleUserInputs(myScene.m_Observer, timestep);

	myScene.SetObserver(myShader);

	// Render
	glClearColor(0.0f, 0.05f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//		glBindVertexArray(vertexArray);
	//		cube_buffer.Bind();
	//		cube_index_buffer.Bind();
	//		myShader.Bind();

	//		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 0.0f));
	//		myShader.UploadUniformFloat("body_scale", 1.0f);
	//		myScene.Draw();
	//		
	//		myShader.UploadUniformFloat3("body_translation", glm::vec3(5.0f, 0.0f, 0.0f));
	//		myShader.UploadUniformFloat("body_scale", 1.0f);
	//		myScene.Draw();
	//
	//		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 5.0f, 0.0f));
	//		myShader.UploadUniformFloat("body_scale", 1.0f);
	//		myScene.Draw();
	//
	//		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 5.0f));
	//		myShader.UploadUniformFloat("body_scale", 2.0f);
	//		myScene.Draw();


	static int dim = 10;
	myShader.UploadUniformMat3("body_orientation", glm::mat3(1.0f));
	myShader.UploadUniformFloat("body_scale", 2.0f);
	myShader.UploadUniformFloat("alpha", 1.0f);

	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			for (int k = 0; k < dim; k++)
			{
				myShader.UploadUniformFloat3("body_translation", glm::vec3((float)i*20.0f, (float)j*20.0f, (float)k*20.0f));
				myScene.Draw();
			}
		}
	}

	myShader.UploadUniformFloat3("body_translation", myScene.m_Observer.translation.Glm());
	myShader.UploadUniformMat3("body_orientation", glm::mat3(1.0f));
	myShader.UploadUniformFloat("alpha", 0.05f);
	for (int i = 50; i > 0; i--)
	{
		myShader.UploadUniformFloat("body_scale", 2.0f*(float)i);
		myScene.Draw();
	}


	// Swap the screen buffers
	glfwSwapBuffers(appWindow.GetWindow());
}

// Terminates GLFW, clearing any resources allocated by GLFW.
glfwTerminate();
return 0;
}



*/





