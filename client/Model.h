#pragma once

class FMesh;
class FRenderNode;
class FMaterial;

class FModel {
public:
	void Init(const char* ModelPath);
	void PrepareRenderNode();
	FRenderNode* GetRenderNode() { return mRenderNode; }
private:
	FMesh* mMesh = nullptr;
	FMaterial* mMaterial = nullptr;
	FRenderNode* mRenderNode = nullptr;
};
