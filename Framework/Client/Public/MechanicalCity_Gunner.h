#pragma once

#include "Client_Defines.h"
#include "Enemy_Normal.h"


BEGIN(Client)
/*기계도시 총잡이*/
class CMechanicalCity_Gunner final : public CEnemy_Normal
{
public:
	CMechanicalCity_Gunner();
	CMechanicalCity_Gunner(const CMechanicalCity_Gunner& rhs);
	virtual ~CMechanicalCity_Gunner() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();

	HRESULT SetUp_State_Handler();


public:
	static CMechanicalCity_Gunner* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END