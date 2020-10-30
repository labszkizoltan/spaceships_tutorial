
#version 450 core

layout(location = 0) out vec4 color;

in vec2 vTexCoord;
in float vTexID;

uniform sampler2D u_Textures[32];

// uniform sampler2D u_Textures0;
// uniform sampler2D u_Textures1;
// uniform sampler2D u_Textures2;
// uniform sampler2D u_Textures3;
// uniform sampler2D u_Textures4;
// uniform sampler2D u_Textures5;

void main()
{
//	color = 0.5 * texture(u_Textures[int(vTexID)], vTexCoord) + 0.5*vec4(vTexCoord, 0, 1);
//	color = 0.5 * texture(u_Textures[int(vTexID)], vTexCoord) + 0.5*vec4(vTexCoord, vTexID / 5.0, 1);
	color = texture(u_Textures[int(vTexID)], vTexCoord);
//	color = texture(u_Textures[0], vTexCoord);
//	color = vec4(vTexCoord, 0, 1);

//	switch (int(vTexID))
//	{
//	case 0 : color = 0.5 * texture(u_Textures[ 0], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 1 : color = 0.5 * texture(u_Textures[ 1], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 2 : color = 0.5 * texture(u_Textures[ 2], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 3 : color = 0.5 * texture(u_Textures[ 3], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 4 : color = 0.5 * texture(u_Textures[ 4], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 5 : color = 0.5 * texture(u_Textures[ 5], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 6 : color = 0.5 * texture(u_Textures[ 6], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 7 : color = 0.5 * texture(u_Textures[ 7], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 8 : color = 0.5 * texture(u_Textures[ 8], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 9 : color = 0.5 * texture(u_Textures[ 9], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 10: color = 0.5 * texture(u_Textures[10], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 11: color = 0.5 * texture(u_Textures[11], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 12: color = 0.5 * texture(u_Textures[12], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 13: color = 0.5 * texture(u_Textures[13], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 14: color = 0.5 * texture(u_Textures[14], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 15: color = 0.5 * texture(u_Textures[15], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 16: color = 0.5 * texture(u_Textures[16], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 17: color = 0.5 * texture(u_Textures[17], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 18: color = 0.5 * texture(u_Textures[18], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 19: color = 0.5 * texture(u_Textures[19], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 20: color = 0.5 * texture(u_Textures[20], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 21: color = 0.5 * texture(u_Textures[21], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 22: color = 0.5 * texture(u_Textures[22], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 23: color = 0.5 * texture(u_Textures[23], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 24: color = 0.5 * texture(u_Textures[24], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 25: color = 0.5 * texture(u_Textures[25], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 26: color = 0.5 * texture(u_Textures[26], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 27: color = 0.5 * texture(u_Textures[27], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 28: color = 0.5 * texture(u_Textures[28], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 29: color = 0.5 * texture(u_Textures[29], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 30: color = 0.5 * texture(u_Textures[30], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 31: color = 0.5 * texture(u_Textures[31], vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	}



//	switch (int(vTexID))
//	{
//	case 0 : color = 0.5 * texture(u_Textures0, vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 1 : color = 0.5 * texture(u_Textures1, vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 2 : color = 0.5 * texture(u_Textures2, vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 3 : color = 0.5 * texture(u_Textures3, vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 4 : color = 0.5 * texture(u_Textures4, vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	case 5 : color = 0.5 * texture(u_Textures5, vTexCoord) + 0.5*vec4(vTexCoord, vTexID/5.0, 1); break;
//	}

}



