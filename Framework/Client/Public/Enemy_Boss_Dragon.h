#pragma once

#include "Client_Defines.h"
#include "Enemy_Boss.h"

BEGIN(Client)
class CEnemy_Boss_FrostDragon final : public CEnemy_Boss
{
public:
	enum DragonState
	{
		DRAGON_RUN,
		DRAGON_ATTACK1,
		DRAGON_ATTACK2,
		DRAGON_ATTACK3,
		DRAGON_ATTACK4,
		DRAGON_ATTACK5,
		DRAGON_ATTACK6,
		DRAGON_ATTACK7,
		DRAGON_END,
	};
	enum DragonBody
	{
		BODY_00,
		BODY_01,
		BODY_02,
		BODY_03,
		BODY_04,
		BODY_05,
		BODY_06,
		BODY_07,
		BODY_08,
		BODY_09,
		BODY_10,
		BODY_11,
		BODY_12,
		BODY_13,
		BODY_14,
		BODY_15,
		BODY_16,
		BODY_END,
	};

public:
	CEnemy_Boss_FrostDragon();
	CEnemy_Boss_FrostDragon(const CEnemy_Boss& rhs);
	virtual ~CEnemy_Boss_FrostDragon() = default;

public:
	void Set_PhaseStart() { m_bPhaseStart = true; }

	void Set_Color(_float4 fAlpha) { m_vColor = fAlpha; }
	_float4 Get_Color() { return m_vColor; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_bool	m_bPhaseStart = false;

private:
	CCollider*	m_pCol_Body[BODY_END] = {nullptr};
	CCollider*	m_pCol_Attack = nullptr;
	CCollider*	m_pCol_TailAttack = nullptr;
	CCollider*	m_pCol_Laser = nullptr;
	CCollider*	m_pCol_AttackRange = nullptr;

private:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_ShaderResource();
	virtual	HRESULT SetUp_State_Handler();

private:
	void SetUp_ColMatrix();


public:
	static CEnemy_Boss_FrostDragon* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END