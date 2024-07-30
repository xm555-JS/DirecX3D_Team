#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Terrain;
class CVIBuffer_Terrain_Custom;
END

BEGIN(Client)

class CTerrain final : public CGameObject
{
public:

public:
	CTerrain();
	CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render(void* pArg);

private:
	CShader*						m_pShaderCom = nullptr;
	CTexture*						m_pTextureCom[TYPE_END] = { nullptr };		
//#if defined(_TESSELLATION)
//	CVIBuffer_Terrain_Custom*		m_pVIBufferCom = nullptr;
//#else
	CVIBuffer_Terrain*				m_pVIBufferCom = nullptr;
//#endif

	ID3D11ShaderResourceView*		m_pFilterTexture = nullptr;

	ComPtr<ID3D11Device>				m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext>			m_pContext = nullptr;


	_uint						m_iDiffuseIdx = 8;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();
	HRESULT Create_FilterTexture();

public:
	static CTerrain* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END