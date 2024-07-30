#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Skill_Enable final : public CUI
{
public:
	CUI_Skill_Enable();
	CUI_Skill_Enable(const CUI_Skill_Enable& rhs);
	virtual ~CUI_Skill_Enable() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_bool	m_bEnable = false;
	_float	m_fAccTime = 0.f;
	_float	m_fAlphaRatio = 1.f;

	class CPlayer* m_pPlayer = nullptr;

private:
	void Resizing(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Skill_Enable* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END