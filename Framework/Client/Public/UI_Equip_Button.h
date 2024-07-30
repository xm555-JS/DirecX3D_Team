#pragma once

#include "Client_Defines.h"
#include "UI_Equip_Window.h"

BEGIN(Client)

class CUI_Equip_Button final : public CUI_Equip_Window
{
public:
	enum EQUIP_BUTTON
	{
		EQUIPB_EXIT,

		EQUIPB_END
	};

public:
	CUI_Equip_Button();
	CUI_Equip_Button(const CUI_Equip_Button& rhs);
	virtual ~CUI_Equip_Button() = default;

public:
	CUI* Get_UI_SelectWeapon() { return m_pUI_SelectWeapon; }
	CUI* Get_UI_WeaponDetail() { return m_pUI_WeaponDetail; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	CUI*	m_pUI_SelectWeapon = nullptr;
	CUI*	m_pUI_WeaponDetail = nullptr;

private:
	void Interact_Button(_float fTimeDelta);
	void Pooling_Equip_Objects();

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Equip_Button* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END