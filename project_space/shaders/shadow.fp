
//varying vec4 gl_Position;
varying vec4 vPos;
varying vec3 normal;
varying vec3 lightVec;
varying vec3 eyeVec;

vec4 getPhongLighting(vec3 vpos, vec3 n)
{// just do color calculation in the phong`s way
	// the material properties are embedded in the shader (for now)
	vec4 mat_ambient = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 mat_diffuse = vec4(0.7, 0.90, 0.9, 1.0);
	vec4 mat_specular = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);
	// ambient term
	vec4 ambient = mat_ambient * gl_LightSource[0].ambient;	
	// diffuse color
	vec4 kd = mat_diffuse * gl_LightSource[0].diffuse;	
	// specular color
	vec4 ks = mat_specular * gl_LightSource[0].specular;
	// diffuse term
	vec3 lightDir = normalize(lightVec - vpos.xyz);
	float NdotL = dot(n, lightDir);
	if (NdotL > 0.0)
		diffuse = kd * NdotL;
	
	// specular term
	vec3 rVector = normalize(2.0 * n * dot(n, lightDir) - lightDir.xyz);
	vec3 viewVector = normalize(eyeVec - vpos.xyz);
	float RdotV = dot(rVector, viewVector);
	if(dot(n, lightDir) > 0.0 && RdotV > 0.0){
		specular = ks * pow(RdotV, 100);
	}
	specular *= 0;
	vec4 fColor = ambient + diffuse + specular;
	return fColor;
}
void main()
{		
 	vec4 color = vec4(1, 1, 1, 1.0);

	gl_FragColor = color * getPhongLighting(vPos.xyz, normalize(normal));
	//gl_FragColor = color * getPhongLighting(gl_Position.xyz, normalize(normal));
}

