
#version 450 core

layout(location = 0) out vec4 color;

in vec2 vTexCoord;

uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, vTexCoord);
}



