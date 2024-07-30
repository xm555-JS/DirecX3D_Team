#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Effect_Manager.h"

BEGIN(Client)
class CEffect_Base : public CGameObject
{
protected:
	CEffect_Base();
	CEffect_Base(const CEffect_Base& rhs);
	virtual ~CEffect_Base() = default;

public:
	void Set_EffectOff(void) { m_bIsEffectOff = true; }

protected:
	_bool	m_bIsEffectOff = false;

public:
	static CEffect_Base* Create();
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END