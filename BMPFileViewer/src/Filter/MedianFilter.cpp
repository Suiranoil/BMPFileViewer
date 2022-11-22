#include "MedianFilter.h"

#include <algorithm>
#include <vector>

#include <imgui.h>

MedianFilter::MedianFilter()
	: ImageFilter("Median")
{
}

Image MedianFilter::Apply(const Image& source)
{
	Image result = source;

	std::vector<uint8_t> reds;
	std::vector<uint8_t> greens;
	std::vector<uint8_t> blues;

	reds.reserve((2 * (uint64_t)m_Radius + 1) * (2 * (uint64_t)m_Radius + 1));
	greens.reserve((2 * (uint64_t)m_Radius + 1) * (2 * (uint64_t)m_Radius + 1));
	blues.reserve((2 * (uint64_t)m_Radius + 1) * (2 * (uint64_t)m_Radius + 1));

	for (uint32_t y = m_Radius; y < source.GetHeight() - m_Radius; y++)
	{
		for (uint32_t x = m_Radius; x < source.GetWidth() - m_Radius; x++)
		{
			for (int i = -m_Radius; i <= m_Radius; i++)
			{
				for (int j = -m_Radius; j <= m_Radius; j++)
				{
					uint32_t pixelX = x + j;
					uint32_t pixelY = y + i;

					reds.push_back(source.GetPixel(pixelX, pixelY).R);
					greens.push_back(source.GetPixel(pixelX, pixelY).G);
					blues.push_back(source.GetPixel(pixelX, pixelY).B);
				}
			}

			std::ranges::sort(reds);
			std::ranges::sort(greens);
			std::ranges::sort(blues);

			result.SetPixel(x, y, Pixel(reds[reds.size() / 2], greens[greens.size() / 2], blues[blues.size() / 2], source.GetPixel(x, y).A));

			reds.clear();
			greens.clear();
			blues.clear();
		}
	}

	return result;
}

void MedianFilter::Reset()
{
	m_Radius = 1;
}

void MedianFilter::OnImGuiRender()
{
	m_isDirty |= ImGui::SliderInt("Radius", &m_Radius, 1, 10);
}
