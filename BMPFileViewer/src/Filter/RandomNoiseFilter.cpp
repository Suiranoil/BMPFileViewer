#include "RandomNoiseFilter.h"

#include <random>

#include <imgui.h>

RandomNoiseFilter::RandomNoiseFilter()
	: ImageFilter("Random Noise")
{
}

Image RandomNoiseFilter::Apply(const Image& source)
{
	Image result = source;

	std::mt19937_64 rng(m_Seed);
	std::uniform_int_distribution<uint32_t> xDist(0, source.GetWidth() - 1);
	std::uniform_int_distribution<uint32_t> yDist(0, source.GetHeight() - 1);
	std::uniform_int_distribution valDist(m_Min, m_Max);
	
	for (uint32_t i = 0; i < source.GetWidth() * source.GetHeight() * m_Amount; ++i)
	{
		uint32_t x = xDist(rng);
		uint32_t y = yDist(rng);
		
		Pixel pixel = source.GetPixel(x, y);
		
		if (m_NoiseSpecificChannels)
		{
			int r = valDist(rng);
			int g = valDist(rng);
			int b = valDist(rng);

			if (m_NoiseRed)
				pixel.R = (uint8_t)std::clamp(pixel.R + r, 0, 255);

			if (m_NoiseGreen)
				pixel.G = (uint8_t)std::clamp(pixel.G + g, 0, 255);

			if (m_NoiseBlue)
				pixel.B = (uint8_t)std::clamp(pixel.B + b, 0, 255);
		}
		else
		{
			int value = valDist(rng);

			pixel.R = (uint8_t)std::clamp(pixel.R + value, 0, 255);
			pixel.G = (uint8_t)std::clamp(pixel.G + value, 0, 255);
			pixel.B = (uint8_t)std::clamp(pixel.B + value, 0, 255);
		}

		result.SetPixel(x, y, pixel);
	}

	return result;
}

void RandomNoiseFilter::Reset()
{
	m_Min = -127;
	m_Max = 128;

	m_Amount = 0.0f;
	m_Seed = 0LL;
	
	m_NoiseSpecificChannels = false;

	m_NoiseRed = true;
	m_NoiseGreen = true;
	m_NoiseBlue = true;
}

bool RandomNoiseFilter::RenderImGui()
{
	bool changed = false;

	ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f - 40);
	if(ImGui::SliderInt("Min", &m_Min, -255, 255))
	{
		if (m_Max < m_Min)
			m_Max = m_Min;

		changed = true;
	}
	ImGui::SameLine();
	if (ImGui::SliderInt("Max", &m_Max, -255, 255))
	{
		if (m_Min > m_Max)
			m_Min = m_Max;

		changed = true;
	}
	ImGui::PopItemWidth();

	changed |= ImGui::SliderFloat("Amount", &m_Amount, 0.0f, 1.0f);
	changed |= ImGui::InputScalar("Seed", ImGuiDataType_U64, &m_Seed);

	changed |= ImGui::Checkbox("Specific Channels", &m_NoiseSpecificChannels);
	
	if (m_NoiseSpecificChannels)
	{
		ImGui::Indent();
		ImGui::Text("Noise Channels");
		changed |= ImGui::Checkbox("R", &m_NoiseRed);
		ImGui::SameLine();
		changed |= ImGui::Checkbox("G", &m_NoiseGreen);
		ImGui::SameLine();
		changed |= ImGui::Checkbox("B", &m_NoiseBlue);
		ImGui::Unindent();
	}

	return changed;
}

void RandomNoiseFilter::Serialize(toml::table& table) const
{
	table.insert("min", m_Min);
	table.insert("max", m_Max);
	table.insert("amount", m_Amount);
	table.insert("seed", (int64_t)m_Seed);
	table.insert("noise_specific_channels", toml::array{ m_NoiseSpecificChannels, m_NoiseRed, m_NoiseGreen, m_NoiseBlue });
}

void RandomNoiseFilter::Deserialize(const toml::table& table)
{
	m_Min = table["min"].value_or(m_Min);
	m_Max = table["max"].value_or(m_Max);
	m_Amount = table["amount"].value_or(m_Amount);
	m_Seed = (uint64_t)table["seed"].value_or((int64_t)m_Seed);

	auto& noiseSpecificChannels = *table["noise_specific_channels"].as_array();
	
	m_NoiseSpecificChannels = noiseSpecificChannels[0].value_or(m_NoiseSpecificChannels);
	m_NoiseRed = noiseSpecificChannels[1].value_or(m_NoiseRed);
	m_NoiseGreen = noiseSpecificChannels[2].value_or(m_NoiseGreen);
	m_NoiseBlue = noiseSpecificChannels[3].value_or(m_NoiseBlue);
}
