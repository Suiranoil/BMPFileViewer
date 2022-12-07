#pragma once

#include "Filter/ImageFilter.h"

class StripeNoiseFilter final : public ImageFilter
{
private:
	enum class Direction : int
	{
		Vertical = 0,
		Horizontal
	};

	int m_Min = -127;
	int m_Max = 128;

	float m_Amount = 0.0f;
	uint64_t m_Seed = 0LL;

	bool m_NoiseSpecificChannels = false;

	bool m_NoiseRed = true;
	bool m_NoiseGreen = true;
	bool m_NoiseBlue = true;

	Direction m_Direction = Direction::Vertical;

public:
	StripeNoiseFilter();

	virtual Image Apply(const Image& source) override;
	virtual void Reset() override;
	virtual bool RenderImGui() override;

	virtual void Serialize(toml::table& table) const override;
	virtual void Deserialize(const toml::table& table) override;
};
