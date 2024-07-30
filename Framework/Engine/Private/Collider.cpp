#include "..\Public\Collider.h"
#include "DebugDraw.h"
#include "PipeLine.h"
#include "GameObject.h"
#include "GameInstance.h"

CCollider::CCollider()
{
}

CCollider::CCollider(const CCollider& Prototype)
	: CComponent(Prototype)
	, m_eType(Prototype.m_eType)
#ifdef _COLLIDER_RENDER
	, m_pBatch(Prototype.m_pBatch)
	, m_pEffect(Prototype.m_pEffect)
	, m_pInputLayout(Prototype.m_pInputLayout)
	, m_pDevice(Prototype.m_pDevice)
	, m_pContext(Prototype.m_pContext)
#endif // _DEBUG
{
#ifdef _COLLIDER_RENDER
	Safe_AddRef(m_pInputLayout);
#endif // _DEBUG
}

void CCollider::Set_Scale(_fvector vScale)
{
	switch (m_eType)
	{
	case TYPE_AABB:
		XMStoreFloat3(&m_pAABB_Original->Extents, vScale);
		break;
	case TYPE_OBB:
		XMStoreFloat3(&m_pOBB_Original->Extents, vScale);
		break;
	case TYPE_SPHERE:
		XMStoreFloat(&m_pSphere_Original->Radius, vScale);
		break;
	}
}

_vector CCollider::Get_Scale()
{
	_vector vScale;
	switch (m_eType)
	{
	case TYPE_AABB:
		vScale = XMLoadFloat3(&m_pAABB_Original->Extents);
		break;
	case TYPE_OBB:
		vScale = XMLoadFloat3(&m_pOBB_Original->Extents);
		break;
	case TYPE_SPHERE:
		vScale = XMLoadFloat(&m_pSphere_Original->Radius);
		break;
	}
	return vScale;
}

void CCollider::Set_Position(_fvector vPos)
{
	switch (m_eType)
	{
	case TYPE_AABB:
		XMStoreFloat3(&m_pAABB_Original->Center, vPos);
		break;
	case TYPE_OBB:
		XMStoreFloat3(&m_pOBB_Original->Center, vPos);
		break;
	case TYPE_SPHERE:
		XMStoreFloat3(&m_pSphere_Original->Center, vPos);
		break;
	}
}

_vector CCollider::Get_Position()
{
	_vector vPos;

	switch (m_eType)
	{
	case TYPE_AABB:
		vPos = XMLoadFloat3(&m_pAABB_Original->Center);
		break;
	case TYPE_OBB:
		vPos = XMLoadFloat3(&m_pOBB_Original->Center);
		break;
	case TYPE_SPHERE:
		vPos = XMLoadFloat3(&m_pSphere_Original->Center);
		break;
	}
	return vPos;
}

void CCollider::Clear_Colliders()
{
	m_pEnterColliders.clear();
	m_pStayColliders.clear();
	m_pExitColliders.clear();

	m_pCurColliders.clear();
	m_pPreColliders.clear();
}

HRESULT CCollider::Initialize_Prototype(TYPE eType)
{
	m_eType = eType;

#ifdef _COLLIDER_RENDER
	USEGAMEINSTANCE;
	m_pDevice = DEVICE;
	m_pContext = CONTEXT;

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext.Get());
	m_pEffect = new BasicEffect(m_pDevice.Get());
	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		ShaderByteCodeLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &ShaderByteCodeLength);

	if (FAILED(m_pDevice.Get()->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, ShaderByteCodeLength, &m_pInputLayout)))
		__debugbreak();

