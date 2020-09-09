#version 450 core
layout (location = 0) in vec3 LocalPosition;



void main()
{
	gl_Position = vec4(LocalPosition, 1.0);
}	

