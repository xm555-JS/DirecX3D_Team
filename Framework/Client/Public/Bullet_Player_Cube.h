#pragma once

#include "Client_Defines.h"
#include "Bullet.h"




BEGIN(Client)
class CBullet_Player_Cube final : public CBullet
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
	CBullet_Player_Cube();
	CBullet_Player_Cube(const CBullet_Player_Cube& rhs);
	virtual ~CBullet_Player_Cube() = default;

public:
	void Set_StartPos(_float4 vPos) { m_vPos = vPos; }
	void Reset_Turn() { m_fTurnAccTime = 0.f; }
	void Set_Shoot() { m_bShoot = true; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render() override;


private:
	BULLETDESC		m_BulletDesc;
	_float			m_fDeadTimeAcc = 0.f;
	_float4			m_vPos = { 0.f, 0.f, 0.f, 1.f };
	_float4			m_vTargetPos = { 0.f, 0.f, 0.f, 1.f };
	_bool			m_bShoot = false;
	CGameObject*	m_pOwner = nullptr;
	_float			m_fAccTime = 0.f;
	_float			m_fAccLerfTime = 0.f;
	_float			m_fTurnAccTime = 0.f;
	_float4			m_vStartLook;

	CGameObject*	m_pTarget = nullptr;
	_bool			m_bSetTarget = false;
	_float4			m_pActionDescs;

	_float			m_fAccTime_Effect = 0.f;
	_uint			m_iEffect = 0;

	DECL_ONCE(m_bSound);
	_uint			m_iSoundChannel =1;

private:
	HRESULT SetUp_Components();
	//HRESULT SetUp_ShaderResource();

public:
	static CBullet_Player_Cube* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END