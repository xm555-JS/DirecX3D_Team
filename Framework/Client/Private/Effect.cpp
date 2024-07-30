#include "stdafx.h"
#include "Effect.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Effect_Rect.h"
#include "Effect_Mesh.h"

CEffect::CEffect()
{
}

HRESULT CEffect::Initialize(const EFFECT_DESC& EffectDesc)
{
#if defined(_EFFECT)
	m_EffectDesc.Effect_Name = EffectDesc.Effect_Name;

	for (auto& Particle : EffectDesc.ParticleDesc)
	{
		PARTICLE_DESC ParticleDesc;
		ParticleDesc.iMaskIndex = Particle.iMaskIndex;
		ParticleDesc.iNumInstance = Particle.iNumInstance;
		ParticleDesc.bIsBillBoard = Particle.bIsBillBoard;
		ParticleDesc.fLifeTime.x = Particle.fLifeTime.x;
		ParticleDesc.fLifeTime.y = Particle.fLifeTime.y;
		ParticleDesc.bIsLoop = Particle.bIsLoop;
		ParticleDesc.bIsReverseXY_Mask = Particle.bIsReverseXY_Mask;
		ParticleDesc.iUVScaleXCur = Particle.iUVScaleXCur;
		ParticleDesc.iUVScaleYCur = Particle.iUVScaleYCur;
		ParticleDesc.iUVScaleXMax = Particle.iUVScaleXMax;
		ParticleDesc.iUVScaleYMax = Particle.iUVScaleYMax;

		ParticleDesc.vStartDiffuseColorRGBA_Min.x = Particle.vStartDiffuseColorRGBA_Min.x;
		ParticleDesc.vStartDiffuseColorRGBA_Min.y = Particle.vStartDiffuseColorRGBA_Min.y;
		ParticleDesc.vStartDiffuseColorRGBA_Min.z = Particle.vStartDiffuseColorRGBA_Min.z;
		ParticleDesc.vStartDiffuseColorRGBA_Min.w = Particle.vStartDiffuseColorRGBA_Min.w;
		ParticleDesc.vStartDiffuseColorRGBA_Max.x = Particle.vStartDiffuseColorRGBA_Max.x;
		ParticleDesc.vStartDiffuseColorRGBA_Max.y = Particle.vStartDiffuseColorRGBA_Max.y;
		ParticleDesc.vStartDiffuseColorRGBA_Max.z = Particle.vStartDiffuseColorRGBA_Max.z;
		ParticleDesc.vStartDiffuseColorRGBA_Max.w = Particle.vStartDiffuseColorRGBA_Max.w;
		ParticleDesc.iDiffuseColorEasing_End = Particle.iDiffuseColorEasing_End;
		ParticleDesc.iDiffuseColorEasing_Middle = Particle.iDiffuseColorEasing_Middle;
		ParticleDesc.fMiddleDiffuseColorTimePoint = Particle.fMiddleDiffuseColorTimePoint;
		ParticleDesc.vMiddleDiffuseColorRGBA.x = Particle.vMiddleDiffuseColorRGBA.x;
		ParticleDesc.vMiddleDiffuseColorRGBA.y = Particle.vMiddleDiffuseColorRGBA.y;
		ParticleDesc.vMiddleDiffuseColorRGBA.z = Particle.vMiddleDiffuseColorRGBA.z;
		ParticleDesc.vMiddleDiffuseColorRGBA.w = Particle.vMiddleDiffuseColorRGBA.w;
		ParticleDesc.vEndDiffuseColorRGBA.x = Particle.vEndDiffuseColorRGBA.x;
		ParticleDesc.vEndDiffuseColorRGBA.y = Particle.vEndDiffuseColorRGBA.y;
		ParticleDesc.vEndDiffuseColorRGBA.z = Particle.vEndDiffuseColorRGBA.z;
		ParticleDesc.vEndDiffuseColorRGBA.w = Particle.vEndDiffuseColorRGBA.w;

		ParticleDesc.bIsGlowFollowDiffuseColor = Particle.bIsGlowFollowDiffuseColor;
		ParticleDesc.vStartGlowColorRGBA_Min.x = Particle.vStartGlowColorRGBA_Min.x;
		ParticleDesc.vStartGlowColorRGBA_Min.y = Particle.vStartGlowColorRGBA_Min.y;
		ParticleDesc.vStartGlowColorRGBA_Min.z = Particle.vStartGlowColorRGBA_Min.z;
		ParticleDesc.vStartGlowColorRGBA_Min.w = Particle.vStartGlowColorRGBA_Min.w;
		ParticleDesc.vStartGlowColorRGBA_Max.x = Particle.vStartGlowColorRGBA_Max.x;
		ParticleDesc.vStartGlowColorRGBA_Max.y = Particle.vStartGlowColorRGBA_Max.y;
		ParticleDesc.vStartGlowColorRGBA_Max.z = Particle.vStartGlowColorRGBA_Max.z;
		ParticleDesc.vStartGlowColorRGBA_Max.w = Particle.vStartGlowColorRGBA_Max.w;
		ParticleDesc.iGlowColorEasing_End = Particle.iGlowColorEasing_End;
		ParticleDesc.iGlowColorEasing_Middle = Particle.iGlowColorEasing_Middle;
		ParticleDesc.fMiddleGlowColorTimePoint = Particle.fMiddleGlowColorTimePoint;
		ParticleDesc.vMiddleGlowColorRGBA.x = Particle.vMiddleGlowColorRGBA.x;
		ParticleDesc.vMiddleGlowColorRGBA.y = Particle.vMiddleGlowColorRGBA.y;
		ParticleDesc.vMiddleGlowColorRGBA.z = Particle.vMiddleGlowColorRGBA.z;
		ParticleDesc.vMiddleGlowColorRGBA.w = Particle.vMiddleGlowColorRGBA.w;
		ParticleDesc.vEndGlowColorRGBA.x = Particle.vEndGlowColorRGBA.x;
		ParticleDesc.vEndGlowColorRGBA.y = Particle.vEndGlowColorRGBA.y;
		ParticleDesc.vEndGlowColorRGBA.z = Particle.vEndGlowColorRGBA.z;
		ParticleDesc.vEndGlowColorRGBA.w = Particle.vEndGlowColorRGBA.w;

		ParticleDesc.bIsGlow = Particle.bIsGlow;
		ParticleDesc.bIsBlur = Particle.bIsBlur;
		ParticleDesc.vStartScaleXYZ_Min.x = Particle.vStartScaleXYZ_Min.x;
		ParticleDesc.vStartScaleXYZ_Min.y = Particle.vStartScaleXYZ_Min.y;
		ParticleDesc.vStartScaleXYZ_Min.z = Particle.vStartScaleXYZ_Min.z;
		ParticleDesc.vStartScaleXYZ_Max.x = Particle.vStartScaleXYZ_Max.x;
		ParticleDesc.vStartScaleXYZ_Max.y = Particle.vStartScaleXYZ_Max.y;
		ParticleDesc.vStartScaleXYZ_Max.z = Particle.vStartScaleXYZ_Max.z;
		ParticleDesc.iScaleEasing_End = Particle.iScaleEasing_End;
		ParticleDesc.iScaleEasing_Middle = Particle.iScaleEasing_Middle;
		ParticleDesc.fMiddleScaleTimePoint = Particle.fMiddleScaleTimePoint;
		ParticleDesc.vMiddleScaleXYZ.x = Particle.vMiddleScaleXYZ.x;
		ParticleDesc.vMiddleScaleXYZ.y = Particle.vMiddleScaleXYZ.y;
		ParticleDesc.vMiddleScaleXYZ.z = Particle.vMiddleScaleXYZ.z;
		ParticleDesc.vEndScaleXYZ.x = Particle.vEndScaleXYZ.x;
		ParticleDesc.vEndScaleXYZ.y = Particle.vEndScaleXYZ.y;
		ParticleDesc.vEndScaleXYZ.z = Particle.vEndScaleXYZ.z;
		ParticleDesc.vPositionOffset.x = Particle.vPositionOffset.x;
		ParticleDesc.vPositionOffset.y = Particle.vPositionOffset.y;
		ParticleDesc.vPositionOffset.z = Particle.vPositionOffset.z;
		ParticleDesc.iMode = Particle.iMode;
		ParticleDesc.vRangeXYZ_Min.x = Particle.vRangeXYZ_Min.x;
		ParticleDesc.vRangeXYZ_Min.y = Particle.vRangeXYZ_Min.y;
		ParticleDesc.vRangeXYZ_Min.z = Particle.vRangeXYZ_Min.z;
		ParticleDesc.vRangeXYZ_Max.x = Particle.vRangeXYZ_Max.x;
		ParticleDesc.vRangeXYZ_Max.y = Particle.vRangeXYZ_Max.y;
		ParticleDesc.vRangeXYZ_Max.z = Particle.vRangeXYZ_Max.z;
		ParticleDesc.bIsSpeedEasingMode = Particle.bIsSpeedEasingMode;
		ParticleDesc.vStartSpeedXYZ_Min.x = Particle.vStartSpeedXYZ_Min.x;
		ParticleDesc.vStartSpeedXYZ_Min.y = Particle.vStartSpeedXYZ_Min.y;
		ParticleDesc.vStartSpeedXYZ_Min.z = Particle.vStartSpeedXYZ_Min.z;
		ParticleDesc.vStartSpeedXYZ_Max.x = Particle.vStartSpeedXYZ_Max.x;
		ParticleDesc.vStartSpeedXYZ_Max.y = Particle.vStartSpeedXYZ_Max.y;
		ParticleDesc.vStartSpeedXYZ_Max.z = Particle.vStartSpeedXYZ_Max.z;
		ParticleDesc.iSpeedEasing_End = Particle.iSpeedEasing_End;
		ParticleDesc.iSpeedEasing_Middle = Particle.iSpeedEasing_Middle;
		ParticleDesc.fMiddleSpeedTimePoint = Particle.fMiddleSpeedTimePoint;
		ParticleDesc.vMiddleSpeedXYZ.x = Particle.vMiddleSpeedXYZ.x;
		ParticleDesc.vMiddleSpeedXYZ.y = Particle.vMiddleSpeedXYZ.y;
		ParticleDesc.vMiddleSpeedXYZ.z = Particle.vMiddleSpeedXYZ.z;
		ParticleDesc.vEndSpeedXYZ.x = Particle.vEndSpeedXYZ.x;
		ParticleDesc.vEndSpeedXYZ.y = Particle.vEndSpeedXYZ.y;
		ParticleDesc.vEndSpeedXYZ.z = Particle.vEndSpeedXYZ.z;
		ParticleDesc.fEachAccelation_MinMax.x = Particle.fEachAccelation_MinMax.x;
		ParticleDesc.fEachAccelation_MinMax.y = Particle.fEachAccelation_MinMax.y;
		ParticleDesc.fWholeAccelation.x = Particle.fWholeAccelation.x;
		ParticleDesc.fWholeAccelation.y = Particle.fWholeAccelation.y;
		ParticleDesc.fWholeAccelation.z = Particle.fWholeAccelation.z;
		ParticleDesc.vRotateRUL_Min.x = Particle.vRotateRUL_Min.x;
		ParticleDesc.vRotateRUL_Min.y = Particle.vRotateRUL_Min.y;
		ParticleDesc.vRotateRUL_Min.z = Particle.vRotateRUL_Min.z;
		ParticleDesc.vRotateRUL_Max.x = Particle.vRotateRUL_Max.x;
		ParticleDesc.vRotateRUL_Max.y = Particle.vRotateRUL_Max.y;
		ParticleDesc.vRotateRUL_Max.z = Particle.vRotateRUL_Max.z;
		ParticleDesc.fRotateAccelation_Min.x = Particle.fRotateAccelation_Min.x;
		ParticleDesc.fRotateAccelation_Min.y = Particle.fRotateAccelation_Min.y;
		ParticleDesc.fRotateAccelation_Min.z = Particle.fRotateAccelation_Min.z;
		ParticleDesc.fRotateAccelation_Max.x = Particle.fRotateAccelation_Max.x;
		ParticleDesc.fRotateAccelation_Max.y = Particle.fRotateAccelation_Max.y;
		ParticleDesc.fRotateAccelation_Max.z = Particle.fRotateAccelation_Max.z;
		m_EffectDesc.ParticleDesc.push_back(ParticleDesc);
	}

	for (auto& MeshEffect : EffectDesc.MeshEffectDesc)
	{
		MESHEFFECT_DESC MeshEffectDesc;
		MeshEffectDesc.iMeshIndex = MeshEffect.iMeshIndex;
		MeshEffectDesc.iNoiseIndex = MeshEffect.iNoiseIndex;
		MeshEffectDesc.iMaskIndex = MeshEffect.iMaskIndex;
		MeshEffectDesc.bIsBillBoard = MeshEffect.bIsBillBoard;
		MeshEffectDesc.fLifeTime.x = MeshEffect.fLifeTime.x;
		MeshEffectDesc.fLifeTime.y = MeshEffect.fLifeTime.y;
		MeshEffectDesc.bIsLoop = MeshEffect.bIsLoop;
		MeshEffectDesc.bIsReverseXY_Noise = MeshEffect.bIsReverseXY_Noise;
		MeshEffectDesc.bIsClampSampling_Noise = MeshEffect.bIsClampSampling_Noise;
		MeshEffectDesc.fUVSpeedXY_Noise.x = MeshEffect.fUVSpeedXY_Noise.x;
		MeshEffectDesc.fUVSpeedXY_Noise.y = MeshEffect.fUVSpeedXY_Noise.y;
		MeshEffectDesc.iUVScaleXMax_Noise = MeshEffect.iUVScaleXMax_Noise;
		MeshEffectDesc.iUVScaleYMax_Noise = MeshEffect.iUVScaleYMax_Noise;
		MeshEffectDesc.bIsUVSpeedMode = MeshEffect.bIsUVSpeedMode;
		MeshEffectDesc.bIsReverseXY_Mask = MeshEffect.bIsReverseXY_Mask;
		MeshEffectDesc.bIsClampSampling_Mask = MeshEffect.bIsClampSampling_Mask;
		MeshEffectDesc.fUVSpeedXY_Mask.x = MeshEffect.fUVSpeedXY_Mask.x;
		MeshEffectDesc.fUVSpeedXY_Mask.y = MeshEffect.fUVSpeedXY_Mask.y;
		MeshEffectDesc.iUVScaleXMax_Mask = MeshEffect.iUVScaleXMax_Mask;
		MeshEffectDesc.iUVScaleYMax_Mask = MeshEffect.iUVScaleYMax_Mask;
		MeshEffectDesc.fSpriteSpeed = MeshEffect.fSpriteSpeed;
		MeshEffectDesc.vStartDiffuseColorRGBA.x = MeshEffect.vStartDiffuseColorRGBA.x;
		MeshEffectDesc.vStartDiffuseColorRGBA.y = MeshEffect.vStartDiffuseColorRGBA.y;
		MeshEffectDesc.vStartDiffuseColorRGBA.z = MeshEffect.vStartDiffuseColorRGBA.z;
		MeshEffectDesc.vStartDiffuseColorRGBA.w = MeshEffect.vStartDiffuseColorRGBA.w;
		MeshEffectDesc.iDiffuseColorEasing_End = MeshEffect.iDiffuseColorEasing_End;
		MeshEffectDesc.iDiffuseColorEasing_Middle = MeshEffect.iDiffuseColorEasing_Middle;
		MeshEffectDesc.fMiddleDiffuseColorTimePoint = MeshEffect.fMiddleDiffuseColorTimePoint;
		MeshEffectDesc.vMiddleDiffuseColorRGBA.x = MeshEffect.vMiddleDiffuseColorRGBA.x;
		MeshEffectDesc.vMiddleDiffuseColorRGBA.y = MeshEffect.vMiddleDiffuseColorRGBA.y;
		MeshEffectDesc.vMiddleDiffuseColorRGBA.z = MeshEffect.vMiddleDiffuseColorRGBA.z;
		MeshEffectDesc.vMiddleDiffuseColorRGBA.w = MeshEffect.vMiddleDiffuseColorRGBA.w;
		MeshEffectDesc.vEndDiffuseColorRGBA.x = MeshEffect.vEndDiffuseColorRGBA.x;
		MeshEffectDesc.vEndDiffuseColorRGBA.y = MeshEffect.vEndDiffuseColorRGBA.y;
		MeshEffectDesc.vEndDiffuseColorRGBA.z = MeshEffect.vEndDiffuseColorRGBA.z;
		MeshEffectDesc.vEndDiffuseColorRGBA.w = MeshEffect.vEndDiffuseColorRGBA.w;
		MeshEffectDesc.bIsGlowFollowDiffuseColor = MeshEffect.bIsGlowFollowDiffuseColor;
		MeshEffectDesc.vStartGlowColorRGBA.x = MeshEffect.vStartGlowColorRGBA.x;
		MeshEffectDesc.vStartGlowColorRGBA.y = MeshEffect.vStartGlowColorRGBA.y;
		MeshEffectDesc.vStartGlowColorRGBA.z = MeshEffect.vStartGlowColorRGBA.z;
		MeshEffectDesc.vStartGlowColorRGBA.w = MeshEffect.vStartGlowColorRGBA.w;
		MeshEffectDesc.iGlowColorEasing_End = MeshEffect.iGlowColorEasing_End;
		MeshEffectDesc.iGlowColorEasing_Middle = MeshEffect.iGlowColorEasing_Middle;
		MeshEffectDesc.fMiddleGlowColorTimePoint = MeshEffect.fMiddleGlowColorTimePoint;
		MeshEffectDesc.vMiddleGlowColorRGBA.x = MeshEffect.vMiddleGlowColorRGBA.x;
		MeshEffectDesc.vMiddleGlowColorRGBA.y = MeshEffect.vMiddleGlowColorRGBA.y;
		MeshEffectDesc.vMiddleGlowColorRGBA.z = MeshEffect.vMiddleGlowColorRGBA.z;
		MeshEffectDesc.vMiddleGlowColorRGBA.w = MeshEffect.vMiddleGlowColorRGBA.w;
		MeshEffectDesc.vEndGlowColorRGBA.x = MeshEffect.vEndGlowColorRGBA.x;
		MeshEffectDesc.vEndGlowColorRGBA.y = MeshEffect.vEndGlowColorRGBA.y;
		MeshEffectDesc.vEndGlowColorRGBA.z = MeshEffect.vEndGlowColorRGBA.z;
		MeshEffectDesc.vEndGlowColorRGBA.w = MeshEffect.vEndGlowColorRGBA.w;
		MeshEffectDesc.bIsGlow = MeshEffect.bIsGlow;
		MeshEffectDesc.bIsBlur = MeshEffect.bIsBlur;
		MeshEffectDesc.vPositionOffset.x = MeshEffect.vPositionOffset.x;
		MeshEffectDesc.vPositionOffset.y = MeshEffect.vPositionOffset.y;
		MeshEffectDesc.vPositionOffset.z = MeshEffect.vPositionOffset.z;
		MeshEffectDesc.bIsSpeedEasingMode = MeshEffect.bIsSpeedEasingMode;
		MeshEffectDesc.vStartSpeedXYZ.x = MeshEffect.vStartSpeedXYZ.x;
		MeshEffectDesc.vStartSpeedXYZ.y = MeshEffect.vStartSpeedXYZ.y;
		MeshEffectDesc.vStartSpeedXYZ.z = MeshEffect.vStartSpeedXYZ.z;
		MeshEffectDesc.iSpeedEasing_End = MeshEffect.iSpeedEasing_End;
		MeshEffectDesc.iSpeedEasing_Middle = MeshEffect.iSpeedEasing_Middle;
		MeshEffectDesc.fMiddleSpeedTimePoint = MeshEffect.fMiddleSpeedTimePoint;
		MeshEffectDesc.vMiddleSpeedXYZ.x = MeshEffect.vMiddleSpeedXYZ.x;
		MeshEffectDesc.vMiddleSpeedXYZ.y = MeshEffect.vMiddleSpeedXYZ.y;
		MeshEffectDesc.vMiddleSpeedXYZ.z = MeshEffect.vMiddleSpeedXYZ.z;
		MeshEffectDesc.vEndSpeedXYZ.x = MeshEffect.vEndSpeedXYZ.x;
		MeshEffectDesc.vEndSpeedXYZ.y = MeshEffect.vEndSpeedXYZ.y;
		MeshEffectDesc.vEndSpeedXYZ.z = MeshEffect.vEndSpeedXYZ.z;
		MeshEffectDesc.fSpeedAccelation = MeshEffect.fSpeedAccelation;
		MeshEffectDesc.vStartScaleXYZ.x = MeshEffect.vStartScaleXYZ.x;
		MeshEffectDesc.vStartScaleXYZ.y = MeshEffect.vStartScaleXYZ.y;
		MeshEffectDesc.vStartScaleXYZ.z = MeshEffect.vStartScaleXYZ.z;
		MeshEffectDesc.iScaleEasing_End = MeshEffect.iScaleEasing_End;
		MeshEffectDesc.iScaleEasing_Middle = MeshEffect.iScaleEasing_Middle;
		MeshEffectDesc.fMiddleScaleTimePoint = MeshEffect.fMiddleScaleTimePoint;
		MeshEffectDesc.vMiddleScaleXYZ.x = MeshEffect.vMiddleScaleXYZ.x;
		MeshEffectDesc.vMiddleScaleXYZ.y = MeshEffect.vMiddleScaleXYZ.y;
		MeshEffectDesc.vMiddleScaleXYZ.z = MeshEffect.vMiddleScaleXYZ.z;
		MeshEffectDesc.vEndScaleXYZ.x = MeshEffect.vEndScaleXYZ.x;
		MeshEffectDesc.vEndScaleXYZ.y = MeshEffect.vEndScaleXYZ.y;
		MeshEffectDesc.vEndScaleXYZ.z = MeshEffect.vEndScaleXYZ.z;
		MeshEffectDesc.vRotationRUL.x = MeshEffect.vRotationRUL.x;
		MeshEffectDesc.vRotationRUL.y = MeshEffect.vRotationRUL.y;
		MeshEffectDesc.vRotationRUL.z = MeshEffect.vRotationRUL.z;
		MeshEffectDesc.vRotationSpeedRUL.x = MeshEffect.vRotationSpeedRUL.x;
		MeshEffectDesc.vRotationSpeedRUL.y = MeshEffect.vRotationSpeedRUL.y;
		MeshEffectDesc.vRotationSpeedRUL.z = MeshEffect.vRotationSpeedRUL.z;
		MeshEffectDesc.fRotatationAccelation.x = MeshEffect.fRotatationAccelation.x;
		MeshEffectDesc.fRotatationAccelation.y = MeshEffect.fRotatationAccelation.y;
		MeshEffectDesc.fRotatationAccelation.z = MeshEffect.fRotatationAccelation.z;
		m_EffectDesc.MeshEffectDesc.push_back(MeshEffectDesc);
	}
#endif

	return S_OK;
}

