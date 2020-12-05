#pragma once
#include "glad/glad.h"

class FMesh;

class FRenderNode {
public:
	bool IsInited() { return mVAO != 0; }
	void InitGeometry(const FMesh*);
	void RelaseGeometry();
	void Render();
private:
	GLuint mVAO = 0;
	size_t mVerticesNum = 0;
};
