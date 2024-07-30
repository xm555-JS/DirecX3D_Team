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
class CStructs_Desert : public CGameObject
{
public:
	CStructs_Desert();
	CStructs_Desert(const CStructs_Desert& rhs);
	virtual ~CStructs_Desert() = default;

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

protected:
	class CShader* m_pShaderCom = nullptr;
	class CModel* m_pModelCom = nullptr;

protected:
	HRESULT SetUp_Model(ID_UINT iPrototypeID);

private:
	_uint m_iNumMeshContainers = 0;

public:
	static CStructs_Desert* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END