#pragma once

#include "shader.h"
#include "Portals/src/controls/observer.h"
#include "Portals/src/renderer/buffer.h"
#include "Portals/src/renderer/coloured_mesh.h"
#include "Portals/src/renderer/scene.h"
#include "Portals/src/utils/Vector_3D.h"

class Highlighter
{
public:
	Highlighter();
	~Highlighter();
	Highlighter(const std::string& vertexSrc, const std::string& fragSrc);

	void DrawMarker(Vec3D location, Vec3D colour);
	void DrawAllMarkers(Scene& scene, Player& player);

	void SetAspectRatio(float aspectRatio);
	void SetObserverInShader(Observer& obs);

private:
	Shader m_Shader;
//	Shader m_CenterMarkerShader;
	ColouredMesh m_BlueMarkerTriangle, m_ForwardDirectionMarker, m_BackwardDirectionMarker, m_ScreenCenterMarker;
};

