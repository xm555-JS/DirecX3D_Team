#include "stdafx.h"
#include "Cloud.h"
#include "GameInstance.h"
#include "Light.h"

#include "Imgui_Manager.h"

CCloud::CCloud()
	: CGameObject()
{
}

CCloud::CCloud(const CCloud& rhs)
	: CGameObject(rhs)
{
}

HRESULT CCloud::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCloud::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	FAILEDCHECK(__super::Initialize(&TransformDesc));

	FAILEDCHECK(SetUp_Components());
	
	_matrix TranslationMat = XMMatrixTranslation(256.f, 0.f, 256.f);
	XMStoreFloat4x4(&m_ParentMat, TranslationMat);

	Reset_CloudInfo();
	//_float	fRandomScaleX = fRandom(300.f, 550.f);
	//_float	fRandomScaleY = fRandom(150.f, 200.f);
	//_matrix ScaleMat = XMMatrixScaling(fRandomScaleX, fRandomScaleY, fRandomScaleX);
	//_matrix TranslationMat = XMMatrixTranslation(fRandom(400.f, 600.f), fRandom(200.f, 500.f), 0.f);
	//_matrix RotateMat = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRandom(0.f, 100.f));
	//_matrix WorldMat = ScaleMat * TranslationMat * RotateMat;
	//WorldMat.r[3] += XMVectorSet(256.f, 0.f, 256.f, 0.f);

	//m_pTransformCom->Set_WorldMatrix(WorldMat);

	//m_iNumTexIdx = Random(0, 5);


	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_MAP, this);
	pGameInstance->Add_LateTickGroup(LTG_MAP, this);

	return S_OK;
}

void CCloud::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	//m_fMoveSpeed = DEBUGFLOAT4(0).y;
	//m_vUVSpeed_0.x += fTimeDelta * m_fBaseMoveSpeed_0x;
	//m_vUVSpeed_0.y += fTimeDelta * m_fBaseMoveSpeed_0y;

	//m_vUVSpeed_1.x += fTimeDelta * m_fBaseMoveSpeed_1x;
	//m_vUVSpeed_1.y += fTimeDelta * m_fBaseMoveSpeed_1y;

	if(m_bAppear)
		m_fTimeAcc += fTimeDelta;

	if (0.f == m_fTimeAcc)
	{
		m_fAlphaRate += fTimeDelta * 0.1f;

		if (m_fAlphaRate > 1.f)
		{
			m_bAppear = true;
		}
	}

	if (m_fTimeAcc > m_fLifeTime)
	{
		m_fAlphaRate -= fTimeDelta;
		if(m_fAlphaRate < 0.f)
			Reset_CloudInfo();
	}

	Move_Cloud(fTimeDelta);

	//m_pTransformCom->SetBilBoard_CamLook();
}

void CCloud::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_CLOUD, this);
}

HRESULT CCloud::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	FAILEDCHECK(SetUp_ShaderResource());

	m_pShaderCom->Begin(6);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CCloud::Render(void* pArg)
{
	RENDERDESC	Desc;
	memcpy(&Desc, pArg, sizeof(RENDERDESC));

	if (RENDERTYPE_DEPTH == Desc.eType)
	{
		FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));

		CLight* pTargetLight = (CLight*)Desc.lParam;
		pTargetLight->SetUp_DepthComponents(m_pShaderCom);

		m_pShaderCom->Begin(2);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CCloud::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXTEX), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));

	/* For.Com_Model*/
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER, BUFFER_RECT), (CComponent**)&m_pVIBufferCom));

	//FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(VIBUFFER, BUFFER_RECT), ID_UINT(VIBUFFER, BUFFER_RECT), (CComponent**)&m_pVIBufferCom));

	FAILEDCHECK(__super::Add_Component(LEVEL_GAMEPLAY, ID_UINT(TEXTURE, TEXTURE_CLOUD), ID_UINT(TEXTURE, TEXTURE_CLOUD), (CComponent**)&m_pTextureCom));
	
#if defined(_NOISE_TEX)
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), ID_UINT(TEXTURE, TEXTURE_EFFECT_NOISE), (CComponent**)&m_pNoiseTexture));
#endif // _EFFECT

#ifdef _EFFECT

	//FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_CLOUD), ID_UINT(TEXTURE, TEXTURE_EFFECT_MASK), (CComponent**)&m_pTextureCom));

#endif // _EFFECT

	return S_OK;
}

