
varying vec4 vPos;
varying vec3 normal;
varying vec2 uv;
varying vec3 VertexColor;

uniform mat4 matrixM;
uniform vec4 eyePos;

void main()
{
	normal = normalize(gl_NormalMatrix * gl_Normal);
	vPos = gl_ModelViewMatrix * gl_Vertex;
	uv = gl_MultiTexCoord0.xy;
	VertexColor = gl_Color.xyz;
	//lightVec = vec3(gl_ModelViewMatrix * vec4(0, 0, 0, 0));
	//eyeVec = vec3(gl_ModelViewMatrix * vec4(0, 0, 0, 0));
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	//gl_Position = ftransform();
}	

