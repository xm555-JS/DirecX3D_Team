
//#include "Engine_Shader_Defines.hpp"
#include "Engine_Shader_Function.hlsl"


matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

//float g_fWinCX = 1280.f;
//float g_fWinCY = 720.f;
float g_fFar;

float       g_fSaturationRate = 1.f;
float       g_fBrightness = 0.f;
float       g_fBright = 0.f;
float       g_fFogRate = 0.f;
vector      g_vFogColor = {1.f, 1.f, 1.f, 1.f};

float       g_fMiddleGrey = 0.5f;
float       g_fLumWhiteSqr = 1.f;

float2      g_vRCADir = {0.f, 0.f};
float2      g_vGCADir = {0.f, 0.f};
float2      g_vBCADir = {0.f, 0.f};

float2      g_vSunPos = {0.f, 0.f};

float2      g_vRadialCAPos = {0.f, 0.f};
vector      g_vRadialCAPower = {0.f, 0.f, 0.f, 0.f};

float       g_fGammaPower = 0.454545f;
float       g_fRevGammaPower = 0.f;

float       g_fDOFRate = 10.f;
int         g_iDOFPower = 10;

int         g_iBlurPower = 10;

float       g_fSunPower = 1.f;
float       g_fPowerSSAO = 1.f;
float       g_fPowerSSAO2 = 1.f;

bool        g_bIsSun = false;

float4x4    g_CamViewMatrixInv;
float4x4    g_CamProjMatrixInv;

texture2D	g_DecalTexture;


texture2D	g_Texture;
texture2D	g_AOTexture;
texture2D	g_ShadowTexture;
texture2D	g_OutlineTexture;
texture2D	g_BloomTexture;
texture2D	g_TargetOutlineTexture;


texture2D	g_DiffuseTexture;
texture2D	g_ShadeTexture;
texture2D	g_SpecularTexture;
texture2D	g_DepthTexture;
texture2D   g_EffectTexture;
texture2D   g_ShadeFlagTexture;

texture2D   g_ToonFlagTexture;

texture2D	g_AlphaBlendTexture;
texture2D	g_NonLightTexture;
texture2D	g_UITexture;
texture2D	g_GlowTexture;
texture2D	g_RimLightTexture;
texture2D	g_GlowBluredTexture;
texture2D	g_GlowBluredDepthTexture;
texture2D   g_DistortionTexture;
texture2D   g_SSRTexture;
texture2D   g_WaterTexture;
texture2D   g_BlurFlagTexture;
texture2D   g_NormalTexture;
texture2D   g_TerrainFlagTexture;

texture2D   g_CloudTexture;
texture2D   g_CloudSkyTexture;

texture2D g_EnvironmentTexture;
texture2D g_ScreenEffectTexture;

texture2D   g_LumiNewTexture;
texture2D   g_LumiOldTexture;

texture2D   g_NoiseTexture;

texture2D       g_LightDepthTexture;
float           g_fCamDepth;
float2          g_vLightUV;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);

	return Out;
}


