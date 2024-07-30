#pragma once

#include "Client_Defines.h"
#include "Enemy_Boss.h"


BEGIN(Client)
class CCursor;
class CEnemy_Boss_Weaver final : public CEnemy_Boss
{
public:
	enum WeaverState
	{
		WEAVER_ATTACK01,
		WEAVER_ATTACK02,
		WEAVER_ATTACK03,
		WEAVER_ATTACK04,
		WEAVER_ATTACK05,
		WEAVER_ATTACK06,
		WEAVER_ATTACK07,
		WEAVER_ATTACK08,
		WEAVER_ATTACK09,
		WEAVER_ATTACK10,
		WEAVER_ATTACK11,
		WEAVER_ATTACK12,
		WEAVER_END,
	};
	enum WeaverAttack
	{
		ATTACK_00,
		ATTACK_01,
		ATTACK_02,
		ATTACK_03,
		ATTACK_04,
		ATTACK_05,
		ATTACK_06,
		ATTACK_END,
	};

public:
	CEnemy_Boss_Weaver();
	CEnemy_Boss_Weaver(const CEnemy_Boss& rhs);
	virtual ~CEnemy_Boss_Weaver() = default;

public:
	_uint Get_Phase() { return m_iPhase; }
	_bool Get_RenderKnife() { return m_bRenderKnife; }

	void Set_Phase(_uint iPhase) { m_iPhase = iPhase; }
	void Set_RenderKnife(_bool bRender) { m_bRenderKnife = bRender; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void On_Dead();

private:
	_float m_fAttackTime[WEAVER_END] = { 0.f };

private:
	CCollider* m_pCol_Laser = nullptr;
	CCollider* m_pCol_Attack[ATTACK_END] = { nullptr };
	CCollider* m_pCol_Body = nullptr;
	CCollider* m_pCol_Volume = nullptr;
	CCollider* m_pCol_AttackRange = nullptr;
	CCollider* m_pCol_ChaseRange = nullptr;

private:
	_uint		m_iPhase = 0;
	_bool		m_bRenderKnife = true;

	DECL_ONCE(m_bEffect0);

private:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_ShaderResource();
	virtual	HRESULT SetUp_State_Handler();

private:
	void SetUp_ColMatrix();

public:
	static CEnemy_Boss_Weaver* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END