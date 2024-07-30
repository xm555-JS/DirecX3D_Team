#pragma once

#include "Client_Defines.h"
#include "Bullet.h"




BEGIN(Client)
class CBullet_Player_2Gun_Special final : public CBullet
{
public:
	typedef struct BulletDesc
	{
		_float4			vStartPos;
		_float4			vLookAt;
		_float			fBulletSpeed;
		_float			fBulletDeadTime;
	}BULLETDESC;

public:
	CBullet_Player_2Gun_Special();
	CBullet_Player_2Gun_Special(const CBullet_Player_2Gun_Special& rhs);
	virtual ~CBullet_Player_2Gun_Special() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render() override;


private:
	CGameObject*	m_pOwner = nullptr;
	CCollider*		m_pTargetCol = nullptr;
	BULLETDESC		m_BulletDesc;
	_float			m_fDeadTimeAcc = 0.f;
	_float4			m_vPos = { 0.f, 0.f, 0.f, 1.f };

	_float			m_fTimeAcc_Effect0 = 0.f;
	_float			m_fTimeAcc_Effect1 = 0.f;

	DECL_ONCE(m_bEffect0);
	DECL_ONCE(m_bEffect1);
	DECL_ONCE(m_bEffect2);
	DECL_ONCE(m_bEffect3);
	DECL_ONCE(m_bEffect4);

private:
	HRESULT SetUp_Components();
	//HRESULT SetUp_ShaderResource();

public:
	static CBullet_Player_2Gun_Special* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END