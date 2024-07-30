#include "..\Public\Transform.h"
#include "Shader.h"
#include "PipeLine.h"
#include "Navigation.h"
#include "Cell.h"

CTransform::CTransform()
	: CComponent()
{
}

CTransform::CTransform(const CTransform& rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
	, m_CombinedTransformationMatrix(rhs.m_CombinedTransformationMatrix)
{
	XMStoreFloat4x4(&m_RootBoneMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_vPreRootMatrix, XMMatrixIdentity());
}

_vector CTransform::Get_Quaternion()
{
	_vector		vRight = XMVector3Normalize(Get_State(CTransform::STATE_RIGHT));
	_vector		vUp = XMVector3Normalize(Get_State(CTransform::STATE_UP));
	_vector		vLook = XMVector3Normalize(Get_State(CTransform::STATE_LOOK));

	_float4x4 WorldMatrix;
	ZeroMemory(&WorldMatrix, sizeof(_float4x4));
	_float4		vData;

	XMStoreFloat4(&vData, vRight);
	memcpy(&WorldMatrix.m[STATE_RIGHT], &vData, sizeof(_float4));

	XMStoreFloat4(&vData, vUp);
	memcpy(&WorldMatrix.m[STATE_UP], &vData, sizeof(_float4));

	XMStoreFloat4(&vData, vLook);
	memcpy(&WorldMatrix.m[STATE_LOOK], &vData, sizeof(_float4));

	return XMQuaternionRotationMatrix(XMLoadFloat4x4(&WorldMatrix));
}

_matrix CTransform::Get_RotationMatrix()
{
	_vector		vRight = XMVector3Normalize(Get_State(CTransform::STATE_RIGHT));
	_vector		vUp = XMVector3Normalize(Get_State(CTransform::STATE_UP));
	_vector		vLook = XMVector3Normalize(Get_State(CTransform::STATE_LOOK));

	_float4x4 WorldMatrix;
	ZeroMemory(&WorldMatrix, sizeof(_float4x4));
	_float4		vData;

	XMStoreFloat4(&vData, vRight);
	memcpy(&WorldMatrix.m[STATE_RIGHT], &vData, sizeof(_float4));

	XMStoreFloat4(&vData, vUp);
	memcpy(&WorldMatrix.m[STATE_UP], &vData, sizeof(_float4));

	XMStoreFloat4(&vData, vLook);
	memcpy(&WorldMatrix.m[STATE_LOOK], &vData, sizeof(_float4));

	WorldMatrix._44 = 1.f;

	return XMLoadFloat4x4(&WorldMatrix);
}

void CTransform::Set_Offset(_vector vOffset)
{
	_vector		vPos = Get_State(CTransform::STATE_TRANSLATION);

	_vector		vRight = XMVector3Normalize(Get_State(CTransform::STATE_RIGHT));
	//_vector		vUp = XMVector3Normalize(Get_State(CTransform::STATE_UP));
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector		vLook = XMVector3Normalize(Get_State(CTransform::STATE_LOOK));

	vPos += XMVector3Normalize(vRight) * XMVectorGetX(vOffset);
	vPos += XMVector3Normalize(vUp) * XMVectorGetY(vOffset);
	vPos += XMVector3Normalize(vLook) * XMVectorGetZ(vOffset);
	vPos = XMVectorSetW(vPos, 1.f);

	Set_State(CTransform::STATE_TRANSLATION, vPos);
}

void CTransform::Set_Scaled(_float3 vScale)
{
	_vector		vRight = XMVector3Normalize(Get_State(CTransform::STATE_RIGHT));
	_vector		vUp = XMVector3Normalize(Get_State(CTransform::STATE_UP));
	_vector		vLook = XMVector3Normalize(Get_State(CTransform::STATE_LOOK));

	vRight = XMVector3Normalize(vRight) * vScale.x;
	vUp = XMVector3Normalize(vUp) * vScale.y;
	vLook = XMVector3Normalize(vLook) * vScale.z;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Set_RootBoneMatrix(_float4x4 RootBoneMatrix, CNavigation* pNaviCom)
{
	if (m_bRootBoneReset)
	{
		m_bRootBoneReset = false;
		return;
	}

	_vector	vPos = XMLoadFloat4x4(&m_WorldMatrix).r[3];


	_matrix RootMatrix = XMLoadFloat4x4(&RootBoneMatrix);
	_vector vRootPos = RootMatrix.r[3];
	_vector vCurRootPos = vRootPos;
	_vector vMoveDir = vRootPos - XMLoadFloat3(&m_vPreRootPos);

	XMStoreFloat3(&m_vPreRootPos, vCurRootPos);


	_matrix	RotationMat = Get_RotationMatrix();
	_vector vFinalPos = XMVector3TransformCoord(vMoveDir, RotationMat);


	if (nullptr != pNaviCom &&
		false == m_bIsInAir_Glide)
	{
		_float fCellAngleSpeed = XMVectorGetX(XMVector3Dot(XMVector3Normalize(pNaviCom->Get_CurCellDesc()->Get_PlaneNormal()), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
		//cout << "test : " << fCellAngleSpeed << "Test2 : " << fCellAngleSpeed * fCellAngleSpeed * fCellAngleSpeed << endl;

		if (true == m_bIsInAir)
			fCellAngleSpeed = 1.f;

		vPos += vFinalPos * fCellAngleSpeed;
		Set_State(STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPos));
	}
	else
		Set_State(STATE_TRANSLATION, vFinalPos + vPos);
}

void CTransform::Set_RootBoneMatrix_Rot(_float4x4 RootBoneMatrix, CNavigation* pNaviCom)
{
	if (m_bRootBoneReset)
	{
		m_bRootBoneReset = false;
		return;
	}

	_matrix	vWorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);


	_matrix RootMatrix = XMLoadFloat4x4(&RootBoneMatrix);

	//_vector vRootPos = RootMatrix.r[3];
	//_vector vCurRootPos = vRootPos;
	_matrix vCurRootMatrix = RootMatrix;
	_matrix MoveMatrix = RootMatrix * XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_vPreRootMatrix));

	XMStoreFloat4x4(&m_vPreRootMatrix, vCurRootMatrix);


	//_matrix	RotationMat = Get_RotationMatrix();
	//_vector vFinalPos = XMVector3TransformCoord(vMoveDir, RotationMat);
	_matrix	FinalMatrix =  MoveMatrix * vWorldMatrix;

	Set_WorldMatrix(FinalMatrix);

	//if (nullptr != pNaviCom &&
	//	false == m_bIsInAir_Glide)
	//{
	//	_float fCellAngleSpeed = XMVectorGetX(XMVector3Dot(XMVector3Normalize(pNaviCom->Get_CurCellDesc()->Get_PlaneNormal()), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	//	//cout << "test : " << fCellAngleSpeed << "Test2 : " << fCellAngleSpeed * fCellAngleSpeed * fCellAngleSpeed << endl;
	//
	//if (true == m_bIsInAir)
	//	fCellAngleSpeed = 1.f;
	//	vPos += vFinalPos * fCellAngleSpeed;
	// 
	//	Set_State(STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPos));
	//}
	//else
	//	Set_State(STATE_TRANSLATION, vFinalPos + vPos);
}

