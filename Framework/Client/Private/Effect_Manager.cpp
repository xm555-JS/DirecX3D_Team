#include "stdafx.h"
#include "Effect_Manager.h"
#include "Effect_Factory.h"
#include "Effect_Base.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CEffect_Manager)

// ????? Tool ??????? ???? ????? ??????? ???.
// ???????? ????? ?????????.
// Update : ??? ????.
#define UPDATE

CEffect_Manager::CEffect_Manager()
{
}

HRESULT CEffect_Manager::Load_EffectsData(string strFileName, EFFECTS* pEffects)
{
	USEGAMEINSTANCE;
	json jsonEffectData;
	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(towstring("Effect/" + strFileName).c_str()).c_str(), &jsonEffectData);

	//json ?¬Ò?
	for (auto& Effect : jsonEffectData)
	{
		string Effect_Name;
		vector<MESHEFFECT_DESC> Effect_Mesh;
		vector<PARTICLE_DESC> Effect_Particle;

		Effect_Name = Effect["Effect_Name"];
		for (auto& EffectCom : Effect["MeshEffects"])
		{
			MESHEFFECT_DESC MeshEffectDesc;

			MeshEffectDesc.iMeshIndex = EffectCom["iMeshIndex"];
			MeshEffectDesc.iNoiseIndex = EffectCom["iNoiseIndex"];
			MeshEffectDesc.iMaskIndex = EffectCom["iMaskIndex"];
			MeshEffectDesc.bIsBillBoard = EffectCom["bIsBillBoard"];
			MeshEffectDesc.fLifeTime.x = EffectCom["fLifeTime.x"];
			MeshEffectDesc.fLifeTime.y = EffectCom["fLifeTime.y"];
			MeshEffectDesc.bIsLoop = EffectCom["bIsLoop"];
			MeshEffectDesc.bIsClampSampling_Noise = EffectCom["bIsClampSampling_Noise"];
			MeshEffectDesc.bIsReverseXY_Noise = EffectCom["bIsReverseXY_Noise"];
			MeshEffectDesc.fUVSpeedXY_Noise.x = EffectCom["fUVSpeedXY_Noise.x"];
			MeshEffectDesc.fUVSpeedXY_Noise.y = EffectCom["fUVSpeedXY_Noise.y"];
			MeshEffectDesc.iUVScaleXMax_Noise = EffectCom["iUVScaleXMax_Noise"];
			MeshEffectDesc.iUVScaleYMax_Noise = EffectCom["iUVScaleYMax_Noise"];
			MeshEffectDesc.bIsUVSpeedMode = EffectCom["bIsUVSpeedMode"];
			MeshEffectDesc.bIsClampSampling_Mask = EffectCom["bIsClampSampling_Mask"];
			MeshEffectDesc.bIsReverseXY_Mask = EffectCom["bIsReverseXY_Mask"];
			MeshEffectDesc.fUVSpeedXY_Mask.x = EffectCom["fUVSpeedXY_Mask.x"];
			MeshEffectDesc.fUVSpeedXY_Mask.y = EffectCom["fUVSpeedXY_Mask.y"];
			MeshEffectDesc.iUVScaleXMax_Mask = EffectCom["iUVScaleXMax_Mask"];
			MeshEffectDesc.iUVScaleYMax_Mask = EffectCom["iUVScaleYMax_Mask"];
			MeshEffectDesc.fSpriteSpeed = EffectCom["fSpriteSpeed"];
			MeshEffectDesc.vStartDiffuseColorRGBA.x = EffectCom["vStartDiffuseColorRGBA.x"];
			MeshEffectDesc.vStartDiffuseColorRGBA.y = EffectCom["vStartDiffuseColorRGBA.y"];
			MeshEffectDesc.vStartDiffuseColorRGBA.z = EffectCom["vStartDiffuseColorRGBA.z"];
			MeshEffectDesc.vStartDiffuseColorRGBA.w = EffectCom["vStartDiffuseColorRGBA.w"];
			MeshEffectDesc.iDiffuseColorEasing_End = EffectCom["iDiffuseColorEasing_End"];
			MeshEffectDesc.iDiffuseColorEasing_Middle = EffectCom["iDiffuseColorEasing_Middle"];
			MeshEffectDesc.fMiddleDiffuseColorTimePoint = EffectCom["fMiddleDiffuseColorTimePoint"];
			MeshEffectDesc.vMiddleDiffuseColorRGBA.x = EffectCom["vMiddleDiffuseColorRGBA.x"];
			MeshEffectDesc.vMiddleDiffuseColorRGBA.y = EffectCom["vMiddleDiffuseColorRGBA.y"];
			MeshEffectDesc.vMiddleDiffuseColorRGBA.z = EffectCom["vMiddleDiffuseColorRGBA.z"];
			MeshEffectDesc.vMiddleDiffuseColorRGBA.w = EffectCom["vMiddleDiffuseColorRGBA.w"];
			MeshEffectDesc.vEndDiffuseColorRGBA.x = EffectCom["vEndDiffuseColorRGBA.x"];
			MeshEffectDesc.vEndDiffuseColorRGBA.y = EffectCom["vEndDiffuseColorRGBA.y"];
			MeshEffectDesc.vEndDiffuseColorRGBA.z = EffectCom["vEndDiffuseColorRGBA.z"];
			MeshEffectDesc.vEndDiffuseColorRGBA.w = EffectCom["vEndDiffuseColorRGBA.w"];
			MeshEffectDesc.bIsGlow = EffectCom["bIsGlow"];
			MeshEffectDesc.bIsGlowFollowDiffuseColor = EffectCom["bIsGlowFollowDiffuseColor"];
			MeshEffectDesc.vStartGlowColorRGBA.x = EffectCom["vStartGlowColorRGBA.x"];
			MeshEffectDesc.vStartGlowColorRGBA.y = EffectCom["vStartGlowColorRGBA.y"];
			MeshEffectDesc.vStartGlowColorRGBA.z = EffectCom["vStartGlowColorRGBA.z"];
			MeshEffectDesc.vStartGlowColorRGBA.w = EffectCom["vStartGlowColorRGBA.w"];
			MeshEffectDesc.iGlowColorEasing_End = EffectCom["iGlowColorEasing_End"];
			MeshEffectDesc.iGlowColorEasing_Middle = EffectCom["iGlowColorEasing_Middle"];
			MeshEffectDesc.fMiddleGlowColorTimePoint = EffectCom["fMiddleGlowColorTimePoint"];
			MeshEffectDesc.vMiddleGlowColorRGBA.x = EffectCom["vMiddleGlowColorRGBA.x"];
			MeshEffectDesc.vMiddleGlowColorRGBA.y = EffectCom["vMiddleGlowColorRGBA.y"];
			MeshEffectDesc.vMiddleGlowColorRGBA.z = EffectCom["vMiddleGlowColorRGBA.z"];
			MeshEffectDesc.vMiddleGlowColorRGBA.w = EffectCom["vMiddleGlowColorRGBA.w"];
			MeshEffectDesc.vEndGlowColorRGBA.x = EffectCom["vEndGlowColorRGBA.x"];
			MeshEffectDesc.vEndGlowColorRGBA.y = EffectCom["vEndGlowColorRGBA.y"];
			MeshEffectDesc.vEndGlowColorRGBA.z = EffectCom["vEndGlowColorRGBA.z"];
			MeshEffectDesc.vEndGlowColorRGBA.w = EffectCom["vEndGlowColorRGBA.w"];
#ifdef UPDATE
			MeshEffectDesc.bIsSoftEffect = EffectCom["bIsSoftEffect"];
			MeshEffectDesc.bIsDistortion = EffectCom["bIsDistortion"];
			MeshEffectDesc.fStartDistortionStrength = EffectCom["fStartDistortionStrength"];
			MeshEffectDesc.iDistortionStrengthEasing_End = EffectCom["iDistortionStrengthEasing_End"];
			MeshEffectDesc.iDistortionStrengthEasing_Middle = EffectCom["iDistortionStrengthEasing_Middle"];
			MeshEffectDesc.fMiddleDistortionStrengthTimePoint = EffectCom["fMiddleDistortionStrengthTimePoint"];
			MeshEffectDesc.fMiddleDistortionStrength = EffectCom["fMiddleDistortionStrength"];
			MeshEffectDesc.fEndDistortionStrength = EffectCom["fEndDistortionStrength"];
			MeshEffectDesc.bIsRimLight = EffectCom["bIsRimLight"];
			MeshEffectDesc.vStartRimLightColorRGBA.x = EffectCom["vStartRimLightColorRGBA.x"];
			MeshEffectDesc.vStartRimLightColorRGBA.y = EffectCom["vStartRimLightColorRGBA.y"];
			MeshEffectDesc.vStartRimLightColorRGBA.z = EffectCom["vStartRimLightColorRGBA.z"];
			MeshEffectDesc.vStartRimLightColorRGBA.w = EffectCom["vStartRimLightColorRGBA.w"];
			MeshEffectDesc.iRimLightColorEasing_End = EffectCom["iRimLightColorEasing_End"];
			MeshEffectDesc.iRimLightColorEasing_Middle = EffectCom["iRimLightColorEasing_Middle"];
			MeshEffectDesc.fMiddleRimLightColorTimePoint = EffectCom["fMiddleRimLightColorTimePoint"];
			MeshEffectDesc.vMiddleRimLightColorRGBA.x = EffectCom["vMiddleRimLightColorRGBA.x"];
			MeshEffectDesc.vMiddleRimLightColorRGBA.y = EffectCom["vMiddleRimLightColorRGBA.y"];
			MeshEffectDesc.vMiddleRimLightColorRGBA.z = EffectCom["vMiddleRimLightColorRGBA.z"];
			MeshEffectDesc.vMiddleRimLightColorRGBA.w = EffectCom["vMiddleRimLightColorRGBA.w"];
			MeshEffectDesc.vEndRimLightColorRGBA.x = EffectCom["vEndRimLightColorRGBA.x"];
			MeshEffectDesc.vEndRimLightColorRGBA.y = EffectCom["vEndRimLightColorRGBA.y"];
			MeshEffectDesc.vEndRimLightColorRGBA.z = EffectCom["vEndRimLightColorRGBA.z"];
			MeshEffectDesc.vEndRimLightColorRGBA.w = EffectCom["vEndRimLightColorRGBA.w"];
			MeshEffectDesc.bIsDissolve = EffectCom["bIsDissolve"];
			MeshEffectDesc.iDissolveNoiseTextureIndex = EffectCom["iDissolveNoiseTextureIndex"];
			MeshEffectDesc.iDissolveColorTextureIndex = EffectCom["iDissolveColorTextureIndex"];
			MeshEffectDesc.vDissolveColorRGBA.x = EffectCom["vDissolveColorRGBA.x"];
			MeshEffectDesc.vDissolveColorRGBA.y = EffectCom["vDissolveColorRGBA.y"];
			MeshEffectDesc.vDissolveColorRGBA.z = EffectCom["vDissolveColorRGBA.z"];
			MeshEffectDesc.vDissolveColorRGBA.w = EffectCom["vDissolveColorRGBA.w"];
			MeshEffectDesc.fDissolveStrength = EffectCom["fDissolveStrength"];
			MeshEffectDesc.bIsTrail = EffectCom["bIsTrail"];
			MeshEffectDesc.vTrailColor.x = EffectCom["vTrailColor.x"];
			MeshEffectDesc.vTrailColor.y = EffectCom["vTrailColor.y"];
			MeshEffectDesc.vTrailColor.z = EffectCom["vTrailColor.z"];
			MeshEffectDesc.vTrailColor.w = EffectCom["vTrailColor.w"];
			MeshEffectDesc.fTrailStrengh = EffectCom["fTrailStrengh"];
#endif // 0

			MeshEffectDesc.bIsBlur = EffectCom["bIsBlur"];
			MeshEffectDesc.vPositionOffset.x = EffectCom["vPositionOffset.x"];
			MeshEffectDesc.vPositionOffset.y = EffectCom["vPositionOffset.y"];
			MeshEffectDesc.vPositionOffset.z = EffectCom["vPositionOffset.z"];
			MeshEffectDesc.bIsSpeedEasingMode = EffectCom["bIsSpeedEasingMode"];
			MeshEffectDesc.vStartSpeedXYZ.x = EffectCom["vStartSpeedXYZ.x"];
			MeshEffectDesc.vStartSpeedXYZ.y = EffectCom["vStartSpeedXYZ.y"];
			MeshEffectDesc.vStartSpeedXYZ.z = EffectCom["vStartSpeedXYZ.z"];
			MeshEffectDesc.iSpeedEasing_End = EffectCom["iSpeedEasing_End"];
			MeshEffectDesc.iSpeedEasing_Middle = EffectCom["iSpeedEasing_Middle"];
			MeshEffectDesc.fMiddleSpeedTimePoint = EffectCom["fMiddleSpeedTimePoint"];
			MeshEffectDesc.vMiddleSpeedXYZ.x = EffectCom["vMiddleSpeedXYZ.x"];
			MeshEffectDesc.vMiddleSpeedXYZ.y = EffectCom["vMiddleSpeedXYZ.y"];
			MeshEffectDesc.vMiddleSpeedXYZ.z = EffectCom["vMiddleSpeedXYZ.z"];
			MeshEffectDesc.vEndSpeedXYZ.x = EffectCom["vEndSpeedXYZ.x"];
			MeshEffectDesc.vEndSpeedXYZ.y = EffectCom["vEndSpeedXYZ.y"];
			MeshEffectDesc.vEndSpeedXYZ.z = EffectCom["vEndSpeedXYZ.z"];
			MeshEffectDesc.fSpeedAccelation.x = EffectCom["fSpeedAccelation.x"];
			MeshEffectDesc.fSpeedAccelation.y = EffectCom["fSpeedAccelation.y"];
			MeshEffectDesc.fSpeedAccelation.z = EffectCom["fSpeedAccelation.z"];
			MeshEffectDesc.vStartScaleXYZ.x = EffectCom["vStartScaleXYZ.x"];
			MeshEffectDesc.vStartScaleXYZ.y = EffectCom["vStartScaleXYZ.y"];
			MeshEffectDesc.vStartScaleXYZ.z = EffectCom["vStartScaleXYZ.z"];
			MeshEffectDesc.iScaleEasing_End = EffectCom["iScaleEasing_End"];
			MeshEffectDesc.iScaleEasing_Middle = EffectCom["iScaleEasing_Middle"];
			MeshEffectDesc.fMiddleScaleTimePoint = EffectCom["fMiddleScaleTimePoint"];
			MeshEffectDesc.vMiddleScaleXYZ.x = EffectCom["vMiddleScaleXYZ.x"];
			MeshEffectDesc.vMiddleScaleXYZ.y = EffectCom["vMiddleScaleXYZ.y"];
			MeshEffectDesc.vMiddleScaleXYZ.z = EffectCom["vMiddleScaleXYZ.z"];
			MeshEffectDesc.vEndScaleXYZ.x = EffectCom["vEndScaleXYZ.x"];
			MeshEffectDesc.vEndScaleXYZ.y = EffectCom["vEndScaleXYZ.y"];
			MeshEffectDesc.vEndScaleXYZ.z = EffectCom["vEndScaleXYZ.z"];
			MeshEffectDesc.vRotationRUL.x = EffectCom["vRotationRUL.x"];
			MeshEffectDesc.vRotationRUL.y = EffectCom["vRotationRUL.y"];
			MeshEffectDesc.vRotationRUL.z = EffectCom["vRotationRUL.z"];
			MeshEffectDesc.vRotationSpeedRUL.x = EffectCom["vRotationSpeedRUL.x"];
			MeshEffectDesc.vRotationSpeedRUL.y = EffectCom["vRotationSpeedRUL.y"];
			MeshEffectDesc.vRotationSpeedRUL.z = EffectCom["vRotationSpeedRUL.z"];
			MeshEffectDesc.fRotatationAccelation.x = EffectCom["fRotatationAccelation.x"];
			MeshEffectDesc.fRotatationAccelation.y = EffectCom["fRotatationAccelation.y"];
			MeshEffectDesc.fRotatationAccelation.z = EffectCom["fRotatationAccelation.z"];

			Effect_Mesh.push_back(MeshEffectDesc);
		}

		for (auto& EffectCom : Effect["Particles"])
		{
			PARTICLE_DESC ParticleDesc;

			ParticleDesc.iMaskIndex = EffectCom["iMaskIndex"];
			ParticleDesc.iNumInstance = EffectCom["iNumInstance"];
#ifdef UPDATE
			ParticleDesc.iBillBoardMode = EffectCom["iBillBoardMode"];
#else
			_bool bTempBool;
			bTempBool = EffectCom["bIsBillBoard"];
			if (bTempBool) ParticleDesc.iBillBoardMode = 2; //Directional
			else ParticleDesc.iBillBoardMode = 0;// Normal (Rotate)
#endif // 0

			ParticleDesc.fLifeTime.x = EffectCom["fLifeTime.x"];
			ParticleDesc.fLifeTime.y = EffectCom["fLifeTime.y"];
			ParticleDesc.bIsLoop = EffectCom["bIsLoop"];
			ParticleDesc.bIsReverseXY_Mask = EffectCom["bIsReverseXY_Mask"];
			ParticleDesc.iUVScaleXCur = EffectCom["iUVScaleXCur"];
			ParticleDesc.iUVScaleYCur = EffectCom["iUVScaleYCur"];
			ParticleDesc.iUVScaleXMax = EffectCom["iUVScaleXMax_Mask"];
			ParticleDesc.iUVScaleYMax = EffectCom["iUVScaleYMax_Mask"];

			ParticleDesc.vStartDiffuseColorRGBA_Min.x = EffectCom["vStartDiffuseColorRGBA_Min.x"];
			ParticleDesc.vStartDiffuseColorRGBA_Min.y = EffectCom["vStartDiffuseColorRGBA_Min.y"];
			ParticleDesc.vStartDiffuseColorRGBA_Min.z = EffectCom["vStartDiffuseColorRGBA_Min.z"];
			ParticleDesc.vStartDiffuseColorRGBA_Min.w = EffectCom["vStartDiffuseColorRGBA_Min.w"];
			ParticleDesc.vStartDiffuseColorRGBA_Max.x = EffectCom["vStartDiffuseColorRGBA_Max.x"];
			ParticleDesc.vStartDiffuseColorRGBA_Max.y = EffectCom["vStartDiffuseColorRGBA_Max.y"];
			ParticleDesc.vStartDiffuseColorRGBA_Max.z = EffectCom["vStartDiffuseColorRGBA_Max.z"];
			ParticleDesc.vStartDiffuseColorRGBA_Max.w = EffectCom["vStartDiffuseColorRGBA_Max.w"];
			ParticleDesc.iDiffuseColorEasing_End = EffectCom["iDiffuseColorEasing_End"];
			ParticleDesc.iDiffuseColorEasing_Middle = EffectCom["iDiffuseColorEasing_Middle"];
			ParticleDesc.fMiddleDiffuseColorTimePoint = EffectCom["fMiddleDiffuseColorTimePoint"];
			ParticleDesc.vMiddleDiffuseColorRGBA.x = EffectCom["vMiddleDiffuseColorRGBA.x"];
			ParticleDesc.vMiddleDiffuseColorRGBA.y = EffectCom["vMiddleDiffuseColorRGBA.y"];
			ParticleDesc.vMiddleDiffuseColorRGBA.z = EffectCom["vMiddleDiffuseColorRGBA.z"];
			ParticleDesc.vMiddleDiffuseColorRGBA.w = EffectCom["vMiddleDiffuseColorRGBA.w"];
			ParticleDesc.vEndDiffuseColorRGBA.x = EffectCom["vEndDiffuseColorRGBA.x"];
			ParticleDesc.vEndDiffuseColorRGBA.y = EffectCom["vEndDiffuseColorRGBA.y"];
			ParticleDesc.vEndDiffuseColorRGBA.z = EffectCom["vEndDiffuseColorRGBA.z"];
			ParticleDesc.vEndDiffuseColorRGBA.w = EffectCom["vEndDiffuseColorRGBA.w"];

			ParticleDesc.bIsGlowFollowDiffuseColor = EffectCom["bIsGlowFollowDiffuseColor"];
			ParticleDesc.vStartGlowColorRGBA_Min.x = EffectCom["vStartGlowColorRGBA_Min.x"];
			ParticleDesc.vStartGlowColorRGBA_Min.y = EffectCom["vStartGlowColorRGBA_Min.y"];
			ParticleDesc.vStartGlowColorRGBA_Min.z = EffectCom["vStartGlowColorRGBA_Min.z"];
			ParticleDesc.vStartGlowColorRGBA_Min.w = EffectCom["vStartGlowColorRGBA_Min.w"];
			ParticleDesc.vStartGlowColorRGBA_Max.x = EffectCom["vStartGlowColorRGBA_Max.x"];
			ParticleDesc.vStartGlowColorRGBA_Max.y = EffectCom["vStartGlowColorRGBA_Max.y"];
			ParticleDesc.vStartGlowColorRGBA_Max.z = EffectCom["vStartGlowColorRGBA_Max.z"];
			ParticleDesc.vStartGlowColorRGBA_Max.w = EffectCom["vStartGlowColorRGBA_Max.w"];
			ParticleDesc.iGlowColorEasing_End = EffectCom["iGlowColorEasing_End"];
			ParticleDesc.iGlowColorEasing_Middle = EffectCom["iGlowColorEasing_Middle"];
			ParticleDesc.fMiddleGlowColorTimePoint = EffectCom["fMiddleGlowColorTimePoint"];
			ParticleDesc.vMiddleGlowColorRGBA.x = EffectCom["vMiddleGlowColorRGBA.x"];
			ParticleDesc.vMiddleGlowColorRGBA.y = EffectCom["vMiddleGlowColorRGBA.y"];
			ParticleDesc.vMiddleGlowColorRGBA.z = EffectCom["vMiddleGlowColorRGBA.z"];
			ParticleDesc.vMiddleGlowColorRGBA.w = EffectCom["vMiddleGlowColorRGBA.w"];
			ParticleDesc.vEndGlowColorRGBA.x = EffectCom["vEndGlowColorRGBA.x"];
			ParticleDesc.vEndGlowColorRGBA.y = EffectCom["vEndGlowColorRGBA.y"];
			ParticleDesc.vEndGlowColorRGBA.z = EffectCom["vEndGlowColorRGBA.z"];
			ParticleDesc.vEndGlowColorRGBA.w = EffectCom["vEndGlowColorRGBA.w"];

			ParticleDesc.bIsGlow = EffectCom["bIsGlow"];
			ParticleDesc.bIsBlur = EffectCom["bIsBlur"];
			ParticleDesc.vStartScaleXYZ_Min.x = EffectCom["vStartScaleXYZ_Min.x"];
			ParticleDesc.vStartScaleXYZ_Min.y = EffectCom["vStartScaleXYZ_Min.y"];
			ParticleDesc.vStartScaleXYZ_Min.z = EffectCom["vStartScaleXYZ_Min.z"];
			ParticleDesc.vStartScaleXYZ_Max.x = EffectCom["vStartScaleXYZ_Max.x"];
			ParticleDesc.vStartScaleXYZ_Max.y = EffectCom["vStartScaleXYZ_Max.y"];
			ParticleDesc.vStartScaleXYZ_Max.z = EffectCom["vStartScaleXYZ_Max.z"];
			ParticleDesc.iScaleEasing_End = EffectCom["iScaleEasing_End"];
			ParticleDesc.iScaleEasing_Middle = EffectCom["iScaleEasing_Middle"];
			ParticleDesc.fMiddleScaleTimePoint = EffectCom["fMiddleScaleTimePoint"];
			ParticleDesc.vMiddleScaleXYZ.x = EffectCom["vMiddleScaleXYZ.x"];
			ParticleDesc.vMiddleScaleXYZ.y = EffectCom["vMiddleScaleXYZ.y"];
			ParticleDesc.vMiddleScaleXYZ.z = EffectCom["vMiddleScaleXYZ.z"];
			ParticleDesc.vEndScaleXYZ.x = EffectCom["vEndScaleXYZ.x"];
			ParticleDesc.vEndScaleXYZ.y = EffectCom["vEndScaleXYZ.y"];
			ParticleDesc.vEndScaleXYZ.z = EffectCom["vEndScaleXYZ.z"];
			ParticleDesc.vPositionOffset.x = EffectCom["vPositionOffset.x"];
			ParticleDesc.vPositionOffset.y = EffectCom["vPositionOffset.y"];
			ParticleDesc.vPositionOffset.z = EffectCom["vPositionOffset.z"];
#ifdef UPDATE
			ParticleDesc.iStartPointMode = EffectCom["iStartPointMode"];
#else
			ParticleDesc.iStartPointMode = EffectCom["iMode"];	// 2: Sphere ?? ???? ????.
#endif // 0

			ParticleDesc.vRangeXYZ_Min.x = EffectCom["vRangeXYZ_Min.x"];
			ParticleDesc.vRangeXYZ_Min.y = EffectCom["vRangeXYZ_Min.y"];
			ParticleDesc.vRangeXYZ_Min.z = EffectCom["vRangeXYZ_Min.z"];
			ParticleDesc.vRangeXYZ_Max.x = EffectCom["vRangeXYZ_Max.x"];
			ParticleDesc.vRangeXYZ_Max.y = EffectCom["vRangeXYZ_Max.y"];
			ParticleDesc.vRangeXYZ_Max.z = EffectCom["vRangeXYZ_Max.z"];
#ifdef UPDATE
			ParticleDesc.fRangeRadius = EffectCom["fRangeRadius"];
#else
			ParticleDesc.fRangeRadius = 0.f;
#endif // 0

#ifdef UPDATE
			ParticleDesc.iMovingMode = EffectCom["iMovingMode"];
#else
			ParticleDesc.iMovingMode = EffectCom["bIsSpeedEasingMode"];
#endif // UPDATE

			ParticleDesc.vStartSpeedXYZ_Min.x = EffectCom["vStartSpeedXYZ_Min.x"];
			ParticleDesc.vStartSpeedXYZ_Min.y = EffectCom["vStartSpeedXYZ_Min.y"];
			ParticleDesc.vStartSpeedXYZ_Min.z = EffectCom["vStartSpeedXYZ_Min.z"];
			ParticleDesc.vStartSpeedXYZ_Max.x = EffectCom["vStartSpeedXYZ_Max.x"];
			ParticleDesc.vStartSpeedXYZ_Max.y = EffectCom["vStartSpeedXYZ_Max.y"];
			ParticleDesc.vStartSpeedXYZ_Max.z = EffectCom["vStartSpeedXYZ_Max.z"];
			ParticleDesc.iSpeedEasing_End = EffectCom["iSpeedEasing_End"];
			ParticleDesc.iSpeedEasing_Middle = EffectCom["iSpeedEasing_Middle"];
			ParticleDesc.fMiddleSpeedTimePoint = EffectCom["fMiddleSpeedTimePoint"];
			ParticleDesc.vMiddleSpeedXYZ.x = EffectCom["vMiddleSpeedXYZ.x"];
			ParticleDesc.vMiddleSpeedXYZ.y = EffectCom["vMiddleSpeedXYZ.y"];
			ParticleDesc.vMiddleSpeedXYZ.z = EffectCom["vMiddleSpeedXYZ.z"];
			ParticleDesc.vEndSpeedXYZ.x = EffectCom["vEndSpeedXYZ.x"];
			ParticleDesc.vEndSpeedXYZ.y = EffectCom["vEndSpeedXYZ.y"];
			ParticleDesc.vEndSpeedXYZ.z = EffectCom["vEndSpeedXYZ.z"];
#if 0
			ParticleDesc.vTargetPosition.x = EffectCom["vTargetPosition.x"];
			ParticleDesc.vTargetPosition.y = EffectCom["vTargetPosition.y"];
			ParticleDesc.vTargetPosition.z = EffectCom["vTargetPosition.z"];
			ParticleDesc.bIsGatheringOnce = EffectCom["bIsGatheringOnce"];
			ParticleDesc.iPositioningEasing_End = EffectCom["iPositioningEasing_End"];
			ParticleDesc.iPositioningEasing_Middle = EffectCom["iPositioningEasing_Middle"];
			ParticleDesc.fMiddlePositioningTimePoint = EffectCom["fMiddlePositioningTimePoint"];
#endif // 0

			ParticleDesc.fEachAccelation_MinMax.x = EffectCom["fEachAccelation_MinMax.x"];
			ParticleDesc.fEachAccelation_MinMax.y = EffectCom["fEachAccelation_MinMax.y"];
			ParticleDesc.fWholeAccelation.x = EffectCom["fWholeAccelation.x"];
			ParticleDesc.fWholeAccelation.y = EffectCom["fWholeAccelation.y"];
			ParticleDesc.fWholeAccelation.z = EffectCom["fWholeAccelation.z"];
			ParticleDesc.vRotateRUL_Min.x = EffectCom["vRotateRUL_Min.x"];
			ParticleDesc.vRotateRUL_Min.y = EffectCom["vRotateRUL_Min.y"];
			ParticleDesc.vRotateRUL_Min.z = EffectCom["vRotateRUL_Min.z"];
			ParticleDesc.vRotateRUL_Max.x = EffectCom["vRotateRUL_Max.x"];
			ParticleDesc.vRotateRUL_Max.y = EffectCom["vRotateRUL_Max.y"];
			ParticleDesc.vRotateRUL_Max.z = EffectCom["vRotateRUL_Max.z"];
			ParticleDesc.fRotateAccelation_Min.x = EffectCom["fRotateAccelation_Min.x"];
			ParticleDesc.fRotateAccelation_Min.y = EffectCom["fRotateAccelation_Min.y"];
			ParticleDesc.fRotateAccelation_Min.z = EffectCom["fRotateAccelation_Min.z"];
			ParticleDesc.fRotateAccelation_Max.x = EffectCom["fRotateAccelation_Max.x"];
			ParticleDesc.fRotateAccelation_Max.y = EffectCom["fRotateAccelation_Max.y"];
			ParticleDesc.fRotateAccelation_Max.z = EffectCom["fRotateAccelation_Max.z"];

			Effect_Particle.push_back(ParticleDesc);
		}

		// ??????? ???.
		if (nullptr != Find_Effect(Effect_Name, pEffects))
		{
			string str = "Effect Name: \"" + Effect_Name + "\"" + "\nis Overlapped!!";
			wstring str_w;
			str_w.assign(str.begin(), str.end());
			MessageBox(0, str_w.c_str(), TEXT("### Warning !! ###"), MB_OK);

			//return E_FAIL;
			continue;
		}
		//if (nullptr != Find_Effect(Effect_Desc.Effect_Name, pEffects))
			//continue;

		pEffects->emplace(Effect_Name, CEffect_Factory::Create(Effect_Name, Effect_Particle, Effect_Mesh));
	}

	return S_OK;
}

