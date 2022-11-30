#include "StatisticalFilter.h"

#include <algorithm>
#include <chrono>
#include <vector>
#include <cmath>
#include <iostream>
#include <numeric>

#include "imgui.h"

StatisticalFilter::StatisticalFilter()
	: ImageFilter("Statistical")
{
}

Image StatisticalFilter::Apply(const Image& source)
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	Image result = m_isMask ? Image(source.GetWidth(), source.GetHeight()) : source;

	if (m_isMask)
		result.Fill({0, 0, 0, 255});

	uint32_t width = m_Direction == Direction::Vertical ? source.GetWidth() : source.GetHeight();
	uint32_t height = m_Direction == Direction::Vertical ? source.GetHeight() : source.GetWidth();

	std::vector<float> meanRed(width);
	std::vector<float> meanGreen(width);
	std::vector<float> meanBlue(width);
	
	for (uint32_t i = 0; i < width; i++)
	{
		for (uint32_t j = 0; j < height; j++)
		{
			Pixel pixel = m_Direction == Direction::Vertical ? source.GetPixel(i, j) : source.GetPixel(j, i);
			
			meanRed[i] += (float)pixel.R / (float)height;
			meanGreen[i] += (float)pixel.G / (float)height;
			meanBlue[i] += (float)pixel.B / (float)height;
		}
	}

	std::vector<float> diffRed;
	std::vector<float> diffGreen;
	std::vector<float> diffBlue;

	diffRed.resize(2 * m_Radius);
	diffGreen.resize(2 * m_Radius);
	diffBlue.resize(2 * m_Radius);

	for (uint32_t i = m_Radius; i < width - m_Radius; i++)
	{
		for (int j = -m_Radius; j <= m_Radius; j++)
		{
			if (j == 0)
				continue;

			diffRed.emplace_back(meanRed[i] - meanRed[i + j]);
			diffGreen.emplace_back(meanGreen[i] - meanGreen[i + j]);
			diffBlue.emplace_back(meanBlue[i] - meanBlue[i + j]);
		}

		float avgDiffRed = std::accumulate(diffRed.begin(), diffRed.end(), 0.0f) / diffRed.size();
		float avgDiffGreen = std::accumulate(diffGreen.begin(), diffGreen.end(), 0.0f) / diffGreen.size();
		float avgDiffBlue = std::accumulate(diffBlue.begin(), diffBlue.end(), 0.0f) / diffBlue.size();

		int count = 0;
		for (uint32_t j = 0; j < 2 * m_Radius; j++)
		{
			float diff = (diffRed[j] * diffRed[j] + diffGreen[j] * diffGreen[j] + diffBlue[j] * diffBlue[j]) / (255.0f * 255.0f);
			if (diff >= m_Threshold * m_Threshold)
				++count;
		}

		bool isStripe = count > m_Radius;
		if (m_isMask)
		{
			for (uint32_t j = 0; j < height; j++)
			{
				Pixel pixel = m_Direction == Direction::Vertical ? source.GetPixel(i, j) : source.GetPixel(j, i);

				pixel.R = isStripe ? 255 : 0;
				pixel.G = isStripe ? 255 : 0;
				pixel.B = isStripe ? 255 : 0;

				if (m_Direction == Direction::Vertical)
					result.SetPixel(i, j, pixel);
				else
					result.SetPixel(j, i, pixel);
			}
		}
		else
		{
			if (isStripe)
			{
				for (uint32_t j = 0; j < height; j++)
				{
					Pixel pixel = m_Direction == Direction::Vertical ? source.GetPixel(i, j) : source.GetPixel(j, i);

					if (m_Method == Method::NeighborAverage)
					{
						Pixel pixelLeft = m_Direction == Direction::Vertical ? source.GetPixel(i - 1, j) : source.GetPixel(j, i - 1);
						Pixel pixelRight = m_Direction == Direction::Vertical ? source.GetPixel(i + 1, j) : source.GetPixel(j, i + 1);

						pixel.R = (pixelLeft.R + pixelRight.R) / 2;
						pixel.G = (pixelLeft.G + pixelRight.G) / 2;
						pixel.B = (pixelLeft.B + pixelRight.B) / 2;
					}
					else if (m_Method == Method::NeighborAverageSubtract)
					{
						pixel.R = (uint8_t)std::clamp((int)(pixel.R - avgDiffRed), 0, 255);
						pixel.G = (uint8_t)std::clamp((int)(pixel.G - avgDiffGreen), 0, 255);
						pixel.B = (uint8_t)std::clamp((int)(pixel.B - avgDiffBlue), 0, 255);
					}

					if (m_Direction == Direction::Vertical)
						result.SetPixel(i, j, pixel);
					else
						result.SetPixel(j, i, pixel);
				}
			}
		}

		diffRed.clear();
		diffGreen.clear();
		diffBlue.clear();
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

	return result;
}

void StatisticalFilter::Reset()
{
	m_Radius = 1;
	m_Threshold = 0.05f;
	m_Direction = Direction::Vertical;
	m_Method = Method::NeighborAverage;
	m_isMask = false;
}

void StatisticalFilter::OnImGuiRender()
{
	m_isDirty |= ImGui::SliderInt("Radius", &m_Radius, 1, 10);
	m_isDirty |= ImGui::SliderFloat("Threshold", &m_Threshold, 0.0f, 1.0f);
	m_isDirty |= ImGui::Combo("Direction", (int*)&m_Direction, "Vertical\0Horizontal\0");
	m_isDirty |= ImGui::Combo("Method", (int*)&m_Method, "Neighbor Average\0Neighbor Average Subtract\0");
	m_isDirty |= ImGui::Checkbox("Mask", &m_isMask);
}
