#pragma once

#include "Filter/ImageFilter.h"

class RandomNoiseFilter final : public ImageFilter
{
private:
	int m_Min = -127;
	int m_Max = 128;

	float m_Amount = 0.0f;
	uint64_t m_Seed = 0LL;

	bool m_NoiseSpecificChannels = false;

	bool m_NoiseRed = true;
	bool m_NoiseGreen = true;
	bool m_NoiseBlue = true;

public:
	RandomNoiseFilter();

	virtual Image Apply(const Image& source) override;
	virtual void Reset() override;
	virtual void OnImGuiRender() override;
};
