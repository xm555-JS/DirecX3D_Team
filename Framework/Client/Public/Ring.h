#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
END

BEGIN(Client)
class CCursor;
class CRing final : public CGameObject
{
public:
	CRing();
	CRing(const CRing& rhs);
	virtual ~CRing() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

	void	Input_Key(_float fTimeDelta);

	void	Collider_ToPlayer();

private:
	class CShader* m_pShaderCom = nullptr;
	class CModel* m_pModelCom = nullptr;
	class CCollider* m_pCol_Sphere = nullptr;

	_uint		m_iNumMeshContainers = 0;

	_float		m_fEffect_StartTime = 0.f;
	_float		m_fTimeAcc = 0.f;

	DECL_ONCE(m_bEffect0);
	DECL_ONCE(m_bEffect1);
	DECL_ONCE(m_bEffect2);

public:
	static CRing* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END