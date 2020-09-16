#pragma once

class ITexture {
public:
	virtual ~ITexture() = 0 {};
	virtual bool IsInited() = 0;
	virtual unsigned int GetResourceID() = 0;
};

class Texture {
public:
	virtual bool IsInited() { return mInited; }
	virtual unsigned int GetResourceID() {
		return mID;
	};

	void Init(const char* Path);
	virtual ~Texture();
private:
	//gl resource id
	unsigned int mID = 0;
	bool mInited = false;

};