void CTransform::SetBilBoard()
{
	_matrix	ViewMatrixInv;

	ViewMatrixInv = CPipeLine::Get_Instance()->Get_Transform(CPipeLine::D3DTS_VIEW);
	ViewMatrixInv = XMMatrixInverse(nullptr, ViewMatrixInv);

	Set_State(CTransform::STATE_RIGHT, ViewMatrixInv.r[0] * XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_RIGHT))));
	Set_State(CTransform::STATE_UP, ViewMatrixInv.r[1] * XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_UP))));
	Set_State(CTransform::STATE_LOOK, ViewMatrixInv.r[2] * XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_LOOK))));
}

void CTransform::SetBilBoard_CamLook()
{
	_vector vComPos = XMVectorSetW(XMLoadFloat4(&CPipeLine::Get_Instance()->Get_CamPosition()), 1.f);

	_vector vPos = Get_State(CTransform::STATE_TRANSLATION);

	_vector		vLook = XMVector3Normalize(vComPos - vPos);
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector		vRight = XMVector3Cross(vUp, vLook);
	vUp = XMVector3Cross(vLook, vRight);

	_float3		vScale = Get_Scaled();

	Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CTransform::Set_RotationMatrix(_matrix RotateMatrix)
{
	_vector		vRight = RotateMatrix.r[0];
	_vector		vUp = RotateMatrix.r[1];
	_vector		vLook = RotateMatrix.r[2];

	_float3		vScale = Get_Scaled();

	Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

_float3 CTransform::Get_Scaled()
{
	return _float3(XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_RIGHT))),
		XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_UP))),
		XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_LOOK))));
}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
	return S_OK;
}

void CTransform::Tick(_float fTimeDelta)
{
	Move_JumpPos(fTimeDelta, m_fPower, m_pNavigation, m_fGravity);
	Update_CombinedTransformationMatrix();
}

HRESULT CTransform::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	_float4x4		WorldMatrixTP;
	XMStoreFloat4x4(&WorldMatrixTP, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));
	pShader->Set_RawValue(pConstantName, &WorldMatrixTP, sizeof(_float4x4));

	return S_OK;
}

