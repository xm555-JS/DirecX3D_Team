#include "stdafx.h"
#include "Effect_Mesh.h"
#include "GameInstance.h"
#include "Easing_Utillity.h"

#include "Model.h"
#include "HIerarchyNode.h"

CEffect_Mesh::CEffect_Mesh()
	: CEffect_Base()
{
}

CEffect_Mesh::CEffect_Mesh(const CEffect_Mesh& rhs)
	: CEffect_Base(rhs)
{
}

HRESULT CEffect_Mesh::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CEffect_Mesh::Initialize(void* pArg)
{

	NULLCHECK(pArg);
	memcpy(&m_MeshEffectDesc, pArg, sizeof(MESHEFFECT_DESC));

	FAILEDCHECK(__super::Initialize());
	FAILEDCHECK(SetUp_Components());
	GAMEINSTANCE->Add_TickGroup(TG_EFFECT, this);
	GAMEINSTANCE->Add_LateTickGroup(LTG_EFFECT, this);

	XMStoreFloat4x4(&m_OffSetMatrix, XMMatrixIdentity());
	SetUp_MeshDesc(m_MeshEffectDesc);


	return S_OK;
}

HRESULT CEffect_Mesh::Initialize_Pooling(void* pArg)
{
	USEGAMEINSTANCE;
	m_bIsEffectOff = false;

	m_iNumMeshContainers = 0;
	m_fTimeSpeed = 1.f;
	m_pOwner = nullptr;
	m_bIsHaveOwner = false;
	m_pOwnerModelCom = nullptr;
	m_strOwnerBoneName = "";
	m_eFollowMode = CEffect_Manager::FOLLOW_MODE_END;

	Remove_Component(ID_UINT(MODEL_STATIC, MODEL_EFFECT));

	m_fTimeAcc = 0.f;
	m_fTimePointOfDuration = 0.f; // For. Middle Easing

	m_vOffsetScale = { 1.f, 1.f, 1.f, 0.f };

	m_vUVSpeedPos_Mask = { 0.f, 0.f }; //유브이 위치(더해지는 양)
	m_vUVSpeedPos_Noise = { 0.f, 0.f }; //유브이 위치(더해지는 양)

	m_fSpriteTimeAcc = 0;
	m_fUVScaleXCur_Mask = 0;
	m_fUVScaleYCur_Mask = 0;

	m_vStartDiffuseColorRGBA = { 1.f, 1.f, 1.f, 1.f };
	m_vCurrentDiffuseColorRGBA = { 1.f, 1.f, 1.f, 1.f };

	//m_vStartGlowColorRGBA = { 1.f, 1.f, 1.f, 1.f };
	m_vCurrentGlowColorRGBA = { 1.f, 1.f, 1.f, 1.f };

	m_vStartSpeedXYZ = { 1.f, 1.f, 1.f };
	m_vCurrentSpeedXYZ = { 1.f, 1.f, 1.f };

	m_vStartScaleRUL = { 1.f, 1.f, 1.f };
	m_vCurrentScaleRUL = { 1.f, 1.f, 1.f };

	m_vStartRotateRUL = { 1.f, 1.f, 1.f };
	m_vCurrentRotateSpeedRUL = { 1.f, 1.f, 1.f };
	m_fRotateAccelation = { 1.f, 1.f, 1.f };

	m_fRotationAcc = 0.f;

	NULLCHECK(pArg);
	memcpy(&m_MeshEffectDesc, pArg, sizeof(MESHEFFECT_DESC));

	ID_UINT iID;
	if (m_MeshEffectDesc.iMeshIndex > 255)
	{
		iID.tNums.Num2 = m_MeshEffectDesc.iMeshIndex % 255;
		iID.tNums.Num3 = m_MeshEffectDesc.iMeshIndex / 255;
	}
	else
	{
		iID.tNums.Num2 = m_MeshEffectDesc.iMeshIndex;
		iID.tNums.Num3 = NULL;
	}

	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(MODEL_STATIC, MODEL_EFFECT, iID.tNums.Num2, iID.tNums.Num3), ID_UINT(MODEL_STATIC, MODEL_EFFECT), (CComponent**)&m_pModelCom));
	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();



	XMStoreFloat4x4(&m_OffSetMatrix, XMMatrixIdentity());
	SetUp_MeshDesc(m_MeshEffectDesc);

	pGameInstance->Add_TickGroup(TG_EFFECT, this);
	pGameInstance->Add_LateTickGroup(LTG_EFFECT, this);

	return S_OK;
}

void CEffect_Mesh::Initialize_Loop(void)
{
	m_fTimeAcc = 0.f;
	m_fTimePointOfDuration = 0.f; // For. Middle Easing

	//m_vOffsetScale = { 1.f, 1.f, 1.f, 0.f }; // loop면 계속 가지고 있어야 하는 값.

	m_vUVSpeedPos_Mask = { 0.f, 0.f };
	m_vUVSpeedPos_Noise = { 0.f, 0.f };

	m_fSpriteTimeAcc = 0;
	m_fUVScaleXCur_Mask = 0;
	m_fUVScaleYCur_Mask = 0;

	m_vStartDiffuseColorRGBA = { 1.f, 1.f, 1.f, 1.f };
	m_vCurrentDiffuseColorRGBA = { 1.f, 1.f, 1.f, 1.f };

	//m_vStartGlowColorRGBA = { 1.f, 1.f, 1.f, 1.f };
	m_vCurrentGlowColorRGBA = { 1.f, 1.f, 1.f, 1.f };

	m_vStartSpeedXYZ = { 1.f, 1.f, 1.f };
	m_vCurrentSpeedXYZ = { 1.f, 1.f, 1.f };

	m_vStartScaleRUL = { 1.f, 1.f, 1.f };
	m_vCurrentScaleRUL = { 1.f, 1.f, 1.f };

	m_vStartRotateRUL = { 1.f, 1.f, 1.f };
	m_vCurrentRotateSpeedRUL = { 1.f, 1.f, 1.f };
	m_fRotateAccelation = { 1.f, 1.f, 1.f };

	m_fRotationAcc = 0.f;

	SetUp_MeshDesc(m_MeshEffectDesc);
	m_pTransformCom->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(&m_OffSetMatrix));
}

