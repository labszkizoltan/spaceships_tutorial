
#pragma once

#include "window_context.h"
#include "Portals/src/renderer/shader.h"
#include "Portals/src/renderer/textured_mesh.h"




class menu
{
public:

	menu();
	~menu();
	void Run();
	void Update(); // update highlighting based on mouse position
	void Draw();

private:

	MyWindow* m_WindowPtr;
	Shader m_QuadShader;
	TexturedMesh m_MenuBarsMesh, m_HighlightPerimeter; // when something is hovered, a quad will be drawn on top of it, which only has a colorful perimeter, and a body with an alpha of 0.0

};








