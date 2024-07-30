#include "stdafx.h"
#include "State_Handler.h"

#include "Unit.h"
#include "GameInstance.h"

#include "Easing_Utillity.h"
#include "Enemy_Robella_PoisonFog.h"
#include "Model.h"
#include "HIerarchyNode.h"

#include "Effect_Manager.h"
#include "Imgui_Manager.h"
//#include "Enemy_Boss_Robella.h"

CEnemy_Robella_PoisonFog::CEnemy_Robella_PoisonFog()
	: CGameObject()
{
}

CEnemy_Robella_PoisonFog::CEnemy_Robella_PoisonFog(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEnemy_Robella_PoisonFog::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CEnemy_Robella_PoisonFog::Initialize(void* pArg)
{
	NULLCHECK(pArg);
	m_pRobella = (CGameObject*)pArg;
	//memcpy(&m_pRobella, pArg, sizeof(CGameObject*));
	m_bIsHaveOwner = true;

	SetUp_MeshDesc();

	FAILEDCHECK(__super::Initialize());
	FAILEDCHECK(SetUp_Components());
	GAMEINSTANCE->Add_TickGroup(TG_EFFECT, this);
	GAMEINSTANCE->Add_LateTickGroup(LTG_EFFECT, this);


	XMStoreFloat4x4(&m_OffSetMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CEnemy_Robella_PoisonFog::Initialize_Pooling(void* pArg)
{
	USEGAMEINSTANCE;

	m_iNumMeshContainers = 0;
	m_fTimeSpeed = 1.f;
	m_pOwner = nullptr;
	m_bIsHaveOwner = false;
	m_pOwnerModelCom = nullptr;
	m_strOwnerBoneName = "";
	m_eFollowMode = CEffect_Manager::FOLLOW_MODE_END;

	//Remove_Component(ID_UINT(MODEL_STATIC, MODEL_EFFECT));
	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);

	m_fTimeAcc = 0.f;
	m_fTimePointOfDuration = 0.f; // For. Middle Easing

	m_vOffsetScale = { 1.f, 1.f, 1.f, 0.f };

	m_fRatio = 0.f;
	m_fRatioSpeed = 1.f;
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
	m_pRobella = (CGameObject*)pArg;
	//memcpy(&m_pRobella, pArg, sizeof(CGameObject*));
	m_bIsHaveOwner = true;
	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	SetUp_MeshDesc();

	XMStoreFloat4x4(&m_OffSetMatrix, XMMatrixIdentity());

	pGameInstance->Add_TickGroup(TG_EFFECT, this);
	pGameInstance->Add_LateTickGroup(LTG_EFFECT, this);

	return S_OK;
}

void CEnemy_Robella_PoisonFog::Tick(_float _fTimeDelta)
{
	USEGAMEINSTANCE;
	_float fTimeDelta = _fTimeDelta * m_fTimeSpeed;
	m_fTimeAcc += fTimeDelta;
	
	// 객체 죽을 때 바로 사라지도록
#if 0
	if (/*m_bIsHaveOwner && m_pRobella->Is_Dead()*/)
	{
		//#ifdef _EFFECTTOOL
		//pGameInstance->Add_DeadObject(this);
		//#else
		pGameInstance->Add_PoolingObject(this);
		//#endif // _EFFECTTOOL
		return;
	}
#endif // 0

	// 독안개 나오는동안 이펙트 
	// m_TimeAcc이 초기 StartTime Offset 보다 클 때 Start.
	if (m_fTimeAcc > m_MeshEffectDesc.fLifeTime.x)
	{
		if(2.f < m_fTimeAcc)
			IMPL_ONCE(m_bEffect0, EFFECT_POS("Boss_Robella_Poision_Mark", m_pRobella->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 0.f, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(3.f, 3.f, 3.f, 1.f)));

		if (4.f < m_fTimeAcc)
			IMPL_ONCE(m_bEffect1, EFFECT_POS("Boss_Robella_Poision_Mark", m_pRobella->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 0.f, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(3.f, 3.f, 3.f, 1.f)));
		// 종료 시점.
		if (m_fTimeAcc > m_MeshEffectDesc.fLifeTime.y + m_MeshEffectDesc.fLifeTime.x) // Duration + Offset
		{
			// Loop 일 때.
			if (m_MeshEffectDesc.bIsLoop)
			{
				m_fTimeAcc = 0.f;
				//Initialize_Loop();
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

void CEnemy_Robella_PoisonFog::LateTick(_float fTimeDelta)
{
	if (m_bIsHaveOwner && CEffect_Manager::FOLLOW_MODE_END != m_eFollowMode)
	{
		if (1/*!m_pRobella->Is_Dead()*/)
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
				_vector vRight = matWorld.r[0] * m_vCurrentScaleRUL.x;
				_vector vUp = matWorld.r[1] * m_vCurrentScaleRUL.y;
				_vector vLook = matWorld.r[2] * m_vCurrentScaleRUL.z;
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
					m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[0]) * m_vCurrentScaleRUL.x);
					m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[1]) * m_vCurrentScaleRUL.y);
					m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[2]) * m_vCurrentScaleRUL.z);
					m_pTransformCom->SetBilBoard();
#if 1
					m_fRotationAcc += m_vCurrentRotateSpeedRUL.z;
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
				_matrix matFinalWorld = matWorld * matFollow;
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

HRESULT CEnemy_Robella_PoisonFog::Render()
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

void CEnemy_Robella_PoisonFog::Set_FollowingMode(_matrix matTransform, _float fTimeSpeed, CGameObject* pOwner, CEffect_Manager::FOLLOW_MODE eFollowMode, string strBoneName)
{
	m_fTimeSpeed = fTimeSpeed;

	XMStoreFloat4x4(&m_OffSetMatrix, matTransform);
	m_pOwner = pOwner;
	if (nullptr != pOwner)
		m_bIsHaveOwner = true;
	m_pOwnerModelCom = static_cast<CModel*>(m_pOwner->Get_Component(ID_UINT(MODEL_DYNAMIC)));
	m_strOwnerBoneName = strBoneName;
	m_eFollowMode = eFollowMode;
}

void CEnemy_Robella_PoisonFog::UVSpeed_Sprite(_float fTimeDelta)
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

void CEnemy_Robella_PoisonFog::Moving(_float fTimeDelta)
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

void CEnemy_Robella_PoisonFog::Scaling_Rotating(_float fTimeDelta)
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
		// X, Y ,Z 축 회전 
		_vector vRadianAngles = { m_vCurrentRotateSpeedRUL.x, m_vCurrentRotateSpeedRUL.y, m_vCurrentRotateSpeedRUL.z };
		_vector	vQuaternion = XMQuaternionRotationRollPitchYawFromVector(vRadianAngles);
		_matrix RotationMatrix = XMMatrixRotationQuaternion(vQuaternion);

		// _vector vQuaternionR = XMQuaternionRotationAxis(m_pTransformCom->Get_WorldMatrix().r[0], m_vCurrentRotateSpeedRUL.x);
		// _vector vQuaternionU = XMQuaternionRotationAxis(m_pTransformCom->Get_WorldMatrix().r[1], m_vCurrentRotateSpeedRUL.y);
		// _vector vQuaternionL = XMQuaternionRotationAxis(m_pTransformCom->Get_WorldMatrix().r[2], m_vCurrentRotateSpeedRUL.z);
		// _matrix RotationMatrix = XMMatrixRotationQuaternion(vQuaternionR + vQuaternionU + vQuaternionL);

		_vector		vRight, vUp, vLook;
		vRight = XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[0]) * m_vCurrentScaleRUL.x * m_vOffsetScale.x;
		vUp = XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[1]) * m_vCurrentScaleRUL.y * m_vOffsetScale.y;
		vLook = XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[2]) * m_vCurrentScaleRUL.z * m_vOffsetScale.z;
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
		m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
	}
}