PS_OUT PS_MAIN_BLEND(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

    
    vector          vDistortion = g_DistortionTexture.Sample(LinearSampler, In.vTexUV);
    float			fDistortionPower = 0.f;

    // 1000.f 는 민감도이다.
    if (0.f != vDistortion.g)
        fDistortionPower = (1.f / vDistortion.g) / 1000.f;
    
	vector			vDiffuse = g_DiffuseTexture.Sample(LinearClampSampler, In.vTexUV + vDistortion.r * fDistortionPower) * 1.f;
	vector			vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexUV) * 1.f;
	vector			vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);
	vector			vDepth = g_DepthTexture.Sample(LinearSampler, In.vTexUV);
    vector          vEffect = g_EffectTexture.Sample(LinearClampSampler, In.vTexUV + vDistortion.r * fDistortionPower) * 1.f;
    
    

	vector			vAlphaBlend = g_AlphaBlendTexture.Sample(LinearSampler, In.vTexUV);
	vector			vNonLight = g_NonLightTexture.Sample(LinearSampler, In.vTexUV);
	vector			vUI = g_UITexture.Sample(LinearSampler, In.vTexUV);

	vector			vGlow = g_GlowTexture.Sample(LinearSampler, In.vTexUV);
	vector			vGlowBlured = g_GlowBluredTexture.Sample(LinearSampler, In.vTexUV);
    
    vector          vToonFlag = g_ToonFlagTexture.Sample(LinearSampler, In.vTexUV);
    
	//vector			vGlowBlurDepth = g_GlowBluredDepthTexture.Sample(LinearSampler, In.vTexUV);
	vector			vShadow = g_ShadowTexture.Sample(LinearSampler, In.vTexUV + vDistortion.r * fDistortionPower);
	
	vector			vWater = g_WaterTexture.Sample(LinearSampler, In.vTexUV + vDistortion.r * fDistortionPower);
	vector			vSSR = g_SSRTexture.Sample(LinearSampler, In.vTexUV);
    

	//vector			vAO = g_AOTexture.Sample(LinearSampler, In.vTexUV);
	//vector			vDecal = g_DecalTexture.Sample(LinearSampler, In.vTexUV);
	//vector			vOutline = g_OutlineTexture.Sample(LinearSampler, In.vTexUV);
	//vector			vBloom = g_BloomTexture.Sample(LinearSampler, In.vTexUV);
	//vector			vTargetOutline = g_TargetOutlineTexture.Sample(LinearSampler, In.vTexUV);
	
    
	//Cartoon Shader
    if (1.f == vToonFlag.r)
    {
        int iToonLevel = 3;
        vShade = ceil(vShade * iToonLevel) / iToonLevel;
    }

    
	vShade = vShade - vShadow.r;

    // skybox
    if (1.f == vDepth.y)
        Out.vColor = vDiffuse;
    else
	    Out.vColor = vDiffuse * vShade + vSpecular;

    //Out.vColor.rgb = Out.vColor.rgb * (1.f - vEffect.a) + vEffect.rgb * vEffect.a;
    vDiffuse.rgb = vDiffuse.rgb * (1.f - vEffect.a) + vEffect.rgb * vEffect.a;
    
    
	//Out.vColor = (1.f - vAlphaBlend.a) * Out.vColor + vAlphaBlend.a * vAlphaBlend;
	//Out.vColor.a = vAlphaBlend.a;

	//Out.vColor = (1.f - vNonLight.a) * Out.vColor + vNonLight.a * vNonLight;
	//Out.vColor.a = vNonLight.a;

	//Out.vColor = ((1.f - vUI.a) * Out.vColor) + (vUI.a * vUI);
	//Out.vColor.a = vUI.a;

	if (0.f < vAlphaBlend.a)
		Out.vColor += vAlphaBlend;

	if (0.f < vNonLight.a)
		Out.vColor = vNonLight;
    
    //water shader
    if (0.f < vWater.a)
    {
        Out.vColor.rgb = vWater.a * vWater.rgb + (1.f - vWater.a) * Out.vColor.rgb;
        Out.vColor.a = vWater.a;
    }
    
    if (0.f < vSSR.a)
        Out.vColor.rgb = vSSR.a * vSSR.rgb + (1.f - vSSR.a) * Out.vColor.rgb;
    
	//float	fBius = 0.01f;
	////if (0.f == vGlow.a)
	////{
	//	if((vDepth.y - fBius > vGlowBlurDepth.y) || (0.f == vGlow.a))
	//		Out.vColor.rgb = (1.f - vGlowBlurDepth.a) * Out.vColor.rgb + vGlowBlurDepth.a * vGlowBlured.rgb;
	//		//Out.vColor.rgb = (1.f - vGlowBlured.a) * Out.vColor.rgb + vGlowBlured.a * vGlowBlured.rgb;
	////}


	//if (0.f == vGlow.a)
    //vector vOriginColor = Out.vColor;
	

    //Out.vColor.rgb += vGlowBlured.rgb;
    //Out.vColor.rgb = (1.f - vGlow.a) * Out.vColor.rgb + vGlow.a * vDiffuse.rgb;

    // skybox check
    if (1.f != vDepth.y)
    {
        float fFogRate = vDepth.y * g_fFogRate;
        Out.vColor = Out.vColor * (1.f - fFogRate) + g_vFogColor * fFogRate;
    }

    
    //float fDOFRate = vDepth.y * g_fDOFRate;
    //Out.vColor = Get_Blur();
    
    

	if (0.f < length(vUI))
    {
        vDiffuse = Out.vColor = ((1.f - vUI.a) * Out.vColor) + (vUI.a * vUI);
        //vDiffuse = ((1.f - vUI.a) * Out.vColor) + (vUI.a * vUI);
        //vDiffuse.rgb = Out.vColor.rgb;
		//Out.vColor = vUI;
    }

    
    
    float fAverColor = (Out.vColor.r + Out.vColor.g + Out.vColor.b) / 3.f;
    float fAtt = g_fSaturationRate; // 채도 비율

    Out.vColor.r = fAverColor + (Out.vColor.r - fAverColor) * fAtt + g_fBrightness;
    Out.vColor.g = fAverColor + (Out.vColor.g - fAverColor) * fAtt + g_fBrightness;
    Out.vColor.b = fAverColor + (Out.vColor.b - fAverColor) * fAtt + g_fBrightness;
    
    Out.vColor.rgb = Out.vColor.rgb * (1.f - vEffect.a) + vEffect.rgb * vEffect.a;
    Out.vColor.rgb += vGlowBlured.rgb;
    Out.vColor.rgb = (1.f - vGlow.a) * Out.vColor.rgb + vGlow.a * vDiffuse.rgb;


	//if (0.f < vUI.a )
	//	Out.vColor = vUI;

	//vShade = vShade * vOutline - vShadow.r;

	//vDiffuse = vDiffuse * vShade + vSpecular;

	//if (0.f != vTargetOutline.a)
	//	Out.vColor = vTargetOutline;
	//else
	//	Out.vColor = vDecal * vDecal.a + (vDiffuse * (1.f - vBloom.a) + vBloom * (vBloom.a)) * (1.f - vDecal.a);


	//Out.vColor = vDiffuse * vShade + vSpecular;

	//Out.vColor = vDecal.a;


	//if (Out.vColor.a == 0.f)
	//	discard;

	return Out;
}


