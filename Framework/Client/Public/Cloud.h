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
class CCloud : public CGameObject
{
public:
	CCloud();
	CCloud(const CCloud& rhs);
	virtual ~CCloud() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(void* pArg);
	virtual HRESULT Render();

protected:
	CShader*						m_pShaderCom = nullptr;
	CTexture*						m_pNoiseTexture = nullptr;
	CTexture*						m_pTextureCom = nullptr;
	CVIBuffer_Rect*					m_pVIBufferCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

private:


private:
	_uint	m_iNumTexIdx = 0;
	_float	m_fLifeTime = 0.f;
	_float	m_fTimeAcc = 0.f;

private:
	_float		m_fAlphaRate = 0.f;
	//_float2		m_vUVSpeed_0;
	//_float2		m_vUVSpeed_1;
	//_float		m_fBaseMoveSpeed_0x = 0.f;
	//_float		m_fBaseMoveSpeed_0y = 0.f;
	//_float		m_fBaseMoveSpeed_1x = 0.f;
	//_float		m_fBaseMoveSpeed_1y = 0.f;

	_bool		m_bAppear = false;
	_float		m_fMoveSpeed = 0.f;

private:
	_float4x4	m_ParentMat;

private:
	void		Reset_CloudInfo();
	void		Move_Cloud(_float fTimeDelta);

public:
	static CCloud* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END