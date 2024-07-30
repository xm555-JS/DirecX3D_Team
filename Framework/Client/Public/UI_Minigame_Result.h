#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Minigame_Result final : public CUI
{
public:
	CUI_Minigame_Result();
	CUI_Minigame_Result(const CUI_Minigame_Result& rhs);
	virtual ~CUI_Minigame_Result() = default;

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
	_float m_fAlphaRatio = 1.f;
	_float m_fAccTime = 0.f;

private:
	void  BackIcon_Anim(_float fTimeDelta, _float fInitX);
	void  FadeIn_Icon(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Minigame_Result* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END