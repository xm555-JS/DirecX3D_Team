#pragma once

#include "Client_Defines.h"
#include "Weapon.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CHierarchyNode;
END

BEGIN(Client)
class CState_Handler;

class CMobility_MotorCycle final : public CWeapon
{
public:
	typedef struct tagWeaponDesc
	{
		const char*		pBoneName;
	}WEAPONDESC;
public:
	CMobility_MotorCycle();
	CMobility_MotorCycle(const CMobility_MotorCycle& rhs);
	virtual ~CMobility_MotorCycle() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	//virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render(void* pArg);

private:
	CState_Handler* m_pState_Handler = nullptr;
	CCollider* m_pCol_Sphere = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();
	HRESULT SetUp_State_Handler();
	
public:
	static CMobility_MotorCycle* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END