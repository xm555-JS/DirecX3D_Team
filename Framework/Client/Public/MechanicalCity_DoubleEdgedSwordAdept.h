#pragma once

#include "Client_Defines.h"
#include "Enemy_Normal.h"


BEGIN(Client)
/*기계도시 양날검 숙련자*/
class CMechanicalCity_DoubleEdgedSwordAdept final : public CEnemy_Normal
{
public:
	CMechanicalCity_DoubleEdgedSwordAdept();
	CMechanicalCity_DoubleEdgedSwordAdept(const CMechanicalCity_DoubleEdgedSwordAdept& rhs);
	virtual ~CMechanicalCity_DoubleEdgedSwordAdept() = default;

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
	static CMechanicalCity_DoubleEdgedSwordAdept* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END