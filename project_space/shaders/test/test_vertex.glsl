#version 450 core
layout (location = 0) in vec3 In_LocalPosition;
layout (location = 1) in vec3 In_LocalNormal;
layout (location = 2) in vec2 In_Texcoord0;
layout (location = 3) in vec3 In_LocalColor;

uniform mat4 _ViewMatrix;
uniform mat4 _ProjectionMatrix;

out vec4 VO_PositionClipSpace;
out vec4 VO_LocalNormal;
out vec4 VO_Texcoord0;
out vec3 VO_PositionWorldSpace;


void main()
{
	VO_PositionClipSpace = _ProjectionMatrix * _ViewMatrix * vec4(In_LocalPosition, 1.0);
	VO_LocalNormal.xyz = In_LocalNormal.xyz;
	VO_Texcoord0.xy = In_Texcoord0.xy;
	VO_PositionWorldSpace.xyz = In_LocalPosition;
	gl_Position = VO_PositionClipSpace;
}	