PS_OUT PS_MAIN_BLEND_ENVIRONMENT(PS_IN In)
{
    PS_OUT Out = (PS_OUT)0;

    
    vector vDistortion = g_DistortionTexture.Sample(LinearSampler, In.vTexUV);
    float fDistortionPower = 0.f;

    // 1000.f 는 민감도이다.
    if (0.f != vDistortion.g)
        fDistortionPower = (1.f / vDistortion.g) / 1000.f;
    
    vector vDiffuse = g_DiffuseTexture.Sample(LinearClampSampler, In.vTexUV + vDistortion.r * fDistortionPower) * 1.f;
    //vDiffuse.rgb = Gamma_LinearSpace(vDiffuse.rgb);
    
    vector vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexUV) * 1.f;
    // vector vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);
    vector vDepth = g_DepthTexture.Sample(LinearSampler, In.vTexUV);
    //vector vNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
    //vector vEffect = g_EffectTexture.Sample(LinearClampSampler, In.vTexUV + vDistortion.r * fDistortionPower) * 1.f;
    
    vector vAlphaBlend = g_AlphaBlendTexture.Sample(LinearSampler, In.vTexUV);
    vector vNonLight = g_NonLightTexture.Sample(LinearSampler, In.vTexUV);

    vector vOutline = g_OutlineTexture.Sample(LinearSampler, In.vTexUV);
    vector vToonFlag = g_ToonFlagTexture.Sample(LinearSampler, In.vTexUV);
    vector vTerrainFlag = g_TerrainFlagTexture.Sample(LinearSampler, In.vTexUV);
    
    
    vector vShadow = g_ShadowTexture.Sample(LinearSampler, In.vTexUV + vDistortion.r * fDistortionPower);
	
    vector vWater = g_WaterTexture.Sample(LinearSampler, In.vTexUV + vDistortion.r * fDistortionPower);
    vector vSSR = g_SSRTexture.Sample(LinearSampler, In.vTexUV);
    
    vector vCloud = g_CloudTexture.Sample(LinearSampler, In.vTexUV);
    vector vCloudSky = g_CloudSkyTexture.Sample(LinearSampler, In.vTexUV);
    vCloud = pow(vCloud, 2.1f);
    vCloudSky = pow(vCloudSky, 1.3f);
    vCloudSky.a *= vCloud.a;
    
    //vector vShadeFlag = g_ShadeFlagTexture.Sample(LinearSampler, In.vTexUV);
    
    vDiffuse.rgb = Gamma_LinearSpace(vDiffuse.rgb);
    //vEffect.rgb = Gamma_LinearSpace(vEffect.rgb);
    vAlphaBlend = pow(vAlphaBlend, 2.2);
    vNonLight.rgb = Gamma_LinearSpace(vNonLight.rgb);
    vOutline.rgb = Gamma_LinearSpace(vOutline.rgb);
    

    
	//Cartoon Shader
    if (0.f < vToonFlag.a)
    {
        int iToonLevel = 3;
        vShade = ceil(vShade * iToonLevel) / iToonLevel;
    }

    //vNormal, vDepth, g_fFar, g_CamViewMatrixInv, g_CamProjMatrixInv
    //float3 p = getPosition(vUV);
    //float3 n = getNormal(vUV);
    //float ao = 0.f;
    //float rad = SAMPLE_RAD / p.z;
    //ao = spiralAO(vUV, p, n, rad);
    //ao = 1.f - ao * INTENSITY;
    float ao = 0.f;
    if (1.f == vTerrainFlag.r)
    {
        ao = Get_SSAO(In.vTexUV, g_NormalTexture, g_DepthTexture, g_fFar, g_CamViewMatrixInv, g_CamProjMatrixInv, 1.18f);
        ao = pow(ao, 0.97f);
    }
    else
    {
        ao = Get_SSAO(In.vTexUV, g_NormalTexture, g_DepthTexture, g_fFar, g_CamViewMatrixInv, g_CamProjMatrixInv, 0.295f);
        ao = pow(ao, 0.815f);
    }

    
    
    vShade = vShade - vShadow.r - ao;

    // skybox
    if (1.f == vDepth.y)
        Out.vColor = vDiffuse;
    else
        Out.vColor = vDiffuse * vShade/* + vSpecular*/;
    
    if (0.f < vOutline.a)
        Out.vColor = vOutline;

    //vDiffuse.rgb = vDiffuse.rgb * (1.f - vEffect.a) + vEffect.rgb * vEffect.a;
    

    //if (0.f < vAlphaBlend.a)
    Out.vColor.rgb = vAlphaBlend.a * vAlphaBlend.rgb + (1.f - vAlphaBlend.a) * Out.vColor.rgb;
    
    Out.vColor.rgb = vCloudSky.a * vCloudSky.rgb + (1.f - vCloudSky.a) * Out.vColor.rgb;

    if (0.f < vNonLight.a)
        Out.vColor = vNonLight;
    
