#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Effect_Manager.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CModel;
END

BEGIN(Client)
class CEnemy_Robella_PoisonFog final : public CGameObject
{
public:
	CEnemy_Robella_PoisonFog();
	CEnemy_Robella_PoisonFog(const CGameObject& rhs);
	virtual ~CEnemy_Robella_PoisonFog() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	
	void Set_FollowingMode(_matrix matTransform, _float fTimeSpeed, CGameObject* pOwner, CEffect_Manager::FOLLOW_MODE eFollowMode, string strBoneName = "");

private:
	_uint							m_iNumMeshContainers = 0;
	_float							m_fTimeSpeed         = 1.f;
	CGameObject*					m_pOwner             = nullptr;	// Camera
	_bool							m_bIsHaveOwner       = false;
	CModel*							m_pOwnerModelCom     = nullptr;
	string							m_strOwnerBoneName   = "";
	CEffect_Manager::FOLLOW_MODE	m_eFollowMode        = CEffect_Manager::FOLLOW_MODE_END;

	CGameObject*			m_pRobella = nullptr;

private:
	_float4							m_vOffsetScale = { 1.f, 1.f, 1.f, 0.f };

private:
	CShader* m_pShaderCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CTexture* m_pTextureCom_Mask = nullptr;
	CTexture* m_pTextureCom_Noise = nullptr;
	_uint		m_iShaderPass = 0;

	_float			m_fTimeAcc = 0.f;
	_float			m_fTimePointOfDuration = 0.f; // For. Middle Easing

private:
	MESHEFFECT_DESC	m_MeshEffectDesc;

	_float			m_fRatio = 0.f;
	_float			m_fRatioSpeed = 1.f;
	_float2			m_vUVSpeedPos_Mask = { 0.f, 0.f }; //유브이 위치(더해지는 양)
	_float2			m_vUVSpeedPos_Noise = { 0.f, 0.f }; //유브이 위치(더해지는 양)

	_float			m_fSpriteTimeAcc = 0;
	_uint			m_fUVScaleXCur_Mask = 0;
	_uint			m_fUVScaleYCur_Mask = 0;

	_float4			m_vStartDiffuseColorRGBA = { 1.f, 1.f, 1.f, 1.f };
	_float4			m_vCurrentDiffuseColorRGBA = { 1.f, 1.f, 1.f, 1.f };

	//_float4			m_vStartGlowColorRGBA   = { 1.f, 1.f, 1.f, 1.f };
	_float4			m_vCurrentGlowColorRGBA = { 1.f, 1.f, 1.f, 1.f };

	_float3			m_vStartSpeedXYZ = { 1.f, 1.f, 1.f };
	_float3			m_vCurrentSpeedXYZ = { 1.f, 1.f, 1.f };

	_float3			m_vStartScaleRUL = { 1.f, 1.f, 1.f };
	_float3			m_vCurrentScaleRUL = { 1.f, 1.f, 1.f };

	_float3			m_vStartRotateRUL = { 1.f, 1.f, 1.f };
	_float3			m_vCurrentRotateSpeedRUL = { 1.f, 1.f, 1.f };
	_float3			m_fRotateAccelation = { 1.f, 1.f, 1.f };

	_float	m_fRotationAcc = 0.f;

	_float4x4 m_OffSetMatrix;

	// Shader
	_float	m_fCurrentDistortionStrength = 0.f;
	_float4	m_vCurrentRimLightColorRGBA = { 1.f, 1.f, 1.f, 1.f };

	DECL_ONCE(m_bEffect0);
	DECL_ONCE(m_bEffect1);

private:
	void UVSpeed_Sprite(_float fTimeDelta);
	void Moving(_float fTimeDelta);
	void Scaling_Rotating(_float fTimeDelta);
	void Easing_DiffuseColor(void);
	void Easing_GlowColor(void);
	void Easing_Shader(void);

private:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_ShaderResource();

	HRESULT	SetUp_MeshDesc(void);
	HRESULT	SetUp_CurDesc(MESHEFFECT_DESC& Desc);

public:
	static CEnemy_Robella_PoisonFog* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END