void CEnemy_Robella_PoisonFog::Easing_DiffuseColor(void)
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

	USEGAMEINSTANCE;

	NULLCHECK(m_pRobella);
	_vector vRobellaPos = XMLoadFloat4x4(m_pRobella->Get_WorldMatrix()).r[3];
	_float fRobellaDistance	= XMVectorGetX(XMVector3Length(vRobellaPos - pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(PLAYER), 0)->Get_Pos()));//플레이어
	if (5.f < fRobellaDistance)
	{
		m_vCurrentDiffuseColorRGBA.w = min(max(0, (10.f - (fRobellaDistance - 5.f/*공격범위*/)) / 10.f), m_MeshEffectDesc.vMiddleDiffuseColorRGBA.w);
	}

}

void CEnemy_Robella_PoisonFog::Easing_GlowColor(void)
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

void CEnemy_Robella_PoisonFog::Easing_Shader(void)
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

HRESULT CEnemy_Robella_PoisonFog::SetUp_Components()
{
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODELEFFECT), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	/* For.Com_Texture */
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_MASK), ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_MASK), (CComponent**)&m_pTextureCom_Mask));
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), (CComponent**)&m_pTextureCom_Noise));

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

HRESULT CEnemy_Robella_PoisonFog::SetUp_ShaderResource()
{
	NULLCHECK(m_pShaderCom);
	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));
	FAILEDCHECK(m_pTextureCom_Mask->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", m_MeshEffectDesc.iMaskIndex));
	FAILEDCHECK(m_pTextureCom_Noise->Set_ShaderResourceView(m_pShaderCom, "g_NoiseTexture", m_MeshEffectDesc.iNoiseIndex));
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
	}


	if (0 < m_fTimePointOfDuration)
	{
		FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsDissolve", &m_MeshEffectDesc.bIsDissolve, sizeof(_bool)));
		if (m_MeshEffectDesc.bIsDissolve)
		{
			FAILEDCHECK(m_pTextureCom_Noise->Set_ShaderResourceView(m_pShaderCom, "g_DissolveNoiseTexture", m_MeshEffectDesc.iDissolveNoiseTextureIndex));
			FAILEDCHECK(m_pTextureCom_Noise->Set_ShaderResourceView(m_pShaderCom, "g_DissolveColorTexture", m_MeshEffectDesc.iDissolveColorTextureIndex));
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
	return S_OK;
}

HRESULT CEnemy_Robella_PoisonFog::SetUp_MeshDesc(void)
{
	// MESH DESC SETTING
	// MESH DESC SETTING
	// 기존 Poision 이펙트에서 사용되던 인자값.
	m_MeshEffectDesc.bIsBillBoard = false;
	m_MeshEffectDesc.bIsBlur = false;
	m_MeshEffectDesc.bIsClampSampling_Mask = false;
	m_MeshEffectDesc.bIsClampSampling_Noise = false;
	m_MeshEffectDesc.bIsDissolve = false;
	m_MeshEffectDesc.bIsDistortion = false;
	m_MeshEffectDesc.bIsGlow = false;
	m_MeshEffectDesc.bIsGlowFollowDiffuseColor = false;
	m_MeshEffectDesc.bIsLoop = false;
	m_MeshEffectDesc.bIsReverseXY_Mask = false;
	m_MeshEffectDesc.bIsReverseXY_Noise = false;
	m_MeshEffectDesc.bIsRimLight = false;
	m_MeshEffectDesc.bIsSoftEffect = true;
	m_MeshEffectDesc.bIsSpeedEasingMode = true;
	m_MeshEffectDesc.bIsTrail = false;
	m_MeshEffectDesc.bIsUVSpeedMode = true;
	m_MeshEffectDesc.fDissolveStrength = 0.10000000149011612;
	m_MeshEffectDesc.fEndDistortionStrength = 1.0;
	m_MeshEffectDesc.fLifeTime.x = 0.0;
	m_MeshEffectDesc.fLifeTime.y = 6.0;
	m_MeshEffectDesc.fMiddleDiffuseColorTimePoint = 3.0;
	m_MeshEffectDesc.fMiddleDistortionStrength = 1.0;
	m_MeshEffectDesc.fMiddleDistortionStrengthTimePoint = 1.0;
	m_MeshEffectDesc.fMiddleGlowColorTimePoint = 1.0;
	m_MeshEffectDesc.fMiddleRimLightColorTimePoint = 1.0;
	m_MeshEffectDesc.fMiddleScaleTimePoint = 1.0;
	m_MeshEffectDesc.fMiddleSpeedTimePoint = 1.0;
	m_MeshEffectDesc.fRotatationAccelation.x = 0.0;
	m_MeshEffectDesc.fRotatationAccelation.y = 0.0;
	m_MeshEffectDesc.fRotatationAccelation.z = 0.0;
	m_MeshEffectDesc.fSpeedAccelation.x = 0.0;
	m_MeshEffectDesc.fSpeedAccelation.y = 0.0;
	m_MeshEffectDesc.fSpeedAccelation.z = 0.0;
	m_MeshEffectDesc.fSpriteSpeed = 0.05000000074505806;
	m_MeshEffectDesc.fStartDistortionStrength = 0.0;
	m_MeshEffectDesc.fTrailStrengh = 0.10000000149011612;
	m_MeshEffectDesc.fUVSpeedXY_Mask.x = 0.029999999329447746;
	m_MeshEffectDesc.fUVSpeedXY_Mask.y = 0.0;
	m_MeshEffectDesc.fUVSpeedXY_Noise.x = 0.00800000037997961;
	m_MeshEffectDesc.fUVSpeedXY_Noise.y = 0.0;
	m_MeshEffectDesc.iDiffuseColorEasing_End = 16;
	m_MeshEffectDesc.iDiffuseColorEasing_Middle = 8;
	m_MeshEffectDesc.iDissolveColorTextureIndex = 0;
	m_MeshEffectDesc.iDissolveNoiseTextureIndex = 0;
	m_MeshEffectDesc.iDistortionStrengthEasing_End = 27;
	m_MeshEffectDesc.iDistortionStrengthEasing_Middle = 27;
	m_MeshEffectDesc.iGlowColorEasing_End = 27;
	m_MeshEffectDesc.iGlowColorEasing_Middle = 27;
	m_MeshEffectDesc.iMaskIndex = 1941;
	m_MeshEffectDesc.iMeshIndex = 1792;
	m_MeshEffectDesc.iNoiseIndex = 656;
	m_MeshEffectDesc.iRimLightColorEasing_End = 27;
	m_MeshEffectDesc.iRimLightColorEasing_Middle = 27;
	m_MeshEffectDesc.iScaleEasing_End = 27;
	m_MeshEffectDesc.iScaleEasing_Middle = 27;
	m_MeshEffectDesc.iSpeedEasing_End = 27;
	m_MeshEffectDesc.iSpeedEasing_Middle = 27;
	m_MeshEffectDesc.iUVScaleXMax_Mask = 1;
	m_MeshEffectDesc.iUVScaleXMax_Noise = 1;
	m_MeshEffectDesc.iUVScaleYMax_Mask = 1;
	m_MeshEffectDesc.iUVScaleYMax_Noise = 1;
	m_MeshEffectDesc.vDissolveColorRGBA.w = 1.0;
	m_MeshEffectDesc.vDissolveColorRGBA.x = 1.0;
	m_MeshEffectDesc.vDissolveColorRGBA.y = 1.0;
	m_MeshEffectDesc.vDissolveColorRGBA.z = 1.0;
	m_MeshEffectDesc.vEndDiffuseColorRGBA.w = 0.0;
	m_MeshEffectDesc.vEndDiffuseColorRGBA.x = 0.88400000333786011;
	m_MeshEffectDesc.vEndDiffuseColorRGBA.y = 1.0;
	m_MeshEffectDesc.vEndDiffuseColorRGBA.z = 0.0099999997764825821;
	m_MeshEffectDesc.vEndGlowColorRGBA.w = 1.0;
	m_MeshEffectDesc.vEndGlowColorRGBA.x = 1.0;
	m_MeshEffectDesc.vEndGlowColorRGBA.y = 1.0;
	m_MeshEffectDesc.vEndGlowColorRGBA.z = 1.0;
	m_MeshEffectDesc.vEndRimLightColorRGBA.w = 1.0;
	m_MeshEffectDesc.vEndRimLightColorRGBA.x = 1.0;
	m_MeshEffectDesc.vEndRimLightColorRGBA.y = 1.0;
	m_MeshEffectDesc.vEndRimLightColorRGBA.z = 1.0;
	m_MeshEffectDesc.vEndScaleXYZ.x = 6.0;
	m_MeshEffectDesc.vEndScaleXYZ.y = 6.0;
	m_MeshEffectDesc.vEndScaleXYZ.z = 6.0;
	m_MeshEffectDesc.vEndSpeedXYZ.x = 1.0;
	m_MeshEffectDesc.vEndSpeedXYZ.y = 1.0;
	m_MeshEffectDesc.vEndSpeedXYZ.z = 1.0;
	m_MeshEffectDesc.vMiddleDiffuseColorRGBA.w = 1.0;
	m_MeshEffectDesc.vMiddleDiffuseColorRGBA.x = 0.88400000333786011;
	m_MeshEffectDesc.vMiddleDiffuseColorRGBA.y = 1.0;
	m_MeshEffectDesc.vMiddleDiffuseColorRGBA.z = 0.0099999997764825821;
	m_MeshEffectDesc.vMiddleGlowColorRGBA.w = 1.0;
	m_MeshEffectDesc.vMiddleGlowColorRGBA.x = 1.0;
	m_MeshEffectDesc.vMiddleGlowColorRGBA.y = 1.0;
	m_MeshEffectDesc.vMiddleGlowColorRGBA.z = 1.0;
	m_MeshEffectDesc.vMiddleRimLightColorRGBA.w = 1.0;
	m_MeshEffectDesc.vMiddleRimLightColorRGBA.x = 1.0;
	m_MeshEffectDesc.vMiddleRimLightColorRGBA.y = 1.0;
	m_MeshEffectDesc.vMiddleRimLightColorRGBA.z = 1.0;
	m_MeshEffectDesc.vMiddleScaleXYZ.x = 1.0;
	m_MeshEffectDesc.vMiddleScaleXYZ.y = 1.0;
	m_MeshEffectDesc.vMiddleScaleXYZ.z = 1.0;
	m_MeshEffectDesc.vMiddleSpeedXYZ.x = 1.0;
	m_MeshEffectDesc.vMiddleSpeedXYZ.y = 1.0;
	m_MeshEffectDesc.vMiddleSpeedXYZ.z = 1.0;
	m_MeshEffectDesc.vPositionOffset.x = 0.0;
	m_MeshEffectDesc.vPositionOffset.y = 0.0;
	m_MeshEffectDesc.vPositionOffset.z = 0.0;
	m_MeshEffectDesc.vRotationRUL.x = 0.0;
	m_MeshEffectDesc.vRotationRUL.y = 0.0;
	m_MeshEffectDesc.vRotationRUL.z = 0.0;
	m_MeshEffectDesc.vRotationSpeedRUL.x = 0.0;
	m_MeshEffectDesc.vRotationSpeedRUL.y = 0.0;
	m_MeshEffectDesc.vRotationSpeedRUL.z = 0.0;
	m_MeshEffectDesc.vStartDiffuseColorRGBA.w = 0.0;
	m_MeshEffectDesc.vStartDiffuseColorRGBA.x = 0.88350635766983032;
	m_MeshEffectDesc.vStartDiffuseColorRGBA.y = 1.0;
	m_MeshEffectDesc.vStartDiffuseColorRGBA.z = 0.0098039507865905762;
	m_MeshEffectDesc.vStartGlowColorRGBA.w = 1.0;
	m_MeshEffectDesc.vStartGlowColorRGBA.x = 1.0;
	m_MeshEffectDesc.vStartGlowColorRGBA.y = 0.0;
	m_MeshEffectDesc.vStartGlowColorRGBA.z = 0.0;
	m_MeshEffectDesc.vStartRimLightColorRGBA.w = 1.0;
	m_MeshEffectDesc.vStartRimLightColorRGBA.x = 1.0;
	m_MeshEffectDesc.vStartRimLightColorRGBA.y = 1.0;
	m_MeshEffectDesc.vStartRimLightColorRGBA.z = 1.0;
	m_MeshEffectDesc.vStartScaleXYZ.x = 0.15000000596046448;
	m_MeshEffectDesc.vStartScaleXYZ.y = 0.15000000596046448;
	m_MeshEffectDesc.vStartScaleXYZ.z = 0.15000000596046448;
	m_MeshEffectDesc.vStartSpeedXYZ.x = 0.0;
	m_MeshEffectDesc.vStartSpeedXYZ.y = 0.0;
	m_MeshEffectDesc.vStartSpeedXYZ.z = 0.0;
	m_MeshEffectDesc.vTrailColor.w = 1.0;
	m_MeshEffectDesc.vTrailColor.x = 1.0;
	m_MeshEffectDesc.vTrailColor.y = 1.0;
	m_MeshEffectDesc.vTrailColor.z = 1.0;
	// 
	// MESH DESC SETTING
	// MESH DESC SETTING

	SetUp_CurDesc(m_MeshEffectDesc);

	return S_OK;
}

HRESULT CEnemy_Robella_PoisonFog::SetUp_CurDesc(MESHEFFECT_DESC& Desc)
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
	m_vCurrentRotateSpeedRUL.x = m_vStartRotateRUL.x = XMVectorGetX(vRadianAngles);
	m_vCurrentRotateSpeedRUL.y = m_vStartRotateRUL.y = XMVectorGetY(vRadianAngles);
	m_vCurrentRotateSpeedRUL.z = m_vStartRotateRUL.z = XMVectorGetZ(vRadianAngles);

	if (Desc.bIsBillBoard)
	{
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[0]) * m_vCurrentScaleRUL.x * m_vOffsetScale.x);
		m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[1]) * m_vCurrentScaleRUL.y * m_vOffsetScale.y);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector4Normalize(m_pTransformCom->Get_WorldMatrix().r[2]) * m_vCurrentScaleRUL.z * m_vOffsetScale.z);
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

CEnemy_Robella_PoisonFog* CEnemy_Robella_PoisonFog::Create()
{
	CEnemy_Robella_PoisonFog* pInstance = new CEnemy_Robella_PoisonFog();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Robella_PoisonFog");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Robella_PoisonFog::Clone(void* pArg)
{
	CEnemy_Robella_PoisonFog* pInstance = new CEnemy_Robella_PoisonFog(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Robella_PoisonFog");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Robella_PoisonFog::Free()
{
	__super::Free();
}