HRESULT CEffect::Execute(_matrix matTransform, _float fTimeSpeed, CGameObject* ParentObject, CEffect_Manager::FOLLOW_MODE eFollowMode, string pBoneName)
{
#if defined(_EFFECT)

	USEGAMEINSTANCE;

	// Position Offset 생각해보자.
	for (auto& Particle : m_EffectDesc.ParticleDesc)
	{
		PARTICLE_DESC ParticleDesc;
		ParticleDesc.iMaskIndex = Particle.iMaskIndex;
		ParticleDesc.iNumInstance = Particle.iNumInstance;
		ParticleDesc.bIsBillBoard = Particle.bIsBillBoard;
		ParticleDesc.fLifeTime.x = Particle.fLifeTime.x;
		ParticleDesc.fLifeTime.y = Particle.fLifeTime.y;
		ParticleDesc.bIsLoop = Particle.bIsLoop;
		ParticleDesc.bIsReverseXY_Mask = Particle.bIsReverseXY_Mask;
		ParticleDesc.iUVScaleXCur = Particle.iUVScaleXCur;
		ParticleDesc.iUVScaleYCur = Particle.iUVScaleYCur;
		ParticleDesc.iUVScaleXMax = Particle.iUVScaleXMax;
		ParticleDesc.iUVScaleYMax = Particle.iUVScaleYMax;
		ParticleDesc.vStartDiffuseColorRGBA_Min.x = Particle.vStartDiffuseColorRGBA_Min.x;
		ParticleDesc.vStartDiffuseColorRGBA_Min.y = Particle.vStartDiffuseColorRGBA_Min.y;
		ParticleDesc.vStartDiffuseColorRGBA_Min.z = Particle.vStartDiffuseColorRGBA_Min.z;
		ParticleDesc.vStartDiffuseColorRGBA_Min.w = Particle.vStartDiffuseColorRGBA_Min.w;
		ParticleDesc.vStartDiffuseColorRGBA_Max.x = Particle.vStartDiffuseColorRGBA_Max.x;
		ParticleDesc.vStartDiffuseColorRGBA_Max.y = Particle.vStartDiffuseColorRGBA_Max.y;
		ParticleDesc.vStartDiffuseColorRGBA_Max.z = Particle.vStartDiffuseColorRGBA_Max.z;
		ParticleDesc.vStartDiffuseColorRGBA_Max.w = Particle.vStartDiffuseColorRGBA_Max.w;
		ParticleDesc.iDiffuseColorEasing_End = Particle.iDiffuseColorEasing_End;
		ParticleDesc.iDiffuseColorEasing_Middle = Particle.iDiffuseColorEasing_Middle;
		ParticleDesc.fMiddleDiffuseColorTimePoint = Particle.fMiddleDiffuseColorTimePoint;
		ParticleDesc.vMiddleDiffuseColorRGBA.x = Particle.vMiddleDiffuseColorRGBA.x;
		ParticleDesc.vMiddleDiffuseColorRGBA.y = Particle.vMiddleDiffuseColorRGBA.y;
		ParticleDesc.vMiddleDiffuseColorRGBA.z = Particle.vMiddleDiffuseColorRGBA.z;
		ParticleDesc.vMiddleDiffuseColorRGBA.w = Particle.vMiddleDiffuseColorRGBA.w;
		ParticleDesc.vEndDiffuseColorRGBA.x = Particle.vEndDiffuseColorRGBA.x;
		ParticleDesc.vEndDiffuseColorRGBA.y = Particle.vEndDiffuseColorRGBA.y;
		ParticleDesc.vEndDiffuseColorRGBA.z = Particle.vEndDiffuseColorRGBA.z;
		ParticleDesc.vEndDiffuseColorRGBA.w = Particle.vEndDiffuseColorRGBA.w;

		ParticleDesc.bIsGlowFollowDiffuseColor = Particle.bIsGlowFollowDiffuseColor;
		ParticleDesc.vStartGlowColorRGBA_Min.x = Particle.vStartGlowColorRGBA_Min.x;
		ParticleDesc.vStartGlowColorRGBA_Min.y = Particle.vStartGlowColorRGBA_Min.y;
		ParticleDesc.vStartGlowColorRGBA_Min.z = Particle.vStartGlowColorRGBA_Min.z;
		ParticleDesc.vStartGlowColorRGBA_Min.w = Particle.vStartGlowColorRGBA_Min.w;
		ParticleDesc.vStartGlowColorRGBA_Max.x = Particle.vStartGlowColorRGBA_Max.x;
		ParticleDesc.vStartGlowColorRGBA_Max.y = Particle.vStartGlowColorRGBA_Max.y;
		ParticleDesc.vStartGlowColorRGBA_Max.z = Particle.vStartGlowColorRGBA_Max.z;
		ParticleDesc.vStartGlowColorRGBA_Max.w = Particle.vStartGlowColorRGBA_Max.w;
		ParticleDesc.iGlowColorEasing_End = Particle.iGlowColorEasing_End;
		ParticleDesc.iGlowColorEasing_Middle = Particle.iGlowColorEasing_Middle;
		ParticleDesc.fMiddleGlowColorTimePoint = Particle.fMiddleGlowColorTimePoint;
		ParticleDesc.vMiddleGlowColorRGBA.x = Particle.vMiddleGlowColorRGBA.x;
		ParticleDesc.vMiddleGlowColorRGBA.y = Particle.vMiddleGlowColorRGBA.y;
		ParticleDesc.vMiddleGlowColorRGBA.z = Particle.vMiddleGlowColorRGBA.z;
		ParticleDesc.vMiddleGlowColorRGBA.w = Particle.vMiddleGlowColorRGBA.w;
		ParticleDesc.vEndGlowColorRGBA.x = Particle.vEndGlowColorRGBA.x;
		ParticleDesc.vEndGlowColorRGBA.y = Particle.vEndGlowColorRGBA.y;
		ParticleDesc.vEndGlowColorRGBA.z = Particle.vEndGlowColorRGBA.z;
		ParticleDesc.vEndGlowColorRGBA.w = Particle.vEndGlowColorRGBA.w;
		ParticleDesc.bIsGlow = Particle.bIsGlow;
		ParticleDesc.bIsBlur = Particle.bIsBlur;
		ParticleDesc.vStartScaleXYZ_Min.x = Particle.vStartScaleXYZ_Min.x;
		ParticleDesc.vStartScaleXYZ_Min.y = Particle.vStartScaleXYZ_Min.y;
		ParticleDesc.vStartScaleXYZ_Min.z = Particle.vStartScaleXYZ_Min.z;
		ParticleDesc.vStartScaleXYZ_Max.x = Particle.vStartScaleXYZ_Max.x;
		ParticleDesc.vStartScaleXYZ_Max.y = Particle.vStartScaleXYZ_Max.y;
		ParticleDesc.vStartScaleXYZ_Max.z = Particle.vStartScaleXYZ_Max.z;
		ParticleDesc.iScaleEasing_End = Particle.iScaleEasing_End;
		ParticleDesc.iScaleEasing_Middle = Particle.iScaleEasing_Middle;
		ParticleDesc.fMiddleScaleTimePoint = Particle.fMiddleScaleTimePoint;
		ParticleDesc.vMiddleScaleXYZ.x = Particle.vMiddleScaleXYZ.x;
		ParticleDesc.vMiddleScaleXYZ.y = Particle.vMiddleScaleXYZ.y;
		ParticleDesc.vMiddleScaleXYZ.z = Particle.vMiddleScaleXYZ.z;
		ParticleDesc.vEndScaleXYZ.x = Particle.vEndScaleXYZ.x;
		ParticleDesc.vEndScaleXYZ.y = Particle.vEndScaleXYZ.y;
		ParticleDesc.vEndScaleXYZ.z = Particle.vEndScaleXYZ.z;
		ParticleDesc.vPositionOffset.x = Particle.vPositionOffset.x;
		ParticleDesc.vPositionOffset.y = Particle.vPositionOffset.y;
		ParticleDesc.vPositionOffset.z = Particle.vPositionOffset.z;
		ParticleDesc.iMode = Particle.iMode;
		ParticleDesc.vRangeXYZ_Min.x = Particle.vRangeXYZ_Min.x;
		ParticleDesc.vRangeXYZ_Min.y = Particle.vRangeXYZ_Min.y;
		ParticleDesc.vRangeXYZ_Min.z = Particle.vRangeXYZ_Min.z;
		ParticleDesc.vRangeXYZ_Max.x = Particle.vRangeXYZ_Max.x;
		ParticleDesc.vRangeXYZ_Max.y = Particle.vRangeXYZ_Max.y;
		ParticleDesc.vRangeXYZ_Max.z = Particle.vRangeXYZ_Max.z;
		ParticleDesc.bIsSpeedEasingMode = Particle.bIsSpeedEasingMode;
		ParticleDesc.vStartSpeedXYZ_Min.x = Particle.vStartSpeedXYZ_Min.x;
		ParticleDesc.vStartSpeedXYZ_Min.y = Particle.vStartSpeedXYZ_Min.y;
		ParticleDesc.vStartSpeedXYZ_Min.z = Particle.vStartSpeedXYZ_Min.z;
		ParticleDesc.vStartSpeedXYZ_Max.x = Particle.vStartSpeedXYZ_Max.x;
		ParticleDesc.vStartSpeedXYZ_Max.y = Particle.vStartSpeedXYZ_Max.y;
		ParticleDesc.vStartSpeedXYZ_Max.z = Particle.vStartSpeedXYZ_Max.z;
		ParticleDesc.iSpeedEasing_End = Particle.iSpeedEasing_End;
		ParticleDesc.iSpeedEasing_Middle = Particle.iSpeedEasing_Middle;
		ParticleDesc.fMiddleSpeedTimePoint = Particle.fMiddleSpeedTimePoint;
		ParticleDesc.vMiddleSpeedXYZ.x = Particle.vMiddleSpeedXYZ.x;
		ParticleDesc.vMiddleSpeedXYZ.y = Particle.vMiddleSpeedXYZ.y;
		ParticleDesc.vMiddleSpeedXYZ.z = Particle.vMiddleSpeedXYZ.z;
		ParticleDesc.vEndSpeedXYZ.x = Particle.vEndSpeedXYZ.x;
		ParticleDesc.vEndSpeedXYZ.y = Particle.vEndSpeedXYZ.y;
		ParticleDesc.vEndSpeedXYZ.z = Particle.vEndSpeedXYZ.z;
		ParticleDesc.fEachAccelation_MinMax.x = Particle.fEachAccelation_MinMax.x;
		ParticleDesc.fEachAccelation_MinMax.y = Particle.fEachAccelation_MinMax.y;
		ParticleDesc.fWholeAccelation.x = Particle.fWholeAccelation.x;
		ParticleDesc.fWholeAccelation.y = Particle.fWholeAccelation.y;
		ParticleDesc.fWholeAccelation.z = Particle.fWholeAccelation.z;
		ParticleDesc.vRotateRUL_Min.x = Particle.vRotateRUL_Min.x;
		ParticleDesc.vRotateRUL_Min.y = Particle.vRotateRUL_Min.y;
		ParticleDesc.vRotateRUL_Min.z = Particle.vRotateRUL_Min.z;
		ParticleDesc.vRotateRUL_Max.x = Particle.vRotateRUL_Max.x;
		ParticleDesc.vRotateRUL_Max.y = Particle.vRotateRUL_Max.y;
		ParticleDesc.vRotateRUL_Max.z = Particle.vRotateRUL_Max.z;
		ParticleDesc.fRotateAccelation_Min.x = Particle.fRotateAccelation_Min.x;
		ParticleDesc.fRotateAccelation_Min.y = Particle.fRotateAccelation_Min.y;
		ParticleDesc.fRotateAccelation_Min.z = Particle.fRotateAccelation_Min.z;
		ParticleDesc.fRotateAccelation_Max.x = Particle.fRotateAccelation_Max.x;
		ParticleDesc.fRotateAccelation_Max.y = Particle.fRotateAccelation_Max.y;
		ParticleDesc.fRotateAccelation_Max.z = Particle.fRotateAccelation_Max.z;

		//XMStoreFloat3(&ParticleDesc.vPositionOffset, XMLoadFloat3(&Particle.vPositionOffset) + matTransform.r[3]);
		NULLCHECK(pGameInstance->Add_GameObject(pGameInstance->Get_CurLevelIndex(), ID_UINT(EFFECT, EFFECT_PARTICLE_RECT), ID_UINT(EFFECT, EFFECT_PARTICLE_RECT), &ParticleDesc));
		CEffect_Rect* pLastParticle = static_cast<CEffect_Rect*>(pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ID_UINT(EFFECT, EFFECT_PARTICLE_RECT))->back());
		if (nullptr != ParentObject)
			pLastParticle->Set_FollowingMode(matTransform, fTimeSpeed, ParentObject, eFollowMode, pBoneName);
		else
			pLastParticle->Set_PositioningMode(matTransform, fTimeSpeed);

	}

	for (auto& MeshEffect : m_EffectDesc.MeshEffectDesc)
	{
		MESHEFFECT_DESC MeshEffectDesc;

		MeshEffectDesc.iMeshIndex = MeshEffect.iMeshIndex;
		MeshEffectDesc.iNoiseIndex = MeshEffect.iNoiseIndex;
		MeshEffectDesc.iMaskIndex = MeshEffect.iMaskIndex;
		MeshEffectDesc.bIsBillBoard = MeshEffect.bIsBillBoard;
		MeshEffectDesc.fLifeTime.x = MeshEffect.fLifeTime.x;
		MeshEffectDesc.fLifeTime.y = MeshEffect.fLifeTime.y;
		MeshEffectDesc.bIsLoop = MeshEffect.bIsLoop;
		MeshEffectDesc.bIsClampSampling_Noise = MeshEffect.bIsClampSampling_Noise;
		MeshEffectDesc.bIsReverseXY_Noise = MeshEffect.bIsReverseXY_Noise;
		MeshEffectDesc.fUVSpeedXY_Noise.x = MeshEffect.fUVSpeedXY_Noise.x;
		MeshEffectDesc.fUVSpeedXY_Noise.y = MeshEffect.fUVSpeedXY_Noise.y;
		MeshEffectDesc.iUVScaleXMax_Noise = MeshEffect.iUVScaleXMax_Noise;
		MeshEffectDesc.iUVScaleYMax_Noise = MeshEffect.iUVScaleYMax_Noise;
		MeshEffectDesc.bIsUVSpeedMode = MeshEffect.bIsUVSpeedMode;
		MeshEffectDesc.bIsClampSampling_Mask = MeshEffect.bIsClampSampling_Mask;
		MeshEffectDesc.bIsReverseXY_Mask = MeshEffect.bIsReverseXY_Mask;
		MeshEffectDesc.fUVSpeedXY_Mask.x = MeshEffect.fUVSpeedXY_Mask.x;
		MeshEffectDesc.fUVSpeedXY_Mask.y = MeshEffect.fUVSpeedXY_Mask.y;
		MeshEffectDesc.iUVScaleXMax_Mask = MeshEffect.iUVScaleXMax_Mask;
		MeshEffectDesc.iUVScaleYMax_Mask = MeshEffect.iUVScaleYMax_Mask;
		MeshEffectDesc.fSpriteSpeed = MeshEffect.fSpriteSpeed;
		MeshEffectDesc.vStartDiffuseColorRGBA.x = MeshEffect.vStartDiffuseColorRGBA.x;
		MeshEffectDesc.vStartDiffuseColorRGBA.y = MeshEffect.vStartDiffuseColorRGBA.y;
		MeshEffectDesc.vStartDiffuseColorRGBA.z = MeshEffect.vStartDiffuseColorRGBA.z;
		MeshEffectDesc.vStartDiffuseColorRGBA.w = MeshEffect.vStartDiffuseColorRGBA.w;
		MeshEffectDesc.iDiffuseColorEasing_End = MeshEffect.iDiffuseColorEasing_End;
		MeshEffectDesc.iDiffuseColorEasing_Middle = MeshEffect.iDiffuseColorEasing_Middle;
		MeshEffectDesc.fMiddleDiffuseColorTimePoint = MeshEffect.fMiddleDiffuseColorTimePoint;
		MeshEffectDesc.vMiddleDiffuseColorRGBA.x = MeshEffect.vMiddleDiffuseColorRGBA.x;
		MeshEffectDesc.vMiddleDiffuseColorRGBA.y = MeshEffect.vMiddleDiffuseColorRGBA.y;
		MeshEffectDesc.vMiddleDiffuseColorRGBA.z = MeshEffect.vMiddleDiffuseColorRGBA.z;
		MeshEffectDesc.vMiddleDiffuseColorRGBA.w = MeshEffect.vMiddleDiffuseColorRGBA.w;
		MeshEffectDesc.vEndDiffuseColorRGBA.x = MeshEffect.vEndDiffuseColorRGBA.x;
		MeshEffectDesc.vEndDiffuseColorRGBA.y = MeshEffect.vEndDiffuseColorRGBA.y;
		MeshEffectDesc.vEndDiffuseColorRGBA.z = MeshEffect.vEndDiffuseColorRGBA.z;
		MeshEffectDesc.vEndDiffuseColorRGBA.w = MeshEffect.vEndDiffuseColorRGBA.w;
		MeshEffectDesc.bIsGlowFollowDiffuseColor = MeshEffect.bIsGlowFollowDiffuseColor;
		MeshEffectDesc.vStartGlowColorRGBA.x = MeshEffect.vStartGlowColorRGBA.x;
		MeshEffectDesc.vStartGlowColorRGBA.y = MeshEffect.vStartGlowColorRGBA.y;
		MeshEffectDesc.vStartGlowColorRGBA.z = MeshEffect.vStartGlowColorRGBA.z;
		MeshEffectDesc.vStartGlowColorRGBA.w = MeshEffect.vStartGlowColorRGBA.w;
		MeshEffectDesc.iGlowColorEasing_End = MeshEffect.iGlowColorEasing_End;
		MeshEffectDesc.iGlowColorEasing_Middle = MeshEffect.iGlowColorEasing_Middle;
		MeshEffectDesc.fMiddleGlowColorTimePoint = MeshEffect.fMiddleGlowColorTimePoint;
		MeshEffectDesc.vMiddleGlowColorRGBA.x = MeshEffect.vMiddleGlowColorRGBA.x;
		MeshEffectDesc.vMiddleGlowColorRGBA.y = MeshEffect.vMiddleGlowColorRGBA.y;
		MeshEffectDesc.vMiddleGlowColorRGBA.z = MeshEffect.vMiddleGlowColorRGBA.z;
		MeshEffectDesc.vMiddleGlowColorRGBA.w = MeshEffect.vMiddleGlowColorRGBA.w;
		MeshEffectDesc.vEndGlowColorRGBA.x = MeshEffect.vEndGlowColorRGBA.x;
		MeshEffectDesc.vEndGlowColorRGBA.y = MeshEffect.vEndGlowColorRGBA.y;
		MeshEffectDesc.vEndGlowColorRGBA.z = MeshEffect.vEndGlowColorRGBA.z;
		MeshEffectDesc.vEndGlowColorRGBA.w = MeshEffect.vEndGlowColorRGBA.w;
		MeshEffectDesc.bIsGlow = MeshEffect.bIsGlow;
		MeshEffectDesc.bIsBlur = MeshEffect.bIsBlur;
		MeshEffectDesc.vPositionOffset.x = MeshEffect.vPositionOffset.x;
		MeshEffectDesc.vPositionOffset.y = MeshEffect.vPositionOffset.y;
		MeshEffectDesc.vPositionOffset.z = MeshEffect.vPositionOffset.z;
		MeshEffectDesc.bIsSpeedEasingMode = MeshEffect.bIsSpeedEasingMode;
		MeshEffectDesc.vStartSpeedXYZ.x = MeshEffect.vStartSpeedXYZ.x;
		MeshEffectDesc.vStartSpeedXYZ.y = MeshEffect.vStartSpeedXYZ.y;
		MeshEffectDesc.vStartSpeedXYZ.z = MeshEffect.vStartSpeedXYZ.z;
		MeshEffectDesc.iSpeedEasing_End = MeshEffect.iSpeedEasing_End;
		MeshEffectDesc.iSpeedEasing_Middle = MeshEffect.iSpeedEasing_Middle;
		MeshEffectDesc.fMiddleSpeedTimePoint = MeshEffect.fMiddleSpeedTimePoint;
		MeshEffectDesc.vMiddleSpeedXYZ.x = MeshEffect.vMiddleSpeedXYZ.x;
		MeshEffectDesc.vMiddleSpeedXYZ.y = MeshEffect.vMiddleSpeedXYZ.y;
		MeshEffectDesc.vMiddleSpeedXYZ.z = MeshEffect.vMiddleSpeedXYZ.z;
		MeshEffectDesc.vEndSpeedXYZ.x = MeshEffect.vEndSpeedXYZ.x;
		MeshEffectDesc.vEndSpeedXYZ.y = MeshEffect.vEndSpeedXYZ.y;
		MeshEffectDesc.vEndSpeedXYZ.z = MeshEffect.vEndSpeedXYZ.z;
		MeshEffectDesc.fSpeedAccelation = MeshEffect.fSpeedAccelation;
		MeshEffectDesc.vStartScaleXYZ.x = MeshEffect.vStartScaleXYZ.x;
		MeshEffectDesc.vStartScaleXYZ.y = MeshEffect.vStartScaleXYZ.y;
		MeshEffectDesc.vStartScaleXYZ.z = MeshEffect.vStartScaleXYZ.z;
		MeshEffectDesc.iScaleEasing_End = MeshEffect.iScaleEasing_End;
		MeshEffectDesc.iScaleEasing_Middle = MeshEffect.iScaleEasing_Middle;
		MeshEffectDesc.fMiddleScaleTimePoint = MeshEffect.fMiddleScaleTimePoint;
		MeshEffectDesc.vMiddleScaleXYZ.x = MeshEffect.vMiddleScaleXYZ.x;
		MeshEffectDesc.vMiddleScaleXYZ.y = MeshEffect.vMiddleScaleXYZ.y;
		MeshEffectDesc.vMiddleScaleXYZ.z = MeshEffect.vMiddleScaleXYZ.z;
		MeshEffectDesc.vEndScaleXYZ.x = MeshEffect.vEndScaleXYZ.x;
		MeshEffectDesc.vEndScaleXYZ.y = MeshEffect.vEndScaleXYZ.y;
		MeshEffectDesc.vEndScaleXYZ.z = MeshEffect.vEndScaleXYZ.z;
		MeshEffectDesc.vRotationRUL.x = MeshEffect.vRotationRUL.x;
		MeshEffectDesc.vRotationRUL.y = MeshEffect.vRotationRUL.y;
		MeshEffectDesc.vRotationRUL.z = MeshEffect.vRotationRUL.z;
		MeshEffectDesc.vRotationSpeedRUL.x = MeshEffect.vRotationSpeedRUL.x;
		MeshEffectDesc.vRotationSpeedRUL.y = MeshEffect.vRotationSpeedRUL.y;
		MeshEffectDesc.vRotationSpeedRUL.z = MeshEffect.vRotationSpeedRUL.z;
		MeshEffectDesc.fRotatationAccelation.x = MeshEffect.fRotatationAccelation.x;
		MeshEffectDesc.fRotatationAccelation.y = MeshEffect.fRotatationAccelation.y;
		MeshEffectDesc.fRotatationAccelation.z = MeshEffect.fRotatationAccelation.z;

		NULLCHECK(pGameInstance->Add_GameObject(pGameInstance->Get_CurLevelIndex(), ID_UINT(EFFECT, EFFECT_MESH), ID_UINT(EFFECT, EFFECT_MESH), &MeshEffectDesc));
		CEffect_Mesh* pLastMeshEffect = static_cast<CEffect_Mesh*>(pGameInstance->Get_Objects(pGameInstance->Get_CurLevelIndex(), ID_UINT(EFFECT, EFFECT_MESH))->back());
		if (nullptr != ParentObject)
			pLastMeshEffect->Set_FollowingMode(matTransform, fTimeSpeed, ParentObject, eFollowMode, pBoneName);
		else
			pLastMeshEffect->Set_PositioningMode(matTransform, fTimeSpeed);
	}

#endif
	return S_OK;
}

CEffect* CEffect::Create(const EFFECT_DESC& EffectDesc)
{
	CEffect* pInstance = new CEffect();

	if (FAILED(pInstance->Initialize(EffectDesc)))
	{
		MSG_BOX("Failed to Created : CEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect::Free()
{
	__super::Free();
}
