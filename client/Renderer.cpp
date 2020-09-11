#include "Renderer.h"
#include "glad/glad.h"
#include <vector>
#include "tiny_obj_loader.h"
#include "GLFW/glfw3.h"
#include "loadShader.h"
#include "FreeImage.h"
#include "glm/glm.hpp"
//#include <glm/ext/matrix_clip_space.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"

GLuint vertexBO, texcoordBO, normalBO, indexBO, ColorBO;
tinyobj::attrib_t attrib;
std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;
GLuint Tex1;

GLuint VBO;
GLuint VAO;

void Renderer::OnWindowSizeChange(uint16_t width, uint16_t height)
{
	m_window_width = width;
	m_window_height = height;
}

void Renderer::PrepareRender()
{
	std::string warn, err;
	std::string file_name = "res/models/brick_rough_ue4ifa0va/ue4ifa0va__LOD0.obj";
	std::string dir_name = "res/models/brick_rough_ue4ifa0va";

	bool re = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file_name.c_str(), dir_name.c_str());

	glGenBuffers(1, &vertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBO);
	glBufferData(GL_ARRAY_BUFFER, attrib.vertices.size() * sizeof(GLfloat), attrib.vertices.data(), GL_STREAM_DRAW);

	glGenBuffers(1, &texcoordBO);
	glBindBuffer(GL_ARRAY_BUFFER, texcoordBO);
	glBufferData(GL_ARRAY_BUFFER, attrib.texcoords.size() * sizeof(GLfloat), attrib.texcoords.data(), GL_STREAM_DRAW);

	glGenBuffers(1, &normalBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalBO);
	glBufferData(GL_ARRAY_BUFFER, attrib.normals.size() * sizeof(GLfloat), attrib.normals.data(), GL_STREAM_DRAW);

	glGenBuffers(1, &ColorBO);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBO);
	glBufferData(GL_ARRAY_BUFFER, attrib.colors.size() * sizeof(GLfloat), attrib.colors.data(), GL_STREAM_DRAW);

	unsigned int* index = new unsigned int[shapes[0].mesh.indices.size()];
	for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
	{
		auto idx = shapes[0].mesh.indices[i];
		if (idx.normal_index != idx.vertex_index)
			int a = 10;
		index[i] = shapes[0].mesh.indices[i].vertex_index;
	}

	glGenBuffers(1, &indexBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[0].mesh.indices.size() * sizeof(unsigned int), index, GL_STATIC_DRAW);


	FIBITMAP* Img = FreeImage_Load(FREE_IMAGE_FORMAT::FIF_JPEG, "res\\models\\brick_rough_ue4ifa0va\\ue4ifa0va_4K_Albedo.jpg");
	//FIBITMAP* Img = FreeImage_Load(FREE_IMAGE_FORMAT::FIF_PNG, "res\\models\\brick_rough_ue4ifa0va\\preview.png");
	auto ImgWidth = FreeImage_GetWidth(Img);
	auto ImgHeight = FreeImage_GetHeight(Img);
	BYTE* data = FreeImage_GetBits(Img);
	
	glGenTextures(1, &Tex1);
	//glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Tex1);
	// 为当前绑定的纹理对象设置环绕、过滤方式,不可遗漏否则采样不出来。
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImgWidth, ImgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char*)data);

	FreeImage_Unload(Img);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
GLhandleARB vertexShader = -1;
GLhandleARB fragShader = -1;
GLhandleARB shaderPro = -1;

void Renderer::Render(const Camera& cam)
{

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	FShader Shader("shaders\\test\\test_vertex.glsl", "shaders\\test\\test_frag.glsl");
	Shader.Use();
	glBindVertexArray(VAO);

	//glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);

	//glCreateTextures()
	float Ratio = m_window_width / (float)m_window_height;

	std::cout << cam.m_position[0] << " " << cam.m_position[1] << " " << cam.m_position[2] << std::endl;

	Eigen::Vector3f center = cam.m_position + cam.m_axis[Camera::FORWARD];
	glm::vec3 Eye(cam.m_position[0], cam.m_position[1], cam.m_position[2]);
	glm::vec3 Center(center[0], center[1], center[2]);
	glm::vec3 Up(cam.m_axis[Camera::UP][0], cam.m_axis[Camera::UP][1], cam.m_axis[Camera::UP][2]);
	glm::mat4x4 ViewMat = glm::lookAt(Eye, Center, Up);
	
	Shader.SetMatrix4x4("_ViewMatrix", &ViewMat[0].x);

	glm::mat4x4 ProjectionMat = glm::perspective(glm::radians(45.0f), Ratio, 0.1f, 100.0f);
	Shader.SetMatrix4x4("_ProjectionMatrix", &ProjectionMat[0].x);

	//glActiveTexture(GL_TEXTURE1);
	glActiveTexture(1);
	glBindTexture(GL_TEXTURE_2D, Tex1);

	Shader.SetInt("Tex", 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);


	////add the uniform info to shader
	//glUseProgram(shaderPro);
	//glUniform4f(glGetUniformLocation(shaderPro, "eyePos"), cam.m_position.x(), cam.m_position.y(), cam.m_position.z(), 0);
	//GLfloat matrix[16];
	//glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	//glUniformMatrix4fv(glGetUniformLocation(shaderPro, "matrixM"), 1, 0, matrix);

	////GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS

	////清除缓冲区的。
	//glClearColor(0.0, 0.0, 0.0, 255);
	//glEnable(GL_DEPTH_TEST);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//

	////glutWireSphere(10, 10, 10);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexBO);
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, 0);

	//glBindBuffer(GL_ARRAY_BUFFER, normalBO);
	//glEnableClientState(GL_NORMAL_ARRAY);
	//glNormalPointer(GL_FLOAT, 0, 0);

	//glBindBuffer(GL_ARRAY_BUFFER, texcoordBO);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glTexCoordPointer(2, GL_FLOAT, 0, 0);
	////glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ColorBO);
	//glEnableClientState(GL_COLOR_ARRAY);
	//glColorPointer(3, GL_FLOAT, 0, 0);
	////glColorP

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO);
	//glEnableClientState(GL_INDEX_ARRAY);
	//glIndexPointer(GL_UNSIGNED_INT, 0, 0);

	////glVertexAttribPointer()

	////glDrawArrays(GL_QUADS, 0, attrib.vertices.size());
	////glDrawBuffer(GL_QUADS);
	//glDrawElements(GL_TRIANGLES, shapes[0].mesh.indices.size(), GL_UNSIGNED_INT, 0);
	////glDrawArrays(GL_TRIANGLES, 0, attrib.vertices.size());

	//glDisableClientState(GL_INDEX_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_COLOR_ARRAY);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}