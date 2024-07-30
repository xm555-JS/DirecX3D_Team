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
class CGrass : public CGameObject
{
public:
	CGrass();
	CGrass(const CGrass& rhs);
	virtual ~CGrass() = default;

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
	CShader*	m_pShaderCom = nullptr;
	CModel*		m_pModelCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;

private:
	_float		m_fDistortionPower = 0.f;
	_float2		m_vUVSpeed;

protected:
	HRESULT SetUp_Model(ID_UINT iPrototypeID);

private:
	_uint m_iNumMeshContainers = 0;

public:
	static CGrass* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END