#if defined(_WATER)
    //water shader
    if (0.f < vWater.a)
    {
        Out.vColor.rgb = vWater.a * vWater.rgb + (1.f - vWater.a) * Out.vColor.rgb;
        Out.vColor.a = vWater.a;
    }
#endif
    
#if defined(_SSR)
    if (0.f < vSSR.a)
        Out.vColor.rgb = vSSR.a * vSSR.rgb + (1.f - vSSR.a) * Out.vColor.rgb;
#endif

    // Fog Shader, skybox check
    if (1.f != vDepth.y)
    {
        float fFogRate = vDepth.y * g_fFogRate;
        Out.vColor = Out.vColor * (1.f - fFogRate) + g_vFogColor * fFogRate;
    }

    Out.vColor.rgb = Gamma_ForMonitor(Out.vColor.rgb);
    return Out;
}


PS_OUT PS_MAIN_BLEND_SCREENEFFECT(PS_IN In)
{

    PS_OUT Out = (PS_OUT)0;

    vector vEnvDesc = g_EnvironmentTexture.Sample(LinearClampSampler, In.vTexUV);
    Out.vColor = vEnvDesc;
    
    vector vDistortion = g_DistortionTexture.Sample(LinearSampler, In.vTexUV);
    float fDistortionPower = 0.f;

    // 1000.f 는 민감도이다.
    if (0.f != vDistortion.g)
        fDistortionPower = (1.f / vDistortion.g) / 1000.f;
    
    vector vDepth = g_DepthTexture.Sample(LinearSampler, In.vTexUV);
    float fViewZ = vDepth.y * g_fFar;
    

    vector vBlurFlag = g_BlurFlagTexture.Sample(LinearSampler, In.vTexUV);
    vEnvDesc.rgb = Gamma_LinearSpace(vEnvDesc.rgb);

    
#if defined(_BLUR)
    int     iBlurPower = 0.f;
    
    //DOF Shader
    float2 vResolution = { g_fWinCX, g_fWinCY };
    int iDOFPower = max(min(pow(fViewZ * vDepth.x, g_fDOFRate), 8), 2);
    if (1.f == vDepth.y)
        iDOFPower = 8;
    iBlurPower += iDOFPower;
    
    if (0.f != vBlurFlag.r)
        iBlurPower += g_iBlurPower;
    
    if (0 != iBlurPower)
        Out.vColor = Get_Blur(g_EnvironmentTexture, In.vTexUV, vResolution, iBlurPower);
#endif

    
    //SaturationRate, Brightness
    float fAverColor = (Out.vColor.r + Out.vColor.g + Out.vColor.b) / 3.f;
    float fAtt = g_fSaturationRate; // 채도 비율

    Out.vColor.r = (fAverColor + (Out.vColor.r - fAverColor) * fAtt) * g_fBrightness;
    Out.vColor.g = (fAverColor + (Out.vColor.g - fAverColor) * fAtt) * g_fBrightness;
    Out.vColor.b = (fAverColor + (Out.vColor.b - fAverColor) * fAtt) * g_fBrightness;
    
    
#if defined(_LENSFLARE)
    vector vLightDepth = g_LightDepthTexture.Sample(LinearSampler, g_vLightUV);
    bool bLensflare = true;
    if (1.f == vLightDepth.y || g_fCamDepth > vLightDepth.x)
        bLensflare = false;
    
    if (g_bIsSun && bLensflare)
        Out.vColor.rgb += Get_lensflare(In.vTexUV, g_vSunPos) * g_fSunPower;
#endif
    //Out.vColor.rgb *= 2.f;
    //Out.vColor.rgb = ToneMapACES(Out.vColor.rgb);

#if defined(_HDR)
    vector vLumiNew = g_LumiNewTexture.Sample(LinearClampSampler, float2(1.f / g_fWinCX, 1.f / g_fWinCY));
    vector vLumiOld = g_LumiOldTexture.Sample(LinearClampSampler, float2(1.f / g_fWinCX, 1.f / g_fWinCY));
    Out.vColor.rgb = ToneMap(Out.vColor.rgb, vLumiOld.x, vLumiNew.x, g_fMiddleGrey, g_fLumWhiteSqr);
    Out.vColor.rgb = Gamma_ForMonitor(Out.vColor.rgb);
#endif
    
    vector vDiffuse = g_DiffuseTexture.Sample(LinearClampSampler, In.vTexUV + vDistortion.r * fDistortionPower) * 1.f;
    vector vEffect = g_EffectTexture.Sample(LinearClampSampler, In.vTexUV + vDistortion.r * fDistortionPower) * 1.f;
    vector vUI = g_UITexture.Sample(LinearSampler, In.vTexUV);
    vector vGlow = g_GlowTexture.Sample(LinearSampler, In.vTexUV);
    vector vGlowBlured = g_GlowBluredTexture.Sample(LinearSampler, In.vTexUV);
    vector vRimLight = g_RimLightTexture.Sample(LinearSampler, In.vTexUV);
    
    vDiffuse.rgb = Gamma_LinearSpace(vDiffuse.rgb);
    vEffect.rgb = Gamma_LinearSpace(vEffect.rgb, 1.f / 2.2f);
    vUI.rgb = Gamma_LinearSpace(vUI.rgb, 1.f / 2.2f);
    vGlow.rgb = Gamma_LinearSpace(vGlow.rgb);
    vGlowBlured.rgb = Gamma_LinearSpace(vGlowBlured.rgb, g_fRevGammaPower);
    //vGlowBlured.rgb = Gamma_LinearSpace(vGlowBlured.rgb, 1.f / 2.2f);
    
    //Effect Glow
    vDiffuse.rgb = vDiffuse.rgb * (1.f - vEffect.a) + vEffect.rgb * vEffect.a;
    Out.vColor.rgb = Out.vColor.rgb * (1.f - vEffect.a) + vEffect.rgb * vEffect.a;
    Out.vColor.rgb += vRimLight.rgb;
    Out.vColor.rgb += vGlowBlured.rgb;
    Out.vColor.rgb = (1.f - vGlow.a) * Out.vColor.rgb + vGlow.a * vDiffuse.rgb;
    

    
    //UI
    if (0.f < length(vUI))
        Out.vColor.rgb = ((1.f - vUI.a) * Out.vColor.rgb) + (vUI.a * vUI.rgb);
    return Out;
}


