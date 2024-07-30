#include "stdafx.h"
#include "Effect_Rect.h"
#include "GameInstance.h"
#include "Easing_Utillity.h"

#include "Model.h"
#include "HIerarchyNode.h"
#include "VIBuffer_Rect_Instance.h"

CEffect_Rect::CEffect_Rect()
	: CEffect_Base()
{
}

CEffect_Rect::CEffect_Rect(const CEffect_Rect& rhs)
	: CEffect_Base(rhs)
{
}

HRESULT CEffect_Rect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Rect::Initialize(void* pArg)
{
	NULLCHECK(pArg);
	memcpy(&m_ParticleDesc, pArg, sizeof(PARTICLE_DESC));
	FAILEDCHECK(__super::Initialize());
	FAILEDCHECK(SetUp_Components());
	GAMEINSTANCE->Add_TickGroup(TG_EFFECT, this);
	GAMEINSTANCE->Add_LateTickGroup(LTG_EFFECT, this);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_ParticleDesc.vPositionOffset.x, m_ParticleDesc.vPositionOffset.y, m_ParticleDesc.vPositionOffset.z, 1.f));

	m_vStartDiffuseColorRGBA.x = fRandom(m_ParticleDesc.vStartDiffuseColorRGBA_Min.x, m_ParticleDesc.vStartDiffuseColorRGBA_Max.x);
	m_vStartDiffuseColorRGBA.y = fRandom(m_ParticleDesc.vStartDiffuseColorRGBA_Min.y, m_ParticleDesc.vStartDiffuseColorRGBA_Max.y);
	m_vStartDiffuseColorRGBA.z = fRandom(m_ParticleDesc.vStartDiffuseColorRGBA_Min.z, m_ParticleDesc.vStartDiffuseColorRGBA_Max.z);
	m_vStartDiffuseColorRGBA.w = fRandom(m_ParticleDesc.vStartDiffuseColorRGBA_Min.w, m_ParticleDesc.vStartDiffuseColorRGBA_Max.w);
	m_vCurrentDiffuseColorRGBA = m_vStartDiffuseColorRGBA;

	if (!m_ParticleDesc.bIsGlowFollowDiffuseColor)
	{
		m_vStartGlowColorRGBA.x = fRandom(m_ParticleDesc.vStartGlowColorRGBA_Min.x, m_ParticleDesc.vStartGlowColorRGBA_Max.x);
		m_vStartGlowColorRGBA.y = fRandom(m_ParticleDesc.vStartGlowColorRGBA_Min.y, m_ParticleDesc.vStartGlowColorRGBA_Max.y);
		m_vStartGlowColorRGBA.z = fRandom(m_ParticleDesc.vStartGlowColorRGBA_Min.z, m_ParticleDesc.vStartGlowColorRGBA_Max.z);
		m_vStartGlowColorRGBA.w = fRandom(m_ParticleDesc.vStartGlowColorRGBA_Min.w, m_ParticleDesc.vStartGlowColorRGBA_Max.w);
		m_vCurrentGlowColorRGBA = m_vStartGlowColorRGBA;
	}

	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());

	m_fFrustumRange = max(max(m_ParticleDesc.vStartSpeedXYZ_Max.x, m_ParticleDesc.vStartSpeedXYZ_Max.y), m_ParticleDesc.vStartSpeedXYZ_Max.z);
	switch (m_ParticleDesc.iStartPointMode)
	{
	case CVIBuffer_Rect_Instance::STARTPOINT_TYPE::POINT:
		break;
	case CVIBuffer_Rect_Instance::STARTPOINT_TYPE::CUBE:
		m_fFrustumRange += max(max(m_ParticleDesc.vRangeXYZ_Max.x, m_ParticleDesc.vRangeXYZ_Max.y), m_ParticleDesc.vRangeXYZ_Max.z);
		break;
	case CVIBuffer_Rect_Instance::STARTPOINT_TYPE::SPHERE:
		m_fFrustumRange += m_ParticleDesc.fRangeRadius;
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CEffect_Rect::Initialize_Pooling(void* pArg)
{
	USEGAMEINSTANCE;
	m_bIsEffectOff = false;

	m_pTransformCom->Set_WorldMatrix(XMMatrixIdentity());

	m_fTimeSpeed = 1.f;
	m_pOwner = nullptr;
	m_bIsHaveOwner = false;
	m_pOwnerModelCom = nullptr;
	m_strOwnerBoneName = "";
	m_eFollowMode = CEffect_Manager::FOLLOW_MODE_END;

	m_fTimeAcc = 0.f;
	m_fTimePointOfDuration = 0.f; // For. Middle Easing

	m_vStartDiffuseColorRGBA = { 1.f, 1.f, 1.f, 1.f };
	m_vCurrentDiffuseColorRGBA = { 1.f, 1.f, 1.f, 1.f };

	m_vStartGlowColorRGBA = { 1.f, 1.f, 1.f, 1.f };
	m_vCurrentGlowColorRGBA = { 1.f, 1.f, 1.f, 1.f };



	memcpy(&m_ParticleDesc, pArg, sizeof(PARTICLE_DESC));
	//FAILEDCHECK(__super::Initialize());

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_ParticleDesc.vPositionOffset.x, m_ParticleDesc.vPositionOffset.y, m_ParticleDesc.vPositionOffset.z, 1.f));

	m_vStartDiffuseColorRGBA.x = fRandom(m_ParticleDesc.vStartDiffuseColorRGBA_Min.x, m_ParticleDesc.vStartDiffuseColorRGBA_Max.x);
	m_vStartDiffuseColorRGBA.y = fRandom(m_ParticleDesc.vStartDiffuseColorRGBA_Min.y, m_ParticleDesc.vStartDiffuseColorRGBA_Max.y);
	m_vStartDiffuseColorRGBA.z = fRandom(m_ParticleDesc.vStartDiffuseColorRGBA_Min.z, m_ParticleDesc.vStartDiffuseColorRGBA_Max.z);
	m_vStartDiffuseColorRGBA.w = fRandom(m_ParticleDesc.vStartDiffuseColorRGBA_Min.w, m_ParticleDesc.vStartDiffuseColorRGBA_Max.w);
	m_vCurrentDiffuseColorRGBA = m_vStartDiffuseColorRGBA;

	if (!m_ParticleDesc.bIsGlowFollowDiffuseColor)
	{
		m_vStartGlowColorRGBA.x = fRandom(m_ParticleDesc.vStartGlowColorRGBA_Min.x, m_ParticleDesc.vStartGlowColorRGBA_Max.x);
		m_vStartGlowColorRGBA.y = fRandom(m_ParticleDesc.vStartGlowColorRGBA_Min.y, m_ParticleDesc.vStartGlowColorRGBA_Max.y);
		m_vStartGlowColorRGBA.z = fRandom(m_ParticleDesc.vStartGlowColorRGBA_Min.z, m_ParticleDesc.vStartGlowColorRGBA_Max.z);
		m_vStartGlowColorRGBA.w = fRandom(m_ParticleDesc.vStartGlowColorRGBA_Min.w, m_ParticleDesc.vStartGlowColorRGBA_Max.w);
		m_vCurrentGlowColorRGBA = m_vStartGlowColorRGBA;
	}

	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());

	pGameInstance->Add_TickGroup(TG_EFFECT, this);
	pGameInstance->Add_LateTickGroup(LTG_EFFECT, this);


	m_pVIBufferCom->Initialize_Pooling(&m_ParticleDesc);

	m_fFrustumRange = max(max(m_ParticleDesc.vStartSpeedXYZ_Max.x, m_ParticleDesc.vStartSpeedXYZ_Max.y), m_ParticleDesc.vStartSpeedXYZ_Max.z);
	switch (m_ParticleDesc.iStartPointMode)
	{
	case CVIBuffer_Rect_Instance::STARTPOINT_TYPE::POINT:
		break;
	case CVIBuffer_Rect_Instance::STARTPOINT_TYPE::CUBE:
		m_fFrustumRange += max(max(m_ParticleDesc.vRangeXYZ_Max.x, m_ParticleDesc.vRangeXYZ_Max.y), m_ParticleDesc.vRangeXYZ_Max.z);
		break;
	case CVIBuffer_Rect_Instance::STARTPOINT_TYPE::SPHERE:
		m_fFrustumRange += m_ParticleDesc.fRangeRadius;
		break;
	default:
		break;
	}

	return S_OK;
}

