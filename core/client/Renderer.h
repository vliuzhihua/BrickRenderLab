#pragma once
#include "Camera.h"
#include<stdint.h>

namespace brick {

class Renderer
{
public:
	void OnWindowSizeChange(uint16_t width, uint16_t height);
	void PrepareRender();
	void Render(const Camera& cam);

private:
	uint16_t m_window_width, m_window_height;
	

};
}