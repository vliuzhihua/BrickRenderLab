#version 450 core
out vec4 FragColor;

in vec4 VO_PositionClipSpace;
in vec4 VO_LocalNormal;
in vec4 VO_Texcoord0;

uniform sampler2D Tex;

void main()
{		
	vec2 ScreenUV = (VO_PositionClipSpace.xy / VO_PositionClipSpace.w) * 0.5 + 0.5;
	FragColor = vec4(1.0);
	FragColor = texture(Tex, ScreenUV);
	FragColor = texture(Tex, VO_Texcoord0.xy);
	// FragColor.xy = VO_Texcoord0.xy;
	// FragColor.xyz = VO_LocalNormal.xyz;
	//FragColor = vec4(ScreenUV.x, ScreenUV.y, 0, 0);
	// FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}

