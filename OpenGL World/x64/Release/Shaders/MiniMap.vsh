#version 430 core
layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec4 VertexColor;

out vec4 Color;

uniform mat4 PVM;

void main()
{
	gl_Position = PVM * vec4(VertexPos, 1);
	Color = VertexColor;
}