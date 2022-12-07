#include "ContrastFilter.h"

#include <algorithm>

#include <imgui.h>

ContrastFilter::ContrastFilter()
	: ImageFilter("Contrast")
{
}

Image ContrastFilter::Apply(const Image& source)
{
	Image result = source;

	for (uint32_t y = 0; y < source.GetHeight(); y++)
	{
		for (uint32_t x = 0; x < source.GetWidth(); x++)
		{
			auto pixel = source.GetPixel(x, y);

			pixel.R = (uint8_t)(std::clamp(((float)pixel.R / 255.0f - 0.5f) * m_Contrast + 0.5f, 0.0f, 1.0f) * 255.0f);
			pixel.G = (uint8_t)(std::clamp(((float)pixel.G / 255.0f - 0.5f) * m_Contrast + 0.5f, 0.0f, 1.0f) * 255.0f);
			pixel.B = (uint8_t)(std::clamp(((float)pixel.B / 255.0f - 0.5f) * m_Contrast + 0.5f, 0.0f, 1.0f) * 255.0f);

			result.SetPixel(x, y, pixel);
		}
	}

	return result;
}

void ContrastFilter::Reset()
{
	m_Contrast = 1.0f;
}

void ContrastFilter::OnImGuiRender()
{
	m_isDirty |= ImGui::SliderFloat("Value", &m_Contrast, 0.0f, 5.0f);
}

void ContrastFilter::Serialize(toml::table& table) const
{
	table.insert("contrast", m_Contrast);
}

void ContrastFilter::Deserialize(const toml::table& table)
{
	m_Contrast = table["contrast"].value_or(m_Contrast);
}
