#pragma once
#include "Client_Defines.h"
#include "Base.h"

//#if defined(USE_IMGUI)

BEGIN(Engine)
class CGameInstance;
END


BEGIN(Client)

class CWindow_Imgui;

class CImgui_Manager : public CBase
{
public:
	DECLARE_SINGLETON(CImgui_Manager)

public:
	enum WINDOWS
	{
		IMGUI_DEMO,
		TOOL_UI,
		TOOL_ANIMATION,
		TOOL_EFFECT,
		TOOL_MAP,
		TOOL_OPTION,

		WINDOW_END
	};

public:
	CImgui_Manager();
	virtual ~CImgui_Manager() = default;

public:
	CWindow_Imgui*	Get_Window(WINDOWS eWindow) { return m_Windows[eWindow]; }

public:
	_float		Get_DebugFloat();
	_float4		Get_DebugFloat4(_uint iIdx);
	_fvector	Get_DebugVector(_uint iIdx);

public:
	HRESULT Initialize();
	void Tick(float fTimeDelta);
	HRESULT Render();

private:
	ComPtr<ID3D11Device>			m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext>		m_pContext = nullptr;

private:
	vector<CWindow_Imgui*>			m_Windows;
	vector<_uint>					m_Windows_KeyState;

private:
	_bool		m_bWindowOpen = false;
	_bool		m_bPreWindowOpen = false;


private:
	void	Input_Key();
	void	Create_Windows();
	void	Setup_WindowKey();

	//bool CreateDeviceD3D(HWND hWnd);

	//void CleanupDeviceD3D();

	void CreateRenderTarget();

	//void CleanupRenderTarget();

public:
	virtual void Free();
};

END

//#endif