void CEffect_Mesh::Tick(_float _fTimeDelta)
{
	USEGAMEINSTANCE;
	_float fTimeDelta = _fTimeDelta * m_fTimeSpeed;
	m_fTimeAcc += fTimeDelta;

	// 객체 죽을 때 바로 사라지도록
	if ((m_bIsHaveOwner && m_pOwner->Is_Dead()) || m_bIsEffectOff)
	{
		//#ifdef _EFFECTTOOL
				//pGameInstance->Add_DeadObject(this);
		//#else
		pGameInstance->Add_PoolingObject(this);
		//#endif // _EFFECTTOOL
		return;
	}

	// m_TimeAcc이 초기 StartTime Offset 보다 클 때 Start.
	if (m_fTimeAcc > m_MeshEffectDesc.fLifeTime.x)
	{
		// 종료 시점.
		if (m_fTimeAcc > m_MeshEffectDesc.fLifeTime.y + m_MeshEffectDesc.fLifeTime.x) // Duration + Offset
		{
			// Loop 일 때.
			if (m_MeshEffectDesc.bIsLoop)
			{
				m_fTimeAcc = 0.f;
				Initialize_Loop();
				m_fTimePointOfDuration = m_fTimeAcc - m_MeshEffectDesc.fLifeTime.x;

				UVSpeed_Sprite(fTimeDelta);
				Easing_DiffuseColor();
				if (m_MeshEffectDesc.bIsGlow)
					Easing_GlowColor();
				Moving(fTimeDelta);
				Scaling_Rotating(fTimeDelta);
				Easing_Shader();
			}
			else
			{
				//#ifdef _EFFECTTOOL
								//pGameInstance->Add_DeadObject(this);
				//#else
				pGameInstance->Add_PoolingObject(this);
				//#endif // _EFFECTTOOL
			}
		}
		else // 실행
		{
			m_fTimePointOfDuration = m_fTimeAcc - m_MeshEffectDesc.fLifeTime.x;

			UVSpeed_Sprite(fTimeDelta);
			Easing_DiffuseColor();
			if (m_MeshEffectDesc.bIsGlow)
				Easing_GlowColor();
			Moving(fTimeDelta);
			Scaling_Rotating(fTimeDelta);
			Easing_Shader();
		}
	}

}

void CEffect_Mesh::LateTick(_float fTimeDelta)
{
	if (m_bIsHaveOwner && CEffect_Manager::FOLLOW_MODE_END != m_eFollowMode && !m_bIsEffectOff)
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
				matFollow = m_pOwnerModelCom->Find_HierarcyNode(m_strOwnerBoneName.c_str())->Get_CombinedMatrix() * m_pOwnerModelCom->Find_HierarcyNode(m_strOwnerBoneName.c_str())->Get_TransformMatrix() * XMLoadFloat4x4(m_pOwner->Get_WorldMatrix());
			}

			_matrix matWorld = XMMatrixIdentity();
			if (CEffect_Manager::FOLLOW_POS_RUL == m_eFollowMode)
			{
				// 나는 로컬 기준
				_vector vRadianAngles = { XMConvertToRadians(m_MeshEffectDesc.vRotationRUL.x), XMConvertToRadians(m_MeshEffectDesc.vRotationRUL.y), XMConvertToRadians(m_MeshEffectDesc.vRotationRUL.z) };
				_vector	vQuaternion;
				vQuaternion = XMQuaternionRotationRollPitchYawFromVector(vRadianAngles);
				_matrix RotationMatrix = XMMatrixRotationQuaternion(vQuaternion);
				_vector vRight = matWorld.r[0] * m_vCurrentScaleRUL.x * m_vOffsetScale.x;
				_vector vUp = matWorld.r[1] * m_vCurrentScaleRUL.y * m_vOffsetScale.y;
				_vector vLook = matWorld.r[2] * m_vCurrentScaleRUL.z * m_vOffsetScale.z;
				matWorld.r[0] = XMVector3TransformNormal(vRight, RotationMatrix);
				matWorld.r[1] = XMVector3TransformNormal(vUp, RotationMatrix);
				matWorld.r[2] = XMVector3TransformNormal(vLook, RotationMatrix);
				// 이 방향을 라업룩 방향으로 바꿔주기.
				matWorld.r[3] = XMVectorSet(m_MeshEffectDesc.vPositionOffset.x, m_MeshEffectDesc.vPositionOffset.y, m_MeshEffectDesc.vPositionOffset.z, 1.f);

				matFollow.r[0] = XMVector3Normalize(matFollow.r[0]);
				matFollow.r[1] = XMVector3Normalize(matFollow.r[1]);
				matFollow.r[2] = XMVector3Normalize(matFollow.r[2]);
				m_pTransformCom->Set_WorldMatrix(matWorld * XMLoadFloat4x4(&m_OffSetMatrix) * matFollow);

				if (m_MeshEffectDesc.bIsBillBoard)
				{
					m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[0]) * m_vCurrentScaleRUL.x * m_vOffsetScale.x);
					m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[1]) * m_vCurrentScaleRUL.y * m_vOffsetScale.y);
					m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[2]) * m_vCurrentScaleRUL.z * m_vOffsetScale.z);
					m_pTransformCom->SetBilBoard();
#if 1
					m_fRotationAcc += m_vCurrentRotateSpeedRUL.z;
					_vector vQuaternionL = XMQuaternionRotationAxis(m_pTransformCom->Get_WorldMatrix().r[2], m_fRotationAcc);
					_matrix RotationMatrix = XMMatrixRotationQuaternion(/*vQuaternionR + vQuaternionU + */vQuaternionL);
					m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), RotationMatrix));
					m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3TransformNormal(m_pTransformCom->Get_State(CTransform::STATE_UP), RotationMatrix));
					m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(m_pTransformCom->Get_State(CTransform::STATE_LOOK), RotationMatrix));
#endif // 0
				}
#if 0
				_vector vLastPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
				vLastPos += XMVector3Normalize((m_pTransformCom->Get_State(CTransform::STATE_RIGHT))) * m_vCurrentSpeedXYZ.x * fTimeDelta;
				vLastPos += XMVector3Normalize((m_pTransformCom->Get_State(CTransform::STATE_UP))) * m_vCurrentSpeedXYZ.y * fTimeDelta;
				vLastPos += XMVector3Normalize((m_pTransformCom->Get_State(CTransform::STATE_LOOK))) * m_vCurrentSpeedXYZ.z * fTimeDelta;
				m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vLastPos);
#endif // 0


			}
			else // FOLLOW_POS
			{
				matWorld = XMMatrixTranslation(m_MeshEffectDesc.vPositionOffset.x, m_MeshEffectDesc.vPositionOffset.y, m_MeshEffectDesc.vPositionOffset.z);
				matFollow.r[0] = XMVector3Normalize(matFollow.r[0]);
				matFollow.r[1] = XMVector3Normalize(matFollow.r[1]);
				matFollow.r[2] = XMVector3Normalize(matFollow.r[2]);
				_matrix matFinalWorld = matWorld * XMLoadFloat4x4(&m_OffSetMatrix) * matFollow;
				//m_pTransformCom->Set_WorldMatrix( matFinalWorld);
				m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, matFinalWorld.r[3]);



#if 0
				_vector vLastPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
				vLastPos += XMVector3Normalize((m_pTransformCom->Get_State(CTransform::STATE_RIGHT))) * m_vCurrentSpeedXYZ.x * fTimeDelta;
				vLastPos += XMVector3Normalize((m_pTransformCom->Get_State(CTransform::STATE_UP))) * m_vCurrentSpeedXYZ.y * fTimeDelta;
				vLastPos += XMVector3Normalize((m_pTransformCom->Get_State(CTransform::STATE_LOOK))) * m_vCurrentSpeedXYZ.z * fTimeDelta;
				m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vLastPos);
