#pragma once

#include "Image.h"

#include <cstdint>

class OpenGLTexture
{
private:
	uint32_t m_RendererID;

public:
	OpenGLTexture();
	~OpenGLTexture();

	void Bind(uint32_t slot = 0) const;
	void Unbind() const;

	void UploadImage(const Image& image);

	[[nodiscard]] uint32_t GetRendererID() const { return m_RendererID; }
};
