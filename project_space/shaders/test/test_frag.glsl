#version 450 core
out vec4 FragColor;

in vec4 VO_PositionClipSpace;
in vec4 VO_LocalNormal;
in vec4 VO_Texcoord0;
in vec3 VO_PositionWorldSpace;

uniform sampler2D AlbedoTex;
uniform sampler2D NormalTex;
uniform sampler2D RoughnessTex;
uniform sampler2D SpecularTex;

uniform vec4 _CameraPosition;


const float PI = 3.14159265358979323846;

float sqr(float x) { return x*x; }

float SchlickFresnel(float u)
{
    float m = clamp(1-u, 0, 1);
    float m2 = m*m;
    return m2*m2*m; // pow(m,5)
}

float GTR1(float NdotH, float a)
{
    if (a >= 1) return 1/PI;
    float a2 = a*a;
    float t = 1 + (a2-1)*NdotH*NdotH;
    return (a2-1) / (PI*log(a2)*t);
}

float GTR2(float NdotH, float a)
{
    float a2 = a*a;
    float t = 1 + (a2-1)*NdotH*NdotH;
    return a2 / (PI * t*t);
}

float GTR2_aniso(float NdotH, float HdotX, float HdotY, float ax, float ay)
{
    return 1 / (PI * ax*ay * sqr( sqr(HdotX/ax) + sqr(HdotY/ay) + NdotH*NdotH ));
}

float SmithG_GGX(float NdotV, float alphaG)
{
    float a = alphaG*alphaG;
    float b = NdotV*NdotV;
    return 1 / (NdotV + sqrt(a + b - a*b));
}

float SmithG_GGX_aniso(float NdotV, float VdotX, float VdotY, float ax, float ay)
{
    return 1 / (NdotV + sqrt( sqr(VdotX*ax) + sqr(VdotY*ay) + sqr(NdotV) ));
}

vec3 GammaToLinear(vec3 x)
{
    return vec3(pow(x[0], 2.2), pow(x[1], 2.2), pow(x[2], 2.2));
}

// vec3 BRDF( vec3 L, vec3 V, vec3 N, vec3 X, vec3 Y )
// {
//     float NdotL = dot(N,L);
//     float NdotV = dot(N,V);
//     if (NdotL < 0 || NdotV < 0) return vec3(0);

//     vec3 H = normalize(L+V);
//     float NdotH = dot(N,H);
//     float LdotH = dot(L,H);

//     vec3 Cdlin = GammaToLinear(baseColor);
//     float Cdlum = 0.3 * Cdlin[0] + 0.6*Cdlin[1]  + 0.1*Cdlin[2]; // luminance approx.

//     vec3 Ctint = Cdlum > 0 ? Cdlin / Cdlum : vec3(1.0); // normalize lum. to isolate hue+sat
//     vec3 Cspec0 = mix(specular*.08, Cdlin, metallic);
//     vec3 Csheen = mix(vec3(1), Ctint, sheenTint);

//     // Diffuse fresnel - go from 1 at normal incidence to .5 at grazing
//     // and mix in diffuse retro-reflection based on roughness
//     float FL = SchlickFresnel(NdotL), FV = SchlickFresnel(NdotV);
//     float Fd90 = 0.5 + 2 * LdotH*LdotH * roughness;
//     float Fd = mix(1.0, Fd90, FL) * mix(1.0, Fd90, FV);

//     // Based on Hanrahan-Krueger brdf approximation of isotropic bssrdf
//     // 1.25 scale is used to (roughly) preserve albedo
//     // Fss90 used to "flatten" retroreflection based on roughness
//     float Fss90 = LdotH*LdotH*roughness;
//     float Fss = mix(1.0, Fss90, FL) * mix(1.0, Fss90, FV);
//     float ss = 1.25 * (Fss * (1 / (NdotL + NdotV) - .5) + .5);

//     // specular
//     float aspect = sqrt(1-Anisotropic*.9);
//     float ax = max(.001, sqr(roughness)/aspect);
//     float ay = max(.001, sqr(roughness)*aspect);
//     float Ds = GTR2_aniso(NdotH, dot(H, X), dot(H, Y), ax, ay);
//     float FH = SchlickFresnel(LdotH);
//     vec3 Fs = mix(Cspec0, vec3(1), FH);
//     float Gs;
//     Gs  = SmithG_GGX_aniso(NdotL, dot(L, X), dot(L, Y), ax, ay);
//     Gs *= SmithG_GGX_aniso(NdotV, dot(V, X), dot(V, Y), ax, ay);

//     // sheen
//     vec3 Fsheen = FH * sheen * Csheen;

//     // clearcoat (ior = 1.5 -> F0 = 0.04)
//     float Dr = GTR1(NdotH, mix(.1,.001,clearcoatGloss));
//     float Fr = mix(.04, 1.0, FH);
//     float Gr = SmithG_GGX(NdotL, .25) * SmithG_GGX(NdotV, .25);

//     return ((1/PI) * mix(Fd, ss, subsurface)*Cdlin + Fsheen)
//         * (1-metallic)
//         + Gs*Fs*Ds + .25*clearcoat*Gr*Fr*Dr;
// }

#include "shaders\\test\\test_common.glsl"

void main()
{	
	float SunLightIrradiance = 1.0;
	vec3 SunLightDirection = normalize(vec3(0.5, 0.5, -0.5));
	float PerceptualRoughness = texture(RoughnessTex, VO_Texcoord0.xy).r;
	float LinearRoughness = sqr(PerceptualRoughness);
	vec3 AlbedoColor = texture(AlbedoTex, VO_Texcoord0.xy).rgb;
	vec3 SpecularColor = texture(SpecularTex, VO_Texcoord0.xy).rgb;
	vec3 NormalFactor = texture(NormalTex, VO_Texcoord0.xy).xyz;

	vec3 ViewDirection = normalize(_CameraPosition.xyz - VO_PositionWorldSpace);

	vec3 H = normalize(SunLightDirection + ViewDirection);

	float NdL = max(dot(VO_LocalNormal.xyz, SunLightDirection), 0.0);
	float NdV = max(dot(VO_LocalNormal.xyz, ViewDirection), 0.0);
	float LdH = max(dot(SunLightDirection, H), .0);
	float NdH = max(dot(VO_LocalNormal.xyz, H), .0);

	//Diffuse
	float FL = SchlickFresnel(NdL), FV = SchlickFresnel(NdV);
	float Fd90 = 0.5 + 2 * LdH * LdH * LinearRoughness;
	float Fd = mix(1.0, Fd90, FL) * mix(1.0, Fd90, FV);
	vec3 Diffuse = AlbedoColor * NdL;

    //Specular
    float Ds = GTR2(NdH, LinearRoughness);
    float FH = SchlickFresnel(LdH);
    vec3 Fs = mix(SpecularColor, vec3(1), FH);
    float Gs = SmithG_GGX(NdV, LinearRoughness) * SmithG_GGX(NdL, LinearRoughness);
	vec3 Specular = Fs * Gs * Ds;// * NdL;

	vec3 FinalColor = Diffuse + Specular;

	FragColor = texture(AlbedoTex, VO_Texcoord0.xy) * Fd;
	FragColor = vec4(Fd) * 0.5;
	FragColor = vec4(FinalColor * SunLightIrradiance, 1.0);
	// FragColor = B;
	// FragColor = vec4(Specular, 1.0);
	// FragColor = vec4(SpecularColor, 1.);
	// FragColor = vec4(NdL);

	// FragColor = vec4(PerceptualRoughness);

}

