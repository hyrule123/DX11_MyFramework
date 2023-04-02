#pragma once

#include "ImGui/imgui.h"

namespace ImColorPreset
{
#define	IMCOLOR(R, G, B, A) ImVec4(R / 255.f, G / 255.f , B / 255.f, A / 255.f)

	constexpr ImVec4 Red = ImVec4(1.f, 0.f, 0.f, 1.f);
	constexpr ImVec4 Green = ImVec4(0.f, 1.f, 0.f, 1.f);
	constexpr ImVec4 Blue = ImVec4(0.f, 0.f, 1.f, 1.f);
	constexpr ImVec4 Orange = ImVec4(1.f, 130.f / 255.f, 0.f, 1.f);
	//constexpr ImVec4 DarkRed = ImVec4(144.f / 255.f, 12.f / 255.f, 63.f / 255.f, 1.f);
	constexpr ImVec4 DarkRed = IMCOLOR(144, 12, 63, 255);
}

inline void IMGUI_Indicator(const char* _strName, const ImVec4& _Color)
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, _Color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, _Color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, _Color);
	ImGui::Button(_strName);
	ImGui::PopStyleColor(3);
	ImGui::PopID();
}

inline void IMGUI_AlignedText(const char* _strName, float _fAlignWidth = 100.f)
{
	ImGui::Text(_strName);
	ImGui::SameLine(_fAlignWidth);
}