PS_OUT PS_MAIN_BLEND_AFTEREFFECT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vScreenDesc = (vector) 0;
    

    float2 vPointACDir = In.vTexUV - g_vRadialCAPos;
    
    //CA(ChromaticAberration) Shader

    float2 vRCA = g_vRCADir + vPointACDir * g_vRadialCAPower.r;
    float2 vGCA = g_vGCADir + vPointACDir * g_vRadialCAPower.g;
    float2 vBCA = g_vBCADir + vPointACDir * g_vRadialCAPower.b;
    float2 vACA = 0.f + vPointACDir * g_vRadialCAPower.a;
    
    vScreenDesc.r = g_ScreenEffectTexture.Sample(LinearClampSampler, In.vTexUV + vRCA).r;
    vScreenDesc.g = g_ScreenEffectTexture.Sample(LinearClampSampler, In.vTexUV + vGCA).g;
    vScreenDesc.b = g_ScreenEffectTexture.Sample(LinearClampSampler, In.vTexUV + vBCA).b;
    vScreenDesc.a = g_ScreenEffectTexture.Sample(LinearClampSampler, In.vTexUV + vACA).a;
    
    //float2 vResolution = { g_fWinCX, g_fWinCY };
    //Out.vColor = Get_RadialBlur(g_ScreenEffectTexture, In.vTexUV, vResolution, g_vRBlurPower, g_vPointRBlurDir);

    vScreenDesc.rgb = Gamma_LinearSpace(vScreenDesc.rgb);

    
    
    //vector vLumiNew = g_LumiNewTexture.Sample(LinearClampSampler, In.vTexUV);
    //vector vLumiOld = g_LumiOldTexture.Sample(LinearClampSampler, In.vTexUV);

    
    Out.vColor = vScreenDesc;

    Out.vColor.rgb = Gamma_ForMonitor(Out.vColor.rgb, g_fGammaPower);
    
    //Out.vColor.rgb = Luminance(Out.vColor.rgb, g_fBright, vLumiOld.x, vLumiNew.x);
    //Out.vColor.rgb = ToneMapACES(Out.vColor.rgb);
    
    return Out;
}

technique11 DefaultTechnique
{
	pass Debug
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
	}

	pass Blend
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLEND();
	}

    pass Blend_BaseEnvironment
    {
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLEND_ENVIRONMENT();
    }

    pass Blend_ScreenEffect
    {
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLEND_SCREENEFFECT();
    }

    pass Blend_AfterEffect
    {
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLEND_AFTEREFFECT();
    }
}