HRESULT CTransform::Go_Straight(_float fTimeDelta, _float fMoveSpeed, CNavigation* pNaviCom)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);
	//Set_State(CTransform::STATE_TRANSLATION, vPosition);

	if (nullptr != pNaviCom &&
		false == m_bIsInAir_Glide)
	{
		_float fCellAngleSpeed = XMVectorGetX(XMVector3Dot(XMVector3Normalize(pNaviCom->Get_CurCellDesc()->Get_PlaneNormal()), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
		//cout << "test : " << fCellAngleSpeed << "Test2 : " << fCellAngleSpeed * fCellAngleSpeed * fCellAngleSpeed << endl;

		if (true == m_bIsInAir)
			fCellAngleSpeed = 1.f;

		vPosition += XMVector3Normalize(vLook) * fTimeDelta * fMoveSpeed * fCellAngleSpeed;
		Set_State(CTransform::STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPosition));
	}
	else
	{
		vPosition += XMVector3Normalize(vLook) * fTimeDelta * fMoveSpeed;
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}

	return S_OK;
}

HRESULT CTransform::Go_Front(_float fTimeDelta, _float fMoveSpeed, CNavigation* pNaviCom)
{
#if 0 // 이전 코드
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);
	_float		fYPosition = XMVectorGetY(vPosition);

	vPosition += XMVector3Normalize(vLook) * fTimeDelta * fMoveSpeed;
	vPosition = XMVectorSetY(vPosition, fYPosition);

	Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return S_OK;
#endif // 0


	_vector		vFront = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	vFront = XMVector3Cross(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f));

	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	//vPosition += XMVector3Normalize(vFront) * fMoveSpeed * fTimeDelta;

#if 0	// 라인 안타고 정지하게 하고 싶은 경우.
	if (nullptr != pNaviCom)
	{
		if (false == pNaviCom->isMove(vPosition))
			return S_OK;
}

	Set_State(CTransform::STATE_TRANSLATION, vPosition);

#else	// 라인 타기


	if (nullptr != pNaviCom &&
		false == m_bIsInAir_Glide)
	{
		_float fCellAngleSpeed = XMVectorGetX(XMVector3Dot(XMVector3Normalize(pNaviCom->Get_CurCellDesc()->Get_PlaneNormal()), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
		//cout << "test : " << fCellAngleSpeed << "Test2 : " << fCellAngleSpeed * fCellAngleSpeed * fCellAngleSpeed << endl;

		if (true == m_bIsInAir)
			fCellAngleSpeed = 1.f;

		vPosition += XMVector3Normalize(vFront) * fMoveSpeed * fTimeDelta * fCellAngleSpeed;
		Set_State(CTransform::STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPosition));
	}
	else
	{
		vPosition += XMVector3Normalize(vFront) * fMoveSpeed * fTimeDelta;
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}

#endif // 0

	return S_OK;
}

HRESULT CTransform::Go_Backward(_float fTimeDelta, _float fMoveSpeed, CNavigation* pNaviCom)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * fTimeDelta * fMoveSpeed;

	//Set_State(CTransform::STATE_TRANSLATION, vPosition);

	if (nullptr != pNaviCom)
		Set_State(CTransform::STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPosition));
	else
		Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Back(_float fTimeDelta, _float fMoveSpeed, CNavigation* pNaviCom)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);
	_float		fYPosition = XMVectorGetY(vPosition);

	vPosition -= XMVector3Normalize(vLook) * fTimeDelta * fMoveSpeed;
	vPosition = XMVectorSetY(vPosition, fYPosition);

	//Set_State(CTransform::STATE_TRANSLATION, vPosition);

	if (nullptr != pNaviCom)
		Set_State(CTransform::STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPosition));
	else
		Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Left(_float fTimeDelta, _float fMoveSpeed, CNavigation* pNaviCom)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * fTimeDelta * fMoveSpeed;

	//Set_State(CTransform::STATE_TRANSLATION, vPosition);

	if (nullptr != pNaviCom)
		Set_State(CTransform::STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPosition));
	else
		Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Right(_float fTimeDelta, _float fMoveSpeed, CNavigation* pNaviCom)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * fTimeDelta * fMoveSpeed;

	//Set_State(CTransform::STATE_TRANSLATION, vPosition);

	if (nullptr != pNaviCom)
		Set_State(CTransform::STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPosition));
	else
		Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Up(_float fTimeDelta, _float fMoveSpeed, CNavigation* pNaviCom)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vUp = Get_State(CTransform::STATE_UP);

	vPosition += XMVector3Normalize(vUp) * fTimeDelta * fMoveSpeed;

	//Set_State(CTransform::STATE_TRANSLATION, vPosition);

	if (nullptr != pNaviCom)
		Set_State(CTransform::STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPosition));
	else
		Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Down(_float fTimeDelta, _float fMoveSpeed, CNavigation* pNaviCom)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vUp = Get_State(CTransform::STATE_UP);

	vPosition -= XMVector3Normalize(vUp) * fTimeDelta * fMoveSpeed;

	//Set_State(CTransform::STATE_TRANSLATION, vPosition);

	if (nullptr != pNaviCom)
		Set_State(CTransform::STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPosition));
	else
		Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_AccFront(_float fTimeDelta, _float fMoveSpeed, CNavigation* pNaviCom)
{
	_vector		vFront = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	vFront = XMVector3Cross(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f));

	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);

	_float fPrePos = 0.5f * fMoveSpeed * m_fFrontAccTime * m_fFrontAccTime;
	m_fFrontAccTime += fTimeDelta;
	_float fCurPos = 0.5f * fMoveSpeed * m_fFrontAccTime * m_fFrontAccTime;

	_float fFinalPosDiff = fCurPos - fPrePos;

	if (nullptr != pNaviCom &&
		false == m_bIsInAir_Glide)
	{
		_float fCellAngleSpeed = XMVectorGetX(XMVector3Dot(XMVector3Normalize(pNaviCom->Get_CurCellDesc()->Get_PlaneNormal()), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
		//cout << "test : " << fCellAngleSpeed << "Test2 : " << fCellAngleSpeed * fCellAngleSpeed * fCellAngleSpeed << endl;

		if (true == m_bIsInAir)
			fCellAngleSpeed = 1.f;

		vPosition += XMVector3Normalize(vFront) * fFinalPosDiff * fCellAngleSpeed;
		Set_State(CTransform::STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPosition));
	}
	else
	{
		vPosition += XMVector3Normalize(vFront) * fFinalPosDiff;
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}

	return S_OK;
}

