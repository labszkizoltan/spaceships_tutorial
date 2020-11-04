
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


#include "controls/timestep.h"
#include "utils/shape_creator.h"
#include "glm/gtc/matrix_transform.hpp"
#include "vendor/stb_image/stb_image.h"


const int windowWidth = 1280;
const int windowHeight = 720;

int main()
{
	MyWindow appWindow(windowWidth, windowHeight, "Portals");
//	appWindow.SetKeyCallback(key_callback);
	
	FrameBufferSpecification fb_spec;
//	fb_spec.Height = windowHeight; fb_spec.Width = windowWidth; fb_spec.Samples = 1; fb_spec.SwapChainTarget = false;
	fb_spec.Height = 1000; fb_spec.Width = 1000; fb_spec.Samples = 1; fb_spec.SwapChainTarget = false;
	OpenGLFramebuffer myFramebuffer(fb_spec);
	TexturedMesh portal("assets/textured_meshes/Plane_1.txt", myFramebuffer.GetTextureID());


	Observer observer;
	
	Skybox mySky;
	mySky.SetShaderAspectRatio((float)windowWidth / (float)windowHeight);


	std::vector<std::string> texture_files = {
		"assets/up_mountains.png",
		"assets/down_mountains.png",
		"assets/front_mountains.png",
		"assets/back_mountains.png",
		"assets/left_mountains.png",
		"assets/right_mountains.png"
	};
	Skybox mountainsSkybox(texture_files, 10);
	mountainsSkybox.SetShaderAspectRatio(1.0f);

	Shape3D myShape = CreateSphere(3);
//	myShape.write_to_file("assets/meshes/Sphere_4.txt");
//	Shape3D myShape = CreateCube(1);
//	Shape3D myShape = CreatePlane(1,1);
//	myShape.write_to_file("assets/meshes/Cube_1.txt");

//	Shape3D myPlane = CreatePlane(3, 3); myPlane.center();
	Shape3D myPlane = CreateCube(8);
//	myShape.rotate(Rotation(0.6, {1,0,0}));
//	myShape.normalize();

	/*
	CreateSphere(2).write_to_file("assets/meshes/Sphere_2.txt");
	CreateSphere(3).write_to_file("assets/meshes/Sphere_3.txt");
	CreateSphere(4).write_to_file("assets/meshes/Sphere_4.txt");
	CreateSphere(8).write_to_file("assets/meshes/Sphere_8.txt");
	CreateSphere(10).write_to_file("assets/meshes/Sphere_10.txt");
	CreateSphere(16).write_to_file("assets/meshes/Sphere_16.txt");
	CreateCube(1).write_to_file("assets/meshes/Cube_1.txt");
	CreateCube(2).write_to_file("assets/meshes/Cube_2.txt");
	CreateCube(4).write_to_file("assets/meshes/Cube_4.txt");
	CreateCube(8).write_to_file("assets/meshes/Cube_8.txt");
	CreateCube(10).write_to_file("assets/meshes/Cube_10.txt");
	CreateCube(16).write_to_file("assets/meshes/Cube_16.txt");
	*/

	// create the cube/sphere
//	ColouredMesh cubeMesh("assets/coloured_meshes/Sphere_8.txt");
//	ColouredMesh cubeMesh("assets/coloured_meshes/Sphere_4.txt");
	ColouredMesh cubeMesh("assets/coloured_meshes/Cube_10.txt");

	// create the plane
	std::vector<Vec3D> shape_with_color;
	shape_with_color.resize(2 * myPlane.vertices.size());
	for (int i = 0; i < 2 * myPlane.vertices.size(); i += 2)
	{
		shape_with_color[i] = myPlane.vertices[i / 2];
//		shape_with_color[i + 1] = Vec3D{ 0.1f, 0.1f, 0.1f }; // set the colours to be the same as the coordinates, this is useful for debugging
		shape_with_color[i + 1] = Vec3D{ 1,1,1 }; // set the colours to be the same as the coordinates, this is useful for debugging
	}

	OpenGLVertexBuffer plane_buffer((float*)&shape_with_color[0], shape_with_color.size() * sizeof(Vec3D)); // despite the name, this sould be a sphere
	OpenGLIndexBuffer plane_index_buffer((uint32_t*)&myPlane.indices[0], myPlane.indices.size()); // despite the name, this sould be a sphere
	ColouredMesh planeMesh(plane_buffer, plane_index_buffer);
	
	// create a fourth mesh from a file, a tetrahedron
	ColouredMesh sphereMesh("assets/coloured_meshes/Sphere_10_purple.txt");

	// Create textured meshes, from a file
	TexturedMesh texturedCube("assets/textured_meshes/Cube_1.txt", "assets/all_in_one.png");
	// https://learnopengl.com/Getting-started/Textures : in the comments they mention a picture load issue, probably the one that I faced as well
	// (I solved this by loading the picture in paint, rotating 180 deg, saving and rotating back+saving again)
	// glPixelStorei(GL_UNPACK_ALIGNMENT, 1); - is said to solve the problem. 
	TexturedMesh flatFlower("assets/textured_meshes/Plane_1.txt", "assets/flower.png");


	TexturedShadedMesh shadedCube("assets/meshes_with_normals/Cube_1.txt", "assets/all_in_one.png");
	TexturedShadedMesh shadedSaucer("assets/meshes_with_normals/Saucer_hq.txt", "assets/saucer_texture.png");


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
		myShader.UploadUniformFloat("aspect_ratio", (float)windowWidth /(float)windowHeight);
		myShader.UploadUniformFloat("alpha", 1.0f);
	}

	Shader textureShader(ParseShader("src/renderer/shader_sources/vertex_shader_textured.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader_textured.glsl"));
	{
		textureShader.Bind();
		textureShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 0.0f));
		textureShader.UploadUniformMat3("body_orientation", glm::mat3(1.0f));
		textureShader.UploadUniformFloat("body_scale", 1.0f);
		textureShader.UploadUniformFloat3("observer_translation", glm::vec3(0.0f, 0.0f, 0.0f));
		textureShader.UploadUniformMat3("observer_orientation", glm::mat3(1.0f));
		textureShader.UploadUniformFloat("zoom_level", 1.0f);
		textureShader.UploadUniformFloat("aspect_ratio", (float)windowWidth / (float)windowHeight);
		textureShader.UploadUniformFloat("alpha", 1.0f);

		int samplers[32];
		for (uint32_t i = 0; i < 32; i++) { samplers[i] = i; }
		textureShader.UploadUniformIntArray("u_Textures", samplers, 32);
	}

	Shader shadeShader(ParseShader("src/renderer/shader_sources/vertex_shader_textured_shaded.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader_textured_shaded.glsl"));
	{
		shadeShader.Bind();
		shadeShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 0.0f));
		shadeShader.UploadUniformMat3("body_orientation", glm::mat3(1.0f));
		shadeShader.UploadUniformFloat("body_scale", 1.0f);
		shadeShader.UploadUniformFloat3("observer_translation", glm::vec3(0.0f, 0.0f, 0.0f));
		shadeShader.UploadUniformMat3("observer_orientation", glm::mat3(1.0f));
		shadeShader.UploadUniformFloat("zoom_level", 1.0f);
		shadeShader.UploadUniformFloat("aspect_ratio", (float)windowWidth / (float)windowHeight);
		shadeShader.UploadUniformFloat("alpha", 1.0f);

		int samplers[32];
		for (uint32_t i = 0; i < 32; i++) { samplers[i] = i; }
		shadeShader.UploadUniformIntArray("u_Textures", samplers, 32);
	}



	float time = (float)glfwGetTime();
	Timestep timestep = 0.0f; // timestep can be initialized like this, because its constructor takes in only one float, implicit cast is possible
	float lastFrameTime = 0.0f;
	
	static Mat_3D orientation = Identity(1.0f);

	// Game loop
	while (!glfwWindowShouldClose(appWindow.GetWindow()))
	{
		time = (float)glfwGetTime();
		timestep = time - lastFrameTime;
		lastFrameTime = time;

		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		appWindow.HandleUserInputs(observer, timestep);

		observer.SetObserverInShader(myShader);
		{
			myFramebuffer.Bind();
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//			mySky.SetShaderAspectRatio(1.0f);
//			mySky.Draw(observer);
			mountainsSkybox.Draw(observer);

			myShader.Bind();
			myShader.UploadUniformFloat("aspect_ratio", 1.0f);

			myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 5.0f, 5.0f));
			myShader.UploadUniformFloat("body_scale", 1.0f);
			cubeMesh.Draw();

			myShader.UploadUniformFloat3("body_translation", glm::vec3(5.0f, 5.0f, 5.0f));
			myShader.UploadUniformFloat("body_scale", 1.0f);
			cubeMesh.Draw();

			myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 10.0f, 5.0f));
			myShader.UploadUniformFloat("body_scale", 1.0f);
			cubeMesh.Draw();

			myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 5.0f, 10.0f));
			myShader.UploadUniformFloat("body_scale", 2.0f);
			cubeMesh.Draw();

			myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, -10.0f, 50.0f));
			planeMesh.Draw();
			
			myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 0.0f));
			// tetrahedronMesh.Draw();
			sphereMesh.Draw();

			textureShader.Bind();
			textureShader.UploadUniformFloat("aspect_ratio", 1.0f);

			observer.SetObserverInShader(textureShader);
			textureShader.UploadUniformFloat("body_scale", 1.0f);

			textureShader.UploadUniformFloat3("body_translation", glm::vec3(5.0f, -10.0f, 25.0f));
			texturedCube.Draw();

			textureShader.UploadUniformFloat3("body_translation", glm::vec3(10.0f, -10.0f, 25.0f));
			flatFlower.Draw();

			myFramebuffer.Unbind();
			glViewport(0, 0, windowWidth, windowHeight);
		}

		// Render
		glClearColor(0.0f, 0.05f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mySky.SetShaderAspectRatio((float)windowWidth / (float)windowHeight);
		mySky.Draw(observer);
		
		myShader.Bind();
		myShader.UploadUniformFloat("aspect_ratio", (float)windowWidth / (float)windowHeight);

		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 5.0f, 5.0f));
		myShader.UploadUniformFloat("body_scale", 1.0f);
		cubeMesh.Draw();

		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 5.0f, 10.0f));
		myShader.UploadUniformFloat("body_scale", 2.0f);
		cubeMesh.Draw();

		static float angularVelocity = 1.01f;
		orientation = orientation* Rotation(angularVelocity *timestep, { -1, 1, 0 });
		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, -10.0f, 50.0f));
		myShader.UploadUniformMat3("body_orientation", orientation.Glm());
		planeMesh.Draw();

		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 0.0f));
		// tetrahedronMesh.Draw();
		sphereMesh.Draw();



		textureShader.Bind();
		textureShader.UploadUniformFloat("aspect_ratio", (float)windowWidth / (float)windowHeight);

		observer.SetObserverInShader(textureShader);
		textureShader.UploadUniformFloat("body_scale", 1.0f);

		textureShader.UploadUniformFloat3("body_translation", glm::vec3(5.0f, -10.0f, 25.0f));
		texturedCube.Draw();

		textureShader.UploadUniformFloat3("body_translation", glm::vec3(10.0f, -10.0f, 25.0f));
		flatFlower.Draw();

		textureShader.UploadUniformFloat3("body_translation", glm::vec3(10.0f, -10.0f, 15.0f));
		textureShader.UploadUniformFloat("body_scale", 5.0f);
		portal.Draw();


		shadeShader.Bind();
		observer.SetObserverInShader(shadeShader);
		shadeShader.UploadUniformFloat("aspect_ratio", (float)windowWidth / (float)windowHeight);
		shadeShader.UploadUniformFloat3("body_translation", glm::vec3(10.0f, 10.0f, 5.0f));
		shadeShader.UploadUniformMat3("body_orientation", orientation.Glm());
		shadedCube.Draw();

		shadeShader.UploadUniformFloat3("body_translation", glm::vec3(15.0f, 15.0f, 5.0f));
		shadedSaucer.Draw();



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

