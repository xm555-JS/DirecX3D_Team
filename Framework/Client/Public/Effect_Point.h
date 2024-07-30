#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Point_Instance;
END

BEGIN(Client)

class CEffect_Point final : public CGameObject
{
public:
	CEffect_Point();
	CEffect_Point(const CEffect_Point& rhs);
	virtual ~CEffect_Point() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg = nullptr);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader*						m_pShaderCom = nullptr;
	CTexture*						m_pTextureCom = nullptr;	
	CRenderer*						m_pRendererCom = nullptr;		
	CVIBuffer_Point_Instance*		m_pVIBufferCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	static CEffect_Point* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END