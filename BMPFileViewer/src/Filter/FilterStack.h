#pragma once

#include "Filter/ImageFilter.h"

#include <memory>
#include <vector>

class FilterStack
{
private:
	std::vector<std::unique_ptr<ImageFilter>> m_Filters;
		
public:
	FilterStack() = default;
	~FilterStack() = default;

	template<typename T, typename... Args>
	void PushFilter(Args&&... args)
	{
		static_assert(std::is_base_of_v<ImageFilter, T>, "T must be derived from ImageFilter");

		m_Filters.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
	}

	[[nodiscard]] size_t Size() const { return m_Filters.size(); }
	[[nodiscard]] const ImageFilter& GetFilter(size_t index) const { return *m_Filters[index]; }
	[[nodiscard]] ImageFilter& GetFilter(size_t index) { return *m_Filters[index]; }

	void Apply(Image& image) const;

	void RemoveFilter(size_t index);
	void ClearFilters();
	void SwapFilters(size_t index1, size_t index2);

	void Serialize(toml::table& table) const;
	void Deserialize(const toml::table& table);
};
