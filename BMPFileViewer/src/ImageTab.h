#pragma once

#include "Filter/ImageFilter.h"
#include "Image/Image.h"
#include "Image/OpenGLTexture.h"

#include <memory>
#include <vector>

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

	std::vector<std::unique_ptr<ImageFilter>> m_ImageFilters;

	uint32_t m_FreeFilterID = 0;
	std::vector<uint32_t> m_FilterIDs;

	std::string m_Name;

public:
	explicit ImageTab(std::string_view path);
	~ImageTab() = default;

	[[nodiscard]] const std::string& GetName() const { return m_Name; }

	void SaveImage(std::string_view path) const;

	void ImGuiRender();

private:
	template<typename T, typename... Args>
	void AddFilter(Args&&... args)
	{
		m_ImageFilters.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		m_FilterIDs.emplace_back(m_FreeFilterID++);

		m_ImageChanged = true;
	}
};
