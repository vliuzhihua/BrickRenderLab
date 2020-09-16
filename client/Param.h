#pragma once
#include <variant>
#include <typeindex>
#include "glad/glad.h"
#include "Shader.h"
#include <map>

class ITexture;
//
//template<typename T>
//class MaterialParam {
//public:
//	T mData;
//	size_t mDataSize;
//	void Apply(FShader* Shader) {
//		
//	}
//};

class Matrix4x4MaterialParam {
public:
	float* mData;

};

class TextureMaterialParam {
public:
	ITexture* mTex;
	unsigned int mSlot;
	void ApplyTo(FShader* Shader) {

		//Shader->SetInt()
	}
};
