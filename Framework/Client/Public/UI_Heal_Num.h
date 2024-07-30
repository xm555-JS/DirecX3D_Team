#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Heal_Num final : public CUI
{
public:
	CUI_Heal_Num();
	CUI_Heal_Num(const CUI_Heal_Num& rhs);
	virtual ~CUI_Heal_Num() = default;

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
	_float	m_fAlphaRatio = 1.f;
	_float	m_fAccTime = 0.f;
	_float	m_fAlphaTime = 0.f;
	_float  m_fStartTime = 0.f;

	_float4 m_vTargerPos;

private:
	void SetUp_Animation(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Heal_Num* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END