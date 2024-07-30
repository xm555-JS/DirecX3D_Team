#pragma once

#include "Client_Defines.h"
#include "UI_Equip_Window.h"

BEGIN(Client)

class CUI_Equip_Icon final : public CUI_Equip_Window
{
public:
	CUI_Equip_Icon();
	CUI_Equip_Icon(const CUI_Equip_Icon& rhs);
	virtual ~CUI_Equip_Icon() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	WEAPON_TYPE Get_Selected_Weapon() { return m_eSelected_Weapon; }
	WEAPON_SLOT Get_Selected_Slot() { return m_eSelected_Slot; }

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	WEAPON_TYPE m_eSelected_Weapon = WEAPON_END;
	WEAPON_SLOT m_eSelected_Slot = WPN_SLOT_END;

private:
	void Select_Icon_Anim(_float fTimeDelta);
	void SetUp_Selected_Weapon();

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Equip_Icon* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END