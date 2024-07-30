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

class CTerrain_Navi final : public CGameObject
{
public:

public:
	CTerrain_Navi();
	CTerrain_Navi(const CTerrain_Navi& rhs);
	virtual ~CTerrain_Navi() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader*						m_pShaderCom = nullptr;
	CTexture*						m_pTextureCom[TYPE_END] = { nullptr };		
	CVIBuffer_Terrain*				m_pVIBufferCom = nullptr;

	ID3D11ShaderResourceView*		m_pFilterTexture = nullptr;

	ComPtr<ID3D11Device>				m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext>			m_pContext = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();
	HRESULT Create_FilterTexture();

public:
	static CTerrain_Navi* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END