#include "..\Public\Cell.h"
#include "VIBuffer_Cell.h"
#include "Collider.h"
#include "GameInstance.h"
#include "Shader.h"
#include "DebugDraw.h"

CCell::CCell()
{
}

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex)
{
	m_iIndex = iIndex;

	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

	XMStoreFloat3(&m_vLine[LINE_AB], XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]));
	XMStoreFloat3(&m_vLine[LINE_BC], XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]));
	XMStoreFloat3(&m_vLine[LINE_CA], XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]));

	//m_vNormal[LINE_AB] = _float3(m_vLine[LINE_AB].z * -1.f, 0.f, m_vLine[LINE_AB].x);
	//m_vNormal[LINE_BC] = _float3(m_vLine[LINE_BC].z * -1.f, 0.f, m_vLine[LINE_BC].x);
	//m_vNormal[LINE_CA] = _float3(m_vLine[LINE_CA].z * -1.f, 0.f, m_vLine[LINE_CA].x);


	//// 3D Normal
	//_vector vPlaneNoraml = XMVector3Cross(XMLoadFloat3(&m_vLine[LINE_AB]), XMLoadFloat3(&m_vLine[LINE_BC]));
	//XMStoreFloat3(&m_vNormal[LINE_AB], XMVector3Cross(XMLoadFloat3(&m_vLine[LINE_AB]), vPlaneNoraml));
	//XMStoreFloat3(&m_vNormal[LINE_BC], XMVector3Cross(XMLoadFloat3(&m_vLine[LINE_BC]), vPlaneNoraml));
	//XMStoreFloat3(&m_vNormal[LINE_CA], XMVector3Cross(XMLoadFloat3(&m_vLine[LINE_CA]), vPlaneNoraml));



//#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Cell::Create(m_vPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	for(_uint i  = 0; i < 3; ++i)
	{
		m_vCenter.x += m_vPoints[i].x;
		m_vCenter.y += m_vPoints[i].y;
		m_vCenter.z += m_vPoints[i].z;
	}

	m_vCenter.x /= 3.f;
	m_vCenter.y /= 3.f;
	m_vCenter.z /= 3.f;


	// �߰�
	USEGAMEINSTANCE;

	m_pDevice = DEVICE.Get();
	m_pContext = CONTEXT.Get();

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);
	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		ShaderByteCodeLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &ShaderByteCodeLength);

	FAILEDCHECK(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, ShaderByteCodeLength, &m_pInputLayout));

	m_pColSphere = new BoundingSphere(m_vCenter, 0.1f);
//#endif
	// �߰� ��


//#endif // _DEBUG

	return S_OK;
}

HRESULT CCell::Initialize(CELLDESC CellDesc)
{
	m_iIndex = CellDesc.iIndex;
	m_vPlaneNormal = CellDesc.vPlaneNormal;
	m_iProperty = CellDesc.iProperty;
	memcpy(m_vPoints, CellDesc.vPoints, sizeof(_float3) * POINT_END);
	//memcpy(m_vLine, CellDesc.vLine, sizeof(_float3) * LINE_END);

	memcpy(m_vLine, CellDesc.vLine, sizeof(_float3) * LINE_END);

	//XMStoreFloat3(&m_vLine[LINE_AB], XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]));
	//XMStoreFloat3(&m_vLine[LINE_BC], XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]));
	//XMStoreFloat3(&m_vLine[LINE_CA], XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]));

	//m_vNormal[LINE_AB] = _float3(m_vLine[LINE_AB].z * -1.f, 0.f, m_vLine[LINE_AB].x);
	//m_vNormal[LINE_BC] = _float3(m_vLine[LINE_BC].z * -1.f, 0.f, m_vLine[LINE_BC].x);
	//m_vNormal[LINE_CA] = _float3(m_vLine[LINE_CA].z * -1.f, 0.f, m_vLine[LINE_CA].x);

	//_vector vPlaneNoraml = XMVector3Cross(XMLoadFloat3(&m_vLine[LINE_AB]), XMLoadFloat3(&m_vLine[LINE_BC]));
	//XMStoreFloat3(&m_vNormal[LINE_AB], XMVector3Cross(XMLoadFloat3(&m_vLine[LINE_AB]), vPlaneNoraml));
	//XMStoreFloat3(&m_vNormal[LINE_BC], XMVector3Cross(XMLoadFloat3(&m_vLine[LINE_BC]), vPlaneNoraml));
	//XMStoreFloat3(&m_vNormal[LINE_CA], XMVector3Cross(XMLoadFloat3(&m_vLine[LINE_CA]), vPlaneNoraml));

	memcpy(m_vNormal, CellDesc.vNormal, sizeof(_float3) * LINE_END);
	memcpy(m_iNeighbor, CellDesc.iNeighbor, sizeof(_uint) * LINE_END);

	//XMStoreFloat3(&m_vLine[LINE_AB], XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]));
	//XMStoreFloat3(&m_vLine[LINE_BC], XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]));
	//XMStoreFloat3(&m_vLine[LINE_CA], XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]));

	//m_vNormal[LINE_AB] = _float3(m_vLine[LINE_AB].z * -1.f, 0.f, m_vLine[LINE_AB].x);
	//m_vNormal[LINE_BC] = _float3(m_vLine[LINE_BC].z * -1.f, 0.f, m_vLine[LINE_BC].x);
	//m_vNormal[LINE_CA] = _float3(m_vLine[LINE_CA].z * -1.f, 0.f, m_vLine[LINE_CA].x);

	//m_iNeighbor[LINE_AB] = CellDesc.iNeighbor[LINE_AB];
	//m_iNeighbor[LINE_BC] = CellDesc.iNeighbor[LINE_BC];
	//m_iNeighbor[LINE_CA] = CellDesc.iNeighbor[LINE_CA];

//#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Cell::Create(m_vPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;


//#endif // _DEBUG

	for (_uint i = 0; i < POINT_END; ++i)
	{
		m_vCenter.x += m_vPoints[i].x;
		m_vCenter.y += m_vPoints[i].y;
		m_vCenter.z += m_vPoints[i].z;
	}
	m_vCenter.x /= 3.f;
	m_vCenter.y /= 3.f;
	m_vCenter.z /= 3.f;

#if 0	//�׺� �� ��ġȮ�ο�
	USEGAMEINSTANCE;

	m_pDevice = DEVICE.Get();
	m_pContext = CONTEXT.Get();

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);
	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		ShaderByteCodeLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &ShaderByteCodeLength);

	FAILEDCHECK(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, ShaderByteCodeLength, &m_pInputLayout));

	m_pColSphere = new BoundingSphere(m_vCenter, 0.5f);
