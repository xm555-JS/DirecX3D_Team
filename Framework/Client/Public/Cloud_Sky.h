#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CCloud_Sky : public CGameObject
{
public:
	CCloud_Sky();
	CCloud_Sky(const CCloud_Sky& rhs);
	virtual ~CCloud_Sky() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	CShader*						m_pShaderCom = nullptr;
	CTexture*						m_pNoiseTexture = nullptr;
	CTexture*						m_pTextureCom = nullptr;
	CVIBuffer_Rect*					m_pVIBufferCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

	HRESULT Setup_TransformState();





public:
	static CCloud_Sky* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END