HRESULT CTransform::Go_AccFront_Limit(_float fTimeDelta, _float fMoveSpeed, _float fLimitSpeed, CNavigation* pNaviCom, _float* fOut)
{
	_vector		vFront = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	vFront = XMVector3Cross(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f));

	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);

	_float fPrePos = 0.5f * fMoveSpeed * m_fFrontAccTime * m_fFrontAccTime;
	m_fFrontAccTime += fTimeDelta;
	_float fCurPos = 0.5f * fMoveSpeed * m_fFrontAccTime * m_fFrontAccTime;

	_float fFinalPosDiff = fCurPos - fPrePos;

	if (fLimitSpeed * fTimeDelta < fFinalPosDiff)
		fFinalPosDiff = fLimitSpeed * fTimeDelta;

	if (nullptr != fOut)
		*fOut = fFinalPosDiff;

	if (nullptr != pNaviCom &&
		false == m_bIsInAir_Glide)
	{
		_float fCellAngleSpeed = XMVectorGetX(XMVector3Dot(XMVector3Normalize(pNaviCom->Get_CurCellDesc()->Get_PlaneNormal()), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
		//cout << "test : " << fCellAngleSpeed << "Test2 : " << fCellAngleSpeed * fCellAngleSpeed * fCellAngleSpeed << endl;

		if (true == m_bIsInAir)
			fCellAngleSpeed = 1.f;
		vPosition += XMVector3Normalize(vFront) * fFinalPosDiff * fCellAngleSpeed;

		Set_State(CTransform::STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPosition));
	}
	else
	{
		vPosition += XMVector3Normalize(vFront) * fFinalPosDiff;
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}

	return S_OK;
}

HRESULT CTransform::Go_AccFront_Stop(_float fTimeDelta, _float fFriction, _float fMoveSpeed, CNavigation* pNaviCom, _float* fOut)
{
	_vector		vFront = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	vFront = XMVector3Cross(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f));

	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	//fForce * m_fTimeAcc - 0.5f * fGravity * m_fTimeAcc * m_fTimeAcc;

	_float fPrePos = fMoveSpeed * m_fStopAccTime - 0.5f * fFriction * m_fStopAccTime * m_fStopAccTime;
	m_fStopAccTime += fTimeDelta;
	_float fCurPos = fMoveSpeed * m_fStopAccTime - 0.5f * fFriction * m_fStopAccTime * m_fStopAccTime;

	_float fFinalPosDiff = fCurPos - fPrePos;

	if (0.f > fFinalPosDiff)
		fFinalPosDiff = 0.f;

	if (nullptr != fOut)
		*fOut = fFinalPosDiff;

	if (nullptr != pNaviCom)
	{
		_float fCellAngleSpeed = XMVectorGetX(XMVector3Dot(XMVector3Normalize(pNaviCom->Get_CurCellDesc()->Get_PlaneNormal()), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
		//cout << "test : " << fCellAngleSpeed << "Test2 : " << fCellAngleSpeed * fCellAngleSpeed * fCellAngleSpeed << endl;
		vPosition += XMVector3Normalize(vFront) * fFinalPosDiff * fCellAngleSpeed;
		Set_State(CTransform::STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPosition));
	}
	else
	{
		vPosition += XMVector3Normalize(vFront) * fFinalPosDiff;
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}

	return S_OK;
}

