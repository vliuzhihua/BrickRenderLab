#version 450 core
layout (location = 0) in vec3 In_LocalPosition;

uniform mat4 _ViewMatrix;
uniform mat4 _ProjectionMatrix;

out vec4 In_PositionClipSpace;

void main()
{
	In_PositionClipSpace = _ProjectionMatrix * _ViewMatrix * vec4(In_LocalPosition, 1.0);
	gl_Position = In_PositionClipSpace;
}	

