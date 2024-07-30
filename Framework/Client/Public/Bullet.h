#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CHierarchyNode;
class CCollider;
class CNavigation;
END

BEGIN(Client)

class CBullet abstract : public CGameObject
{
protected:
	CBullet();
	CBullet(const CBullet& rhs);
	virtual ~CBullet() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	CShader*		m_pShaderCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	CNavigation*	m_pNaviCom = nullptr;


	CCollider*		m_pCol_Sphere = nullptr;

protected:
	CGameObject*	m_pTargetObj = nullptr;

protected:
	_float			m_fDamage = 0.f;

protected:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	virtual void Free() override;
};

END

