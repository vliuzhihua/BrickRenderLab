#version 450 core
layout (location = 0) in vec3 In_LocalPosition;
uniform mat4 _ViewMatrix;
uniform mat4 _ProjectionMatrix;

void main()
{
	gl_Position = _ProjectionMatrix * _ViewMatrix * vec4(In_LocalPosition, 1.0);
}	

