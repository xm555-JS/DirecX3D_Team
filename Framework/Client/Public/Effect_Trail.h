#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Effect_Manager.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Trail;
END

BEGIN(Client)
class CEffect_Trail final : public CGameObject
{
private:
	CEffect_Trail();
	CEffect_Trail(const CEffect_Trail& rhs);
	virtual ~CEffect_Trail() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	//virtual HRESULT Initialize_Pooling(void* pArg = nullptr);
	//void Initialize_Loop(void);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

private:
	CShader*						m_pShaderCom        = nullptr;
	CTexture*						m_pTextureCom       = nullptr;
	CVIBuffer_Trail*				m_pVIBufferTrailCom = nullptr;
	CVIBuffer_Trail::TRAIL_DESC		m_stTrailDesc;

public:
	static CEffect_Trail* Create();
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};
END