void CEffect_Rect::Tick(_float _fTimeDelta)
{
	NULLCHECK(m_pVIBufferCom);
	USEGAMEINSTANCE;
	_float fTimeDelta = _fTimeDelta * m_fTimeSpeed;
	m_fTimeAcc += fTimeDelta;


	// 객체 죽을 때 바로 사라지도록
	if ((m_bIsHaveOwner && m_pOwner->Is_Dead()) || m_bIsEffectOff)
	{
#ifdef _EFFECTTOOL
		pGameInstance->Add_DeadObject(this);
#else
		pGameInstance->Add_PoolingObject(this);
#endif // _EFFECTTOOL
		return;
	}

	// m_TimeAcc이 초기 StartTime Offset 보다 클 때 Start.
	if (m_fTimeAcc > m_ParticleDesc.fLifeTime.x)
	{
		// 종료 시점.
		if (m_fTimeAcc > m_ParticleDesc.fLifeTime.y + m_ParticleDesc.fLifeTime.x) // Duration + Offset
		{
			// Loop 일 때.
			if (m_ParticleDesc.bIsLoop)
			{
				m_fTimeAcc = 0.f;

				m_vStartDiffuseColorRGBA.x = fRandom(m_ParticleDesc.vStartDiffuseColorRGBA_Min.x, m_ParticleDesc.vStartDiffuseColorRGBA_Max.x);
				m_vStartDiffuseColorRGBA.y = fRandom(m_ParticleDesc.vStartDiffuseColorRGBA_Min.y, m_ParticleDesc.vStartDiffuseColorRGBA_Max.y);
				m_vStartDiffuseColorRGBA.z = fRandom(m_ParticleDesc.vStartDiffuseColorRGBA_Min.z, m_ParticleDesc.vStartDiffuseColorRGBA_Max.z);
				m_vStartDiffuseColorRGBA.w = fRandom(m_ParticleDesc.vStartDiffuseColorRGBA_Min.w, m_ParticleDesc.vStartDiffuseColorRGBA_Max.w);
				m_vCurrentDiffuseColorRGBA = m_vStartDiffuseColorRGBA;

				if (!m_ParticleDesc.bIsGlowFollowDiffuseColor)
				{
					m_vStartGlowColorRGBA.x = fRandom(m_ParticleDesc.vStartGlowColorRGBA_Min.x, m_ParticleDesc.vStartGlowColorRGBA_Max.x);
					m_vStartGlowColorRGBA.y = fRandom(m_ParticleDesc.vStartGlowColorRGBA_Min.y, m_ParticleDesc.vStartGlowColorRGBA_Max.y);
					m_vStartGlowColorRGBA.z = fRandom(m_ParticleDesc.vStartGlowColorRGBA_Min.z, m_ParticleDesc.vStartGlowColorRGBA_Max.z);
					m_vStartGlowColorRGBA.w = fRandom(m_ParticleDesc.vStartGlowColorRGBA_Min.w, m_ParticleDesc.vStartGlowColorRGBA_Max.w);
					m_vCurrentGlowColorRGBA = m_vStartGlowColorRGBA;
				}

				m_pVIBufferCom->Initialize_Loop();
				m_pVIBufferCom->Update(fTimeDelta);
			}
			else
			{
#ifdef _EFFECTTOOL
				pGameInstance->Add_DeadObject(this);
#else
				pGameInstance->Add_PoolingObject(this);
#endif // _EFFECTTOOL
			}
		}
		else // 실행
		{
			m_fTimePointOfDuration = m_fTimeAcc - m_ParticleDesc.fLifeTime.x;
			Easing_DiffuseColor();
			Easing_GlowColor();
			m_pVIBufferCom->Update(fTimeDelta);
		}
	}

}

