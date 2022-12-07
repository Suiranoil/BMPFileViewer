#pragma once

#include "Image/Image.h"

#include <string>

#include <toml++/toml.h>

class ImageFilter
{
protected:
	bool m_isEnabled = true;

private:
	std::string m_Name;

public:
	explicit ImageFilter(std::string_view name);
	virtual ~ImageFilter() = default;

	[[nodiscard]] const std::string& GetName() const { return m_Name; }

	[[nodiscard]] bool IsEnabled() const { return m_isEnabled; }

	void SetEnabled(bool enabled) { m_isEnabled = enabled; }

	virtual Image Apply(const Image& source) = 0;
	virtual void Reset() = 0;
	virtual bool RenderImGui() = 0;

	virtual void Serialize(toml::table& table) const = 0;
	virtual void Deserialize(const toml::table& table) = 0;
};