#endif // 0

			}
		}
	}

	//_float fLargestScale max(max(
	//	XMVectorGetX(XMVector3Length(m_pTransformCom->Get_WorldMatrix().r[0])),
	//	XMVectorGetX(XMVector3Length(m_pTransformCom->Get_WorldMatrix().r[1]))),
	//	XMVectorGetX(XMVector3Length(m_pTransformCom->Get_WorldMatrix().r[2])));
	//m_fFrustumRange = fLargestScale * 2.f;
	if (m_MeshEffectDesc.fLifeTime.x <= m_fTimeAcc /*&& IsInFrustum()*/)
	{
		if (m_MeshEffectDesc.bIsDistortion)
		{
			GAMEINSTANCE->Add_RenderGroup(RENDER_EFFECT_DISTORTION, this);
			m_iShaderPass = 1;
		}
		else
		{
			GAMEINSTANCE->Add_RenderGroup(RENDER_EFFECT, this);
			m_iShaderPass = 0;
		}
	}
}

HRESULT CEffect_Mesh::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	FAILEDCHECK(SetUp_ShaderResource());

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		//FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NoiseTexture", i, aiTextureType_DIFFUSE));
		//FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_MaskTexture", i, aiTextureType_NORMALS));

		m_pModelCom->Render(i, m_pShaderCom, m_iShaderPass);
	}

	return S_OK;
}

void CEffect_Mesh::UVSpeed_Sprite(_float fTimeDelta)
{
	// Noise
	m_vUVSpeedPos_Noise.x += m_MeshEffectDesc.fUVSpeedXY_Noise.x * fTimeDelta;
	m_vUVSpeedPos_Noise.y += m_MeshEffectDesc.fUVSpeedXY_Noise.y * fTimeDelta;
	if (fabs(m_vUVSpeedPos_Noise.x) > fabs(1.f))
	{
		if (0.f < m_MeshEffectDesc.fUVSpeedXY_Noise.x)
			m_vUVSpeedPos_Noise.x = -1.f;
		else if (0.f > m_MeshEffectDesc.fUVSpeedXY_Noise.x)
			m_vUVSpeedPos_Noise.x = 1.f;
	}
	if (fabs(m_vUVSpeedPos_Noise.y) > fabs(1.f))
	{
		if (0.f < m_MeshEffectDesc.fUVSpeedXY_Noise.y)
			m_vUVSpeedPos_Noise.y = -1.f;
		else if (0.f > m_MeshEffectDesc.fUVSpeedXY_Noise.y)
			m_vUVSpeedPos_Noise.y = 1.f;
	}

	// UV Speed
	if (m_MeshEffectDesc.bIsUVSpeedMode)
	{
		m_vUVSpeedPos_Mask.x += m_MeshEffectDesc.fUVSpeedXY_Mask.x * fTimeDelta;
		m_vUVSpeedPos_Mask.y += m_MeshEffectDesc.fUVSpeedXY_Mask.y * fTimeDelta;
		if (fabs(m_vUVSpeedPos_Mask.x) > fabs(1.f))
		{
			if (0.f < m_MeshEffectDesc.fUVSpeedXY_Mask.x)
				m_vUVSpeedPos_Mask.x = -1.f;
			else if (0.f > m_MeshEffectDesc.fUVSpeedXY_Mask.x)
				m_vUVSpeedPos_Mask.x = 1.f;
		}
		if (fabs(m_vUVSpeedPos_Mask.y) > fabs(1.f))
		{
			if (0.f < m_MeshEffectDesc.fUVSpeedXY_Mask.y)
				m_vUVSpeedPos_Mask.y = -1.f;
			else if (0.f > m_MeshEffectDesc.fUVSpeedXY_Mask.y)
				m_vUVSpeedPos_Mask.y = 1.f;
		}
	}
	else // Sprite
	{
		m_fSpriteTimeAcc += fTimeDelta;
		if (m_MeshEffectDesc.fSpriteSpeed < m_fSpriteTimeAcc)
		{
			if (m_MeshEffectDesc.iUVScaleXMax_Mask - 1 > m_fUVScaleXCur_Mask)
				m_fUVScaleXCur_Mask++;
			else
			{
				m_fUVScaleXCur_Mask = 0;
				if (m_MeshEffectDesc.iUVScaleYMax_Mask - 1 > m_fUVScaleYCur_Mask)
					m_fUVScaleYCur_Mask++;
				else
					m_fUVScaleYCur_Mask = 0;
			}
			m_fSpriteTimeAcc = 0.f;
		}
	}
}

void CEffect_Mesh::Moving(_float fTimeDelta)
{
	// Speed
	if (m_MeshEffectDesc.bIsSpeedEasingMode)
	{
		if (CEasing_Utillity::TYPE_End != m_MeshEffectDesc.iSpeedEasing_End) // 끝 보간이 있을 때.
		{
			// 중간 보간이 있을 때.
			if (CEasing_Utillity::TYPE_End != m_MeshEffectDesc.iSpeedEasing_Middle)
			{
				if (m_MeshEffectDesc.fMiddleSpeedTimePoint > m_fTimePointOfDuration)
				{
					XMStoreFloat3(&m_vCurrentSpeedXYZ, CEasing_Utillity::Easing(m_MeshEffectDesc.iSpeedEasing_Middle, XMLoadFloat3(&m_vStartSpeedXYZ), XMLoadFloat3(&m_MeshEffectDesc.vMiddleSpeedXYZ), m_fTimePointOfDuration, m_MeshEffectDesc.fMiddleSpeedTimePoint));

				}
				else
				{
					XMStoreFloat3(&m_vCurrentSpeedXYZ, CEasing_Utillity::Easing(m_MeshEffectDesc.iSpeedEasing_End, XMLoadFloat3(&m_MeshEffectDesc.vMiddleSpeedXYZ), XMLoadFloat3(&m_MeshEffectDesc.vEndSpeedXYZ), m_fTimePointOfDuration - m_MeshEffectDesc.fMiddleSpeedTimePoint, m_MeshEffectDesc.fLifeTime.y - m_MeshEffectDesc.fMiddleSpeedTimePoint));
				}
			}
			else // 끝 보간만 있을 때.
			{
				XMStoreFloat3(&m_vCurrentSpeedXYZ, CEasing_Utillity::Easing(m_MeshEffectDesc.iSpeedEasing_End, XMLoadFloat3(&m_vStartSpeedXYZ), XMLoadFloat3(&m_MeshEffectDesc.vEndSpeedXYZ), m_fTimePointOfDuration, m_MeshEffectDesc.fLifeTime.y));
			}
		}
	}
	else
	{
		// Each Accelation
		//v = vo + at
		_vector vEachAccelationDir = XMVector3Normalize(XMVectorSet(m_vStartSpeedXYZ.x, m_vStartSpeedXYZ.y, m_vStartSpeedXYZ.z, 0.f));
		m_vCurrentSpeedXYZ.x = m_vStartSpeedXYZ.x + XMVectorGetX(vEachAccelationDir) * m_MeshEffectDesc.fSpeedAccelation.x * m_fTimePointOfDuration;
		m_vCurrentSpeedXYZ.y = m_vStartSpeedXYZ.y + XMVectorGetY(vEachAccelationDir) * m_MeshEffectDesc.fSpeedAccelation.y * m_fTimePointOfDuration;
		m_vCurrentSpeedXYZ.z = m_vStartSpeedXYZ.z + XMVectorGetZ(vEachAccelationDir) * m_MeshEffectDesc.fSpeedAccelation.z * m_fTimePointOfDuration;
	}
	// R U L (회전되어 있는 메시일 경우, 회전되기 전의 각도를 기준으로 한 RUL을 기준으로 보면 된다.
	// 이전 이동량이 누적되어있으므로, 순간의 속력만 누적해서 더해준다.
	_vector vLastPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	vLastPos += XMVector3Normalize((m_pTransformCom->Get_State(CTransform::STATE_RIGHT))) * m_vCurrentSpeedXYZ.x * fTimeDelta;
	vLastPos += XMVector3Normalize((m_pTransformCom->Get_State(CTransform::STATE_UP))) * m_vCurrentSpeedXYZ.y * fTimeDelta;
	vLastPos += XMVector3Normalize((m_pTransformCom->Get_State(CTransform::STATE_LOOK))) * m_vCurrentSpeedXYZ.z * fTimeDelta;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vLastPos);

	// World X Y Z
	//_vector vLastPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	//_float fCurX = XMVectorGetX(vLastPos) + m_vCurrentSpeedXYZ.x * fTimeDelta;
	//_float fCurY = XMVectorGetY(vLastPos) + m_vCurrentSpeedXYZ.y * fTimeDelta;
	//_float fCurZ = XMVectorGetZ(vLastPos) + m_vCurrentSpeedXYZ.z * fTimeDelta;
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(fCurX, fCurY, fCurZ, XMVectorGetW(vLastPos)));
}