void CEffect_Rect::LateTick(_float fTimeDelta)
{
	if (CEffect_Manager::FOLLOW_MODE_END != m_eFollowMode && !m_bIsEffectOff)
	{
		if (!m_pOwner->Is_Dead())
		{
			_matrix matFollow = XMMatrixIdentity();
			if (!m_strOwnerBoneName.compare("")) // 뼈이름이 없다면, 객체의 Transform 기준.
			{
				matFollow = XMLoadFloat4x4(m_pOwner->Get_WorldMatrix());
			}
			else // 뼈 이름이 있는 경우.
			{
				matFollow = m_pOwnerModelCom->Find_HierarcyNode(m_strOwnerBoneName.c_str())->Get_CombinedMatrix() *
					m_pOwnerModelCom->Find_HierarcyNode(m_strOwnerBoneName.c_str())->Get_TransformMatrix() *
					XMLoadFloat4x4(m_pOwner->Get_WorldMatrix());
			}

			_matrix matWorld = XMMatrixIdentity();
			if (CEffect_Manager::FOLLOW_POS_RUL == m_eFollowMode)
			{
				// 나는 로컬 기준
				matWorld.r[3] = XMVectorSet(m_ParticleDesc.vPositionOffset.x, m_ParticleDesc.vPositionOffset.y, m_ParticleDesc.vPositionOffset.z, 1.f);

				matFollow.r[0] = XMVector3Normalize(matFollow.r[0]);
				matFollow.r[1] = XMVector3Normalize(matFollow.r[1]);
				matFollow.r[2] = XMVector3Normalize(matFollow.r[2]);
				m_pTransformCom->Set_WorldMatrix(matWorld * XMLoadFloat4x4(&m_OffsetMatrix) * matFollow);
			}
			else // FOLLOW_POS
			{
				matWorld.r[3] = XMVectorSet(m_ParticleDesc.vPositionOffset.x, m_ParticleDesc.vPositionOffset.y, m_ParticleDesc.vPositionOffset.z, 1.f);
				m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, (matWorld * matFollow).r[3]);
			}
		}
	}

	if (m_ParticleDesc.fLifeTime.x <= m_fTimeAcc && IsInFrustum())
		GAMEINSTANCE->Add_RenderGroup(RENDER_EFFECT, this);
}