#endif

	return S_OK;
}


/* ���޹��� �ΰ��� ���� �� ������ ���� ���� ���� �ΰ���?! */
_bool CCell::Compare_Points(_fvector vSourPoint, _fvector vDestPoint, _int iSourIdx, _int iDestIdx, _bool* bReverseState)
{

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
		{
			//if ((POINT_A == iSourIdx && POINT_B == iDestIdx) ||
			//	(POINT_B == iSourIdx && POINT_C == iDestIdx) ||
			//	(POINT_C == iSourIdx && POINT_A == iDestIdx))
			//{
			//	*bReverseState = true;
			//}
			//else
			//	*bReverseState = false;

			return true;
		}

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
		{
			//if ((POINT_A == iSourIdx && POINT_C == iDestIdx) ||
			//	(POINT_B == iSourIdx && POINT_A == iDestIdx) ||
			//	(POINT_C == iSourIdx && POINT_B == iDestIdx))
			//{
			//	*bReverseState = true;
			//}
			//else
			//	*bReverseState = false;

			return true;
		}
	}


	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
		{
			//if ((POINT_A == iSourIdx && POINT_C == iDestIdx) ||
			//	(POINT_B == iSourIdx && POINT_A == iDestIdx) ||
			//	(POINT_C == iSourIdx && POINT_B == iDestIdx))
			//{
			//	*bReverseState = true;
			//}
			//else
			//	*bReverseState = false;

			return true;
		}

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
		{
			//if ((POINT_A == iSourIdx && POINT_B == iDestIdx) ||
			//	(POINT_B == iSourIdx && POINT_C == iDestIdx) ||
			//	(POINT_C == iSourIdx && POINT_A == iDestIdx))
			//{
			//	*bReverseState = true;
			//}
			//else
			//	*bReverseState = false;

			return true;
		}
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
		{
			//if ((POINT_A == iSourIdx && POINT_B == iDestIdx) ||
			//	(POINT_B == iSourIdx && POINT_C == iDestIdx) ||
			//	(POINT_C == iSourIdx && POINT_A == iDestIdx))
			//{
			//	*bReverseState = true;
			//}
			//else
			//	*bReverseState = false;

			return true;
		}

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
		{
			//if ((POINT_A == iSourIdx && POINT_C == iDestIdx) ||
			//	(POINT_B == iSourIdx && POINT_A == iDestIdx) ||
			//	(POINT_C == iSourIdx && POINT_B == iDestIdx))
			//{
			//	*bReverseState = true;
			//}
			//else
			//	*bReverseState = false;

			return true;
		}
	}

	return false;
}

