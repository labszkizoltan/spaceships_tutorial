
#version 450 core

layout(location = 0) out vec4 color;

in vec2 vTexCoord;
in float vTexID;

uniform sampler2D u_Textures[32];

void main()
{
//	color = 0.5 * texture(u_Textures[int(vTexID)], vTexCoord) + 0.5*vec4(vTexCoord, 0, 1);
//	color = 0.5 * texture(u_Textures[1], vTexCoord) + 0.5*vec4(vTexCoord, 0, 1);
	color = vec4(vTexCoord, 0, 1);

//	switch (int(vTexID))
//	{
//	case 0: color = 0.5 * texture(u_Textures[0], vTexCoord) + 0.5*vec4(vTexCoord, 0, 1); break;
//	case 1: color = 0.5 * texture(u_Textures[1], vTexCoord) + 0.5*vec4(vTexCoord, 0, 1); break;
//	case 2: color = 0.5 * texture(u_Textures[2], vTexCoord) + 0.5*vec4(vTexCoord, 0, 1); break;
//	case 3: color = 0.5 * texture(u_Textures[3], vTexCoord) + 0.5*vec4(vTexCoord, 0, 1); break;
//	case 4: color = 0.5 * texture(u_Textures[4], vTexCoord) + 0.5*vec4(vTexCoord, 0, 1); break;
//	case 5: color = 0.5 * texture(u_Textures[5], vTexCoord) + 0.5*vec4(vTexCoord, 0, 1); break;
//	case 6: color = 0.5 * texture(u_Textures[6], vTexCoord) + 0.5*vec4(vTexCoord, 0, 1); break;
//	case 7: color = 0.5 * texture(u_Textures[7], vTexCoord) + 0.5*vec4(vTexCoord, 0, 1); break;
//	case 8: color = 0.5 * texture(u_Textures[8], vTexCoord) + 0.5*vec4(vTexCoord, 0, 1); break;
//	case 9: color = 0.5 * texture(u_Textures[9], vTexCoord) + 0.5*vec4(vTexCoord, 0, 1); break;
//	case 10: color = 0.5 * texture(u_Textures[10], vTexCoord) + 0.5*vec4(vTexCoord, 0, 1); break;
//
//	}

}



