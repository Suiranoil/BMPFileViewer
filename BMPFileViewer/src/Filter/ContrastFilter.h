﻿#pragma once

#include "Filter/ImageFilter.h"

class ContrastFilter final : public ImageFilter
{
private:
	float m_Contrast = 1.0f;

public:
	explicit ContrastFilter();

	virtual Image Apply(const Image& source) override;
	virtual void Reset() override;
	virtual void OnImGuiRender() override;
};
