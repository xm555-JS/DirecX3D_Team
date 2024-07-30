#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CHierarchyNode;
class CCollider;
END

BEGIN(Client)

class CDragon_Appear_Trigger : public CGameObject
{
protected:
	CDragon_Appear_Trigger();
	CDragon_Appear_Trigger(const CDragon_Appear_Trigger& rhs);
	virtual ~CDragon_Appear_Trigger() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	CShader*		m_pShaderCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	CCollider*		m_pCol_Sphere = nullptr;

protected:
	CGameObject*	m_pOwnerObj = nullptr;
	CGameObject*	m_pTarget = nullptr;

protected:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	static CDragon_Appear_Trigger* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

