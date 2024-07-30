#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CSky final : public CGameObject
{
public:
	CSky();
	CSky(const CSky& rhs);
	virtual ~CSky() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void	On_EventMessege(EVENTDESC tEvent);

private:
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;		
	CVIBuffer_Cube*			m_pVIBufferCom = nullptr;


private:
	_uint					m_iTextureIdx = 0;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	static CSky* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END