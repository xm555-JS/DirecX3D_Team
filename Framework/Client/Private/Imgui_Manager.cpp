#include "stdafx.h"
#include "Imgui_Manager.h"
#include "GameInstance.h"

#if defined(USE_IMGUI)

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Window_Imgui.h"

//#include "Window_LevelInfo.h"
#include "Window_Demo.h"
#include "Window_Tool_Animation.h"
#include "Window_Tool_Effect.h"
#include "Window_Tool_Map.h"
#include "Window_Tool_UI.h"
#include "Window_Tool_Option.h"


IMPLEMENT_SINGLETON(CImgui_Manager)

// Map Tool
// 저장해야하는값 :  Collider type (dynamic or static), Pos, Shape, xyzSize, Density
//

CImgui_Manager::CImgui_Manager()
{
}

_float CImgui_Manager::Get_DebugFloat()
{
	assert(m_Windows[TOOL_OPTION]);
	return ((CWindow_Tool_Option*)m_Windows[TOOL_OPTION])->Get_DebugFloat();
}

_float4 CImgui_Manager::Get_DebugFloat4(_uint iIdx)
{
	assert(m_Windows[TOOL_OPTION]);
	return ((CWindow_Tool_Option*)m_Windows[TOOL_OPTION])->Get_DebugFloat4(iIdx);
}

_fvector CImgui_Manager::Get_DebugVector(_uint iIdx)
{
	assert(m_Windows[TOOL_OPTION]);
	return ((CWindow_Tool_Option*)m_Windows[TOOL_OPTION])->Get_DebugVector(iIdx);
}

HRESULT CImgui_Manager::Initialize()
{
	USEGAMEINSTANCE;
	m_pDevice = DEVICE;
	m_pContext = CONTEXT;

	// Initialize IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//ImGui::StyleColorsLight();
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(*m_pDevice.GetAddressOf(), *m_pContext.GetAddressOf());

	// Initialize base Window
	Create_Windows();
	Setup_WindowKey();

	return S_OK;
}

void CImgui_Manager::Tick(float fTimeDelta)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Input_Key();

	USEGAMEINSTANCE;

	m_bWindowOpen = false;
	for (size_t i = 0; i < m_Windows.size(); ++i)
	{
		if (true == m_Windows[i]->Is_Enable())
		{
			m_Windows[i]->Tick(fTimeDelta);
			m_bWindowOpen = true;
		}
	}

	if (m_bPreWindowOpen != m_bWindowOpen) 
	{
		m_bPreWindowOpen = m_bWindowOpen;
		pGameInstance->Set_CursorFix(!m_bWindowOpen);
		//pGameInstance->Add_Event(EVENT_IMGUIWINDOWSTATE, &m_bWindowOpen, nullptr);
	}



}

HRESULT CImgui_Manager::Render()
{
	USEGAMEINSTANCE;
	ImGui::Render();
#if 1
	ID3D11RenderTargetView* pBackBufferView = nullptr;
	ID3D11DepthStencilView* pDepthStencilView = nullptr;

	m_pContext->OMGetRenderTargets(1, &pBackBufferView, &pDepthStencilView);

	ID3D11ShaderResourceView* pSRVs[8] = { nullptr };
	ID3D11ShaderResourceView* pSRV = pGameInstance->Get_BackBufferSRV();
	pSRVs[0] = pSRV;
	m_pContext->PSSetShaderResources(0, 8, pSRVs);

	ID3D11RenderTargetView* RTVs[8] = { nullptr };
	ID3D11RenderTargetView* pRTV = pGameInstance->Get_BackBufferRTV();
	RTVs[0] = pRTV;
	m_pContext->OMSetRenderTargets(1, RTVs, pDepthStencilView);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Update and Render additional Platform Windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	m_pContext->OMSetRenderTargets(1, &pBackBufferView, pDepthStencilView);
	Safe_Release(pBackBufferView);
	Safe_Release(pDepthStencilView);
#else
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif // 0

	m_Windows[TOOL_MAP]->Render();

	return S_OK;
}


void CImgui_Manager::Input_Key()
{
	USEGAMEINSTANCE;

	// 각 할당된 키 검사
	for (size_t i = 0; i < WINDOW_END; ++i)
	{
		if (true == KEY_TOGGLE(m_Windows_KeyState[i]))
			m_Windows[i]->Set_Enable();
		else
			m_Windows[i]->Set_Disable();
	}
}

void CImgui_Manager::Create_Windows()
{
	// Imgui Windows 생성
	m_Windows.assign(WINDOW_END, nullptr);

	m_Windows[IMGUI_DEMO] = new CWindow_Demo;
	m_Windows[TOOL_UI] = new CWindow_Tool_UI;
	m_Windows[TOOL_ANIMATION] = new CWindow_Tool_Animation;
	m_Windows[TOOL_EFFECT] = new CWindow_Tool_Effect;
	m_Windows[TOOL_MAP] = new CWindow_Tool_Map;
	m_Windows[TOOL_OPTION] = new CWindow_Tool_Option;

	for (auto& elem : m_Windows)
		elem->Initialize();

}

void CImgui_Manager::Setup_WindowKey()
{
	USEGAMEINSTANCE;
	// F1 ~ F12 키 토글 꺼놓기
	for (_uint i = VK_F1; i < VK_F12; ++i)
		MAKE_TOGGLE_OFF(i);

	m_Windows_KeyState.assign(WINDOW_END, 0);
	m_Windows_KeyState[TOOL_OPTION] = VK_F1;
	m_Windows_KeyState[TOOL_UI] = VK_F6;
	m_Windows_KeyState[TOOL_ANIMATION] = VK_F3;
	m_Windows_KeyState[TOOL_EFFECT] = VK_F4;
	m_Windows_KeyState[TOOL_MAP] = VK_F5;
	m_Windows_KeyState[IMGUI_DEMO] = VK_F11;

}

void CImgui_Manager::Free()
{
	for (auto& elem : m_Windows)
	{
		Safe_Release(elem);
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

#endif