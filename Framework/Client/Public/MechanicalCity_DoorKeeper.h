#pragma once

#include "Client_Defines.h"
#include "Enemy_Normal.h"


BEGIN(Client)
/*기계도시 문지기*/
class CMechanicalCity_DoorKeeper final : public CEnemy_Normal
{
public:
	CMechanicalCity_DoorKeeper();
	CMechanicalCity_DoorKeeper(const CMechanicalCity_DoorKeeper& rhs);
	virtual ~CMechanicalCity_DoorKeeper() = default;

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
	static CMechanicalCity_DoorKeeper* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END