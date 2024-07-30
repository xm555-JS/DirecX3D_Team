#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

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
class CItem final : public CGameObject
{
public:
	CItem();
	CItem(const CItem& rhs);
	virtual ~CItem() = default;

public:
	ITEM_TYPE Get_ItemType() { return m_eItem_Type; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Excute_Effect(void);

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

	void	Input_Key(_float fTimeDelta);

private:
	class CShader* m_pShaderCom    = nullptr;
	class CModel* m_pModelCom      = nullptr;
	class CCollider* m_pCol_Sphere = nullptr;

	_uint		m_iNumMeshContainers = 0;
	ITEM_TYPE	m_eItem_Type         = ITEM_END;

	_float		m_fEffect_StartTime = 0.f;
	_float		m_fTimeAcc          = 0.f;

	DECL_ONCE(m_bEffect0);

public:
	static CItem* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END