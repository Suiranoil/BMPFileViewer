#include "ImageTab.h"

#include "Filter/ContrastFilter.h"
#include "Filter/MedianFilter.h"
#include "Filter/RandomNoiseFilter.h"
#include "Filter/StripeNoiseFilter.h"

#include <algorithm>

#include <imgui.h>
#include <iostream>
#include <stb_image_write.h>

#include "Filter/StatisticalFilter.h"

ImageTab::ImageTab(std::string_view path)
	: m_OriginalImage(path), m_Image(m_OriginalImage)
{
	m_Name = path.substr(path.find_last_of("/\\") + 1);

	m_Texture.UploadImage(m_Image);
}

void ImageTab::SaveImage(std::string_view path) const
{
	stbi_write_bmp(path.data(), m_Image.GetWidth(), m_Image.GetHeight(), 4, m_Image.GetPixels());
}

void ImageTab::ImGuiRender()
{
	constexpr float rightPanelWidth = 350.0f;
	
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
	ImGui::BeginChild("##ImageDisplay", ImVec2(std::max(ImGui::GetContentRegionAvail().x - rightPanelWidth, 0.0f), 0), false,
	                  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImGui::SetCursorPos(ImVec2(m_OffsetX, m_OffsetY));

	ImGui::Image((ImTextureID)m_Texture.GetRendererID(),
	             ImVec2((float)m_Image.GetWidth() * m_Zoom, (float)m_Image.GetHeight() * m_Zoom));

	// show pixel data on hover
	if (ImGui::IsItemHovered() && ImGui::IsKeyDown(ImGuiKey_LeftShift))
	{
		ImVec2 mousePos = ImGui::GetMousePos();

		mousePos.x -= ImGui::GetItemRectMin().x;
		mousePos.y -= ImGui::GetItemRectMin().y;

		mousePos.x /= m_Zoom;
		mousePos.y /= m_Zoom;

		if (mousePos.x >= 0 && mousePos.x < m_Image.GetWidth() && mousePos.y >= 0 && mousePos.y < m_Image.GetHeight())
		{
			Pixel pixel = m_Image.GetPixel((uint32_t)mousePos.x, (uint32_t)mousePos.y);

			ImGui::BeginTooltip();
			ImGui::ColorButton("##PixelColor", ImVec4(pixel.R / 255.0f, pixel.G / 255.0f, pixel.B / 255.0f, 1.0f),
				ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoAlpha, ImVec2(20, 20));
			ImGui::SameLine();
			ImGui::Text("RGBA: (%03d, %03d, %03d, %03d)", pixel.R, pixel.G, pixel.B, pixel.A);
			ImGui::Text("XY: (%d, %d)", (uint32_t)mousePos.x, (uint32_t)mousePos.y);
			ImGui::EndTooltip();
		}
	}

	if (ImGui::IsWindowHovered())
	{
		const float prevZoom = m_Zoom;
		if (ImGui::GetIO().MouseWheel > 0)
		{
			m_Zoom += 0.1f;
		}
		else if (ImGui::GetIO().MouseWheel < 0)
		{
			m_Zoom -= 0.1f;
		}
		m_Zoom = std::clamp(m_Zoom, 0.1f, 5.0f);

		const ImVec2 mousePos = ImGui::GetMousePos();
		const auto absMousePos = ImVec2(mousePos.x - m_OffsetX, mousePos.y - m_OffsetY);
		const auto relMousePos = ImVec2(absMousePos.x / prevZoom, absMousePos.y / prevZoom);
		const auto newAbsMousePos = ImVec2(relMousePos.x * m_Zoom, relMousePos.y * m_Zoom);

		m_OffsetX += absMousePos.x - newAbsMousePos.x;
		m_OffsetY += absMousePos.y - newAbsMousePos.y;
	}

	if (ImGui::IsMouseDragging(0) && ImGui::IsWindowHovered())
	{
		const ImVec2 delta = ImGui::GetIO().MouseDelta;

		m_OffsetX += delta.x;
		m_OffsetY += delta.y;
	}

	ImGui::EndChild();
	ImGui::PopStyleColor();

	ImGui::SameLine();
	ImGui::BeginChild("##ImageFilters", ImVec2(0, 0), true);

	ImGui::Text("Image Filters");
	ImGui::Separator();

	m_ImageChanged = false;
	size_t deleteFilter = -1;
	
	size_t swapFilterLeft = -1;
	size_t swapFilterRight = -1;

	for (size_t i = 0; i < m_FilterStack.Size(); i++)
	{
		auto& filter = m_FilterStack.GetFilter(i);
		ImGui::PushID(i);

		ImGui::BeginGroup();

		ImGui::BeginTable("##Filter", 3, ImGuiTableFlags_SizingFixedFit);
		ImGui::TableSetupColumn("##FilterButtons", ImGuiTableColumnFlags_NoHeaderWidth, 0);
		ImGui::TableSetupColumn("##FilterName", ImGuiTableColumnFlags_WidthStretch, 0);
		ImGui::TableSetupColumn("##FilterButtons", ImGuiTableColumnFlags_NoHeaderWidth, 0);

		ImGui::TableNextColumn();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));

		bool isEnabled = filter.IsEnabled();
		if (ImGui::Checkbox("##Enabled", &isEnabled))
			m_ImageChanged = true;

		filter.SetEnabled(isEnabled);

		ImGui::PopStyleVar();

		ImGui::TableNextColumn();
		bool isOpen = ImGui::CollapsingHeader(filter.GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen);
		
		ImGui::TableNextColumn();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
		if (i == 0)
		{
			ImGui::BeginDisabled();
		}
		if (ImGui::ArrowButton("##Up", ImGuiDir_Up))
		{
			swapFilterLeft = i;
			swapFilterRight = i - 1;

			m_ImageChanged = true;
		}
		if (i == 0)
		{
			ImGui::EndDisabled();
		}

		ImGui::SameLine();

		if (i == m_FilterStack.Size() - 1)
		{
			ImGui::BeginDisabled();
		}
		if (ImGui::ArrowButton("##Down", ImGuiDir_Down))
		{
			swapFilterLeft = i;
			swapFilterRight = i + 1;

			m_ImageChanged = true;
		}
		if (i == m_FilterStack.Size() - 1)
		{
			ImGui::EndDisabled();
		}

		ImGui::SameLine();

		float sz = ImGui::GetFrameHeight();
		if (ImGui::Button("X", ImVec2(sz, sz)))
		{
			deleteFilter = i;
			m_ImageChanged = true;
		}

		ImGui::PopStyleVar();
		ImGui::EndTable();

		ImGui::EndGroup();

		if (isOpen)
		{
			ImGui::Indent();
			filter.SetDirty(false);

			ImGui::PushID("##FilterSettings##");

			filter.OnImGuiRender();

			if (filter.IsEnabled())
				m_ImageChanged |= filter.IsDirty();

			ImGui::PopID();

			ImGui::Unindent();
		}

		ImGui::PopID();
	}

	ImGui::Separator();

	if (ImGui::Button("Add Filter", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
	{
		ImGui::OpenPopup("Add Filter");
	}

	if (ImGui::BeginPopup("Add Filter"))
	{
		if (ImGui::MenuItem("Contrast"))
		{
			m_FilterStack.PushFilter<ContrastFilter>();
			m_ImageChanged = true;
		}
		if (ImGui::MenuItem("Random Noise"))
		{
			m_FilterStack.PushFilter<RandomNoiseFilter>();
			m_ImageChanged = true;
		}
		if (ImGui::MenuItem("Stripe Noise"))
		{
			m_FilterStack.PushFilter<StripeNoiseFilter>();
			m_ImageChanged = true;
		}
		if (ImGui::MenuItem("Median"))
		{
			m_FilterStack.PushFilter<MedianFilter>();
			m_ImageChanged = true;
		}
		if (ImGui::MenuItem("Statistical"))
		{
			m_FilterStack.PushFilter<StatisticalFilter>();
			m_ImageChanged = true;
		}

		ImGui::EndPopup();
	}

	ImGui::EndChild();

	if (deleteFilter != static_cast<size_t>(-1))
	{
		m_FilterStack.RemoveFilter(deleteFilter);
	}

	if (swapFilterLeft != static_cast<size_t>(-1) && swapFilterRight != static_cast<size_t>(-1))
	{
		m_FilterStack.SwapFilters(swapFilterLeft, swapFilterRight);
	}

	if (m_ImageChanged)
	{
		m_Image = m_OriginalImage;
		m_FilterStack.Apply(m_Image);

		m_Texture.UploadImage(m_Image);
	}
}
