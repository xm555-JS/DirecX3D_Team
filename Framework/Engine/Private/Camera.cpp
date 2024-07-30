#include "..\Public\Camera.h"
#include "GameInstance.h"
#include "Renderer.h"

CCamera::CCamera()
	: CGameObject()
	, m_pRenderer(CRenderer::Create())
{
}

CCamera::CCamera(const CCamera & Prototype)
	: CGameObject(Prototype)
	, m_pRenderer((CRenderer*)Prototype.m_pRenderer->Clone())
{
}

HRESULT CCamera::Initialize_Prototype()
{
	m_pRenderer->Set_Owner(this);
	return S_OK;
}

HRESULT CCamera::Initialize(void * pArg)
{
	if (nullptr == pArg)
		__debugbreak();

	memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));

	USEGAMEINSTANCE;
	CTransform::TRANSFORMDESC		TransformDesc;
	if (FAILED(CGameObject::Initialize(&TransformDesc)))
		__debugbreak();
	m_Components.emplace(pGameInstance->Get_TransformID().iID, m_pTransformCom);
	//Safe_AddRef(m_pTransformCom);

	m_pTransformCom->Set_TransformDesc(&m_CameraDesc.TransformDesc);
	/*_vector		vPos = XMLoadFloat3(&m_CameraDesc.vEye);
	vPos = XMVectorSetW(vPos, 1.f);*/
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_CameraDesc.vEye));
	m_pTransformCom->LookAt(XMLoadFloat4(&m_CameraDesc.vAt));

	m_pRenderer->Set_Owner(this);

	return S_OK;
}

void CCamera::Tick_Camera(_float fTimeDelta)
{
	if (FAILED(Bind_PipeLine()))
		return;

	__super::Tick(fTimeDelta);
}

HRESULT CCamera::Add_RenderGroup(RENDERGROUP eGroup, CGameObject* pGameObject)
{
	if (eGroup >= RENDER_END ||
		nullptr == pGameObject)
		__debugbreak();


	m_pRenderer->Add_RenderGroup(eGroup, pGameObject);

	return S_OK;
}

HRESULT CCamera::Bind_PipeLine()
{
	CPipeLine*			pPipeLine = GET_INSTANCE(CPipeLine);

	_matrix		ViewMatrix = XMMatrixInverse(nullptr, m_pTransformCom->Get_WorldMatrix());
	pPipeLine->Set_Transform(CPipeLine::D3DTS_VIEW, ViewMatrix);

	_matrix		ProjMatrix = XMMatrixPerspectiveFovLH(m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);
	pPipeLine->Set_Transform(CPipeLine::D3DTS_PROJ, ProjMatrix);

	RELEASE_INSTANCE(CPipeLine);

	return S_OK;
}

CCamera* CCamera::Create()
{
	CCamera* pInstance = new CCamera();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera::Clone(void* pArg)
{
	CCamera* pInstance = new CCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera::Free()
{
	__super::Free();
	Safe_Release(m_pRenderer);
}
