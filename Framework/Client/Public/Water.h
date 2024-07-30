#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_Rect;

END


BEGIN(Client)

class CWater final : public CGameObject
{
public:
	CWater();
	CWater(const CWater& rhs);
	virtual ~CWater() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;


private:
	CShader*				m_pShaderCom = nullptr;
	//CTexture*				m_pDistortionTex = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;


	_float2					m_fUVSpeed_0 = { 0.f, 0.f };
	_float2					m_fUVSpeed_1 = { 0.f, 0.f };
	_float2					m_fUVSpeed_2 = { 0.f, 0.f };
	_float2					m_fUVSpeed_3 = { 0.f, 0.f };

	_float					m_fBaseMoveSpeed = 0.f;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

	HRESULT		Setup_TransformState();

	void		Move_WaterSurface(_float fTimeDelta);

public:
	static CWater* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END