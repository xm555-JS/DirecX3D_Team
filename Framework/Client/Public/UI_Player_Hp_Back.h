#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Player_Hp_Back final : public CUI
{
public:
	CUI_Player_Hp_Back();
	CUI_Player_Hp_Back(const CUI_Player_Hp_Back& rhs);
	virtual ~CUI_Player_Hp_Back() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	class CPlayer*	m_pPlayer = nullptr;
	_float			m_fAccTime = 0.f;
	_float			m_fSizeRatioX = 1.f;
	_float			m_fPreHp = 0.f;
	_bool			m_bDcrsHp = false;
	_bool			m_bIncrsHp = true;

private:
	void SetUp_HpBackGauge(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Player_Hp_Back* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END