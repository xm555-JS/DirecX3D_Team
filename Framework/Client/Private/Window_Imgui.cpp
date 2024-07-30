#include "stdafx.h"
#include "..\Public\Window_Imgui.h"

#if defined(USE_IMGUI)


CWindow_Imgui::CWindow_Imgui()
{
}


CWindow_Imgui::~CWindow_Imgui()
{
}

void CWindow_Imgui::Initialize_Children()
{
	for (size_t i = 0; i < m_pWindows.size(); ++i)
	{
		m_pWindows[i]->Initialize();
	}
}

void CWindow_Imgui::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}


void CWindow_Imgui::Tick_Children(_float fTimeDelta)
{
	for (size_t i = 0; i < m_pWindows.size(); ++i)
	{
		if (true == m_pWindows[i]->Is_Enable())
		{
			m_pWindows[i]->Tick(fTimeDelta);
		}
	}
}


#endif