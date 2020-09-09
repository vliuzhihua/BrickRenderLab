#include "glad/glad.h"
//#include<glut.h>
#include<iostream>

GLhandleARB linkToProgram(GLhandleARB vertexShader,  GLhandleARB fragShader){
	GLhandleARB shaderPro = glCreateProgram();
	//glAttachObjectARB(shaderPro, vertexShader);
	glAttachShader(shaderPro, vertexShader);
	glAttachShader(shaderPro, fragShader);
	glLinkProgram(shaderPro);
	GLint linked;
	glGetProgramiv(shaderPro, GL_LINK_STATUS, &linked);

	if (!linked) {
		char InfoLog[512];
		glGetProgramInfoLog(shaderPro, 512, NULL, InfoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << InfoLog << std::endl;
	}
	return shaderPro;
}
GLhandleARB loadShader(char* filename, unsigned int type)
{
	FILE *pfile;
	GLhandleARB handle;
	const GLcharARB* files[1];
	
	// shader Compilation variable
	GLint Success;				// Compilation code result
	GLint errorLoglength ;
	char* errorLogText;
	GLsizei actualErrorLogLength;
	
	char buffer[400000];
	memset(buffer,0,400000);
	
	// This will raise a warning on MS compiler
	fopen_s(&pfile, filename, "rb");
	if(!pfile)
	{
		printf("Sorry, can't open file: '%s'.\n", filename);
		exit(0);
	}
	
	fread(buffer,sizeof(char),400000,pfile);
	//printf("%s\n",buffer);
	fclose(pfile);
	handle = glCreateShader(type);
	if (!handle){
		//We have failed creating the vertex shader object.
		printf("Failed creating vertex shader object from file: %s.",filename);
		exit(0);
	}	
	files[0] = (const GLcharARB*)buffer;
	glShaderSource(
					  handle, //The handle to our shader
					  1, //The number of files.
					  files, //An array of const char * data, which represents the source code of theshaders
					  NULL);	
	glCompileShader(handle);
	//Compilation checking.
	glGetShaderiv(handle, GL_COMPILE_STATUS, &Success);
	// If an error was detected.
	if (!Success)	{
		//We failed to compile.
		char infoLog[512];
		printf("Shader '%s' failed compilation.\n",filename);
		//Attempt to get the length of our error log.
		glGetShaderInfoLog(handle, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return handle;
}