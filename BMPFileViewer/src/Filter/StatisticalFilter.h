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
		NeighborAverage = 0,
		NeighborAverageSubtract
	};

	int m_Radius = 1;
	float m_Threshold = 0.05f;
	Direction m_Direction = Direction::Vertical;
	Method m_Method = Method::NeighborAverage;
	bool m_isMask = false;

public:
	StatisticalFilter();

	virtual Image Apply(const Image& source) override;
	virtual void Reset() override;
	virtual void OnImGuiRender() override;
};
