#include "FilterStack.h"

void FilterStack::Apply(Image& image) const
{
	for (const auto& filter : m_Filters)
		if (filter->IsEnabled())
			image = filter->Apply(image);
}

void FilterStack::RemoveFilter(size_t index)
{
	m_Filters.erase(m_Filters.begin() + index);
}

void FilterStack::SwapFilters(size_t index1, size_t index2)
{
	std::swap(m_Filters[index1], m_Filters[index2]);
}
