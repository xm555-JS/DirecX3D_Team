#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Guide_Hand_Anim final : public CUI
{
public:
	CUI_Guide_Hand_Anim();
	CUI_Guide_Hand_Anim(const CUI_Guide_Hand_Anim& rhs);
	virtual ~CUI_Guide_Hand_Anim() = default;

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
	_float m_fInitTime = 0.f;
	_float m_fAlphaRatio = 0.f;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Guide_Hand_Anim* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END