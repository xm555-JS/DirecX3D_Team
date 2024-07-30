#include "..\Public\Light_Target.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"
#include "GameInstance.h"
#include "RenderTarget.h"
#include "Light_Manager.h"
#include "GameObject.h"

CLight_Target::CLight_Target()
{
}

void CLight_Target::Set_TargetObject(CGameObject* pTarget)
{
	m_pTargetTransform = (CTransform*)pTarget->Get_Component(ID_UINT(TRANSFORM));
	m_pTarget = pTarget;

	assert(m_pTargetTransform);
}

void CLight_Target::Set_Offset(_fvector vOffset)
{
	assert(m_pTarget);
	assert(m_pTargetTransform);
	XMStoreFloat3(&m_vOffset, vOffset);
	Update_Target(m_pTargetTransform);
}

void CLight_Target::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	if (m_pTarget->Is_Dead())
	{
		pGameInstance->Add_DeadBase(this);
		return;
	}

	Update_Target(m_pTargetTransform);

	CLight::Tick(fTimeDelta);
}

HRESULT CLight_Target::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	_uint			iPassIndex = 0;
	if (true == m_bUseForShadow)
		return S_OK;

	FAILEDCHECK(pShader->Set_RawValue("g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4)));
	FAILEDCHECK(pShader->Set_RawValue("g_fRange", &m_LightDesc.fRange, sizeof(_float)));

	FAILEDCHECK(pShader->Set_RawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4)));
	FAILEDCHECK(pShader->Set_RawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4)));
	FAILEDCHECK(pShader->Set_RawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4)));


	pShader->Begin(iPassIndex);
	pVIBuffer->Render();

	return S_OK;
}


HRESULT CLight_Target::Render_Shadows(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	_float			fPower = 3.f;
	FAILEDCHECK(pShader->Set_RawValue("g_fPower", &fPower, sizeof(_float)));
	FAILEDCHECK(pShader->Set_RawValue("g_fRange", &m_LightDesc.fRange, sizeof(_float)));

	_float4x4	ViewMatrix, ProjMatrix;
	XMStoreFloat4x4(&ViewMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_ViewMatrix)));
	XMStoreFloat4x4(&ProjMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_ProjMatrix)));

	FAILEDCHECK(pShader->Set_RawValue("g_LightViewMatrix", &ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(pShader->Set_RawValue("g_LightProjMatrix", &ProjMatrix, sizeof(_float4x4)));
	FAILEDCHECK(pShader->Set_ShaderResourceView("g_LightDepthTexture", m_pLightDepth->Get_SRV()));

	pShader->Begin(0);
	pVIBuffer->Render();

	//m_pLightDepth->Render_Debug(pShader, pVIBuffer);

	return S_OK;
}

void CLight_Target::Update_Target(CTransform* pTargetTransform)
{
	_vector vTargetPos = pTargetTransform->Get_State(CTransform::STATE_TRANSLATION);
	_vector vFinalPos = XMLoadFloat3(&m_vOffset) + vTargetPos;
	vFinalPos = XMVectorSetW(vFinalPos, 1.f);
	XMStoreFloat4(&m_LightDesc.vPosition, vFinalPos);
}

void CLight_Target::Setup_ViewPort()
{
	_vector		vLightEye = XMLoadFloat4(&m_LightDesc.vPosition);
	_vector		vLightAt;

	NULLCHECK(m_pTarget);

	if (m_pTarget)
	{
		if (true == m_pTarget->Is_Dead())
		{
			m_pTarget = nullptr;
			vLightAt = XMVectorSet(256.f, 0.f, 256.f, 1.f);
		}
		else
		{
			vLightAt = m_pTarget->Get_Pos();
			vLightEye = XMLoadFloat4(&m_LightDesc.vPosition);
		}
	}
	else
		vLightAt = XMVectorSet(256.f, 0.f, 256.f, 1.f);

	_vector		vLightUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp));
}

CLight_Target* CLight_Target::Create(const LIGHTDESC& LightDesc)
{
	CLight_Target* pInstance = new CLight_Target();

	if (FAILED(pInstance->Initialize(LightDesc)))
	{
		MSG_BOX("Failed to Created : CLight_Target");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight_Target::Free()
{
	__super::Free();
}
