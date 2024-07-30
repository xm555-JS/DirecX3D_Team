#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Minigame_Start final : public CUI
{
public:
	CUI_Minigame_Start();
	CUI_Minigame_Start(const CUI_Minigame_Start& rhs);
	virtual ~CUI_Minigame_Start() = default;

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
	void InitBar_Anim(_float fTimeDelta, _float fOffsetX);

private:
	_float m_fAccTime = 0.f;
	DECL_ONCE(m_bCreateReady);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Minigame_Start* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END