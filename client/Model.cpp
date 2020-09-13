
#include "Model.h"
#include "Mesh.h"
#include "RenderNode.h"
#include "Material.h"
#include <string>


void FModel::Init(const char* ModelPath) {
	mMesh = new FMesh();
	std::string BasePath = ModelPath;
	BasePath = BasePath.substr(0, BasePath.rfind('/'));
	mMesh->LoadFromObj(BasePath.c_str(), ModelPath);
	
	PrepareRenderNode();
}

void FModel::PrepareRenderNode() {
	if (mRenderNode != nullptr)
		delete mRenderNode;
	mRenderNode = new FRenderNode();
	mRenderNode->InitGeometry(mMesh);
}

