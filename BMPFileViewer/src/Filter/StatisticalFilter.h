#pragma once

#include "Filter/ImageFilter.h"

class StatisticalFilter final : public ImageFilter
{
private:
	enum class Direction : int
	{
		Vertical = 0,
		Horizontal
	};

	enum class Method : int
	{
		Average = 0,
		Subtract
	};

	int m_Radius = 1;
	float m_Threshold = 0.05f;
	Direction m_Direction = Direction::Vertical;
	Method m_Method = Method::Average;
	bool m_isMask = false;

public:
	StatisticalFilter();

	virtual Image Apply(const Image& source) override;
	virtual void Reset() override;
	virtual void OnImGuiRender() override;
};
