#pragma once

#include <string>
#include "Portals/src/controls/observer.h"
#include "Portals/src/controls/player.h"
#include "Portals/src/objects/body.h"
#include "Portals/src/objects/projectile.h"

#include "Portals/src/renderer/shader.h"
#include "Portals/src/renderer/skybox.h"
#include "Portals/src/renderer/textured_shaded_mesh.h"

std::string ParseShader(const std::string& filepath);


// Function signature of the force function. There will be a version of the Update() method, that takes in a customizable force function
// to calculate the acceleration of the bodies.
// typedef std::vector<Vec3D>(*AccelerationFunction)(const std::vector<Body>& bodies);
// typedef void(*AccelerationFunction)(std::vector<Body>& bodies, std::vector<Vec3D>& accelerations);
typedef void(*AccelerationFunction)(std::vector<Body>& bodies, std::vector<Vec3D>& accelerations, std::vector<Vec3D>& angular_accelerations);

class Scene
{
public:
//	Scene() = default;
	Scene(const std::string& filename);
	~Scene();

	void Update(float deltaTime);
	void Update(float deltaTime, AccelerationFunction accelerationFunc);
	void UpdateWithCollision(float deltaTime, AccelerationFunction accelerationFunc);
	void OnShoot(Body* ownerBodyPtr);

	void Draw(Observer obs);
	void Draw(Player& player);
	void SetAspectRatio(float aspectRatio);
	void SetSceneTranslation(glm::vec3 scene_translation);

	Body* GetBodyPtr(int i);
	

private:
	void CalcMinDistances(float deltaTime, float dvThreshold = 0.001f);
//	void OnHit(int bodyIndex, Projectile& projectile, float hitStrength);
	void OnHit(int bodyIndex, float hitStrength = 1.0f);

private:
	std::vector<Body> m_Bodies;
	std::vector<float> m_Integrities; // Stores the structural integrity of the bodies, if it reaches below 0, the body is destroyed, initially set to be equal to the mass
	ProjectilePool m_ProjectilePool;
	std::vector<int> m_MeshIndices; // this and m_BodyTypes are kind of redundant, since they essentially code the same info, just in different ways

	// Pre-allocated helper memory blocks for state update
	std::vector<Vec3D> m_Accelerations; // Stores the result of the acceleration calculations
	std::vector<Vec3D> m_AngularAccelerations; // Stores the result of the angular acceleration calculations
	std::vector<float> m_Distances; // Stores the result of the distance matrix calculation, which is later used for collision detection


//	Shader m_ColourShader; // (ParseShader("src/renderer/shader_sources/vertex_shader.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader.glsl"));
	Shader m_TextureShader; // (ParseShader("src/renderer/shader_sources/vertex_shader_textured.glsl"), ParseShader("src/renderer/shader_sources/fragment_shader_textured.glsl"));

	Skybox m_Skybox;

	std::vector<TexturedShadedMesh> m_Meshes;
};







