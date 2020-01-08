#version 330 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Color;
layout (location = 2) in vec2 a_Texcoord;

out vec2 TexCoord;
out vec3 Color;

void main()
{
	gl_Position = vec4(a_Pos, 1.0f);
	TexCoord = a_Texcoord;
	Color = a_Color;
}