HRESULT CEffect_Manager::Save_EffectsData(string strFileName, EFFECTS* pEffects)
{
	json Save;
	for (auto& Effect : *pEffects)
	{
		string Effect_Name = Effect.second->Get_EffectDesc_EffectName();
		vector<MESHEFFECT_DESC> stMeshEffectDesc = Effect.second->Get_EffectDesc_Mesh();
		vector<PARTICLE_DESC> stParticleDesc = Effect.second->Get_EffectDesc_Particle();

		//json EffectType;
		json EffectType, MeshEffects, Particles;
		EffectType.emplace("Effect_Name", Effect_Name);

		// MeshEffect
		for (auto& MeshEffectDesc : stMeshEffectDesc)
		{
			json dataM;

			dataM.emplace("iMeshIndex", MeshEffectDesc.iMeshIndex);
			dataM.emplace("iNoiseIndex", MeshEffectDesc.iNoiseIndex);
			dataM.emplace("iMaskIndex", MeshEffectDesc.iMaskIndex);
			dataM.emplace("bIsBillBoard", MeshEffectDesc.bIsBillBoard);
			dataM.emplace("fLifeTime.x", MeshEffectDesc.fLifeTime.x);
			dataM.emplace("fLifeTime.y", MeshEffectDesc.fLifeTime.y);
			dataM.emplace("bIsLoop", MeshEffectDesc.bIsLoop);
			dataM.emplace("bIsReverseXY_Noise", MeshEffectDesc.bIsReverseXY_Noise);
			dataM.emplace("bIsClampSampling_Noise", MeshEffectDesc.bIsClampSampling_Noise);
			dataM.emplace("fUVSpeedXY_Noise.x", MeshEffectDesc.fUVSpeedXY_Noise.x);
			dataM.emplace("fUVSpeedXY_Noise.y", MeshEffectDesc.fUVSpeedXY_Noise.y);
			dataM.emplace("iUVScaleXMax_Noise", MeshEffectDesc.iUVScaleXMax_Noise);
			dataM.emplace("iUVScaleYMax_Noise", MeshEffectDesc.iUVScaleYMax_Noise);
			dataM.emplace("bIsUVSpeedMode", MeshEffectDesc.bIsUVSpeedMode);
			dataM.emplace("bIsClampSampling_Mask", MeshEffectDesc.bIsClampSampling_Mask);
			dataM.emplace("bIsReverseXY_Mask", MeshEffectDesc.bIsReverseXY_Mask);
			dataM.emplace("fUVSpeedXY_Mask.x", MeshEffectDesc.fUVSpeedXY_Mask.x);
			dataM.emplace("fUVSpeedXY_Mask.y", MeshEffectDesc.fUVSpeedXY_Mask.y);
			dataM.emplace("iUVScaleXMax_Mask", MeshEffectDesc.iUVScaleXMax_Mask);
			dataM.emplace("iUVScaleYMax_Mask", MeshEffectDesc.iUVScaleYMax_Mask);
			dataM.emplace("fSpriteSpeed", MeshEffectDesc.fSpriteSpeed);
			dataM.emplace("vStartDiffuseColorRGBA.x", MeshEffectDesc.vStartDiffuseColorRGBA.x);
			dataM.emplace("vStartDiffuseColorRGBA.y", MeshEffectDesc.vStartDiffuseColorRGBA.y);
			dataM.emplace("vStartDiffuseColorRGBA.z", MeshEffectDesc.vStartDiffuseColorRGBA.z);
			dataM.emplace("vStartDiffuseColorRGBA.w", MeshEffectDesc.vStartDiffuseColorRGBA.w);
			dataM.emplace("iDiffuseColorEasing_End", MeshEffectDesc.iDiffuseColorEasing_End);
			dataM.emplace("iDiffuseColorEasing_Middle", MeshEffectDesc.iDiffuseColorEasing_Middle);
			dataM.emplace("fMiddleDiffuseColorTimePoint", MeshEffectDesc.fMiddleDiffuseColorTimePoint);
			dataM.emplace("vMiddleDiffuseColorRGBA.x", MeshEffectDesc.vMiddleDiffuseColorRGBA.x);
			dataM.emplace("vMiddleDiffuseColorRGBA.y", MeshEffectDesc.vMiddleDiffuseColorRGBA.y);
			dataM.emplace("vMiddleDiffuseColorRGBA.z", MeshEffectDesc.vMiddleDiffuseColorRGBA.z);
			dataM.emplace("vMiddleDiffuseColorRGBA.w", MeshEffectDesc.vMiddleDiffuseColorRGBA.w);
			dataM.emplace("vEndDiffuseColorRGBA.x", MeshEffectDesc.vEndDiffuseColorRGBA.x);
			dataM.emplace("vEndDiffuseColorRGBA.y", MeshEffectDesc.vEndDiffuseColorRGBA.y);
			dataM.emplace("vEndDiffuseColorRGBA.z", MeshEffectDesc.vEndDiffuseColorRGBA.z);
			dataM.emplace("vEndDiffuseColorRGBA.w", MeshEffectDesc.vEndDiffuseColorRGBA.w);
			dataM.emplace("bIsGlow", MeshEffectDesc.bIsGlow);
			dataM.emplace("bIsGlowFollowDiffuseColor", MeshEffectDesc.bIsGlowFollowDiffuseColor);
			dataM.emplace("vStartGlowColorRGBA.x", MeshEffectDesc.vStartGlowColorRGBA.x);
			dataM.emplace("vStartGlowColorRGBA.y", MeshEffectDesc.vStartGlowColorRGBA.y);
			dataM.emplace("vStartGlowColorRGBA.z", MeshEffectDesc.vStartGlowColorRGBA.z);
			dataM.emplace("vStartGlowColorRGBA.w", MeshEffectDesc.vStartGlowColorRGBA.w);
			dataM.emplace("iGlowColorEasing_End", MeshEffectDesc.iGlowColorEasing_End);
			dataM.emplace("iGlowColorEasing_Middle", MeshEffectDesc.iGlowColorEasing_Middle);
			dataM.emplace("fMiddleGlowColorTimePoint", MeshEffectDesc.fMiddleGlowColorTimePoint);
			dataM.emplace("vMiddleGlowColorRGBA.x", MeshEffectDesc.vMiddleGlowColorRGBA.x);
			dataM.emplace("vMiddleGlowColorRGBA.y", MeshEffectDesc.vMiddleGlowColorRGBA.y);
			dataM.emplace("vMiddleGlowColorRGBA.z", MeshEffectDesc.vMiddleGlowColorRGBA.z);
			dataM.emplace("vMiddleGlowColorRGBA.w", MeshEffectDesc.vMiddleGlowColorRGBA.w);
			dataM.emplace("vEndGlowColorRGBA.x", MeshEffectDesc.vEndGlowColorRGBA.x);
			dataM.emplace("vEndGlowColorRGBA.y", MeshEffectDesc.vEndGlowColorRGBA.y);
			dataM.emplace("vEndGlowColorRGBA.z", MeshEffectDesc.vEndGlowColorRGBA.z);
			dataM.emplace("vEndGlowColorRGBA.w", MeshEffectDesc.vEndGlowColorRGBA.w);
			dataM.emplace("bIsSoftEffect", MeshEffectDesc.bIsSoftEffect);
			dataM.emplace("bIsDistortion", MeshEffectDesc.bIsDistortion);
			dataM.emplace("fStartDistortionStrength", MeshEffectDesc.fStartDistortionStrength);
			dataM.emplace("iDistortionStrengthEasing_End", MeshEffectDesc.iDistortionStrengthEasing_End);
			dataM.emplace("iDistortionStrengthEasing_Middle", MeshEffectDesc.iDistortionStrengthEasing_Middle);
			dataM.emplace("fMiddleDistortionStrengthTimePoint", MeshEffectDesc.fMiddleDistortionStrengthTimePoint);
			dataM.emplace("fMiddleDistortionStrength", MeshEffectDesc.fMiddleDistortionStrength);
			dataM.emplace("fEndDistortionStrength", MeshEffectDesc.fEndDistortionStrength);
			dataM.emplace("bIsRimLight", MeshEffectDesc.bIsRimLight);
			dataM.emplace("vStartRimLightColorRGBA.x", MeshEffectDesc.vStartRimLightColorRGBA.x);
			dataM.emplace("vStartRimLightColorRGBA.y", MeshEffectDesc.vStartRimLightColorRGBA.y);
			dataM.emplace("vStartRimLightColorRGBA.z", MeshEffectDesc.vStartRimLightColorRGBA.z);
			dataM.emplace("vStartRimLightColorRGBA.w", MeshEffectDesc.vStartRimLightColorRGBA.w);
			dataM.emplace("iRimLightColorEasing_End", MeshEffectDesc.iRimLightColorEasing_End);
			dataM.emplace("iRimLightColorEasing_Middle", MeshEffectDesc.iRimLightColorEasing_Middle);
			dataM.emplace("fMiddleRimLightColorTimePoint", MeshEffectDesc.fMiddleRimLightColorTimePoint);
			dataM.emplace("vMiddleRimLightColorRGBA.x", MeshEffectDesc.vMiddleRimLightColorRGBA.x);
			dataM.emplace("vMiddleRimLightColorRGBA.y", MeshEffectDesc.vMiddleRimLightColorRGBA.y);
			dataM.emplace("vMiddleRimLightColorRGBA.z", MeshEffectDesc.vMiddleRimLightColorRGBA.z);
			dataM.emplace("vMiddleRimLightColorRGBA.w", MeshEffectDesc.vMiddleRimLightColorRGBA.w);
			dataM.emplace("vEndRimLightColorRGBA.x", MeshEffectDesc.vEndRimLightColorRGBA.x);
			dataM.emplace("vEndRimLightColorRGBA.y", MeshEffectDesc.vEndRimLightColorRGBA.y);
			dataM.emplace("vEndRimLightColorRGBA.z", MeshEffectDesc.vEndRimLightColorRGBA.z);
			dataM.emplace("vEndRimLightColorRGBA.w", MeshEffectDesc.vEndRimLightColorRGBA.w);
			dataM.emplace("bIsDissolve", MeshEffectDesc.bIsDissolve);
			dataM.emplace("iDissolveNoiseTextureIndex", MeshEffectDesc.iDissolveNoiseTextureIndex);
			dataM.emplace("iDissolveColorTextureIndex", MeshEffectDesc.iDissolveColorTextureIndex);
			dataM.emplace("vDissolveColorRGBA.x", MeshEffectDesc.vDissolveColorRGBA.x);
			dataM.emplace("vDissolveColorRGBA.y", MeshEffectDesc.vDissolveColorRGBA.y);
			dataM.emplace("vDissolveColorRGBA.z", MeshEffectDesc.vDissolveColorRGBA.z);
			dataM.emplace("vDissolveColorRGBA.w", MeshEffectDesc.vDissolveColorRGBA.w);
			dataM.emplace("fDissolveStrength", MeshEffectDesc.fDissolveStrength);
			dataM.emplace("bIsTrail", MeshEffectDesc.bIsTrail);
			dataM.emplace("vTrailColor.x", MeshEffectDesc.vTrailColor.x);
			dataM.emplace("vTrailColor.y", MeshEffectDesc.vTrailColor.y);
			dataM.emplace("vTrailColor.z", MeshEffectDesc.vTrailColor.z);
			dataM.emplace("vTrailColor.w", MeshEffectDesc.vTrailColor.w);
			dataM.emplace("fTrailStrengh", MeshEffectDesc.fTrailStrengh);
			dataM.emplace("bIsBlur", MeshEffectDesc.bIsBlur);
			dataM.emplace("vPositionOffset.x", MeshEffectDesc.vPositionOffset.x);
			dataM.emplace("vPositionOffset.y", MeshEffectDesc.vPositionOffset.y);
			dataM.emplace("vPositionOffset.z", MeshEffectDesc.vPositionOffset.z);
			dataM.emplace("bIsSpeedEasingMode", MeshEffectDesc.bIsSpeedEasingMode);
			dataM.emplace("vStartSpeedXYZ.x", MeshEffectDesc.vStartSpeedXYZ.x);
			dataM.emplace("vStartSpeedXYZ.y", MeshEffectDesc.vStartSpeedXYZ.y);
			dataM.emplace("vStartSpeedXYZ.z", MeshEffectDesc.vStartSpeedXYZ.z);
			dataM.emplace("iSpeedEasing_End", MeshEffectDesc.iSpeedEasing_End);
			dataM.emplace("iSpeedEasing_Middle", MeshEffectDesc.iSpeedEasing_Middle);
			dataM.emplace("fMiddleSpeedTimePoint", MeshEffectDesc.fMiddleSpeedTimePoint);
			dataM.emplace("vMiddleSpeedXYZ.x", MeshEffectDesc.vMiddleSpeedXYZ.x);
			dataM.emplace("vMiddleSpeedXYZ.y", MeshEffectDesc.vMiddleSpeedXYZ.y);
			dataM.emplace("vMiddleSpeedXYZ.z", MeshEffectDesc.vMiddleSpeedXYZ.z);
			dataM.emplace("vEndSpeedXYZ.x", MeshEffectDesc.vEndSpeedXYZ.x);
			dataM.emplace("vEndSpeedXYZ.y", MeshEffectDesc.vEndSpeedXYZ.y);
			dataM.emplace("vEndSpeedXYZ.z", MeshEffectDesc.vEndSpeedXYZ.z);
			dataM.emplace("fSpeedAccelation.x", MeshEffectDesc.fSpeedAccelation.x);
			dataM.emplace("fSpeedAccelation.y", MeshEffectDesc.fSpeedAccelation.y);
			dataM.emplace("fSpeedAccelation.z", MeshEffectDesc.fSpeedAccelation.z);
			dataM.emplace("vStartScaleXYZ.x", MeshEffectDesc.vStartScaleXYZ.x);
			dataM.emplace("vStartScaleXYZ.y", MeshEffectDesc.vStartScaleXYZ.y);
			dataM.emplace("vStartScaleXYZ.z", MeshEffectDesc.vStartScaleXYZ.z);
			dataM.emplace("iScaleEasing_End", MeshEffectDesc.iScaleEasing_End);
			dataM.emplace("iScaleEasing_Middle", MeshEffectDesc.iScaleEasing_Middle);
			dataM.emplace("fMiddleScaleTimePoint", MeshEffectDesc.fMiddleScaleTimePoint);
			dataM.emplace("vMiddleScaleXYZ.x", MeshEffectDesc.vMiddleScaleXYZ.x);
			dataM.emplace("vMiddleScaleXYZ.y", MeshEffectDesc.vMiddleScaleXYZ.y);
			dataM.emplace("vMiddleScaleXYZ.z", MeshEffectDesc.vMiddleScaleXYZ.z);
			dataM.emplace("vEndScaleXYZ.x", MeshEffectDesc.vEndScaleXYZ.x);
			dataM.emplace("vEndScaleXYZ.y", MeshEffectDesc.vEndScaleXYZ.y);
			dataM.emplace("vEndScaleXYZ.z", MeshEffectDesc.vEndScaleXYZ.z);
			dataM.emplace("vRotationRUL.x", MeshEffectDesc.vRotationRUL.x);
			dataM.emplace("vRotationRUL.y", MeshEffectDesc.vRotationRUL.y);
			dataM.emplace("vRotationRUL.z", MeshEffectDesc.vRotationRUL.z);
			dataM.emplace("vRotationSpeedRUL.x", MeshEffectDesc.vRotationSpeedRUL.x);
			dataM.emplace("vRotationSpeedRUL.y", MeshEffectDesc.vRotationSpeedRUL.y);
			dataM.emplace("vRotationSpeedRUL.z", MeshEffectDesc.vRotationSpeedRUL.z);
			dataM.emplace("fRotatationAccelation.x", MeshEffectDesc.fRotatationAccelation.x);
			dataM.emplace("fRotatationAccelation.y", MeshEffectDesc.fRotatationAccelation.y);
			dataM.emplace("fRotatationAccelation.z", MeshEffectDesc.fRotatationAccelation.z);

			MeshEffects.push_back(dataM);
		}
		EffectType.emplace("MeshEffects", MeshEffects);

		// Particles
		for (auto& ParticleDesc : stParticleDesc)
		{
			json dataP;

			dataP.emplace("iMaskIndex", ParticleDesc.iMaskIndex);
			dataP.emplace("iNumInstance", ParticleDesc.iNumInstance);
			dataP.emplace("iBillBoardMode", ParticleDesc.iBillBoardMode);
			dataP.emplace("fLifeTime.x", ParticleDesc.fLifeTime.x);
			dataP.emplace("fLifeTime.y", ParticleDesc.fLifeTime.y);
			dataP.emplace("bIsLoop", ParticleDesc.bIsLoop);
			dataP.emplace("bIsReverseXY_Mask", ParticleDesc.bIsReverseXY_Mask);
			dataP.emplace("iUVScaleXCur", ParticleDesc.iUVScaleXCur);
			dataP.emplace("iUVScaleYCur", ParticleDesc.iUVScaleYCur);
			dataP.emplace("iUVScaleXMax_Mask", ParticleDesc.iUVScaleXMax);
			dataP.emplace("iUVScaleYMax_Mask", ParticleDesc.iUVScaleYMax);

			dataP.emplace("vStartDiffuseColorRGBA_Min.x", ParticleDesc.vStartDiffuseColorRGBA_Min.x);
			dataP.emplace("vStartDiffuseColorRGBA_Min.y", ParticleDesc.vStartDiffuseColorRGBA_Min.y);
			dataP.emplace("vStartDiffuseColorRGBA_Min.z", ParticleDesc.vStartDiffuseColorRGBA_Min.z);
			dataP.emplace("vStartDiffuseColorRGBA_Min.w", ParticleDesc.vStartDiffuseColorRGBA_Min.w);
			dataP.emplace("vStartDiffuseColorRGBA_Max.x", ParticleDesc.vStartDiffuseColorRGBA_Max.x);
			dataP.emplace("vStartDiffuseColorRGBA_Max.y", ParticleDesc.vStartDiffuseColorRGBA_Max.y);
			dataP.emplace("vStartDiffuseColorRGBA_Max.z", ParticleDesc.vStartDiffuseColorRGBA_Max.z);
			dataP.emplace("vStartDiffuseColorRGBA_Max.w", ParticleDesc.vStartDiffuseColorRGBA_Max.w);
			dataP.emplace("iDiffuseColorEasing_End", ParticleDesc.iDiffuseColorEasing_End);
			dataP.emplace("iDiffuseColorEasing_Middle", ParticleDesc.iDiffuseColorEasing_Middle);
			dataP.emplace("fMiddleDiffuseColorTimePoint", ParticleDesc.fMiddleDiffuseColorTimePoint);
			dataP.emplace("vMiddleDiffuseColorRGBA.x", ParticleDesc.vMiddleDiffuseColorRGBA.x);
			dataP.emplace("vMiddleDiffuseColorRGBA.y", ParticleDesc.vMiddleDiffuseColorRGBA.y);
			dataP.emplace("vMiddleDiffuseColorRGBA.z", ParticleDesc.vMiddleDiffuseColorRGBA.z);
			dataP.emplace("vMiddleDiffuseColorRGBA.w", ParticleDesc.vMiddleDiffuseColorRGBA.w);
			dataP.emplace("vEndDiffuseColorRGBA.x", ParticleDesc.vEndDiffuseColorRGBA.x);
			dataP.emplace("vEndDiffuseColorRGBA.y", ParticleDesc.vEndDiffuseColorRGBA.y);
			dataP.emplace("vEndDiffuseColorRGBA.z", ParticleDesc.vEndDiffuseColorRGBA.z);
			dataP.emplace("vEndDiffuseColorRGBA.w", ParticleDesc.vEndDiffuseColorRGBA.w);

			dataP.emplace("bIsGlowFollowDiffuseColor", ParticleDesc.bIsGlowFollowDiffuseColor);
			dataP.emplace("vStartGlowColorRGBA_Min.x", ParticleDesc.vStartGlowColorRGBA_Min.x);
			dataP.emplace("vStartGlowColorRGBA_Min.y", ParticleDesc.vStartGlowColorRGBA_Min.y);
			dataP.emplace("vStartGlowColorRGBA_Min.z", ParticleDesc.vStartGlowColorRGBA_Min.z);
			dataP.emplace("vStartGlowColorRGBA_Min.w", ParticleDesc.vStartGlowColorRGBA_Min.w);
			dataP.emplace("vStartGlowColorRGBA_Max.x", ParticleDesc.vStartGlowColorRGBA_Max.x);
			dataP.emplace("vStartGlowColorRGBA_Max.y", ParticleDesc.vStartGlowColorRGBA_Max.y);
			dataP.emplace("vStartGlowColorRGBA_Max.z", ParticleDesc.vStartGlowColorRGBA_Max.z);
			dataP.emplace("vStartGlowColorRGBA_Max.w", ParticleDesc.vStartGlowColorRGBA_Max.w);
			dataP.emplace("iGlowColorEasing_End", ParticleDesc.iGlowColorEasing_End);
			dataP.emplace("iGlowColorEasing_Middle", ParticleDesc.iGlowColorEasing_Middle);
			dataP.emplace("fMiddleGlowColorTimePoint", ParticleDesc.fMiddleGlowColorTimePoint);
			dataP.emplace("vMiddleGlowColorRGBA.x", ParticleDesc.vMiddleGlowColorRGBA.x);
			dataP.emplace("vMiddleGlowColorRGBA.y", ParticleDesc.vMiddleGlowColorRGBA.y);
			dataP.emplace("vMiddleGlowColorRGBA.z", ParticleDesc.vMiddleGlowColorRGBA.z);
			dataP.emplace("vMiddleGlowColorRGBA.w", ParticleDesc.vMiddleGlowColorRGBA.w);
			dataP.emplace("vEndGlowColorRGBA.x", ParticleDesc.vEndGlowColorRGBA.x);
			dataP.emplace("vEndGlowColorRGBA.y", ParticleDesc.vEndGlowColorRGBA.y);
			dataP.emplace("vEndGlowColorRGBA.z", ParticleDesc.vEndGlowColorRGBA.z);
			dataP.emplace("vEndGlowColorRGBA.w", ParticleDesc.vEndGlowColorRGBA.w);

			dataP.emplace("bIsGlow", ParticleDesc.bIsGlow);
			dataP.emplace("bIsBlur", ParticleDesc.bIsBlur);
			dataP.emplace("vStartScaleXYZ_Min.x", ParticleDesc.vStartScaleXYZ_Min.x);
			dataP.emplace("vStartScaleXYZ_Min.y", ParticleDesc.vStartScaleXYZ_Min.y);
			dataP.emplace("vStartScaleXYZ_Min.z", ParticleDesc.vStartScaleXYZ_Min.z);
			dataP.emplace("vStartScaleXYZ_Max.x", ParticleDesc.vStartScaleXYZ_Max.x);
			dataP.emplace("vStartScaleXYZ_Max.y", ParticleDesc.vStartScaleXYZ_Max.y);
			dataP.emplace("vStartScaleXYZ_Max.z", ParticleDesc.vStartScaleXYZ_Max.z);
			dataP.emplace("iScaleEasing_End", ParticleDesc.iScaleEasing_End);
			dataP.emplace("iScaleEasing_Middle", ParticleDesc.iScaleEasing_Middle);
			dataP.emplace("fMiddleScaleTimePoint", ParticleDesc.fMiddleScaleTimePoint);
			dataP.emplace("vMiddleScaleXYZ.x", ParticleDesc.vMiddleScaleXYZ.x);
			dataP.emplace("vMiddleScaleXYZ.y", ParticleDesc.vMiddleScaleXYZ.y);
			dataP.emplace("vMiddleScaleXYZ.z", ParticleDesc.vMiddleScaleXYZ.z);
			dataP.emplace("vEndScaleXYZ.x", ParticleDesc.vEndScaleXYZ.x);
			dataP.emplace("vEndScaleXYZ.y", ParticleDesc.vEndScaleXYZ.y);
			dataP.emplace("vEndScaleXYZ.z", ParticleDesc.vEndScaleXYZ.z);
			dataP.emplace("vPositionOffset.x", ParticleDesc.vPositionOffset.x);
			dataP.emplace("vPositionOffset.y", ParticleDesc.vPositionOffset.y);
			dataP.emplace("vPositionOffset.z", ParticleDesc.vPositionOffset.z);
			dataP.emplace("iStartPointMode", ParticleDesc.iStartPointMode);
			dataP.emplace("vRangeXYZ_Min.x", ParticleDesc.vRangeXYZ_Min.x);
			dataP.emplace("vRangeXYZ_Min.y", ParticleDesc.vRangeXYZ_Min.y);
			dataP.emplace("vRangeXYZ_Min.z", ParticleDesc.vRangeXYZ_Min.z);
			dataP.emplace("vRangeXYZ_Max.x", ParticleDesc.vRangeXYZ_Max.x);
			dataP.emplace("vRangeXYZ_Max.y", ParticleDesc.vRangeXYZ_Max.y);
			dataP.emplace("vRangeXYZ_Max.z", ParticleDesc.vRangeXYZ_Max.z);
			dataP.emplace("fRangeRadius", ParticleDesc.fRangeRadius);
			dataP.emplace("vStartSpeedXYZ_Min.x", ParticleDesc.vStartSpeedXYZ_Min.x);
			dataP.emplace("vStartSpeedXYZ_Min.y", ParticleDesc.vStartSpeedXYZ_Min.y);
			dataP.emplace("vStartSpeedXYZ_Min.z", ParticleDesc.vStartSpeedXYZ_Min.z);
			dataP.emplace("vStartSpeedXYZ_Max.x", ParticleDesc.vStartSpeedXYZ_Max.x);
			dataP.emplace("vStartSpeedXYZ_Max.y", ParticleDesc.vStartSpeedXYZ_Max.y);
			dataP.emplace("vStartSpeedXYZ_Max.z", ParticleDesc.vStartSpeedXYZ_Max.z);
			dataP.emplace("iMovingMode", ParticleDesc.iMovingMode);
			dataP.emplace("iSpeedEasing_End", ParticleDesc.iSpeedEasing_End);
			dataP.emplace("iSpeedEasing_Middle", ParticleDesc.iSpeedEasing_Middle);
			dataP.emplace("fMiddleSpeedTimePoint", ParticleDesc.fMiddleSpeedTimePoint);
			dataP.emplace("vMiddleSpeedXYZ.x", ParticleDesc.vMiddleSpeedXYZ.x);
			dataP.emplace("vMiddleSpeedXYZ.y", ParticleDesc.vMiddleSpeedXYZ.y);
			dataP.emplace("vMiddleSpeedXYZ.z", ParticleDesc.vMiddleSpeedXYZ.z);
			dataP.emplace("vEndSpeedXYZ.x", ParticleDesc.vEndSpeedXYZ.x);
			dataP.emplace("vEndSpeedXYZ.y", ParticleDesc.vEndSpeedXYZ.y);
			dataP.emplace("vEndSpeedXYZ.z", ParticleDesc.vEndSpeedXYZ.z);
			dataP.emplace("vTargetPosition.x", ParticleDesc.vTargetPosition.x);
			dataP.emplace("vTargetPosition.y", ParticleDesc.vTargetPosition.y);
			dataP.emplace("vTargetPosition.z", ParticleDesc.vTargetPosition.z);
			dataP.emplace("bIsGatheringOnce", ParticleDesc.bIsGatheringOnce);
			dataP.emplace("iPositioningEasing_End", ParticleDesc.iPositioningEasing_End);
			dataP.emplace("iPositioningEasing_Middle", ParticleDesc.iPositioningEasing_Middle);
			dataP.emplace("fMiddlePositioningTimePoint", ParticleDesc.fMiddlePositioningTimePoint);
			dataP.emplace("fEachAccelation_MinMax.x", ParticleDesc.fEachAccelation_MinMax.x);
			dataP.emplace("fEachAccelation_MinMax.y", ParticleDesc.fEachAccelation_MinMax.y);
			dataP.emplace("fWholeAccelation.x", ParticleDesc.fWholeAccelation.x);
			dataP.emplace("fWholeAccelation.y", ParticleDesc.fWholeAccelation.y);
			dataP.emplace("fWholeAccelation.z", ParticleDesc.fWholeAccelation.z);
			dataP.emplace("vRotateRUL_Min.x", ParticleDesc.vRotateRUL_Min.x);
			dataP.emplace("vRotateRUL_Min.y", ParticleDesc.vRotateRUL_Min.y);
			dataP.emplace("vRotateRUL_Min.z", ParticleDesc.vRotateRUL_Min.z);
			dataP.emplace("vRotateRUL_Max.x", ParticleDesc.vRotateRUL_Max.x);
			dataP.emplace("vRotateRUL_Max.y", ParticleDesc.vRotateRUL_Max.y);
			dataP.emplace("vRotateRUL_Max.z", ParticleDesc.vRotateRUL_Max.z);
			dataP.emplace("fRotateAccelation_Min.x", ParticleDesc.fRotateAccelation_Min.x);
			dataP.emplace("fRotateAccelation_Min.y", ParticleDesc.fRotateAccelation_Min.y);
			dataP.emplace("fRotateAccelation_Min.z", ParticleDesc.fRotateAccelation_Min.z);
			dataP.emplace("fRotateAccelation_Max.x", ParticleDesc.fRotateAccelation_Max.x);
			dataP.emplace("fRotateAccelation_Max.y", ParticleDesc.fRotateAccelation_Max.y);
			dataP.emplace("fRotateAccelation_Max.z", ParticleDesc.fRotateAccelation_Max.z);

			Particles.push_back(dataP);
		}
		EffectType.emplace("Particles", Particles);

		Save.push_back(EffectType);
	}

	GAMEINSTANCE->Save_Json(GAMEINSTANCE->json_Complete_Path(towstring("Effect/" + strFileName).c_str()).c_str(), Save);

	return S_OK;
}

