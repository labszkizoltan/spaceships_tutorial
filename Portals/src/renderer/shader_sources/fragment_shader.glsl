

#version 450 core

in vec3 outColor;

uniform float alpha;

out vec4 FragColor;

void main()
{
	FragColor = vec4(outColor, 1.0f);
//	FragColor = vec4(outColor, alpha);
}






// The fragment can be discarded like this (https://learnopengl.com/Advanced-OpenGL/Blending):
// 
// void main()
// {
// 	vec4 texColor = texture(texture1, TexCoords);
// 	if (texColor.a < 0.1)
// 		discard;
// 	FragColor = texColor;
// }