void CEffect_Mesh::Scaling_Rotating(_float fTimeDelta)
{
	// Scale
	if (CEasing_Utillity::TYPE_End != m_MeshEffectDesc.iScaleEasing_End) // 끝 보간이 있을 때.
	{
		// 중간 보간이 있을 때.
		if (CEasing_Utillity::TYPE_End != m_MeshEffectDesc.iScaleEasing_Middle)
		{
			if (m_MeshEffectDesc.fMiddleScaleTimePoint > m_fTimePointOfDuration)
			{
				XMStoreFloat3(&m_vCurrentScaleRUL, CEasing_Utillity::Easing(m_MeshEffectDesc.iScaleEasing_Middle, XMLoadFloat3(&m_vStartScaleRUL), XMLoadFloat3(&m_MeshEffectDesc.vMiddleScaleXYZ), m_fTimePointOfDuration, m_MeshEffectDesc.fMiddleScaleTimePoint));

			}
			else
			{
				XMStoreFloat3(&m_vCurrentScaleRUL, CEasing_Utillity::Easing(m_MeshEffectDesc.iScaleEasing_End, XMLoadFloat3(&m_MeshEffectDesc.vMiddleScaleXYZ), XMLoadFloat3(&m_MeshEffectDesc.vEndScaleXYZ), m_fTimePointOfDuration - m_MeshEffectDesc.fMiddleScaleTimePoint, m_MeshEffectDesc.fLifeTime.y - m_MeshEffectDesc.fMiddleScaleTimePoint));
			}
		}
		else // 끝 보간만 있을 때.
		{
			XMStoreFloat3(&m_vCurrentScaleRUL, CEasing_Utillity::Easing(m_MeshEffectDesc.iScaleEasing_End, XMLoadFloat3(&m_vStartScaleRUL), XMLoadFloat3(&m_MeshEffectDesc.vEndScaleXYZ), m_fTimePointOfDuration, m_MeshEffectDesc.fLifeTime.y));
		}
	}
	// Rotation 
	// m_vCurrentRotateSpeedRUL : 누적(더할)할 값 (이전 회전이 그대로 남아있으므로, 순간의 회전량만 누적해서 더해준다.)
	m_vCurrentRotateSpeedRUL.x = (XMConvertToRadians(m_MeshEffectDesc.vRotationSpeedRUL.x) + XMConvertToRadians(m_MeshEffectDesc.fRotatationAccelation.x) * m_fTimePointOfDuration) * fTimeDelta;
	m_vCurrentRotateSpeedRUL.y = (XMConvertToRadians(m_MeshEffectDesc.vRotationSpeedRUL.y) + XMConvertToRadians(m_MeshEffectDesc.fRotatationAccelation.y) * m_fTimePointOfDuration) * fTimeDelta;
	m_vCurrentRotateSpeedRUL.z = (XMConvertToRadians(m_MeshEffectDesc.vRotationSpeedRUL.z) + XMConvertToRadians(m_MeshEffectDesc.fRotatationAccelation.z) * m_fTimePointOfDuration) * fTimeDelta;

	if (m_MeshEffectDesc.bIsBillBoard)
	{
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[0]) * m_vCurrentScaleRUL.x * m_vOffsetScale.x);
		m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[1]) * m_vCurrentScaleRUL.y * m_vOffsetScale.y);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[2]) * m_vCurrentScaleRUL.z * m_vOffsetScale.z);
		m_pTransformCom->SetBilBoard();


#if 1
		m_fRotationAcc += m_vCurrentRotateSpeedRUL.z;
		// Rotation
		//_vector vRadianAngles = { m_vCurrentRotateSpeedRUL.x, m_vCurrentRotateSpeedRUL.y, m_vCurrentRotateSpeedRUL.z };
		//_vector	vQuaternion = XMQuaternionRotationRollPitchYawFromVector(vRadianAngles);
		//_matrix RotationMatrix = XMMatrixRotationQuaternion(vQuaternion);
		//_vector vQuaternionR = XMQuaternionRotationAxis(m_pTransformCom->Get_WorldMatrix().r[0], m_vCurrentRotateSpeedRUL.x);
		//_vector vQuaternionU = XMQuaternionRotationAxis(m_pTransformCom->Get_WorldMatrix().r[1], m_vCurrentRotateSpeedRUL.y);
		_vector vQuaternionL = XMQuaternionRotationAxis(m_pTransformCom->Get_WorldMatrix().r[2], m_fRotationAcc);
		_matrix RotationMatrix = XMMatrixRotationQuaternion(/*vQuaternionR + vQuaternionU + */vQuaternionL);
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), RotationMatrix));
		m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3TransformNormal(m_pTransformCom->Get_State(CTransform::STATE_UP), RotationMatrix));
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(m_pTransformCom->Get_State(CTransform::STATE_LOOK), RotationMatrix));
#endif // 0
	}
	else
	{
#if 0
		// World X, Y ,Z 축 회전 
		_vector vRadianAngles = { m_vCurrentRotateSpeedRUL.x, m_vCurrentRotateSpeedRUL.y, m_vCurrentRotateSpeedRUL.z };
		_vector	vQuaternion = XMQuaternionRotationRollPitchYawFromVector(vRadianAngles);
		_matrix RotationMatrix = XMMatrixRotationQuaternion(vQuaternion);

		_vector		vRight, vUp, vLook;
		vRight = XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[0]) * m_vCurrentScaleRUL.x * m_vOffsetScale.x;
		vUp = XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[1]) * m_vCurrentScaleRUL.y * m_vOffsetScale.y;
		vLook = XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[2]) * m_vCurrentScaleRUL.z * m_vOffsetScale.z;
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
		m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