HRESULT CEffect_Manager::Load_EffectsFile(string strFileName)
{
#ifdef _EFFECT
	if (NULL != m_Effects.size())
	{
		for (auto& Pair : m_Effects)
			Safe_Release(Pair.second);

		m_Effects.clear();
	}

#ifdef UPDATE
	if ("EffectData" == strFileName)
		Update_EffectDataFile();
#endif

	Load_EffectsData(strFileName, &m_Effects);
#endif
	return S_OK;
}

HRESULT CEffect_Manager::Save_EffectsFile(string strFileName)
{
	if (FAILED(Save_EffectsData(strFileName, &m_Effects)))
		return E_FAIL;

#ifdef UPDATE
	// All File Save to "EffectData.json"
	return Update_EffectDataFile();
#else
	return S_OK;
#endif
}

HRESULT CEffect_Manager::ExcuteEffect(const string& EffectName, _matrix matTransform, _float fTimeSpeed, CGameObject* ParentObject, FOLLOW_MODE eFollowMode, string pBoneName, list<CEffect_Base*>* pOutList)
{
#ifdef _EFFECT
	if (nullptr == Find_Effect(EffectName))
	{
		wstring str = towstring(EffectName);
		MSG_BOX_USER(TEXT("Failed to Find EffectName"), str.c_str());
		return E_FAIL;
	}

	CEffect_Factory* pEffect = m_Effects.find(EffectName)->second;
	if (nullptr == pEffect)
		return E_FAIL;

	return pEffect->Execute(matTransform, fTimeSpeed, ParentObject, eFollowMode, pBoneName, pOutList);
#else
	return S_OK;
#endif // _EFFECT

}

