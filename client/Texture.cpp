#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "Texture.h"
#include "glad/glad.h"
#include "FreeImage.h"

std::unordered_map<std::string, FREE_IMAGE_FORMAT> ExtToImageFormat = {
	{".jpg", FREE_IMAGE_FORMAT::FIF_JPEG},
	{".jpeg", FREE_IMAGE_FORMAT::FIF_JPEG},
	{".png", FREE_IMAGE_FORMAT::FIF_PNG},
	{".bmp", FREE_IMAGE_FORMAT::FIF_BMP}
};

FREE_IMAGE_FORMAT GetImageFormatByTail(const std::string Path) {
	size_t Pos = Path.rfind('.');
	if (Pos == std::string::npos)
		return FREE_IMAGE_FORMAT::FIF_UNKNOWN;
	std::string Ext = Path.substr(Pos, std::string::npos);
	std::transform(Ext.begin(), Ext.end(), Ext.begin(), ::tolower);
	auto Ret = ExtToImageFormat.find(Ext);
	if (Ret != ExtToImageFormat.end())
		return Ret->second;
	return FREE_IMAGE_FORMAT::FIF_UNKNOWN;
}

Texture::~Texture() {
	glDeleteTextures(1, &mID);
}

void Texture::Init(const char* Path) {

	FREE_IMAGE_FORMAT ImageFormat = GetImageFormatByTail(Path);
	if (ImageFormat == FREE_IMAGE_FORMAT::FIF_UNKNOWN) {
		std::cout << "Image Type Unknow" << std::endl;
		return;
	}

	FIBITMAP* Img = FreeImage_Load(GetImageFormatByTail(Path), Path);
	auto ImgWidth = FreeImage_GetWidth(Img);
	auto ImgHeight = FreeImage_GetHeight(Img);
	BYTE* data = FreeImage_GetBits(Img);

	glGenTextures(1, &mID);
	//glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mID);
	// 为当前绑定的纹理对象设置环绕、过滤方式,不可遗漏否则采样不出来。
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//GL_BGR ?????
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImgWidth, ImgHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, (unsigned char*)data);

	FreeImage_Unload(Img);

	mInited = true;
}
