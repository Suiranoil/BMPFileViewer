#pragma once

#include "Filter/ImageFilter.h"

class MedianFilter final : public ImageFilter
{
private:
	int m_Radius = 1;

public:
	MedianFilter();

	virtual Image Apply(const Image& source) override;
	virtual void Reset() override;
	virtual void OnImGuiRender() override;

	virtual void Serialize(toml::table& table) const override;
	virtual void Deserialize(const toml::table& table) override;
};
