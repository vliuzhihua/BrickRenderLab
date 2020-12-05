#include "RenderNode.h"
#include "Mesh.h"

#define offsetof(s,m) (size_t) &(((s*)0)->m)

void FRenderNode::InitGeometry(const FMesh* Mesh) {
	if (IsInited()) {
		RelaseGeometry();
	}

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	mVerticesNum = Mesh->GetVerticesNum();

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Mesh->GetVerticesNum() * sizeof(FVertex), Mesh->GetVertices(), GL_STATIC_DRAW);
	//position location 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(FVertex), (void*)offsetof(FVertex, Position) );
	glEnableVertexAttribArray(0);
	//normal location 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(FVertex), (void*)offsetof(FVertex, Normal) );
	glEnableVertexAttribArray(1);
	//uv location 2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(FVertex), (void*)offsetof(FVertex, UV) );
	glEnableVertexAttribArray(2);
	
	glBindVertexArray(0);
}

void FRenderNode::RelaseGeometry() {
	glDeleteVertexArrays(1, &mVAO);
	mVAO = 0;
}
void FRenderNode::Render() {
	if (IsInited()) {
		glBindVertexArray(mVAO);
		glDrawArrays(GL_TRIANGLES, 0, mVerticesNum);
	}
}