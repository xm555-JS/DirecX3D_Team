#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CVIBuffer;
class CTexture;
class CCollider;
END

BEGIN(Client)
class CCursor;
class CMapObj_Instance : public CGameObject
{
public:


public:
	typedef struct MapObjInstance
	{
		ID_UINT					ModelID;
		_uint					NumModel;
	}MAPOBJINSTANCEDESC;

public:
	CMapObj_Instance();
	CMapObj_Instance(const CMapObj_Instance& rhs);
	virtual ~CMapObj_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render(void* pArg);

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

	void	Input_Key(_float fTimeDelta);

protected:
	CShader* m_pShaderCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CTexture* m_pNoiseTexture = nullptr;
	

private:
	_float		m_fDistortionPower = 0.f;
	_float2		m_vUVSpeed;
	_uint		m_iNumPass = 0;

protected:
	HRESULT SetUp_Model(ID_UINT iPrototypeID);

private:
	MAPOBJINSTANCEDESC		m_MapObjInstanceDesc;

private:
	_uint m_iNumMeshContainers = 0;
	_bool m_bIsEnableFrustum = true;

public:
	static CMapObj_Instance* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END