HRESULT CCloud::SetUp_ShaderResource()
{
	NULLCHECK(m_pShaderCom);

	USEGAMEINSTANCE;

	// 빌보드
	_float4x4		WorldMatrixTP;
	_matrix			WorldMatrix = m_pTransformCom->Get_WorldMatrix();
	WorldMatrix *= XMLoadFloat4x4(&m_ParentMat);

	_vector vComPos = XMVectorSetW(XMLoadFloat4(&pGameInstance->Get_CamPosition()), 1.f);

	_vector vPos = WorldMatrix.r[3];

	_vector		vLook = XMVector3Normalize(vComPos - vPos);
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector		vRight = XMVector3Cross(vUp, vLook);
	vUp = XMVector3Cross(vLook, vRight);

	_float3		vScale = _float3(
		XMVectorGetX(XMVector3Length(WorldMatrix.r[0])),
		XMVectorGetX(XMVector3Length(WorldMatrix.r[1])),
		XMVectorGetX(XMVector3Length(WorldMatrix.r[2])));

	WorldMatrix.r[0] = XMVector3Normalize(vRight) * vScale.x;
	WorldMatrix.r[1] = XMVector3Normalize(vUp) * vScale.y;
	WorldMatrix.r[2] = XMVector3Normalize(vLook) * vScale.z;


	XMStoreFloat4x4(&WorldMatrixTP, XMMatrixTranspose(WorldMatrix));
	m_pShaderCom->Set_RawValue("g_WorldMatrix", &WorldMatrixTP, sizeof(_float4x4));

	//FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));


	
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fAlphaRate", &m_fAlphaRate, sizeof(_float)));
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vUVSpeed_0", &m_vUVSpeed_0, sizeof(_float2)));
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vUVSpeed_1", &m_vUVSpeed_1, sizeof(_float2)));



	FAILEDCHECK(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_iNumTexIdx));
	//FAILEDCHECK(m_pNoiseTexture->Set_ShaderResourceView(m_pShaderCom, "g_NoiseTexture_0", 271));
	//FAILEDCHECK(m_pNoiseTexture->Set_ShaderResourceView(m_pShaderCom, "g_NoiseTexture_1", 591));


	//FAILEDCHECK(m_pNoiseTexture->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 675));
#if defined(_NOISE_TEX)
	//FAILEDCHECK(m_pNoiseTexture->Set_ShaderResourceView(m_pShaderCom, "g_NoiseTexture", 675));
#endif

	return S_OK;
}

void CCloud::Reset_CloudInfo()
{
	_float	fRandomScaleX = fRandom(400.f, 600.f);
	_float	fRandomScaleY = fRandom(200.f, 300.f);
	_matrix ScaleMat = XMMatrixScaling(fRandomScaleX, fRandomScaleY, fRandomScaleX);
	_matrix TranslationMat = XMMatrixTranslation(fRandom(400.f, 600.f), fRandom(100.f, 400.f), 0.f);
	_matrix RotateMat = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRandom(-3.141592f, 3.141592f));
	_matrix WorldMat = ScaleMat * TranslationMat * RotateMat;
	//WorldMat.r[3] += XMVectorSet(256.f, 0.f, 256.f, 0.f);

	m_fMoveSpeed = fRandom(0.0006f, 0.06f);

	//_vector vUVSpeed_0 = XMVector3Normalize(XMVectorSet(fRandom(-100, 100), fRandom(-100, 100), 0.f, 0.f));
	//_vector vUVSpeed_1 = XMVector3Normalize(XMVectorSet(fRandom(-100, 100), fRandom(-100, 100), 0.f, 0.f));
	
	//XMStoreFloat2(&m_vUVSpeed_0, vUVSpeed_0);
	//XMStoreFloat2(&m_vUVSpeed_1, vUVSpeed_1);
	
	//m_vUVSpeed_0 = { 0.f, 0.f };
	//m_vUVSpeed_1 = { 0.f, 0.f };

	//_float fBaseMoveSpeed = DEBUGFLOAT4(0).x;
	//m_fBaseMoveSpeed_0x = XMVectorGetX(vUVSpeed_0) * fBaseMoveSpeed;
	//m_fBaseMoveSpeed_0y = XMVectorGetY(vUVSpeed_0) * fBaseMoveSpeed;

	//m_fBaseMoveSpeed_1x = XMVectorGetY(vUVSpeed_1) * fBaseMoveSpeed;
	//m_fBaseMoveSpeed_1y = XMVectorGetY(vUVSpeed_1) * fBaseMoveSpeed;


	m_pTransformCom->Set_WorldMatrix(WorldMat);

	m_iNumTexIdx = Random(0, 10);

	m_fLifeTime = fRandom(5, 20);
	m_fTimeAcc = 0.f;
	m_bAppear = false;
	//m_fAlphaRate = 1.f;
}

void CCloud::Move_Cloud(_float fTimeDelta)
{
	_matrix ParentMat = XMLoadFloat4x4(&m_ParentMat);
	_matrix	RotateMat = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fMoveSpeed * fTimeDelta);

	ParentMat.r[0] = XMVector3Normalize(XMVector3TransformNormal(ParentMat.r[0], RotateMat));
	ParentMat.r[1] = XMVector3Normalize(XMVector3TransformNormal(ParentMat.r[1], RotateMat));
	ParentMat.r[2] = XMVector3Normalize(XMVector3TransformNormal(ParentMat.r[2], RotateMat));

	XMStoreFloat4x4(&m_ParentMat, ParentMat);
}

CCloud* CCloud::Create()
{
	CCloud* pInstance = new CCloud();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCloud");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCloud::Clone(void* pArg)
{
	CCloud* pInstance = new CCloud(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCloud");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCloud::Free()
{
	__super::Free();
}