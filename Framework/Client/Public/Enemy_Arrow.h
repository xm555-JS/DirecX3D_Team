#pragma once

#include "Client_Defines.h"
#include "Bullet.h"




BEGIN(Client)
class CEnemy_Arrow final : public CBullet
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
	CEnemy_Arrow();
	CEnemy_Arrow(const CEnemy_Arrow& rhs);
	virtual ~CEnemy_Arrow() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);


private:
	BULLETDESC		m_BulletDesc;
	_float			m_fDeadTimeAcc = 0.f;
	_float			m_fAccTime = 0.f;

	DECL_ONCE(m_bEffect0);

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

private:
	void	MovetoLook();

public:
	static CEnemy_Arrow* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END