#endif // _DEBUG

	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{
	if (nullptr == pArg)
		__debugbreak();

	memcpy(&m_ColliderDesc, pArg, sizeof(COLLIDERDESC));

	switch (m_eType)
	{
	case TYPE_AABB:
		m_pAABB_Original = new BoundingBox(m_ColliderDesc.vTranslation, _float3(m_ColliderDesc.vScale.x * 0.5f, m_ColliderDesc.vScale.y * 0.5f, m_ColliderDesc.vScale.z * 0.5f));
		m_pAABB = new BoundingBox(*m_pAABB_Original);
		break;
	case TYPE_OBB:
		m_pOBB_Original = new BoundingOrientedBox(m_ColliderDesc.vTranslation, _float3(m_ColliderDesc.vScale.x * 0.5f, m_ColliderDesc.vScale.y * 0.5f, m_ColliderDesc.vScale.z * 0.5f), m_ColliderDesc.vRotation);
		m_pOBB = new BoundingOrientedBox(*m_pOBB_Original);
		break;
	case TYPE_SPHERE:
		m_pSphere_Original = new BoundingSphere(m_ColliderDesc.vTranslation, m_ColliderDesc.vScale.x * 0.5f);
		m_pSphere = new BoundingSphere(*m_pSphere_Original);
		break;
	}

	return S_OK;
}

void CCollider::LateTick()
{
	list<CCollider*>	PreColList = m_pPreColliders;
	list<CCollider*>	CurColList = m_pCurColliders;
	list<CCollider*>	TempColList;

	for (auto Curiter : m_pCurColliders)
	{
		for (auto Preiter : m_pPreColliders)
		{
			if (Curiter == Preiter)
			{
				TempColList.push_back(Curiter);
				PreColList.remove(Preiter);
				CurColList.remove(Curiter);
			}
		}
	}

	for (auto& pExitCol : m_pPreColliders)
	{
		if (pExitCol->Get_Owner()->Is_Dead())
			PreColList.remove(pExitCol);
	}

	m_pEnterColliders = CurColList;
	m_pStayColliders = TempColList;
	m_pExitColliders = PreColList;

	//if(CurColList.size())
	//	m_pOwner->On_EventMessege(EVENTDESC(EVENT_COLLISIONENTER, &CurColList, nullptr));

	//if (TempColList.size())
	//	m_pOwner->On_EventMessege(EVENTDESC(EVENT_COLLISIONSTAY, &TempColList, nullptr));

	//if (PreColList.size())
	//	m_pOwner->On_EventMessege(EVENTDESC(EVENT_COLLISIONEXIT, &PreColList, nullptr));
	// 
	//m_pOwner->OnCollision_Enter(CurColList);
	//m_pOwner->OnCollision(TempColList);
	//m_pOwner->OnCollision_Exit(PreColList);

	m_pPreColliders = m_pCurColliders;
	m_pCurColliders.clear();
}

#ifdef _COLLIDER_RENDER
HRESULT CCollider::Render()
{
	_float fLargestVal = max(max(m_ColliderDesc.vScale.x, m_ColliderDesc.vScale.y), m_ColliderDesc.vScale.z);
	_matrix matOwnerWorld = m_pOwner->Get_Transform()->Get_WorldMatrix();
	matOwnerWorld.r[0] = XMVector3Normalize(matOwnerWorld.r[0]);
	matOwnerWorld.r[1] = XMVector3Normalize(matOwnerWorld.r[1]);
	matOwnerWorld.r[2] = XMVector3Normalize(matOwnerWorld.r[2]);

	if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(XMVector3TransformCoord(XMLoadFloat3(&m_ColliderDesc.vTranslation), matOwnerWorld), fLargestVal * 0.5f))
	{
		m_pContext->GSSetShader(nullptr, nullptr, 0);

		m_pContext->IASetInputLayout(m_pInputLayout);

		m_pEffect->SetWorld(XMMatrixIdentity());

		CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

		m_pEffect->SetView(pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW));
		m_pEffect->SetProjection(pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ));

		RELEASE_INSTANCE(CPipeLine);

		m_pEffect->Apply(m_pContext.Get());

		//_vector		vColor = m_isColl == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f);
		_vector		vColor = m_pStayColliders.size() ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f);

		m_pBatch->Begin();

		switch (m_eType)
		{
		case TYPE_AABB:
			DX::Draw(m_pBatch, *m_pAABB, vColor);
			break;
		case TYPE_OBB:
			DX::Draw(m_pBatch, *m_pOBB, vColor);
			break;
		case TYPE_SPHERE:
			DX::Draw(m_pBatch, *m_pSphere, vColor);
			break;
		}

		m_pBatch->End();
	}

	return S_OK;
}
#endif // _DEBUG

