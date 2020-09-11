#version 450 core
out vec4 FragColor;

in vec4 In_PositionClipSpace;

uniform sampler2D Tex;

void main()
{		
	vec2 ScreenUV = (In_PositionClipSpace.xy / In_PositionClipSpace.w) * 0.5 + 0.5;
	FragColor = texture(Tex, ScreenUV);
	//FragColor = vec4(ScreenUV.x, ScreenUV.y, 0, 0);
	//FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}

