#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Damage_Num final : public CUI
{
public:
	CUI_Damage_Num();
	CUI_Damage_Num(const CUI_Damage_Num& rhs);
	virtual ~CUI_Damage_Num() = default;

public:
	void Set_Element(ELEMENTAL_PROPERTIE eElement) { m_eElement = eElement; }
	void Set_Critical(_bool bCritical) { m_bCritical = bCritical; }

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

	_float3 m_vColor = {};
	_float4 m_vTargerPos;

	_bool   m_bCritical = false;
	
	ELEMENTAL_PROPERTIE m_eElement;

private:
	void SetUp_Color();
	void SetUp_Animation(_float fTimeDelta);
	void SetUp_Critical_Animation(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Damage_Num* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END