HRESULT CTransform::Go_AccStraight(_float fTimeDelta, _float fMoveSpeed, CNavigation* pNaviCom)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	_float fPrePos = 0.5f * fMoveSpeed * m_fStraightAccTime * m_fStraightAccTime;
	m_fStraightAccTime += fTimeDelta;
	_float fCurPos = 0.5f * fMoveSpeed * m_fStraightAccTime * m_fStraightAccTime;

	_float fFinalPosDiff = fCurPos - fPrePos;

	if (nullptr != pNaviCom &&
		false == m_bIsInAir_Glide)
	{
		_float fCellAngleSpeed = XMVectorGetX(XMVector3Dot(XMVector3Normalize(pNaviCom->Get_CurCellDesc()->Get_PlaneNormal()), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
		//cout << "test : " << fCellAngleSpeed << "Test2 : " << fCellAngleSpeed * fCellAngleSpeed * fCellAngleSpeed << endl;

		if (true == m_bIsInAir)
			fCellAngleSpeed = 1.f;

		vPosition += XMVector3Normalize(vLook) * fFinalPosDiff * fCellAngleSpeed;
		Set_State(CTransform::STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPosition));
	}
	else
	{
		vPosition += XMVector3Normalize(vLook) * fFinalPosDiff;
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}

	return S_OK;
}
HRESULT CTransform::Go_AccStraight_Limit(_float fTimeDelta, _float fMoveSpeed, _float fLimitSpeed, CNavigation* pNaviCom, _float* fOut)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	_float fPrePos = 0.5f * fMoveSpeed * m_fStraightAccTime * m_fStraightAccTime;
	m_fStraightAccTime += fTimeDelta;
	_float fCurPos = 0.5f * fMoveSpeed * m_fStraightAccTime * m_fStraightAccTime;

	_float fFinalPosDiff = fCurPos - fPrePos;

	if (fLimitSpeed * fTimeDelta < fFinalPosDiff)
		fFinalPosDiff = fLimitSpeed * fTimeDelta;

	if (nullptr != fOut)
		*fOut = fFinalPosDiff;

	if (nullptr != pNaviCom)
	{
		_float fCellAngleSpeed = XMVectorGetX(XMVector3Dot(XMVector3Normalize(pNaviCom->Get_CurCellDesc()->Get_PlaneNormal()), XMVectorSet(0.f, 1.f, 0.f, 0.f)));

		if (true == m_bIsInAir)
			fCellAngleSpeed = 1.f;

		//cout << "test : " << fCellAngleSpeed << "Test2 : " << fCellAngleSpeed * fCellAngleSpeed * fCellAngleSpeed << endl;
		vPosition += XMVector3Normalize(vLook) * fFinalPosDiff * fCellAngleSpeed;
		Set_State(CTransform::STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPosition));
	}
	else
	{
		vPosition += XMVector3Normalize(vLook) * fFinalPosDiff;
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}

	return S_OK;
}
HRESULT CTransform::Go_AccStraight_Stop(_float fTimeDelta, _float fFriction, _float fMoveSpeed, CNavigation* pNaviCom, _float* fOut)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	_float fPrePos = fMoveSpeed * m_fStopAccTime - 0.5f * fFriction * m_fStopAccTime * m_fStopAccTime;
	m_fStopAccTime += fTimeDelta;
	_float fCurPos = fMoveSpeed * m_fStopAccTime - 0.5f * fFriction * m_fStopAccTime * m_fStopAccTime;

	_float fFinalPosDiff = fCurPos - fPrePos;

	if (0.f > fFinalPosDiff)
		fFinalPosDiff = 0.f;

	if (nullptr != fOut)
		*fOut = fFinalPosDiff;

	if (nullptr != pNaviCom &&
		false == m_bIsInAir_Glide)
	{
		_float fCellAngleSpeed = XMVectorGetX(XMVector3Dot(XMVector3Normalize(pNaviCom->Get_CurCellDesc()->Get_PlaneNormal()), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
		//cout << "test : " << fCellAngleSpeed << "Test2 : " << fCellAngleSpeed * fCellAngleSpeed * fCellAngleSpeed << endl;

		vPosition += XMVector3Normalize(vLook) * fFinalPosDiff * fCellAngleSpeed;
		Set_State(CTransform::STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPosition));
	}
	else
	{
		vPosition += XMVector3Normalize(vLook) * fFinalPosDiff;
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}

	return S_OK;
}
HRESULT CTransform::Go_Hight(_float fTimeDelta, _float fMoveSpeed, _float _fHight, CNavigation* pNaviCom)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);
	_vector		vUp = Get_State(CTransform::STATE_UP);

	vPosition += XMVector3Normalize(vLook) * fTimeDelta * fMoveSpeed;

	vPosition += XMVector3Normalize(vUp) * fTimeDelta * _fHight;

	if (nullptr != pNaviCom)
		Set_State(CTransform::STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPosition));
	else
		Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Drop(_float fTimeDelta, _float fMoveSpeed, _float _fHight, _float _fEndHight, CNavigation* pNaviCom)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);
	_vector		vUp = Get_State(CTransform::STATE_UP);


	vPosition += XMVector3Normalize(vLook) * fTimeDelta * fMoveSpeed;

	vPosition -= XMVector3Normalize(vUp) * fTimeDelta * _fHight;

	_float		fGetY = XMVectorGetY(vPosition);

	if (fGetY < _fEndHight)
		vPosition = XMVectorSetY(vPosition, _fEndHight);

	if (nullptr != pNaviCom)
		if (nullptr != pNaviCom)
			Set_State(CTransform::STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPosition));
		else
			Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return S_OK;
}