#else
		// RUL 회전
		m_pTransformCom->Set_Scaled({ m_vCurrentScaleRUL.x * m_vOffsetScale.x, m_vCurrentScaleRUL.y * m_vOffsetScale.y, m_vCurrentScaleRUL.z * m_vOffsetScale.z });

		m_pTransformCom->Turn(m_pTransformCom->Get_WorldMatrix().r[0], m_vCurrentRotateSpeedRUL.x);
		m_pTransformCom->Turn(m_pTransformCom->Get_WorldMatrix().r[1], m_vCurrentRotateSpeedRUL.y);
		m_pTransformCom->Turn(m_pTransformCom->Get_WorldMatrix().r[2], m_vCurrentRotateSpeedRUL.z);
#endif // 0
	}
}

void CEffect_Mesh::Easing_DiffuseColor(void)
{
	// Diffuse Color
	if (CEasing_Utillity::TYPE_End != m_MeshEffectDesc.iDiffuseColorEasing_End) // 끝 보간이 있을 때.
	{
		// 중간 보간이 있을 때.
		if (CEasing_Utillity::TYPE_End != m_MeshEffectDesc.iDiffuseColorEasing_Middle)
		{
			if (m_MeshEffectDesc.fMiddleDiffuseColorTimePoint > m_fTimePointOfDuration)
			{
				XMStoreFloat4(&m_vCurrentDiffuseColorRGBA, CEasing_Utillity::Easing(m_MeshEffectDesc.iDiffuseColorEasing_Middle, XMLoadFloat4(&m_vStartDiffuseColorRGBA), XMLoadFloat4(&m_MeshEffectDesc.vMiddleDiffuseColorRGBA), m_fTimePointOfDuration, m_MeshEffectDesc.fMiddleDiffuseColorTimePoint));

			}
			else
			{
				XMStoreFloat4(&m_vCurrentDiffuseColorRGBA, CEasing_Utillity::Easing(m_MeshEffectDesc.iDiffuseColorEasing_End, XMLoadFloat4(&m_MeshEffectDesc.vMiddleDiffuseColorRGBA), XMLoadFloat4(&m_MeshEffectDesc.vEndDiffuseColorRGBA), m_fTimePointOfDuration - m_MeshEffectDesc.fMiddleDiffuseColorTimePoint, m_MeshEffectDesc.fLifeTime.y - m_MeshEffectDesc.fMiddleDiffuseColorTimePoint));
			}
		}
		else // 끝 보간만 있을 때.
		{
			XMStoreFloat4(&m_vCurrentDiffuseColorRGBA, CEasing_Utillity::Easing(m_MeshEffectDesc.iDiffuseColorEasing_End, XMLoadFloat4(&m_vStartDiffuseColorRGBA), XMLoadFloat4(&m_MeshEffectDesc.vEndDiffuseColorRGBA), m_fTimePointOfDuration, m_MeshEffectDesc.fLifeTime.y));
		}
	}
}

void CEffect_Mesh::Easing_GlowColor(void)
{
	if (!m_MeshEffectDesc.bIsGlowFollowDiffuseColor)
	{
		// Glow Color
		if (CEasing_Utillity::TYPE_End != m_MeshEffectDesc.iGlowColorEasing_End) // 끝 보간이 있을 때.
		{
			// 중간 보간이 있을 때.
			if (CEasing_Utillity::TYPE_End != m_MeshEffectDesc.iGlowColorEasing_Middle)
			{
				if (m_MeshEffectDesc.fMiddleGlowColorTimePoint > m_fTimePointOfDuration)
				{
					XMStoreFloat4(&m_vCurrentGlowColorRGBA, CEasing_Utillity::Easing(m_MeshEffectDesc.iGlowColorEasing_Middle, XMLoadFloat4(&m_MeshEffectDesc.vStartGlowColorRGBA), XMLoadFloat4(&m_MeshEffectDesc.vMiddleGlowColorRGBA), m_fTimePointOfDuration, m_MeshEffectDesc.fMiddleGlowColorTimePoint));

				}
				else
				{
					XMStoreFloat4(&m_vCurrentGlowColorRGBA, CEasing_Utillity::Easing(m_MeshEffectDesc.iGlowColorEasing_End, XMLoadFloat4(&m_MeshEffectDesc.vMiddleGlowColorRGBA), XMLoadFloat4(&m_MeshEffectDesc.vEndGlowColorRGBA), m_fTimePointOfDuration - m_MeshEffectDesc.fMiddleGlowColorTimePoint, m_MeshEffectDesc.fLifeTime.y - m_MeshEffectDesc.fMiddleGlowColorTimePoint));
				}
			}
			else // 끝 보간만 있을 때.
			{
				XMStoreFloat4(&m_vCurrentGlowColorRGBA, CEasing_Utillity::Easing(m_MeshEffectDesc.iGlowColorEasing_End, XMLoadFloat4(&m_MeshEffectDesc.vStartGlowColorRGBA), XMLoadFloat4(&m_MeshEffectDesc.vEndGlowColorRGBA), m_fTimePointOfDuration, m_MeshEffectDesc.fLifeTime.y));
			}
		}
	}
}

