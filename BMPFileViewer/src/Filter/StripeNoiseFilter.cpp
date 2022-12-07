#include "StripeNoiseFilter.h"

#include <random>

#include <imgui.h>

StripeNoiseFilter::StripeNoiseFilter()
	: ImageFilter("Stripe Noise")
{
}

Image StripeNoiseFilter::Apply(const Image& source)
{
	Image result = source;

	std::mt19937_64 rng(m_Seed);
	std::uniform_real_distribution coordDist(0.0f, 1.0f);
	std::uniform_int_distribution valDist(m_Min, m_Max);

	uint32_t width = m_Direction == Direction::Vertical ? source.GetWidth() : source.GetHeight();
	uint32_t height = m_Direction == Direction::Vertical ? source.GetHeight() : source.GetWidth();

	uint32_t lastCoord = -1;
	for (uint32_t i = 0; i < width; i++)
	{
		if (coordDist(rng) <= m_Amount)
		{
			if (lastCoord == i - 1 && i > 0)
				continue;

			lastCoord = i;

			int r = valDist(rng);
			int g;
			int b;

			if (m_NoiseSpecificChannels)
			{
				g = valDist(rng);
				b = valDist(rng);
			}

			for (uint32_t j = 0; j < height; j++)
			{
				Pixel pixel = m_Direction == Direction::Vertical ? source.GetPixel(i, j) : source.GetPixel(j, i);

				if (m_NoiseSpecificChannels)
				{
					if (m_NoiseRed)
						pixel.R = (uint8_t)std::clamp(pixel.R + r, 0, 255);

					if (m_NoiseGreen)
						pixel.G = (uint8_t)std::clamp(pixel.G + g, 0, 255);

					if (m_NoiseBlue)
						pixel.B = (uint8_t)std::clamp(pixel.B + b, 0, 255);
				}
				else
				{
					pixel.R = (uint8_t)std::clamp(pixel.R + r, 0, 255);
					pixel.G = (uint8_t)std::clamp(pixel.G + r, 0, 255);
					pixel.B = (uint8_t)std::clamp(pixel.B + r, 0, 255);
				}

				if (m_Direction == Direction::Vertical)
					result.SetPixel(i, j, pixel);
				else
					result.SetPixel(j, i, pixel);
			}
		}
	}

	return result;
}

void StripeNoiseFilter::Reset()
{
	m_Min = -127;
	m_Max = 128;

	m_Amount = 0.0f;
	m_Seed = 0LL;

	m_NoiseSpecificChannels = false;

	m_NoiseRed = true;
	m_NoiseGreen = true;
	m_NoiseBlue = true;

	m_Direction = Direction::Vertical;
}

void StripeNoiseFilter::OnImGuiRender()
{
	ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f - 40);
	if(ImGui::SliderInt("Min", &m_Min, -255, 255))
	{
		if (m_Max < m_Min)
			m_Max = m_Min;

		m_isDirty |= true;
	}
	ImGui::SameLine();
	if (ImGui::SliderInt("Max", &m_Max, -255, 255))
	{
		if (m_Min > m_Max)
			m_Min = m_Max;

		m_isDirty |= true;
	}
	ImGui::PopItemWidth();

	m_isDirty |= ImGui::SliderFloat("Amount", &m_Amount, 0.0f, 1.0f);
	m_isDirty |= ImGui::InputScalar("Seed", ImGuiDataType_U64, &m_Seed);

	m_isDirty |= ImGui::Combo("Direction", (int*)&m_Direction, "Vertical\0Horizontal\0");

	m_isDirty |= ImGui::Checkbox("Specific Channels", &m_NoiseSpecificChannels);

	if (m_NoiseSpecificChannels)
	{
		ImGui::Indent();
		ImGui::Text("Noise Channels");
		m_isDirty |= ImGui::Checkbox("R", &m_NoiseRed);
		ImGui::SameLine();
		m_isDirty |= ImGui::Checkbox("G", &m_NoiseGreen);
		ImGui::SameLine();
		m_isDirty |= ImGui::Checkbox("B", &m_NoiseBlue);
		ImGui::Unindent();
	}
}

void StripeNoiseFilter::Serialize(toml::table& table) const
{
	table.insert("min", m_Min);
	table.insert("max", m_Max);
	table.insert("amount", m_Amount);
	table.insert("seed", (int64_t)m_Seed);
	table.insert("direction", (int)m_Direction);
	table.insert("noise_specific_channels", toml::array{ m_NoiseSpecificChannels, m_NoiseRed, m_NoiseGreen, m_NoiseBlue });
}

void StripeNoiseFilter::Deserialize(const toml::table& table)
{
	m_Min = table["min"].value_or(m_Min);
	m_Max = table["max"].value_or(m_Max);
	m_Amount = table["amount"].value_or(m_Amount);
	m_Seed = (uint64_t)table["seed"].value_or((int64_t)m_Seed);
	m_Direction = (Direction)table["direction"].value_or((int)m_Direction);

	auto& noiseSpecificChannels = *table["noise_specific_channels"].as_array();
	
	m_NoiseSpecificChannels = noiseSpecificChannels[0].value_or(m_NoiseSpecificChannels);
	m_NoiseRed = noiseSpecificChannels[1].value_or(m_NoiseRed);
	m_NoiseGreen = noiseSpecificChannels[2].value_or(m_NoiseGreen);
	m_NoiseBlue = noiseSpecificChannels[3].value_or(m_NoiseBlue);
}