// 반드시 Transform tick() 을 호출해야한다.
void CTransform::Jump(_float fPower, CNavigation* pNaviCom, _float fGravity)
{
	// Air 가 처음으로 true가 된 것은 점프의 시작을 의미한다.
	m_fPower = fPower;
	m_pNavigation = pNaviCom;
	m_fGravity = fGravity;


	m_bIsInAir_Glide = false;
	m_bIsInAir = true;
	m_fTimeAcc = 0.f;
}

void CTransform::Glide(_float fPower, CNavigation* pNaviCom, _float fGravity)
{
	m_fPower = fPower;
	m_pNavigation = pNaviCom;
	m_fGravity = fGravity;


	m_bIsInAir_Glide = true;
	m_bIsInAir = true;
	m_fTimeAcc = 0.f;
}

void CTransform::Swim(_float fSwimY, CNavigation* pNaviCom)
{
	m_fPower = fSwimY;
	m_pNavigation = pNaviCom;
	m_fGravity = 0.f;

	m_bIsInAir = false;
	m_bIsInWater = true;
	m_fTimeAcc = 0.f;
}

void CTransform::Skate(_float fMinY, CNavigation* pNaviCom)
{
	m_fPower = fMinY;
	m_pNavigation = pNaviCom;
	m_fGravity = 0.f;

	m_bIsInAir = false;
	m_bIsInAir_Glide = false;
	m_bIsInWater = true;
	m_bIsInWater_Skate = true;
	m_fTimeAcc = 0.f;
}

HRESULT CTransform::Go_Around(_float4 vSpot)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);

	_vector		vRight = XMVector3Normalize(vPosition - XMLoadFloat4(&vSpot));
	_vector		vLook = XMVector3Normalize(XMVector3Cross(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_float		fCurDistance = XMVectorGetX(XMVector3Length(vPosition - XMLoadFloat4(&vSpot)));

	_float3		vScale = Get_Scaled();

	Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
	Set_State(CTransform::STATE_UP, vUp * vScale.y);
	Set_State(CTransform::STATE_LOOK, vLook * vScale.z);

	return S_OK;
}

HRESULT CTransform::OrbitY(_float4 StartPos, _float4 ParentsPos, _float fSpeed)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_float3		vScale = Get_Scaled();

	_matrix		OrbitMatrix, OrbitYMatrix;
	OrbitMatrix = XMMatrixRotationY(fSpeed);
	OrbitYMatrix = XMMatrixTranslation(ParentsPos.x, ParentsPos.y, ParentsPos.z);

	OrbitYMatrix = OrbitMatrix * OrbitYMatrix;

	vPosition = XMVector3TransformCoord(XMLoadFloat4(&StartPos), OrbitYMatrix);

	Set_Scaled(vScale);
	Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return S_OK;
}

HRESULT CTransform::Chase_Around(_float fTimeDelta, _float fMoveSpeed, _uint iTime, _float4 vSpot)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);

	_vector		vRight = XMVector3Normalize(vPosition - XMLoadFloat4(&vSpot));
	_vector		vLook = XMVector3Normalize(XMVector3Cross(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	_vector		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));

	_float3		vScale = Get_Scaled();

	for (_uint i = 0; i < iTime; ++i)
	{
		vPosition += XMVector3Normalize(vLook) * fMoveSpeed * fTimeDelta;

		vRight = XMVector3Normalize(vPosition - XMLoadFloat4(&vSpot));
		vLook = XMVector3Normalize(XMVector3Cross(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f)));
		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));
	}

	vPosition = Get_State(CTransform::STATE_TRANSLATION);
	vPosition += XMVector3Normalize(vLook) * fMoveSpeed * fTimeDelta;

	Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
	Set_State(CTransform::STATE_UP, vUp * vScale.y);
	Set_State(CTransform::STATE_LOOK, vLook * vScale.z);
	Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return S_OK;
}

void CTransform::Easing_LookAt(_fvector vTargetPos, _float fTimeDelta, _float fMoveSpeed, _float fEndTime)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);

	_vector		vPreLook, vPreRight, vPreUp;

	vPreLook = Get_State(CTransform::STATE_RIGHT);
	vPreRight = Get_State(CTransform::STATE_UP);
	vPreUp = Get_State(CTransform::STATE_LOOK);

	vPreLook = XMVector3Normalize(vPreLook);
	vPreRight = XMVector3Normalize(vPreRight);
	vPreUp = XMVector3Normalize(vPreUp);

	_float	fRatio = fTimeDelta / fEndTime;

	_float3		vScale = Get_Scaled();

	_vector		vLook = vTargetPos - vPosition;
	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	vRight = XMVector3Normalize(vRight);
	vUp = XMVector3Normalize(vUp);
	vLook = XMVector3Normalize(vLook);

	vRight = XMVectorLerp(vPreRight, vRight, fRatio);
	vUp = XMVectorLerp(vPreUp, vUp, fRatio);
	vLook = XMVectorLerp(vPreLook, vLook, fRatio);

	Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CTransform::Turn(_fvector vAxis, _float fTimeDelta, _float fMoveSpeed)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fTimeDelta * fMoveSpeed);

	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Turn(_fvector vAxis, _float fRadian)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Turn(_matrix RotationMatrix)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

// Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.0f));

void CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * Get_Scaled().x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * Get_Scaled().y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * Get_Scaled().z;

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

}

void CTransform::TurnAngles(_fvector vAngles)
{
	_vector	vQuaternion;
	vQuaternion = XMQuaternionRotationRollPitchYawFromVector(vAngles);
	_matrix RotationMatrix = XMMatrixRotationQuaternion(vQuaternion);

	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::RotationAngles(_fvector vAngles)
{
	_vector	vQuaternion;
	vQuaternion = XMQuaternionRotationRollPitchYawFromVector(vAngles);
	_matrix RotationMatrix = XMMatrixRotationQuaternion(vQuaternion);

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * Get_Scaled().x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * Get_Scaled().y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * Get_Scaled().z;

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::MatrixTurnQuaternion(_fvector vQuaternion)
{
	XMMATRIX	QuaternionMatrix;
	QuaternionMatrix = XMMatrixRotationQuaternion(vQuaternion);
	XMMATRIX	WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
	WorldMatrix *= QuaternionMatrix;
	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
}

void CTransform::MatrixRotationQuaternion(_fvector vQuaternion)
{
	XMMATRIX	QuaternionMatrix;
	QuaternionMatrix = XMMatrixRotationQuaternion(vQuaternion);

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * Get_Scaled().x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * Get_Scaled().y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * Get_Scaled().z;

	vRight = XMVector3TransformNormal(vRight, QuaternionMatrix);
	vUp = XMVector3TransformNormal(vUp, QuaternionMatrix);
	vLook = XMVector3TransformNormal(vLook, QuaternionMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::LookAt(_fvector vTargetPos)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);

	_vector		vLook = XMVectorSetW(vTargetPos, 1.f) - vPosition;

	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);

	_vector		vUp = XMVector3Cross(vLook, vRight);

	_float3		vScale = Get_Scaled();

	Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CTransform::LookTo(_fvector vTargetLook)
{
	_vector		vLook = vTargetLook;
	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vTargetLook);
	_vector		vUp = XMVector3Cross(vTargetLook, vRight);

	_float3		vScale = Get_Scaled();

	Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CTransform::YFixedLookAt(_fvector vTargetPos)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);

	_vector		vLook = vTargetPos - vPosition;

	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector		vRight = XMVector3Cross(vUp, vLook);
	vLook = XMVector3Cross(vRight, vUp);

	_float3		vScale = Get_Scaled();

	Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CTransform::YFixedLook(_fvector vTargetLook)
{
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector		vRight = XMVector3Cross(vUp, vTargetLook);
	_vector		vLook = XMVector3Cross(vRight, vUp);
	_float3		vScale = Get_Scaled();

	Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CTransform::UpNaviLook(_fvector vTargetLook, class CNavigation* pNaviCom)
{
	_vector		vNaviUp = pNaviCom->Get_CurCellDesc()->Get_PlaneNormal();
	_vector		vCurUp = Get_State(CTransform::STATE_UP);
	_vector		vUp = (XMVector3Normalize(vNaviUp) + 30.f * XMVector3Normalize(vCurUp));

	_vector		vRight = XMVector3Cross(vUp, vTargetLook);
	_vector		vLook = XMVector3Cross(vRight, vUp);
	vUp = XMVector3Cross(vLook, vRight);
	_float3		vScale = Get_Scaled();

	Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CTransform::UpTo(_fvector vTargetUp)
{
	_vector	vLook = XMVector3Normalize(Get_State(CTransform::STATE_LOOK));
	_vector vUp = XMVector3Normalize(vTargetUp);
	_vector vRight = XMVector3Normalize(XMVector3Cross(vUp, vLook));
	vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp));
	
	_float3 vScale = Get_Scaled();

	Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CTransform::Chase(_fvector vTargetPos, _float fTimeDelta, _float fMoveSpeed, _float fLimit, CNavigation* pNaviCom)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);

	_vector		vMoveDir = vTargetPos - vPosition;

	_vector		vDistance = XMVector3Length(vMoveDir);

	if (XMVectorGetX(vDistance) >= fLimit)
		vPosition += XMVector3Normalize(vMoveDir) * fMoveSpeed * fTimeDelta;

	//vPosition += XMVector3Normalize(vMoveDir) * fMoveSpeed * fTimeDelta;

	if (nullptr != pNaviCom)
	{
		Set_State(CTransform::STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPosition));
	}
	else
	{
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}
}

