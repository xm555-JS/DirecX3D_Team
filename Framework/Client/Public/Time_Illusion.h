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

class CTime_Illusion final: public CGameObject
{
protected:
	CTime_Illusion();
	CTime_Illusion(const CTime_Illusion& rhs);
	virtual ~CTime_Illusion() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
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
	_float			m_fAccTime = 0.f;
	_float			m_fTimeAcc_Effect0 = 0.f;
	DECL_ONCE(m_bEffect0);

protected:
	HRESULT SetUp_Components();

public:
	static CTime_Illusion* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

