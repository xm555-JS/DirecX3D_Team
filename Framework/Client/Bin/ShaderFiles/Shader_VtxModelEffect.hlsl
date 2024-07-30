
#include "Client_Shader_Defines.hpp"
#include "Client_Shader_Function.hlsl"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float g_fFar;

vector g_DiffuseColor;
texture2D g_NoiseTexture;
texture2D g_MaskTexture;
texture2D g_DepthTexture;
texture2D g_DissolveNoiseTexture;
texture2D g_DissolveColorTexture;

bool g_bIsClampSampling_Noise;
float2 g_vUVSpeedPos_Noise;
float g_fUVScaleXMax_Noise;
float g_fUVScaleYMax_Noise;

// Add New
//bool g_bIsUVSpeedMode; // Pass 로 나눠줌.
bool g_bIsUVSpeedMode;
bool g_bIsClampSampling_Mask;
bool g_bIsReverseXY_Mask;
bool g_bIsReverseXY_Noise;
float2 g_vUVSpeedPos_Mask;
float g_fUVScaleXCur_Mask;
float g_fUVScaleYCur_Mask;
float g_fUVScaleXMax_Mask;
float g_fUVScaleYMax_Mask;

// Shader
bool g_bIsSoftEffect = true;
bool g_bIsGlow = false;
bool g_bIsGlowFollowDiffuseColor;
float4 g_vGlowColor;
bool g_bIsDistortion = false;
float g_fDistortionStrength;
bool g_bIsRimLight = false;
float4 g_vRimLightColor;
float4 g_vLook;

bool g_bIsDissolve;
float4 g_vDissolveColor;
float g_fDissolveStrength;
float g_fTimeAccDissolve;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float3 vTangent : TANGENT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    //float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
    //Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.vTexUV = In.vTexUV;
    Out.vProjPos = Out.vPosition;

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

struct PS_OUT
{
    vector vDiffuse : SV_TARGET0;
    //vector vNormal : SV_TARGET1;
    //vector vDepth : SV_TARGET2;
    vector vGlow : SV_TARGET1;
    vector vRimLight : SV_TARGET2;
    //vector vDistortion : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    float2 vTexUV_Mask;
    if (g_bIsUVSpeedMode)
    {
        vTexUV_Mask.x = In.vTexUV.x + g_vUVSpeedPos_Mask.x;
        vTexUV_Mask.y = In.vTexUV.y + g_vUVSpeedPos_Mask.y;
    }
    else
    {
        vTexUV_Mask.x = In.vTexUV.x / g_fUVScaleXMax_Mask + (g_fUVScaleXCur_Mask / g_fUVScaleXMax_Mask);
        vTexUV_Mask.y = In.vTexUV.y / g_fUVScaleYMax_Mask + (g_fUVScaleYCur_Mask / g_fUVScaleYMax_Mask);
    }
    float2 vTexUV_Noise;
    vTexUV_Noise.x = In.vTexUV.x / g_fUVScaleXMax_Noise + g_vUVSpeedPos_Noise.x / g_fUVScaleXMax_Noise;
    vTexUV_Noise.y = In.vTexUV.y / g_fUVScaleYMax_Noise + g_vUVSpeedPos_Noise.y / g_fUVScaleYMax_Noise;
    
    float2 fSwap;
    if (g_bIsReverseXY_Mask)
    {
        fSwap = vTexUV_Mask.x;
        vTexUV_Mask.x = vTexUV_Mask.y;
        vTexUV_Mask.y = fSwap;
    }
    if (g_bIsReverseXY_Noise)
    {
        fSwap = vTexUV_Noise.x;
        vTexUV_Noise.x = vTexUV_Noise.y;
        vTexUV_Noise.y = fSwap;
    }
    
    vector g_Mask = g_bIsClampSampling_Mask == true ? g_MaskTexture.Sample(LinearClampSampler, vTexUV_Mask) : g_MaskTexture.Sample(LinearSampler, vTexUV_Mask);
    vector g_Noise = g_bIsClampSampling_Noise == true ? g_NoiseTexture.Sample(LinearClampSampler, vTexUV_Noise) : g_NoiseTexture.Sample(LinearSampler, vTexUV_Noise);
    

    Out.vDiffuse.a = max(max(g_Mask.r, g_Mask.g), g_Mask.b) * g_Mask.a * g_DiffuseColor.a;
    if (Out.vDiffuse.a < 0.01f)
        discard;
        
    Out.vDiffuse.rgb = g_DiffuseColor.rgb * g_Noise.rgb;
    
    if (g_bIsGlow)
    {
        if (g_bIsGlowFollowDiffuseColor)
            Out.vGlow.rgb = Out.vDiffuse.rgb;
        else
            Out.vGlow.rgb = g_vGlowColor.rgb;
        Out.vGlow.a = Out.vDiffuse.a * g_vGlowColor.a;
    }
    if (g_bIsSoftEffect)
    {
        /* -1, 1 => 1, -1 : 투영공간상의 위치. */
        vTexUV_Mask.x = In.vProjPos.x / In.vProjPos.w;
        vTexUV_Mask.y = In.vProjPos.y / In.vProjPos.w;
    
	    /* 0, 0 => 1, 1 : 텍스쳐 유브이 좌표. */
        vTexUV_Mask.x = vTexUV_Mask.x * 0.5f + 0.5f;
        vTexUV_Mask.y = vTexUV_Mask.y * -0.5f + 0.5f;
    
        vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, vTexUV_Mask);
    