void CTransform::MoveTo(_vector vPos, CNavigation* pNaviCom)
{
	Set_State(STATE_TRANSLATION, pNaviCom->Get_LineSlidePos(Get_State(CTransform::STATE_TRANSLATION), vPos));
}

void CTransform::Update_CombinedTransformationMatrix()
{
	if (nullptr == m_pParentTransform)
		m_CombinedTransformationMatrix = m_WorldMatrix;

	else
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_WorldMatrix) * XMLoadFloat4x4(&m_pParentTransform->m_CombinedTransformationMatrix));
}


void CTransform::Move_JumpPos(_float fTimeDelta, _float fPower, CNavigation* pNaviCom, _float fGravity)
{
	if (true == m_bIsInAir && true != m_bIsInAir_Glide)
	{
		_float fForce;
		if (false == m_bYFixed)
		{
			if (m_bResetForce)
				fForce = 0;
			else
				fForce = fPower;

			_vector vPos = Get_State(CTransform::STATE_TRANSLATION);
			_float fPrePosY = fForce * m_fTimeAcc - 0.5f * fGravity * m_fTimeAcc * m_fTimeAcc;
			//s = v0 * t + 0.5 * a * t^2

			m_fTimeAcc += fTimeDelta;
			_float fCurPosY = fForce * m_fTimeAcc - 0.5f * fGravity * m_fTimeAcc * m_fTimeAcc;

			_float fFinalPosYDiff = fCurPosY - fPrePosY;
			m_fJumpYDiff = fFinalPosYDiff;

			vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + fFinalPosYDiff);
			Set_State(STATE_TRANSLATION, vPos);
		}
		else
		{
			m_bResetForce = true;
			m_fTimeAcc = 0.f;
			m_fJumpYDiff = 0.f;
		}

		// 점프가 끝났다.
		if (false == pNaviCom->isUpperThanPlane(this))
		{
			m_bIsInAir = false;
			m_bResetForce = false;
			m_fTimeAcc = 0.f;
			m_fPower = 0.f;
			m_pNavigation = nullptr;
			m_fGravity = 0.f;
			m_fJumpYDiff = 0.f;

			pNaviCom->SetUp_OnTerrain(this);
		}
	}
	else if (true == m_bIsInAir && true == m_bIsInAir_Glide)
	{
		_float fForce;
		if (false == m_bYFixed)
		{
			_vector vPos = Get_State(CTransform::STATE_TRANSLATION);
			//_float fCurPosY = - 0.08f * fGravity * fTimeDelta;
			_float fCurPosY = - 0.15f * fGravity * fTimeDelta;


			vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + fCurPosY);
			Set_State(STATE_TRANSLATION, vPos);
		}
		else
		{
			m_bResetForce = true;
			m_bIsInAir = true;
			m_bIsInAir_Glide = false;
			m_fTimeAcc = 0.f;
			m_fJumpYDiff = 0.f;
		}

		// 점프가 끝났다.
		if (false == pNaviCom->isUpperThanPlane(this))
		{
			m_bIsInAir = false;
			m_bIsInAir_Glide = false;
			m_bResetForce = false;
			m_fTimeAcc = 0.f;
			m_fPower = 0.f;
			m_pNavigation = nullptr;
			m_fGravity = 0.f;
			m_fJumpYDiff = 0.f;

			pNaviCom->SetUp_OnTerrain(this);
		}
	}
	else if (true == m_bIsInWater && !m_bIsInWater_Skate )
	{
		_vector vPos = Get_State(CTransform::STATE_TRANSLATION);
		_float fSetPosY = fPower;

		m_fJumpYDiff = fSetPosY;

		vPos = XMVectorSetY(vPos, m_fJumpYDiff);
		Set_State(STATE_TRANSLATION, vPos);

		// 수영이 끝났다.
		if (false == pNaviCom->isUpperThanPlane(this))
		{
			m_bIsInWater = false;

			m_fTimeAcc = 0.f;
			m_fPower = 0.f;
			m_pNavigation = nullptr;
			m_fGravity = 0.f;
			m_fJumpYDiff = 0.f;

			pNaviCom->SetUp_OnTerrain(this);
		}

	}
	else if (true == m_bIsInWater_Skate)
	{
		_vector vPos = Get_State(CTransform::STATE_TRANSLATION);
		_float fSetPosY = fPower;

		if (fSetPosY > XMVectorGetY(vPos))
		{
			m_fJumpYDiff = fPower;

			vPos = XMVectorSetY(vPos, m_fJumpYDiff);
			Set_State(STATE_TRANSLATION, vPos);
		}

		_float fHeightY;
		if (false == pNaviCom->isUpperThanPlane(this, &fHeightY))
		{
			pNaviCom->SetUp_OnTerrain(this);
		}
	}
}

CTransform* CTransform::Create()
{
	CTransform* pInstance = new CTransform();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
