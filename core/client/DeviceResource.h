#pragma once
#include "glad/glad.h"

namespace brick
{
	class DeviceResource {
	public:

	};

	class DeviceBuffer {
	public:
		void Init(int size, void* data) {
			GLenum target = GL_UNIFORM_BUFFER;
			glGenBuffers(1, &mObject);
			glBindBuffer(target, mObject);
			glBufferData(target, size, data, GL_DYNAMIC_DRAW);
			glBindBuffer(target, 0);
		}
		GLuint GetBufferObject() { return mObject; }

	private:
		int				mSize;
		GLuint			mObject;

	};
}
