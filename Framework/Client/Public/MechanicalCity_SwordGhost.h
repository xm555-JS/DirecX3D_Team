#pragma once

#include "Client_Defines.h"
#include "Enemy_Normal.h"


BEGIN(Client)
/*기계도시 검귀*/
class CMechanicalCity_SwordGhost final : public CEnemy_Normal
{
public:
	CMechanicalCity_SwordGhost();
	CMechanicalCity_SwordGhost(const CMechanicalCity_SwordGhost& rhs);
	virtual ~CMechanicalCity_SwordGhost() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

	HRESULT SetUp_State_Handler();


public:
	static CMechanicalCity_SwordGhost* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END