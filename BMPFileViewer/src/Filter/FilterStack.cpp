#include "FilterStack.h"

#include "Filter/ContrastFilter.h"
#include "Filter/MedianFilter.h"
#include "Filter/RandomNoiseFilter.h"
#include "Filter/StatisticalFilter.h"
#include "Filter/StripeNoiseFilter.h"

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

void FilterStack::ClearFilters()
{
	m_Filters.clear();
}

void FilterStack::SwapFilters(size_t index1, size_t index2)
{
	std::swap(m_Filters[index1], m_Filters[index2]);
}

void FilterStack::Serialize(toml::table& table) const
{
	toml::array filters;

	for (const auto& filter : m_Filters)
	{
		toml::table filterTable;
		filterTable.insert("type", filter->GetName());
		filterTable.insert("enabled", filter->IsEnabled());
		filter->Serialize(filterTable);
		filters.push_back(filterTable);
	}

	table.insert_or_assign("filters", filters);
}

void FilterStack::Deserialize(const toml::table& table)
{
	m_Filters.clear();

	if (auto* filters = table["filters"].as_array())
	{
		for (const auto& filter : *filters)
		{
			if (auto* filterTable = filter.as_table())
			{
				auto type = (*filterTable)["type"].value<std::string_view>();

				if (type.has_value())
				{
					if (type.value() == "Contrast")
					{
						PushFilter<ContrastFilter>();
						m_Filters.back()->Deserialize(*filterTable);
					}
					if (type.value() == "Median")
					{
						PushFilter<MedianFilter>();
						m_Filters.back()->Deserialize(*filterTable);
					}
					if (type.value() == "Random Noise")
					{
						PushFilter<RandomNoiseFilter>();
						m_Filters.back()->Deserialize(*filterTable);
					}
					if (type.value() == "Statistical")
					{
						PushFilter<StatisticalFilter>();
						m_Filters.back()->Deserialize(*filterTable);
					}
					if (type.value() == "Stripe Noise")
					{
						PushFilter<StripeNoiseFilter>();
						m_Filters.back()->Deserialize(*filterTable);
					}
					m_Filters.back()->SetEnabled((*filterTable)["enabled"].value_or(true));
				}
			}
		}
	}
}