HRESULT CEffect_Rect::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

void CEffect_Rect::Easing_DiffuseColor(void)
{
	// Diffuse Color
	if (CEasing_Utillity::TYPE_End != m_ParticleDesc.iDiffuseColorEasing_End) // 끝 보간이 있을 때.
	{
		// 중간 보간이 있을 때.
		if (CEasing_Utillity::TYPE_End != m_ParticleDesc.iDiffuseColorEasing_Middle)
		{
			if (m_ParticleDesc.fMiddleDiffuseColorTimePoint > m_fTimePointOfDuration)
			{
				XMStoreFloat4(&m_vCurrentDiffuseColorRGBA, CEasing_Utillity::Easing(m_ParticleDesc.iDiffuseColorEasing_Middle, XMLoadFloat4(&m_vStartDiffuseColorRGBA), XMLoadFloat4(&m_ParticleDesc.vMiddleDiffuseColorRGBA), m_fTimePointOfDuration, m_ParticleDesc.fMiddleDiffuseColorTimePoint));

			}
			else
			{
				XMStoreFloat4(&m_vCurrentDiffuseColorRGBA, CEasing_Utillity::Easing(m_ParticleDesc.iDiffuseColorEasing_End, XMLoadFloat4(&m_ParticleDesc.vMiddleDiffuseColorRGBA), XMLoadFloat4(&m_ParticleDesc.vEndDiffuseColorRGBA), m_fTimePointOfDuration - m_ParticleDesc.fMiddleDiffuseColorTimePoint, m_ParticleDesc.fLifeTime.y - m_ParticleDesc.fMiddleDiffuseColorTimePoint));
			}
		}
		else // 끝 보간만 있을 때.
		{
			XMStoreFloat4(&m_vCurrentDiffuseColorRGBA, CEasing_Utillity::Easing(m_ParticleDesc.iDiffuseColorEasing_End, XMLoadFloat4(&m_vStartDiffuseColorRGBA), XMLoadFloat4(&m_ParticleDesc.vEndDiffuseColorRGBA), m_fTimePointOfDuration, m_ParticleDesc.fLifeTime.y));
		}
	}
}

void CEffect_Rect::Easing_GlowColor(void)
{
	if (!m_ParticleDesc.bIsGlowFollowDiffuseColor)
	{
		// Glow Color
		if (CEasing_Utillity::TYPE_End != m_ParticleDesc.iGlowColorEasing_End) // 끝 보간이 있을 때.
		{
			// 중간 보간이 있을 때.
			if (CEasing_Utillity::TYPE_End != m_ParticleDesc.iGlowColorEasing_Middle)
			{
				if (m_ParticleDesc.fMiddleGlowColorTimePoint > m_fTimePointOfDuration)
				{
					XMStoreFloat4(&m_vCurrentGlowColorRGBA, CEasing_Utillity::Easing(m_ParticleDesc.iGlowColorEasing_Middle, XMLoadFloat4(&m_vStartGlowColorRGBA), XMLoadFloat4(&m_ParticleDesc.vMiddleGlowColorRGBA), m_fTimePointOfDuration, m_ParticleDesc.fMiddleGlowColorTimePoint));

				}
				else
				{
					XMStoreFloat4(&m_vCurrentGlowColorRGBA, CEasing_Utillity::Easing(m_ParticleDesc.iGlowColorEasing_End, XMLoadFloat4(&m_ParticleDesc.vMiddleGlowColorRGBA), XMLoadFloat4(&m_ParticleDesc.vEndGlowColorRGBA), m_fTimePointOfDuration - m_ParticleDesc.fMiddleGlowColorTimePoint, m_ParticleDesc.fLifeTime.y - m_ParticleDesc.fMiddleGlowColorTimePoint));
				}
			}
			else // 끝 보간만 있을 때.
			{
				XMStoreFloat4(&m_vCurrentGlowColorRGBA, CEasing_Utillity::Easing(m_ParticleDesc.iGlowColorEasing_End, XMLoadFloat4(&m_vStartGlowColorRGBA), XMLoadFloat4(&m_ParticleDesc.vEndGlowColorRGBA), m_fTimePointOfDuration, m_ParticleDesc.fLifeTime.y));
			}
		}
	}
}