HRESULT CEffect_Manager::Add_Effect(const string& EffectName, vector<MESHEFFECT_DESC>& MeshEffectDesc, vector<PARTICLE_DESC>& ParticleDesc)
{
	if (nullptr == Find_Effect(EffectName))
		m_Effects.emplace(EffectName, CEffect_Factory::Create(EffectName, ParticleDesc, MeshEffectDesc));
	else
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Manager::Delete_Effect(const string& EffectName)
{
	USEGAMEINSTANCE;
	if (nullptr != Find_Effect(EffectName))
	{
		m_Effects.erase(EffectName);
	}

	return S_OK;
}

HRESULT CEffect_Manager::Update_EffectDataFile(void)
{
	// Clear m_Effects_InAllFiles
	if (NULL != m_Effects_InAllFiles.size())
	{
		for (auto& Pair : m_Effects_InAllFiles)
			Safe_Release(Pair.second);

		m_Effects_InAllFiles.clear();
	}

	stringvec vecFiles;
	read_directory(JSONPATH_EFFECT, vecFiles);
	for (int i = 0; i < vecFiles.size();)
		if (vecFiles[i] == "EffectData.json") // Except Meta File
			vecFiles.erase(vecFiles.begin() + i);
		else
			i++;

	for (auto& File : vecFiles)
	{
		if (FAILED(Load_EffectsData(File.substr(0, File.length() - strlen(".json")).c_str(), &m_Effects_InAllFiles)))
			MSG_BOX("Failed To Load_Effect");
	}

	if (FAILED(Save_EffectsData("EffectData", &m_Effects_InAllFiles)))
		return E_FAIL;

	return S_OK;
}

CEffect_Factory* CEffect_Manager::Find_Effect(const string& EffectName, EFFECTS* pEffects)
{
	if (nullptr != pEffects)
	{
		auto effect = pEffects->find(EffectName);

		if (effect == pEffects->end())
			return nullptr;

		return pEffects->find(EffectName)->second;
	}
	else
	{
		auto effect = m_Effects.find(EffectName);

		if (effect == m_Effects.end())
			return nullptr;

		return m_Effects.find(EffectName)->second;
	}
}

void CEffect_Manager::Create_Effect_Positioning_Rotaion(string strEffectName, _float4x4* matParent, _fvector vPos, _matrix matRotation, _float fPlaySpeed, _fvector vScale, _fvector vRevolution, list<CEffect_Base*>* pOutList)
{
	_matrix OffsetMatrix = XMMatrixScaling(XMVectorGetX(vScale), XMVectorGetY(vScale), XMVectorGetZ(vScale));
	OffsetMatrix *= matRotation;
	OffsetMatrix.r[3] = vPos;
	OffsetMatrix *= XMMatrixRotationRollPitchYawFromVector(vRevolution);

	_matrix ParentMatrix = XMLoadFloat4x4(matParent);

	ParentMatrix.r[0] = XMVector3Normalize(ParentMatrix.r[0]);
	ParentMatrix.r[1] = XMVector3Normalize(ParentMatrix.r[1]);
	ParentMatrix.r[2] = XMVector3Normalize(ParentMatrix.r[2]);

	_matrix TargetMatrix = OffsetMatrix * ParentMatrix;
	EFFECT_EXCUTE(strEffectName.c_str(), TargetMatrix, fPlaySpeed, nullptr, CEffect_Manager::FOLLOW_MODE_END, "", pOutList);
}

void CEffect_Manager::Create_Effect_Positioning(string strEffectName, _float4x4* matParent, _fvector vPos, _fvector vRotation, _float fPlaySpeed, _fvector vScale, _fvector vRevolution, list<CEffect_Base*>* pOutList)
{
	_matrix OffsetMatrix = XMMatrixScaling(XMVectorGetX(vScale), XMVectorGetY(vScale), XMVectorGetZ(vScale));
	OffsetMatrix *= XMMatrixRotationRollPitchYawFromVector(vRotation);
	OffsetMatrix.r[3] = vPos;
	OffsetMatrix.r[3] = XMVectorSetW(OffsetMatrix.r[3], 1.f);
	OffsetMatrix *= XMMatrixRotationRollPitchYawFromVector(vRevolution);

	_matrix ParentMatrix = XMMatrixIdentity();
	if (nullptr != matParent)
	{
		ParentMatrix = XMLoadFloat4x4(matParent);
		ParentMatrix.r[0] = XMVector3Normalize(ParentMatrix.r[0]);
		ParentMatrix.r[1] = XMVector3Normalize(ParentMatrix.r[1]);
		ParentMatrix.r[2] = XMVector3Normalize(ParentMatrix.r[2]);
	}
	_matrix TargetMatrix = OffsetMatrix * ParentMatrix;
	EFFECT_EXCUTE(strEffectName.c_str(), TargetMatrix, fPlaySpeed, nullptr, CEffect_Manager::FOLLOW_MODE_END, "", pOutList);
}

void CEffect_Manager::Free()
{
	__super::Free();

	if (NULL != m_Effects.size())
	{
		for (auto& Pair : m_Effects)
			Safe_Release(Pair.second);

		m_Effects.clear();
	}

	if (NULL != m_Effects_InAllFiles.size())
	{
		for (auto& Pair : m_Effects_InAllFiles)
			Safe_Release(Pair.second);

		m_Effects_InAllFiles.clear();
	}

}