void CCollider::Update(_fmatrix TransformMatrix)
{
	switch (m_eType)
	{
	case TYPE_AABB:
		m_pAABB_Original->Transform(*m_pAABB, Remove_Rotation(TransformMatrix));
		break;
	case TYPE_OBB:
		m_pOBB_Original->Transform(*m_pOBB, TransformMatrix);
		break;
	case TYPE_SPHERE:
		m_pSphere_Original->Transform(*m_pSphere, TransformMatrix);
		break;
	}
}

_bool CCollider::Collision(CCollider* pTargetCollider)
{
	if (false == m_bCollable ||
		false == pTargetCollider->m_bCollable)
		return false;

	if (TYPE_AABB == m_eType)
	{
		if (TYPE_AABB == pTargetCollider->m_eType)
			m_isColl = m_pAABB->Intersects(*pTargetCollider->m_pAABB);
		if (TYPE_OBB == pTargetCollider->m_eType)
			m_isColl = m_pAABB->Intersects(*pTargetCollider->m_pOBB);
		if (TYPE_SPHERE == pTargetCollider->m_eType)
			m_isColl = m_pAABB->Intersects(*pTargetCollider->m_pSphere);
	}

	if (TYPE_OBB == m_eType)
	{
		if (TYPE_AABB == pTargetCollider->m_eType)
			m_isColl = m_pOBB->Intersects(*pTargetCollider->m_pAABB);
		if (TYPE_OBB == pTargetCollider->m_eType)
			m_isColl = m_pOBB->Intersects(*pTargetCollider->m_pOBB);
		if (TYPE_SPHERE == pTargetCollider->m_eType)
			m_isColl = m_pOBB->Intersects(*pTargetCollider->m_pSphere);
	}

	if (TYPE_SPHERE == m_eType)
	{
		if (TYPE_AABB == pTargetCollider->m_eType)
			m_isColl = m_pSphere->Intersects(*pTargetCollider->m_pAABB);
		if (TYPE_OBB == pTargetCollider->m_eType)
			m_isColl = m_pSphere->Intersects(*pTargetCollider->m_pOBB);
		if (TYPE_SPHERE == pTargetCollider->m_eType)
			m_isColl = m_pSphere->Intersects(*pTargetCollider->m_pSphere);
	}

	return m_isColl;
}

_matrix CCollider::Remove_Rotation(_fmatrix TransformMatrix)
{
	_matrix			Transform = TransformMatrix;

	Transform.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * fabsf(XMVectorGetX(XMVector3Length(TransformMatrix.r[0])));
	Transform.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * fabsf(XMVectorGetX(XMVector3Length(TransformMatrix.r[1])));
	Transform.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * fabsf(XMVectorGetX(XMVector3Length(TransformMatrix.r[2])));

	return Transform;
}

void CCollider::OnCollision(CCollider* pOther)
{
	if(pOther->Get_Owner()->Is_Alive())
		m_pCurColliders.push_back(pOther);
}

CCollider* CCollider::Create(TYPE eType)
{
	CCollider* pInstance = new CCollider();

	if (FAILED(pInstance->Initialize_Prototype(eType)))
	{
		MSG_BOX("Failed to Created : CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider::Clone(void* pArg)
{
	CCollider* pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

#ifdef _COLLIDER_RENDER
	if (false == m_isCloned)
	{
		Safe_Delete(m_pEffect);
		Safe_Delete(m_pBatch);
	}
#endif // _DEBUG


	Safe_Delete(m_pAABB_Original);
	Safe_Delete(m_pOBB_Original);
	Safe_Delete(m_pSphere_Original);

	Safe_Delete(m_pAABB);
	Safe_Delete(m_pOBB);
	Safe_Delete(m_pSphere);

#ifdef _COLLIDER_RENDER
	Safe_Release(m_pInputLayout);
#endif // _DEBUG
	//m_pInputLayout->Release();
	//Safe_Release(m_pInputLayout);
}


