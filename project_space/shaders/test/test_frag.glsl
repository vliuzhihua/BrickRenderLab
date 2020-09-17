#version 450 core
out vec4 FragColor;

in vec4 VO_PositionClipSpace;
in vec4 VO_LocalNormal;
in vec4 VO_Texcoord0;

uniform sampler2D AlbedoTex;
uniform sampler2D NormalTex;
uniform sampler2D RoughnessTex;
uniform sampler2D SpecularTex;

void main()
{		
	vec2 ScreenUV = (VO_PositionClipSpace.xy / VO_PositionClipSpace.w) * 0.5 + 0.5;

	float PerceptualRoughness = texture(RoughnessTex, VO_Texcoord0.xy).r;

	FragColor = texture(AlbedoTex, VO_Texcoord0.xy);

	// FragColor = vec4(PerceptualRoughness);

}