void CEffect_Mesh::Easing_Shader(void)
{
	if (m_MeshEffectDesc.bIsDistortion)
	{
		if (CEasing_Utillity::TYPE_End != m_MeshEffectDesc.iDistortionStrengthEasing_End) // 끝 보간이 있을 때.
		{
			// 중간 보간이 있을 때.
			if (CEasing_Utillity::TYPE_End != m_MeshEffectDesc.iDistortionStrengthEasing_Middle)
			{
				if (m_MeshEffectDesc.fMiddleDistortionStrengthTimePoint > m_fTimePointOfDuration)
				{
					XMStoreFloat(&m_fCurrentDistortionStrength, CEasing_Utillity::Easing(m_MeshEffectDesc.iDistortionStrengthEasing_Middle, XMLoadFloat(&m_MeshEffectDesc.fStartDistortionStrength), XMLoadFloat(&m_MeshEffectDesc.fMiddleDistortionStrength), m_fTimePointOfDuration, m_MeshEffectDesc.fMiddleDistortionStrengthTimePoint));

				}
				else
				{
					XMStoreFloat(&m_fCurrentDistortionStrength, CEasing_Utillity::Easing(m_MeshEffectDesc.iDistortionStrengthEasing_End, XMLoadFloat(&m_MeshEffectDesc.fMiddleDistortionStrength), XMLoadFloat(&m_MeshEffectDesc.fEndDistortionStrength), m_fTimePointOfDuration - m_MeshEffectDesc.fMiddleDistortionStrengthTimePoint, m_MeshEffectDesc.fLifeTime.y - m_MeshEffectDesc.fMiddleDistortionStrengthTimePoint));
				}
			}
			else // 끝 보간만 있을 때.
			{
				XMStoreFloat(&m_fCurrentDistortionStrength, CEasing_Utillity::Easing(m_MeshEffectDesc.iDistortionStrengthEasing_End, XMLoadFloat(&m_MeshEffectDesc.fStartDistortionStrength), XMLoadFloat(&m_MeshEffectDesc.fEndDistortionStrength), m_fTimePointOfDuration, m_MeshEffectDesc.fLifeTime.y));
			}
		}
	}

	if (m_MeshEffectDesc.bIsRimLight)
	{
		if (CEasing_Utillity::TYPE_End != m_MeshEffectDesc.iRimLightColorEasing_End) // 끝 보간이 있을 때.
		{
			// 중간 보간이 있을 때.
			if (CEasing_Utillity::TYPE_End != m_MeshEffectDesc.iRimLightColorEasing_Middle)
			{
				if (m_MeshEffectDesc.fMiddleRimLightColorTimePoint > m_fTimePointOfDuration)
				{
					XMStoreFloat4(&m_vCurrentRimLightColorRGBA, CEasing_Utillity::Easing(m_MeshEffectDesc.iRimLightColorEasing_Middle, XMLoadFloat4(&m_MeshEffectDesc.vStartRimLightColorRGBA), XMLoadFloat4(&m_MeshEffectDesc.vMiddleRimLightColorRGBA), m_fTimePointOfDuration, m_MeshEffectDesc.fMiddleRimLightColorTimePoint));

				}
				else
				{
					XMStoreFloat4(&m_vCurrentRimLightColorRGBA, CEasing_Utillity::Easing(m_MeshEffectDesc.iRimLightColorEasing_End, XMLoadFloat4(&m_MeshEffectDesc.vMiddleRimLightColorRGBA), XMLoadFloat4(&m_MeshEffectDesc.vEndRimLightColorRGBA), m_fTimePointOfDuration - m_MeshEffectDesc.fMiddleRimLightColorTimePoint, m_MeshEffectDesc.fLifeTime.y - m_MeshEffectDesc.fMiddleRimLightColorTimePoint));
				}
			}
			else // 끝 보간만 있을 때.
			{
				XMStoreFloat4(&m_vCurrentRimLightColorRGBA, CEasing_Utillity::Easing(m_MeshEffectDesc.iRimLightColorEasing_End, XMLoadFloat4(&m_MeshEffectDesc.vStartRimLightColorRGBA), XMLoadFloat4(&m_MeshEffectDesc.vEndRimLightColorRGBA), m_fTimePointOfDuration, m_MeshEffectDesc.fLifeTime.y));
			}
		}
	}

}

HRESULT CEffect_Mesh::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODELEFFECT), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_MASK), ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_MASK), (CComponent**)&m_pTextureCom_Mask));
#if defined(_NOISE_TEX)
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), (CComponent**)&m_pTextureCom_Noise));
#endif

	ID_UINT iID;
	if (m_MeshEffectDesc.iMeshIndex > 255)
	{
		iID.tNums.Num2 = m_MeshEffectDesc.iMeshIndex % 255;
		iID.tNums.Num3 = m_MeshEffectDesc.iMeshIndex / 255;
	}
	else
	{
		iID.tNums.Num2 = m_MeshEffectDesc.iMeshIndex;
		iID.tNums.Num3 = NULL;
	}

	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(MODEL_STATIC, MODEL_EFFECT, iID.tNums.Num2, iID.tNums.Num3), ID_UINT(MODEL_STATIC, MODEL_EFFECT), (CComponent**)&m_pModelCom));
	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	return S_OK;
}


HRESULT CEffect_Mesh::SetUp_ShaderResource()
{
	NULLCHECK(m_pShaderCom);
	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));
	FAILEDCHECK(m_pTextureCom_Mask->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", m_MeshEffectDesc.iMaskIndex));
#if defined(_NOISE_TEX)
	FAILEDCHECK(m_pTextureCom_Noise->Set_ShaderResourceView(m_pShaderCom, "g_NoiseTexture", m_MeshEffectDesc.iNoiseIndex));
#endif
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_DiffuseColor", &m_vCurrentDiffuseColorRGBA, sizeof(_float4)));

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsClampSampling_Noise", &m_MeshEffectDesc.bIsClampSampling_Noise, sizeof(_bool)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vUVSpeedPos_Noise", &m_vUVSpeedPos_Noise, sizeof(_float2)));
	_float fUVScaleXMax_Noise = (_float)m_MeshEffectDesc.iUVScaleXMax_Noise;
	_float fUVScaleYMax_Noise = (_float)m_MeshEffectDesc.iUVScaleYMax_Noise;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fUVScaleXMax_Noise", &fUVScaleXMax_Noise, sizeof(_float)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fUVScaleYMax_Noise", &fUVScaleYMax_Noise, sizeof(_float)));

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsClampSampling_Mask", &m_MeshEffectDesc.bIsClampSampling_Mask, sizeof(_bool)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsReverseXY_Mask", &m_MeshEffectDesc.bIsReverseXY_Mask, sizeof(_bool)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsReverseXY_Noise", &m_MeshEffectDesc.bIsReverseXY_Noise, sizeof(_bool)));

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsUVSpeedMode", &m_MeshEffectDesc.bIsUVSpeedMode, sizeof(_bool)));
	if (m_MeshEffectDesc.bIsUVSpeedMode)
	{
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vUVSpeedPos_Mask", &m_vUVSpeedPos_Mask, sizeof(_float2)));
	}
	else
	{
		_float fUVScaleXCur_Mask = (_float)m_fUVScaleXCur_Mask;
		_float fUVScaleYCur_Mask = (_float)m_fUVScaleYCur_Mask;
		_float fUVScaleXMax_Mask = (_float)m_MeshEffectDesc.iUVScaleXMax_Mask;
		_float fUVScaleYMax_Mask = (_float)m_MeshEffectDesc.iUVScaleYMax_Mask;
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fUVScaleXCur_Mask", &fUVScaleXCur_Mask, sizeof(_float)));
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fUVScaleYCur_Mask", &fUVScaleYCur_Mask, sizeof(_float)));
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fUVScaleXMax_Mask", &fUVScaleXMax_Mask, sizeof(_float)));
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fUVScaleYMax_Mask", &fUVScaleYMax_Mask, sizeof(_float)));
	}

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsGlow", &m_MeshEffectDesc.bIsGlow, sizeof(_bool)));
	if (m_MeshEffectDesc.bIsGlow)
	{
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsGlowFollowDiffuseColor", &m_MeshEffectDesc.bIsGlowFollowDiffuseColor, sizeof(_bool)));
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vGlowColor", &m_vCurrentGlowColorRGBA, sizeof(_float4)));
	}

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsSoftEffect", &m_MeshEffectDesc.bIsSoftEffect, sizeof(_bool)));
	if (m_MeshEffectDesc.bIsSoftEffect)
	{
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fFar", &g_fFar, sizeof(_float)));
		FAILEDCHECK(m_pShaderCom->Set_ShaderResourceView("g_DepthTexture", pGameInstance->Get_CurRenderer()->Get_SRV(TEXT("Target_Depth"))));
	}

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsDistortion", &m_MeshEffectDesc.bIsDistortion, sizeof(_bool)));
	if (m_MeshEffectDesc.bIsDistortion)
	{
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fDistortionStrength", &m_fCurrentDistortionStrength, sizeof(_float)));
	}

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsRimLight", &m_MeshEffectDesc.bIsRimLight, sizeof(_bool)));
	if (m_MeshEffectDesc.bIsRimLight)
	{
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vRimLightColor", &m_vCurrentRimLightColorRGBA, sizeof(_float4)));

		_float4 vLook;
		_matrix ViewMatrixInv = XMMatrixInverse(nullptr, XMLoadFloat4x4(pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW)));
		XMStoreFloat4(&vLook, XMVector3Normalize(ViewMatrixInv.r[2]));
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vLook", &vLook, sizeof(_float4)));

	}


	if (0 < m_fTimePointOfDuration)
	{
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsDissolve", &m_MeshEffectDesc.bIsDissolve, sizeof(_bool)));
		if (m_MeshEffectDesc.bIsDissolve)
		{
#if defined(_NOISE_TEX)
			FAILEDCHECK(m_pTextureCom_Noise->Set_ShaderResourceView(m_pShaderCom, "g_DissolveNoiseTexture", m_MeshEffectDesc.iDissolveNoiseTextureIndex));
			FAILEDCHECK(m_pTextureCom_Noise->Set_ShaderResourceView(m_pShaderCom, "g_DissolveColorTexture", m_MeshEffectDesc.iDissolveColorTextureIndex));
#endif
			FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vDissolveColor", &m_MeshEffectDesc.vDissolveColorRGBA, sizeof(_float4)));
			FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fDissolveStrength", &m_MeshEffectDesc.fDissolveStrength, sizeof(_float)));
			_float fTimeAccDissove = 0.f;
			fTimeAccDissove = m_fTimePointOfDuration / m_MeshEffectDesc.fLifeTime.y; // 디졸드 지속하고 싶은 시간 만큼 나눠줌.
			FAILEDCHECK(m_pShaderCom->Set_RawValue("g_fTimeAccDissolve", &fTimeAccDissove, sizeof(_float)));
		}
	}
	else
	{
		_bool bDisableDissolve = false;
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsDissolve", &bDisableDissolve, sizeof(_bool)));
	}


	// _bool	bIsDistortion;
	// _float	fStartDistortionStrength;
	// _uint	iDistortionStrengthEasing_End;
	// _uint	iDistortionStrengthEasing_Middle;
	// _float	fMiddleDistortionStrengthTimePoint;
	// _float	fMiddleDistortionStrength;
	// _float	fEndDistortionStrength;

	return S_OK;
}

