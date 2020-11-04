
#include "scene.h"



Scene::Scene(const std::string & filename)
{

	std::vector<std::string> skybox_texture_files; skybox_texture_files.resize(6); //

	std::ifstream scene_definition(filename.c_str());
	int v_count = 0, i_count = 0;

	scene_definition >> v_count;
	scene_definition >> i_count;


	scene_definition.close();


}

Scene::~Scene()
{


}

void Scene::Update(float deltaTime)
{


}

void Scene::Draw(Observer obs)
{


}
