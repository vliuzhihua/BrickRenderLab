#include "Renderer.h"
#include "glew.h"
#include <vector>
#include "tiny_obj_loader.h"
#include "GLFW/glfw3.h"
#include "loadShader.h"


GLuint vertexBO, texcoordBO, normalBO, indexBO;
tinyobj::attrib_t attrib;
std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;

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
}
GLhandleARB vertexShader = -1;
GLhandleARB fragShader = -1;
GLhandleARB shaderPro = -1;

void Renderer::Render(const Camera& cam)
{

	float ratio = m_window_width / (float)m_window_height;

	if (shaderPro != -1)
	{
		glDeleteObjectARB(vertexShader);
		glDeleteObjectARB(fragShader);
		glDeleteProgram(shaderPro);
	}
	vertexShader = loadShader("shaders\\vertex.glsl", GL_VERTEX_SHADER);
	fragShader = loadShader("shaders\\fragment.glsl", GL_FRAGMENT_SHADER);
	shaderPro = linkToProgram(vertexShader, fragShader);

	glViewport(0, 0, m_window_width, m_window_height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glFrustum(-30, 30, -30, 30, 50, -50);
	gluPerspective(60, ratio, 0.1, 500.0);
	//glOrtho(-30, 30, -30.f, 30.f, -50.f, 50.f);
	//glOrtho(xStart, xEnd, yStart, yEnd, zStart, zEnd);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	std::cout << cam.m_position[0] << " " << cam.m_position[1] << " " << cam.m_position[2] << std::endl;

	Eigen::Vector3f center = cam.m_position + cam.m_axis[Camera::FORWARD];

	gluLookAt(cam.m_position[0], cam.m_position[1], cam.m_position[2], center[0], center[1], center[2], cam.m_axis[Camera::UP][0], cam.m_axis[Camera::UP][1], cam.m_axis[Camera::UP][2]);

	//add the uniform info to shader
	glUseProgramObjectARB(shaderPro);
	glUniform4f(glGetUniformLocation(shaderPro, "eyePos"), cam.m_position.x(), cam.m_position.y(), cam.m_position.z(), 0);
	GLfloat matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	glUniformMatrix4fv(glGetUniformLocation(shaderPro, "matrixM"), 1, 0, matrix);

	//GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS

	//����������ġ�
	glClearColor(0.0, 0.0, 0.0, 255);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	//glutWireSphere(10, 10, 10);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBO);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, normalBO);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, 0);

	//glBindBuffer(GL_ARRAY_BUFFER, texcoordBO);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO);
	glEnableClientState(GL_INDEX_ARRAY);
	glIndexPointer(GL_UNSIGNED_INT, 0, 0);

	//glDrawArrays(GL_QUADS, 0, attrib.vertices.size());
	//glDrawBuffer(GL_QUADS);
	glDrawElements(GL_TRIANGLES, shapes[0].mesh.indices.size(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, attrib.vertices.size());

	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}