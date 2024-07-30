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
class CGrass_Desert : public CGameObject
{
public:
	CGrass_Desert();
	CGrass_Desert(const CGrass_Desert& rhs);
	virtual ~CGrass_Desert() = default;

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
	static CGrass_Desert* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END