HRESULT CEffect_Mesh::SetUp_MeshDesc(MESHEFFECT_DESC Desc)
{
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(Desc.vPositionOffset.x, Desc.vPositionOffset.y, Desc.vPositionOffset.z, 1.f));
	// Speed
	m_vCurrentSpeedXYZ.x = m_vStartSpeedXYZ.x = Desc.vStartSpeedXYZ.x;
	m_vCurrentSpeedXYZ.y = m_vStartSpeedXYZ.y = Desc.vStartSpeedXYZ.y;
	m_vCurrentSpeedXYZ.z = m_vStartSpeedXYZ.z = Desc.vStartSpeedXYZ.z;
	// Scale
	m_vCurrentScaleRUL.x = m_vStartScaleRUL.x = Desc.vStartScaleXYZ.x;
	m_vCurrentScaleRUL.y = m_vStartScaleRUL.y = Desc.vStartScaleXYZ.y;
	m_vCurrentScaleRUL.z = m_vStartScaleRUL.z = Desc.vStartScaleXYZ.z;
	// Rotation
#if 1 // World RUL
	_vector vRadianAngles = { XMConvertToRadians(Desc.vRotationRUL.x), XMConvertToRadians(Desc.vRotationRUL.y), XMConvertToRadians(Desc.vRotationRUL.z) };
	_vector	vQuaternion;
	vQuaternion = XMQuaternionRotationRollPitchYawFromVector(vRadianAngles);
	_matrix RotationMatrix = XMMatrixRotationQuaternion(vQuaternion);
	_vector		vRight, vUp, vLook;
	vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * m_vStartScaleRUL.x;
	vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * m_vStartScaleRUL.y;
	vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * m_vStartScaleRUL.z;
	//vRight = XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[0]) * m_vStartScaleRUL.x;
	//vUp = XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[1]) * m_vStartScaleRUL.y;
	//vLook = XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[2]) * m_vStartScaleRUL.z;
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
	m_vStartRotateRUL.x = XMVectorGetX(vRadianAngles);
	m_vStartRotateRUL.y = XMVectorGetY(vRadianAngles);
	m_vStartRotateRUL.z = XMVectorGetZ(vRadianAngles);
	//m_vCurrentRotateSpeedRUL.x = m_vStartRotateRUL.x = XMVectorGetX(vRadianAngles);
	//m_vCurrentRotateSpeedRUL.y = m_vStartRotateRUL.y = XMVectorGetY(vRadianAngles);
	//m_vCurrentRotateSpeedRUL.z = m_vStartRotateRUL.z = XMVectorGetZ(vRadianAngles);  
#else // RUL
	_vector vRadianAngles = { XMConvertToRadians(Desc.vRotationRUL.x), XMConvertToRadians(Desc.vRotationRUL.y), XMConvertToRadians(Desc.vRotationRUL.z) };

	m_pTransformCom->Set_Scaled({ m_vCurrentScaleRUL.x, m_vCurrentScaleRUL.y, m_vCurrentScaleRUL.z });

	m_pTransformCom->Turn(m_pTransformCom->Get_WorldMatrix().r[0], XMVectorGetX(vRadianAngles));
	m_pTransformCom->Turn(m_pTransformCom->Get_WorldMatrix().r[1], XMVectorGetY(vRadianAngles));
	m_pTransformCom->Turn(m_pTransformCom->Get_WorldMatrix().r[2], XMVectorGetZ(vRadianAngles));
