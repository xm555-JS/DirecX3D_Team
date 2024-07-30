#pragma once

#include "Client_Defines.h"
#include "Enemy_Boss.h"

BEGIN(Client)
class CEnemy_Boss_Leviathan final : public CEnemy_Boss
{
public:
	CEnemy_Boss_Leviathan();
	CEnemy_Boss_Leviathan(const CEnemy_Boss& rhs);
	virtual ~CEnemy_Boss_Leviathan() = default;

public:
	_float Get_CoolTime() { return m_fAttackTime; }
	void Set_CoolTime() { m_fAttackTime = 0.f; }

	_vector	Get_IslandPos(_uint iIdx) { return XMLoadFloat4(&m_vTargetIslandPos[iIdx]); }
	_uint	Get_CurIslandIdx() { return m_iCurIslandIdx; }
	_uint	Get_NextIslandIdx();
	_bool	Get_IsApear() { return m_bApear; }

	void	Set_CurIslandIdx(_uint iIdx) { m_iCurIslandIdx = iIdx; }
	void	Set_Apear(_bool isApear) { m_bApear = isApear; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void	On_Dead();

	virtual void On_EventMessege(EVENTDESC tEvent) override;

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


private:
	_float4		m_vTargetIslandPos[3];
	_uint		m_iCurIslandIdx = 0;

	_bool		m_bApear = false;


public:
	static CEnemy_Boss_Leviathan* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END