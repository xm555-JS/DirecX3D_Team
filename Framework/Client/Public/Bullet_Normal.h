#pragma once

#include "Client_Defines.h"
#include "Bullet.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CVIBuffer;
class CTexture;
class CPhyXCollider;
class CCollider;
END

BEGIN(Client)
class CCursor;
class CBullet_Normal abstract : public CBullet
{
public:
	typedef struct BulletDesc
	{
		_float4x4		WorldMatrix;
		_float4			TargetPos;
		CGameObject*	pTarget;
		_float			fBulletSpeed;
	}BULLETDESC;

public:
	CBullet_Normal();
	CBullet_Normal(const CBullet& rhs);
	virtual ~CBullet_Normal() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

protected:
	BULLETDESC		m_BulletDesc;
	_float			m_fBulltSpeed = 0.f;

protected:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_ShaderResource();


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END