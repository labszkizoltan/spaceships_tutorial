
#version 450 core

layout(location = 0) out vec4 color;

in vec2 vTexCoord;

uniform sampler2D u_Textures[32];

void main()
{
	color = texture(u_Textures[6], vTexCoord); // mesh textures are bound to texture unit - 6; texture units 0-5 are used up by the skybox
}



