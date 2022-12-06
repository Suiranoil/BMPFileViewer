#pragma once

#include "Filter/FilterStack.h"
#include "Image/Image.h"
#include "Image/OpenGLTexture.h"

#include <memory>

class ImageTab
{
private:
	Image m_OriginalImage;
	Image m_Image;
	OpenGLTexture m_Texture;

	bool m_ImageChanged = false;

	float m_OffsetX = 0.0f;
	float m_OffsetY = 0.0f;
	float m_Zoom = 1.0f;

	FilterStack m_FilterStack;

	std::string m_Name;

public:
	explicit ImageTab(std::string_view path);
	~ImageTab() = default;

	[[nodiscard]] const std::string& GetName() const { return m_Name; }

	void SaveImage(std::string_view path) const;

	void ImGuiRender();
};