#endif // 0


	if (Desc.bIsBillBoard)
	{
		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);
		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f);
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector4Normalize(vRight) * m_vStartScaleRUL.x * m_vOffsetScale.x);
		m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector4Normalize(vUp) * m_vStartScaleRUL.y * m_vOffsetScale.y);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector4Normalize(vLook) * m_vStartScaleRUL.z * m_vOffsetScale.z);
		m_pTransformCom->SetBilBoard();
		// 초기 Rotation 설정.
		_vector vRadianAngles = { 0.f, 0.f, m_vStartRotateRUL.z };
		_vector vQuaternionL = XMQuaternionRotationAxis(m_pTransformCom->Get_WorldMatrix().r[2], m_vStartRotateRUL.z);
		_matrix RotationMatrix = XMMatrixRotationQuaternion(/*vQuaternionR + vQuaternionU + */vQuaternionL);
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), RotationMatrix));
		m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3TransformNormal(m_pTransformCom->Get_State(CTransform::STATE_UP), RotationMatrix));
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(m_pTransformCom->Get_State(CTransform::STATE_LOOK), RotationMatrix));
		m_fRotationAcc += m_vStartRotateRUL.z;
	}

	//Color
	m_vCurrentDiffuseColorRGBA.x = m_vStartDiffuseColorRGBA.x = Desc.vStartDiffuseColorRGBA.x;
	m_vCurrentDiffuseColorRGBA.y = m_vStartDiffuseColorRGBA.y = Desc.vStartDiffuseColorRGBA.y;
	m_vCurrentDiffuseColorRGBA.z = m_vStartDiffuseColorRGBA.z = Desc.vStartDiffuseColorRGBA.z;
	m_vCurrentDiffuseColorRGBA.w = m_vStartDiffuseColorRGBA.w = Desc.vStartDiffuseColorRGBA.w;
	m_vCurrentGlowColorRGBA.x = Desc.vStartGlowColorRGBA.x;
	m_vCurrentGlowColorRGBA.y = Desc.vStartGlowColorRGBA.y;
	m_vCurrentGlowColorRGBA.z = Desc.vStartGlowColorRGBA.z;
	m_vCurrentGlowColorRGBA.w = Desc.vStartGlowColorRGBA.w;
	m_fCurrentDistortionStrength = Desc.fStartDistortionStrength;
	m_vCurrentRimLightColorRGBA = Desc.vStartRimLightColorRGBA;

	// Noise
	// ClampSampling시 끝부터 서서히 나오게 하기 위해.
	if (0.f < Desc.fUVSpeedXY_Noise.x)
		m_vUVSpeedPos_Noise.x = -1.f;
	else if (0.f > Desc.fUVSpeedXY_Noise.x)
		m_vUVSpeedPos_Noise.x = 1.f;

	if (0.f < Desc.fUVSpeedXY_Noise.y)
		m_vUVSpeedPos_Noise.y = -1.f;
	else if (0.f > Desc.fUVSpeedXY_Noise.y)
		m_vUVSpeedPos_Noise.y = 1.f;

	if (Desc.bIsUVSpeedMode)
	{
		// ClampSampling시 끝부터 서서히 나오게 하기 위해.
		if (0.f < Desc.fUVSpeedXY_Mask.x)
			m_vUVSpeedPos_Mask.x = -1.f;
		else if (0.f > Desc.fUVSpeedXY_Mask.x)
			m_vUVSpeedPos_Mask.x = 1.f;

		if (0.f < Desc.fUVSpeedXY_Mask.y)
			m_vUVSpeedPos_Mask.y = -1.f;
		else if (0.f > Desc.fUVSpeedXY_Mask.y)
			m_vUVSpeedPos_Mask.y = 1.f;
	}
	else
	{
		m_fSpriteTimeAcc = 0;
		m_fUVScaleXCur_Mask = 0;
		m_fUVScaleYCur_Mask = 0;
	}

	return S_OK;
}

void CEffect_Mesh::Set_PositioningMode(_matrix matTransform, _float fTimeSpeed)
{
	m_fTimeSpeed = fTimeSpeed;

	XMStoreFloat4x4(&m_OffSetMatrix, {
		NULL == XMVectorGetX(XMVector3Length(matTransform.r[0])) ? XMVectorSet(1.f, 0.f, 0.f, 0.f) : XMVector3Normalize(matTransform.r[0]),
		NULL == XMVectorGetX(XMVector3Length(matTransform.r[1])) ? XMVectorSet(0.f, 1.f, 0.f, 0.f) : XMVector3Normalize(matTransform.r[1]),
		NULL == XMVectorGetX(XMVector3Length(matTransform.r[2])) ? XMVectorSet(0.f, 0.f, 1.f, 0.f) : XMVector3Normalize(matTransform.r[2]),
		matTransform.r[3] });
	m_vOffsetScale.x = NULL == XMVectorGetX(XMVector3Length(matTransform.r[0])) ? 1.f : XMVectorGetX(XMVector3Length(matTransform.r[0]));
	m_vOffsetScale.y = NULL == XMVectorGetX(XMVector3Length(matTransform.r[1])) ? 1.f : XMVectorGetX(XMVector3Length(matTransform.r[1]));
	m_vOffsetScale.z = NULL == XMVectorGetX(XMVector3Length(matTransform.r[2])) ? 1.f : XMVectorGetX(XMVector3Length(matTransform.r[2]));

	m_pTransformCom->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix() * matTransform);
}

void CEffect_Mesh::Set_FollowingMode(_matrix matTransform, _float fTimeSpeed, CGameObject* pOwner, CEffect_Manager::FOLLOW_MODE eFollowMode, string strBoneName)
{
	m_fTimeSpeed = fTimeSpeed;

	XMStoreFloat4x4(&m_OffSetMatrix, {
		NULL == XMVectorGetX(XMVector3Length(matTransform.r[0])) ? XMVectorSet(1.f, 0.f, 0.f, 0.f) : XMVector3Normalize(matTransform.r[0]),
		NULL == XMVectorGetX(XMVector3Length(matTransform.r[1])) ? XMVectorSet(0.f, 1.f, 0.f, 0.f) : XMVector3Normalize(matTransform.r[1]),
		NULL == XMVectorGetX(XMVector3Length(matTransform.r[2])) ? XMVectorSet(0.f, 0.f, 1.f, 0.f) : XMVector3Normalize(matTransform.r[2]),
		matTransform.r[3] });
	m_vOffsetScale.x = NULL == XMVectorGetX(XMVector3Length(matTransform.r[0])) ? 1.f : XMVectorGetX(XMVector3Length(matTransform.r[0]));
	m_vOffsetScale.y = NULL == XMVectorGetX(XMVector3Length(matTransform.r[1])) ? 1.f : XMVectorGetX(XMVector3Length(matTransform.r[1]));
	m_vOffsetScale.z = NULL == XMVectorGetX(XMVector3Length(matTransform.r[2])) ? 1.f : XMVectorGetX(XMVector3Length(matTransform.r[2]));

	m_pOwner = pOwner;
	if (nullptr != pOwner)
		m_bIsHaveOwner = true;
	m_pOwnerModelCom = static_cast<CModel*>(m_pOwner->Get_Component(ID_UINT(MODEL_DYNAMIC)));
	m_strOwnerBoneName = strBoneName;
	m_eFollowMode = eFollowMode;
}

CEffect_Mesh* CEffect_Mesh::Create()
{
	CEffect_Mesh* pInstance = new CEffect_Mesh();
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Mesh");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Mesh::Clone(void* pArg)
{
	CEffect_Mesh* pInstance = new CEffect_Mesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Mesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Mesh::Free()
{
	__super::Free();
}