HRESULT CEffect_Rect::SetUp_Components()
{
	USEGAMEINSTANCE;
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXRECTINSTANCE), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_MASK), ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_MASK), (CComponent**)&m_pTextureCom));

	/* For.Com_VIBuffer */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT_INSTANCE), ID_UINT(VIBUFFER), (CComponent**)&m_pVIBufferCom, &m_ParticleDesc));

	return S_OK;
}

HRESULT CEffect_Rect::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));
	FAILEDCHECK(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", m_ParticleDesc.iMaskIndex));
	_float fUVScaleXCur = (_float)m_ParticleDesc.iUVScaleXCur;
	_float fUVScaleYCur = (_float)m_ParticleDesc.iUVScaleYCur;
	_float fUVScaleXMax = (_float)m_ParticleDesc.iUVScaleXMax;
	_float fUVScaleYMax = (_float)m_ParticleDesc.iUVScaleYMax;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsReverseXY_Mask", &m_ParticleDesc.bIsReverseXY_Mask, sizeof(_bool)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fUVScaleXCur", &fUVScaleXCur, sizeof(_float)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fUVScaleYCur", &fUVScaleYCur, sizeof(_float)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fUVScaleXMax", &fUVScaleXMax, sizeof(_float)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fUVScaleYMax", &fUVScaleYMax, sizeof(_float)));

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vDiffuseTexture", &m_vCurrentDiffuseColorRGBA, sizeof(_float4)));

	if (m_ParticleDesc.bIsGlowFollowDiffuseColor)
	{
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vGlowColor", &m_vCurrentDiffuseColorRGBA, sizeof(_float4)));
	}
	else
	{
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vGlowColor", &m_vCurrentGlowColorRGBA, sizeof(_float4)));
	}

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bGlow", &m_ParticleDesc.bIsGlow, sizeof(_bool)));
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bBlur", &m_ParticleDesc.bIsBlur, sizeof(_bool)));




	return S_OK;
}

void CEffect_Rect::Set_PositioningMode(_matrix matTransform, _float fTimeSpeed)
{
	m_fTimeSpeed = fTimeSpeed;
	m_pTransformCom->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix() * matTransform);

	if (CVIBuffer_Rect_Instance::BILLBOARD_TYPE::BILLBOARD == m_ParticleDesc.iBillBoardMode)
	{
		_vector vRight = m_pTransformCom->Get_WorldMatrix().r[0];
		_vector vUp = m_pTransformCom->Get_WorldMatrix().r[1];
		_vector vLook = m_pTransformCom->Get_WorldMatrix().r[2];
		_float3 vScale = { XMVectorGetX(XMVector3Length(vRight)), XMVectorGetX(XMVector3Length(vUp)), XMVectorGetX(XMVector3Length(vLook)) };
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x);
		m_pTransformCom->Set_State(CTransform::STATE_UP, XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z);
	}
}

void CEffect_Rect::Set_FollowingMode(_matrix matTransform, _float fTimeSpeed, CGameObject* pOwner, CEffect_Manager::FOLLOW_MODE eFollowMode, string strBoneName)
{
	m_fTimeSpeed = fTimeSpeed;

	XMStoreFloat4x4(&m_OffsetMatrix, matTransform);
	m_pOwner = pOwner;
	if (nullptr != pOwner)
		m_bIsHaveOwner = true;
	m_pOwnerModelCom = static_cast<CModel*>(m_pOwner->Get_Component(ID_UINT(MODEL_DYNAMIC)));
	m_strOwnerBoneName = strBoneName;
	m_eFollowMode = eFollowMode;
}

CEffect_Rect* CEffect_Rect::Create()
{
	CEffect_Rect* pInstance = new CEffect_Rect();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Rect::Clone(void* pArg)
{
	CEffect_Rect* pInstance = new CEffect_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Rect::Free()
{
	__super::Free();
}