_bool CCell::isIn(_fvector vPosition, _int* pNeighborIndex)
{
	/* �� ���� ������ ���ؼ� �������� �ȳ������� �Ǵ��Ѵ�. */
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector	vDir = vPosition - XMLoadFloat3(&m_vPoints[i]);

		/* �ٱ����� ������ �� */
		// �����ؼ� cos ���� 1���� ũ�� ��������.
		// ��¥ �������� üũ�ϰ� ����, 
		if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir), XMVector3Normalize(XMLoadFloat3(&m_vNormal[i])))))
		{
			*pNeighborIndex = m_iNeighbor[i];
			return false;
		}
	}
	return true;
}

_bool CCell::Is_GetSlideDir(_fvector vPosition_Cur, _fvector vPosition_Next, _vector* vDirRtn, _int* iNeighborIndexRtn)
{
	/* �� ���� ������ ���ؼ� �������� �ȳ������� �Ǵ��Ѵ�. */
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector	vDir = vPosition_Next - XMLoadFloat3(&m_vPoints[i]);

		// �����ؼ� cos ���� 1���� ũ�� ��������.
		// ��¥ �������� üũ�ϰ� ����, 
		if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir), XMVector3Normalize(XMLoadFloat3(&m_vNormal[i])))))
		{
			// ���� ���� �����ʿ� �̿��� �ִٸ�, ����.
			if (-1 != m_iNeighbor[i])
				continue;
			// ���� �����ʿ� �̿��� ���ٸ� ��¥ ���� ���̱� ������ �����̵��� ���� ������.

			// �̰� ���⺤��.
			_vector vPosDir = XMVector3Normalize(vPosition_Next - vPosition_Cur);

			// ���� �������Ϳ� ���� ������ �������Ͱ� �����ؼ� �̷�� ���� ���� ������ �Ǵ�.
			if (0 < XMVectorGetX(XMVector3Dot(vPosDir, XMVector3Normalize(XMLoadFloat3(&m_vLine[i])))))
			{
				//���й���
				*vDirRtn = XMVector3Normalize(XMLoadFloat3(&m_vLine[i]));
				return true;
			}
			else
			{
				// ���� �ݴ� ����
				*vDirRtn = -1.f * XMVector3Normalize(XMLoadFloat3(&m_vLine[i]));
				return true;
			}
		}

	}
	// ������ �� ���� ��� �̿� Cell �� �����ϸ�, ����.
	// �˻� ������ �̿� Cell�� �ٲ㼭 �ٽ� �˻��Ѵ�.
	// �̿� �ε��� ���.
	_int iNeighborIndex = -1;
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector	vDir = vPosition_Next - XMLoadFloat3(&m_vPoints[i]);

		if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir), XMVector3Normalize(XMLoadFloat3(&m_vNormal[i])))))
		{
			iNeighborIndex = m_iNeighbor[i];
			break;
		}
	}

	// ���⺤�� �ޱ� ������ ���, �������� �˻��� �̿� �� �ε��� ��ȯ. 
	*iNeighborIndexRtn = iNeighborIndex;
	return false;
}

//#ifdef _DEBUG
HRESULT CCell::Render()
{
#if 0	//�׺� �� ��ġȮ�ο�
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	//// �� Render
	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pEffect->SetWorld(XMMatrixIdentity());

	USEGAMEINSTANCE;
	m_pEffect->SetView(pGameInstance->Get_Transform(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(pGameInstance->Get_Transform(CPipeLine::D3DTS_PROJ));


	m_pEffect->Apply(m_pContext);

	m_pBatch->Begin();

	DX::Draw(m_pBatch, *m_pColSphere, DirectX::Colors::Green);

	m_pBatch->End();
#endif

	m_pVIBuffer->Render();

	return S_OK;
}

HRESULT CCell::Render_Able(_float4 vPos)
{
	_float fDistance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vCenter) - XMLoadFloat4(&vPos)));

	if (fDistance > 50.f)
		return E_FAIL;

	m_pVIBuffer->Render();

	return S_OK;
}
//#endif // _DEBUG

CCell* CCell::Create(const _float3* pPoints, _int iIndex)
{
	CCell* pInstance = new CCell();

	if (FAILED(pInstance->Initialize(pPoints, iIndex)))
	{
		MSG_BOX("Failed to Created : CCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCell* CCell::Create(CELLDESC& CellDesc)
{
	CCell* pInstance = new CCell();

	if (FAILED(pInstance->Initialize(CellDesc)))
	{
		MSG_BOX("Failed to Created : CCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{
//#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);
//#endif // _DEBUG

//#ifdef 	//�׺� �� ��ġȮ�ο�
	//Safe_Delete(m_pEffect);
	//Safe_Delete(m_pBatch);

	//Safe_Release(m_pInputLayout);

	//Safe_Delete(m_pColSphere);
//#endif
}
