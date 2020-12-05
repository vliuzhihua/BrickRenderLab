#include <iostream>
#include "Mesh.h"
#include "RenderNode.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

void FMesh::LoadFromObj(const char* BasePath, const char* FilePath) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, FilePath, BasePath);

	if (!warn.empty()) {
		std::cout << "WARN: " << warn << std::endl;
	}

	if (!err.empty()) {
		std::cerr << "ERR: " << err << std::endl;
	}

	if (!ret) {
		std::cout << "LoadFromObj Failed" << FilePath << std::endl;
		return;
	}
	std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
	std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
	std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2)
		<< std::endl;

	std::cout << "# of shapes    : " << shapes.size() << std::endl;
	std::cout << "# of materials : " << materials.size() << std::endl;

	//load shape
	for (size_t i = 0; i < shapes.size(); i++) {
		// 这部分的名称
		printf("shape[%ld].name = %s\n", static_cast<long>(i),
			shapes[i].name.c_str());
		// 网格的点数
		printf("Size of shape[%ld].mesh.indices: %lu\n", static_cast<long>(i),
			static_cast<unsigned long>(shapes[i].mesh.indices.size()));

		printf("shape[%ld].num_faces: %lu\n", static_cast<long>(i),
			static_cast<unsigned long>(shapes[i].mesh.num_face_vertices.size()));

		// 顶点数量  = face的数量x3 
		mVerticesNum = shapes[i].mesh.num_face_vertices.size() * 3;
		// 开辟空间
		mVertices = new FVertex[mVerticesNum];
		size_t index_offset = 0;
		// For each face
		for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
			size_t fnum = shapes[i].mesh.num_face_vertices[f];
			// 获得所索引下标
			tinyobj::index_t idx;
			int vertex_index[3];
			int normal_index[3];
			int texcoord_index[3];
			for (size_t v = 0; v < fnum; v++) {
				idx = shapes[i].mesh.indices[index_offset + v];
				vertex_index[v] = idx.vertex_index;
				texcoord_index[v] = idx.texcoord_index;
				normal_index[v] = idx.normal_index;
			}
			for (size_t v = 0; v < fnum; v++) {
				// v
				mVertices[index_offset + v].Position[0] = attrib.vertices[(vertex_index[v]) * 3 + 0];
				mVertices[index_offset + v].Position[1] = attrib.vertices[(vertex_index[v]) * 3 + 1];
				mVertices[index_offset + v].Position[2] = attrib.vertices[(vertex_index[v]) * 3 + 2];

				// vn
				mVertices[index_offset + v].Normal[0] = attrib.normals[normal_index[v] * 3 + 0];
				mVertices[index_offset + v].Normal[1] = attrib.normals[normal_index[v] * 3 + 1];
				mVertices[index_offset + v].Normal[2] = attrib.normals[normal_index[v] * 3 + 2];

				// vt
				mVertices[index_offset + v].UV[0] = attrib.texcoords[texcoord_index[v] * 2 + 0];
				mVertices[index_offset + v].UV[1] = attrib.texcoords[texcoord_index[v] * 2 + 1];

				// color
				mVertices[index_offset + v].Color[0] = 1.0f;
				mVertices[index_offset + v].Color[1] = 1.0f;
				mVertices[index_offset + v].Color[2] = 1.0f;
				mVertices[index_offset + v].Color[3] = 1.0f;
			}

			// 偏移
			index_offset += fnum;
		}

	}
}

