#include "Renderer.h"
#include "glad/glad.h"
#include <vector>
#include "GLFW/glfw3.h"
#include "loadShader.h"
#include "FreeImage.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include "Model.h"
#include "RenderNode.h"
#include "Mesh.h"
#include "Texture.h"

FModel* Model = nullptr;
Texture* AlbedoTex = nullptr;
Texture* NormalTex = nullptr;
Texture* AoTex = nullptr;
Texture* RoughnessTex = nullptr;
Texture* SpecularTex = nullptr;

void Renderer::OnWindowSizeChange(uint16_t width, uint16_t height)
{
	m_window_width = width;
	m_window_height = height;
}

static void ImmediateRenderModel(FModel* Model) {
	Model->GetRenderNode()->Render();
}

void SetTexture(unsigned int SlotIndex, const char* ParamName, Texture* Tex, FShader* Shader) {
	glActiveTexture(GL_TEXTURE0 + SlotIndex);
	if (Tex && Tex->IsInited())
		glBindTexture(GL_TEXTURE_2D, Tex->GetResourceID());
	Shader->SetInt(ParamName, SlotIndex);
}

void Renderer::PrepareRender()
{
	//std::string file_name = "res/models/brick_rough_ue4ifa0va/ue4ifa0va__LOD0.obj";
	//std::string dir_name = "res/models/IronMan/IronMan.obj";
	Model = new FModel();
	Model->Init("res/models/edible_fruit_ujcxeblva/ujcxeblva__LOD0.obj");

	AlbedoTex = new Texture();
	AlbedoTex->Init("res\\models\\edible_fruit_ujcxeblva\\ujcxeblva_4K_Albedo.jpg");

	NormalTex = new Texture();
	NormalTex->Init("res\\models\\edible_fruit_ujcxeblva\\ujcxeblva_4K_NormalLOD0.jpg");

	RoughnessTex = new Texture();
	RoughnessTex->Init("res\\models\\edible_fruit_ujcxeblva\\ujcxeblva_4K_RMD.png");

	SpecularTex = new Texture();
	SpecularTex->Init("res\\models\\edible_fruit_ujcxeblva\\ujcxeblva_4K_Specular.jpg");
}

void Renderer::Render(const Camera& cam)
{
	glClearColor(0.176, 0.2666, 0.3215, 0);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static FShader* Shader = FShaderManager::Instance().CreateShader("shaders\\test\\test_vertex.glsl", "shaders\\test\\test_frag.glsl");
	FShader* RealShader = Shader->Use();

	float Ratio = m_window_width / (float)m_window_height;

	Eigen::Vector3f center = cam.m_position + cam.m_axis[Camera::FORWARD];
	glm::vec3 Eye(cam.m_position[0], cam.m_position[1], cam.m_position[2]);
	glm::vec3 Center(center[0], center[1], center[2]);
	glm::vec3 Up(cam.m_axis[Camera::UP][0], cam.m_axis[Camera::UP][1], cam.m_axis[Camera::UP][2]);
	glm::mat4x4 ViewMat = glm::lookAt(Eye, Center, Up);

	RealShader->SetMatrix4x4("_ViewMatrix", &ViewMat[0].x);

	glm::mat4x4 ProjectionMat = glm::perspective(glm::radians(45.0f), Ratio, 0.1f, 100.0f);
	RealShader->SetMatrix4x4("_ProjectionMatrix", &ProjectionMat[0].x);

	float CameraPosition[4] = { Eye.x, Eye.y, Eye.z, 1.0 };
	RealShader->SetFloat4("_CameraPosition", CameraPosition);

	SetTexture(0, "AlbedoTex", AlbedoTex, RealShader);
	SetTexture(1, "NormalTex", NormalTex, RealShader);
	SetTexture(2, "RoughnessTex", RoughnessTex, RealShader);
	SetTexture(3, "SpecularTex", SpecularTex, RealShader);

	ImmediateRenderModel(Model);
	//glDrawElements(GL_TRIANGLES, shapes[0].mesh.indices.size(), GL_UNSIGNED_INT, 0);

}