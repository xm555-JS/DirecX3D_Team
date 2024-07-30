#pragma once
#include "Client_Defines.h"
#include "Window_Imgui.h"

#if defined(USE_IMGUI)

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CWindow_Tool_UI final : public CWindow_Imgui
{

public:
	CWindow_Tool_UI();
	virtual ~CWindow_Tool_UI();

public:
	// CWindow_Imgui을(를) 통해 상속됨
	virtual void Tick(_float fTimeDelta) override;
	virtual void Initialize() override;
	
private:
	bool show_demo_window = true;
	bool show_another_window = false;
	bool show_UIObject_window = false;

private:
	map<_uint, CGameObject*>	m_pUIPrototypes;
	list<CGameObject*>			m_UIObjectlist;
	_float2						m_vDragDelta[MAX_PATH];


private:
	void Show_UIPrototypes();
	void Show_UIObjects();

private:
	void Drag_UI(list<class CUI*> UIlist);
	HRESULT Save_UIFile(string strName);
	HRESULT Load_UIFile(string strName);
	char* SetUp_UIName(_uint iPrototypeID);

public:
	virtual	void	Free() override;
};

END

#endif