        float fViewZ = vDepthDesc.y * g_fFar;
    
        Out.vDiffuse.a = Out.vDiffuse.a * saturate(fViewZ - In.vProjPos.w);
    }
    if (g_bIsDissolve)
    {
        vector vDissolveNoiseTexture = g_DissolveNoiseTexture.Sample(LinearClampSampler, In.vTexUV);
        vector vDissolveColorTexture = g_DissolveColorTexture.Sample(LinearClampSampler, In.vTexUV);
            
        if (g_fTimeAccDissolve + g_fDissolveStrength > vDissolveNoiseTexture.r)
            Out.vDiffuse.rgb = vDissolveColorTexture.rgb * g_vDissolveColor.rgb;
    
        if (g_fTimeAccDissolve > vDissolveNoiseTexture.r)
            discard;
    }
    if (g_bIsRimLight)
    {
        Out.vRimLight.rgb += Get_RimLight(In.vNormal.rgb, g_vLook.rgb, g_vRimLightColor, g_vRimLightColor.a * 10.f).rgb;
        Out.vRimLight.a = 1.f;
        //Out.vGlow += Get_RimLight(In.vNormal, g_vLook, g_vRimLightColor, 1.f).rgb;
    }
    
    return Out;
}

struct PS_OUT_DISTORTION
{
    vector vDistortion : SV_TARGET0;
    vector vDistortionDepth : SV_TARGET1;
};

PS_OUT_DISTORTION PS_MAIN_DISTORTION(PS_IN In)
{
    PS_OUT_DISTORTION Out = (PS_OUT_DISTORTION) 0;
    float2 vTexUV_Mask;
    if (g_bIsUVSpeedMode)
    {
        vTexUV_Mask.x = In.vTexUV.x + g_vUVSpeedPos_Mask.x;
        vTexUV_Mask.y = In.vTexUV.y + g_vUVSpeedPos_Mask.y;
    }
    else
    {
        vTexUV_Mask.x = In.vTexUV.x / g_fUVScaleXMax_Mask + (g_fUVScaleXCur_Mask / g_fUVScaleXMax_Mask);
        vTexUV_Mask.y = In.vTexUV.y / g_fUVScaleYMax_Mask + (g_fUVScaleYCur_Mask / g_fUVScaleYMax_Mask);
    }
    float2 vTexUV_Noise;
    vTexUV_Noise.x = In.vTexUV.x / g_fUVScaleXMax_Noise + g_vUVSpeedPos_Noise.x / g_fUVScaleXMax_Noise;
    vTexUV_Noise.y = In.vTexUV.y / g_fUVScaleYMax_Noise + g_vUVSpeedPos_Noise.y / g_fUVScaleYMax_Noise;
    
    float2 fSwap;
    if (g_bIsReverseXY_Mask)
    {
        fSwap = vTexUV_Mask.x;
        vTexUV_Mask.x = vTexUV_Mask.y;
        vTexUV_Mask.y = fSwap;
    }
    if (g_bIsReverseXY_Noise)
    {
        fSwap = vTexUV_Noise.x;
        vTexUV_Noise.x = vTexUV_Noise.y;
        vTexUV_Noise.y = fSwap;
    }
    
    vector g_Mask = g_bIsClampSampling_Mask == true ? g_MaskTexture.Sample(LinearClampSampler, vTexUV_Mask) : g_MaskTexture.Sample(LinearSampler, vTexUV_Mask);
    vector g_Noise = g_bIsClampSampling_Noise == true ? g_NoiseTexture.Sample(LinearClampSampler, vTexUV_Noise) : g_NoiseTexture.Sample(LinearSampler, vTexUV_Noise);

    Out.vDistortion = g_Mask;
    Out.vDistortion.g = 1.f / g_fDistortionStrength;
    
    Out.vDistortionDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);
    

    /* -1, 1 => 1, -1 : 투영공간상의 위치. */
    vTexUV_Mask.x = In.vProjPos.x / In.vProjPos.w;
    vTexUV_Mask.y = In.vProjPos.y / In.vProjPos.w;
    
	/* 0, 0 => 1, 1 : 텍스쳐 유브이 좌표. */
    vTexUV_Mask.x = vTexUV_Mask.x * 0.5f + 0.5f;
    vTexUV_Mask.y = vTexUV_Mask.y * -0.5f + 0.5f;
    
    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, vTexUV_Mask);
    
    float fViewZ = vDepthDesc.y * g_fFar;
    
    Out.vDistortion.r = Out.vDistortion.r * saturate(fViewZ - In.vProjPos.w);
    
    
    return Out;
}

BlendState BS_Effect_Glow
{
    BlendEnable[0] = true;
    SrcBlend[0] = src_alpha;
    DestBlend[0] = dest_alpha;
    BlendOp[0] = add;
    BlendOpAlpha[0] = max;

    BlendEnable[1] = true;
    SrcBlend[1] = src_alpha;
    DestBlend[1] = dest_alpha;
    BlendOp[1] = max;
    BlendOpAlpha[1] = add;
};



technique11 DefaultTechnique
{
    pass DefaultSampler // 0
    {
        SetBlendState(BS_Effect_Glow, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_CullModeNone);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Distortion // 0
    {
        SetBlendState(BS_Effect_Glow, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_CullModeNone);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DISTORTION();
    }
}