Scene cubeMesh(cube_buffer, cube_index_buffer);


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
	appWindow.HandleUserInputs(cubeMesh.m_Observer, timestep);

	cubeMesh.SetObserver(myShader);

	// Render
	glClearColor(0.0f, 0.05f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//		glBindVertexArray(vertexArray);
	//		cube_buffer.Bind();
	//		cube_index_buffer.Bind();
	//		myShader.Bind();

	//		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 0.0f));
	//		myShader.UploadUniformFloat("body_scale", 1.0f);
	//		cubeMesh.Draw();
	//		
	//		myShader.UploadUniformFloat3("body_translation", glm::vec3(5.0f, 0.0f, 0.0f));
	//		myShader.UploadUniformFloat("body_scale", 1.0f);
	//		cubeMesh.Draw();
	//
	//		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 5.0f, 0.0f));
	//		myShader.UploadUniformFloat("body_scale", 1.0f);
	//		cubeMesh.Draw();
	//
	//		myShader.UploadUniformFloat3("body_translation", glm::vec3(0.0f, 0.0f, 5.0f));
	//		myShader.UploadUniformFloat("body_scale", 2.0f);
	//		cubeMesh.Draw();


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
				cubeMesh.Draw();
			}
		}
	}

	myShader.UploadUniformFloat3("body_translation", cubeMesh.m_Observer.translation.Glm());
	myShader.UploadUniformMat3("body_orientation", glm::mat3(1.0f));
	myShader.UploadUniformFloat("alpha", 0.05f);
	for (int i = 50; i > 0; i--)
	{
		myShader.UploadUniformFloat("body_scale", 2.0f*(float)i);
		cubeMesh.Draw();
	}


	// Swap the screen buffers
	glfwSwapBuffers(appWindow.GetWindow());
}

// Terminates GLFW, clearing any resources allocated by GLFW.
glfwTerminate();
return 0;
}



*/





