#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Effect_Manager.h"

BEGIN(Client)

class CEffect final : public CBase
{
private:
	CEffect();
	virtual ~CEffect() = default;

public:
	HRESULT Initialize(const EFFECT_DESC& EffectDesc);
	HRESULT Execute(_matrix matTransform, _float fTimeSpeed = 1.f, CGameObject* ParentObject = nullptr, CEffect_Manager::FOLLOW_MODE eFollowMode = CEffect_Manager::FOLLOW_MODE_END, string pBoneName = "");
	const EFFECT_DESC& Get_EffectDesc(void) { return m_EffectDesc; };

private:
	EFFECT_DESC						m_EffectDesc;

public:
	static CEffect* Create(const EFFECT_DESC& EffectDesc);
	virtual void Free() override;
};

END