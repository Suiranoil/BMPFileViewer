#pragma once

#include "Image/Image.h"

#include <string>

#include <toml++/toml.h>

class ImageFilter
{
protected:
	bool m_isEnabled = true;
	bool m_isDirty = false;

private:
	std::string m_Name;

public:
	explicit ImageFilter(std::string_view name);
	virtual ~ImageFilter() = default;

	[[nodiscard]] const std::string& GetName() const { return m_Name; }

	[[nodiscard]] bool IsEnabled() const { return m_isEnabled; }
	[[nodiscard]] bool IsDirty() const { return m_isDirty; }

	void SetEnabled(bool enabled) { m_isEnabled = enabled; }
	void SetDirty(bool dirty) { m_isDirty = dirty; }

	virtual Image Apply(const Image& source) = 0;
	virtual void Reset() = 0;
	virtual void OnImGuiRender() = 0;

	virtual void Serialize(toml::table& table) const = 0;
	virtual void Deserialize(const toml::table& table) = 0;
};


