#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CCursor;
class CShelf_Item final : public CGameObject
{
public:
	CShelf_Item();
	CShelf_Item(const CShelf_Item& rhs);
	virtual ~CShelf_Item() = default;

//public:
//	ITEM_TYPE Get_ItemType() { return m_eItem_Type; }

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

private:
	class CShader* m_pShaderCom = nullptr;
	class CModel* m_pModelCom = nullptr;

	_uint		m_iNumMeshContainers = 0;
	ITEM_TYPE	m_eItem_Type = ITEM_END;
	_bool		m_bRender = true;

public:
	static CShelf_Item* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END