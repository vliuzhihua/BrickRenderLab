#pragma once

class FRenderNode;

struct FVertex {
	float Position[3];
	float Normal[3];
	float UV[2];
	float Color[4];
};

class FMesh {
public:
	void LoadFromObj(const char* BasePath, const char* FileName);
	size_t GetVerticesNum() const { return mVerticesNum; }
	FVertex* GetVertices() const { return mVertices; }
private:
	FVertex* mVertices = nullptr;
	size_t mVerticesNum = 0;
	FRenderNode* mRenderNode = nullptr;

};
