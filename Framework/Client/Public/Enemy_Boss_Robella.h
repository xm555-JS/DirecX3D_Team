#pragma once

#include "Client_Defines.h"
#include "Enemy_Boss.h"

BEGIN(Client)
class CCursor;
class CEnemy_Boss_Robella final : public CEnemy_Boss
{
public:
	CEnemy_Boss_Robella();
	CEnemy_Boss_Robella(const CEnemy_Boss& rhs);
	virtual ~CEnemy_Boss_Robella() = default;

public:
	_float Get_CoolTime() { return m_fAttackTime; }
	void Set_CoolTime() { m_fAttackTime = 0.f; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_ShaderResource();
	virtual	HRESULT SetUp_State_Handler();

private:
	_float m_fAttackTime = 0.f;

private:
	CCollider* m_pCol_Attack = nullptr;
	CCollider* m_pCol_Body = nullptr;
	CCollider* m_pCol_Volume = nullptr;
	CCollider* m_pCol_AttackRange = nullptr;
	CCollider* m_pCol_ChaseRange = nullptr;

public:
	static CEnemy_Boss_Robella* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END