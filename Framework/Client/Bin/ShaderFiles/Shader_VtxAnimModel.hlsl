
#include "Client_Shader_Defines.hpp"
#include "Client_Shader_Function.hlsl"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_DiffuseTexture;
texture2D g_MaskTexture;
//texture2D g_NormalTexture;
texture2D g_DissolveNoiseTexture;
texture2D g_DissolveColorTexture;

bool		g_bGlow = false;
bool        g_bToon = false;
bool        g_bDissolve = false;
bool        g_bRimLight = false;
bool        g_bOutline = false;

vector      g_vLook = {0.f, 0.f, 1.f, 0.f};

vector		g_AlphaZeroDiffuse = { 1.f, 1.f, 1.f, 1.f };
vector		g_AlphaZeroGlow = { 1.f, 1.f, 1.f, 1.f };
float       g_fFar = 1000.f;
float       g_fLineBrightness = 1.f;

float       g_fTimeAcc = 0.016f;
float       g_fTotalTime = 2.f;

vector		g_vSourColor = { 1.f, 1.f, 1.f, 1.f };
vector      g_vDissolveFlagColor  = { 1.f, 1.f, 1.f, 1.f };

vector		g_RimLightColor = {0.f, 0.f, 1.f, 1.f};
float		g_fRimLightInterval = 1.f;

bool		g_bIsDissolve = false;
float4		g_vDissolveColor = { 0.f, 0.f, 0.f, 0.f };
float		g_fDissolveStrength = 0.f;
float		g_fTimeAccDissolve = 0.f;

struct		tagBoneMatrix
{
	matrix		BoneMatrices[256];
};

tagBoneMatrix		g_Bones;

sampler DefaultSampler = sampler_state 
{		
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;	
	uint4		vBlendIndex : BLENDINDEX;
	float4		vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
    float4		vProjPos : TEXCOORD2;
    //float3		vTangent : TANGENT;
    //float3		vBinormal : BINORMAL;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float			fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix			BoneMatrix = g_Bones.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_Bones.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_Bones.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_Bones.BoneMatrices[In.vBlendIndex.w] * fWeightW;

	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	vector		vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);

	Out.vPosition = mul(vPosition, matWVP);
    Out.vNormal = normalize(mul(vNormal, g_WorldMatrix)).xyz;
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
    Out.vProjPos = Out.vPosition;
    //Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix)).xyz;
    //Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));

	return Out;	
}

// w나누기연산을 수행하낟. (In 투영스페이스)
// 뷰포트 변환. (In 뷰포트(윈도우좌표))

// 래스터라이즈(픽셀정볼르 생성한다. )

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
    float4		vProjPos : TEXCOORD2;
    //float3		vTangent : TANGENT;
    //float3		vBinormal : BINORMAL;
};

struct PS_OUT
{	
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
	vector		vGlow : SV_TARGET3;
    vector      vToonFlag : SV_TARGET4;
    vector      vOutlineFlag : SV_TARGET5;
	
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);

    
    //if(g_bDissolve)
    //{
    //    vector vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);
    //    tagDissolve_In DissolveIn = (tagDissolve_In) 0;
    //    DissolveIn.vMask = vMask;
    //    DissolveIn.vFlagColor = g_vDissolveFlagColor;
    //    DissolveIn.vDiffuse = Out.vDiffuse;
    //    DissolveIn.fPassedTime = g_fTimeAcc;
    //    DissolveIn.fTotalTime = g_fTotalTime;
    //    
    //    vector vDissolveColor;
    //    Out.vDiffuse = Get_Dissolve(DissolveIn);
    //}
    
        
    if (g_bRimLight)
    {
        tagRimLight_In RimLightIn = (tagRimLight_In) 0;
        RimLightIn.vFlagColor = g_RimLightColor;
        RimLightIn.vLook = g_vLook.xyz;
        RimLightIn.vNormal = In.vNormal.xyz;
		RimLightIn.fInterval = g_fRimLightInterval;
        Out.vDiffuse += Get_RimLight(RimLightIn);
        Out.vDiffuse.a = 1.f;

    }

	if (g_bIsDissolve)
	{
		vector vDissolveNoiseTexture = g_DissolveNoiseTexture.Sample(LinearClampSampler, In.vTexUV);
		vector vDissolveColorTexture = g_DissolveColorTexture.Sample(LinearClampSampler, In.vTexUV);

		if (g_fTimeAccDissolve > vDissolveNoiseTexture.r)
		{
			Out.vDiffuse.rgb = vDissolveColorTexture.rgb * g_vDissolveColor.rgb;
			//Out.vDiffuse.a = 0.1f;
			Out.vGlow.rgb = vDissolveColorTexture.rgb * g_vDissolveColor.rgb;
			Out.vGlow.a = 1.f;

		}

		if (g_fTimeAccDissolve - g_fDissolveStrength > vDissolveNoiseTexture.r)
			discard;
	}

	///* 0 ~ 1 */
    //float3 vPixelNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV).xyz;
	///* -1 ~ 1 */
    //vPixelNormal = vPixelNormal * 2.f - 1.f;
    //float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);
    //vPixelNormal = mul(vPixelNormal, WorldMatrix);
    //Out.vNormal = vector(vPixelNormal * 0.5f + 0.5f, 0.f);

	
	//if (g_bGlow)
    if (0 == Out.vDiffuse.a)
    {
        //Out.vDiffuse = vector(1.f, 0.85f, 0.2f, 1.f);
        //Out.vGlow = vector(1.f, 0.24f, 0.f, 1.f);
        Out.vDiffuse = g_AlphaZeroDiffuse;
        Out.vGlow = g_AlphaZeroGlow;
    }

	Out.vDiffuse *= g_vSourColor;
	Out.vGlow *= g_vSourColor;
    
    if (g_bToon)
    {
        Out.vToonFlag = Out.vDiffuse;
        Out.vToonFlag.a = 1.f;
    }

    if (g_bOutline)
    {
        Out.vOutlineFlag = Out.vDiffuse * g_fLineBrightness;
        Out.vOutlineFlag.a = 1.f;
    }
    
	if (Out.vDiffuse.a < 0.1f)
		discard;

	return Out;	
}

PS_OUT PS_MAIN_WEAPON(PS_IN In)
{
    PS_OUT		Out = (PS_OUT)0;

    vector			vNoise = g_DissolveNoiseTexture.Sample(DefaultSampler, In.vTexUV);
    vector			vDiffuseColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV)/* * vNoise*/;

    Out.vDiffuse = vDiffuseColor;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);

    if (g_bRimLight)
    {
        tagRimLight_In RimLightIn = (tagRimLight_In)0;
        RimLightIn.vFlagColor = g_RimLightColor;
        RimLightIn.vLook = g_vLook.xyz;
        RimLightIn.vNormal = In.vNormal.xyz;
        RimLightIn.fInterval = g_fRimLightInterval;
        Out.vDiffuse += Get_RimLight(RimLightIn);
        Out.vDiffuse.a = 1.f;

    }

    if (g_bIsDissolve)
    {
        vector vDissolveNoiseTexture = g_DissolveNoiseTexture.Sample(LinearClampSampler, In.vTexUV);
        vector vDissolveColorTexture = g_DissolveColorTexture.Sample(LinearClampSampler, In.vTexUV);

        if (g_fTimeAccDissolve > vDissolveNoiseTexture.r)
        {
            Out.vDiffuse.rgb = vDissolveColorTexture.rgb * g_vDissolveColor.rgb;
            //Out.vDiffuse.a = 0.1f;
            Out.vGlow.rgb = vDissolveColorTexture.rgb * g_vDissolveColor.rgb;
            Out.vGlow.a = 1.f;
        }

        if (g_fTimeAccDissolve - g_fDissolveStrength > vDissolveNoiseTexture.r)
            discard;
    }

    //if (g_bGlow)
    if (0 == Out.vDiffuse.a)
    {
        Out.vDiffuse = g_AlphaZeroDiffuse;
        Out.vGlow = g_AlphaZeroGlow;
    }

    Out.vDiffuse *= g_vSourColor;
    Out.vGlow *= g_vSourColor;

    if (g_bToon)
    {
        Out.vToonFlag = Out.vDiffuse;
        Out.vToonFlag.a = 1.f;
    }

    if (Out.vDiffuse.a < 0.1f)
        discard;

    return Out;
}

PS_OUT PS_MAIN_DRAGON_TAIL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);

	
	///* 0 ~ 1 */
    //float3 vPixelNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV).xyz;
	///* -1 ~ 1 */
    //vPixelNormal = vPixelNormal * 2.f - 1.f;
    //float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);
    //vPixelNormal = mul(vPixelNormal, WorldMatrix);
    //Out.vNormal = vector(vPixelNormal * 0.5f + 0.5f, 0.f);
	
    Out.vDiffuse.rgb = float3(0.85f, 1.f, 1.f);
    Out.vGlow = vector(0.f, 0.f, 1.f, Out.vDiffuse.a);

	Out.vDiffuse *= g_vSourColor;
	Out.vGlow *= g_vSourColor;


	if (g_bIsDissolve)
	{
		vector vDissolveNoiseTexture = g_DissolveNoiseTexture.Sample(LinearClampSampler, In.vTexUV);
		vector vDissolveColorTexture = g_DissolveColorTexture.Sample(LinearClampSampler, In.vTexUV);

		if (g_fTimeAccDissolve + g_fDissolveStrength > vDissolveNoiseTexture.r)
			Out.vDiffuse.rgb = vDissolveColorTexture.rgb * g_vDissolveColor.rgb;

		if (g_fTimeAccDissolve > vDissolveNoiseTexture.r)
			discard;
	}
    
    
	if (Out.vDiffuse.a < 0.1f)
		discard;

	return Out;	
}

struct PS_OUT_DEPTH
{
    vector vDepth : SV_TARGET0;
};

PS_OUT_DEPTH PS_DEPTH(PS_IN In)
{
    PS_OUT_DEPTH Out = (PS_OUT_DEPTH) 0;

    if (0.1f > g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV).a)
        discard;
    
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);
    
    
    return Out;
}

technique11 DefaultTechnique
{
	pass Default
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_CullModeNone);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass WireFrame
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Wireframe);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Dragon_Tail
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_CullModeNone);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DRAGON_TAIL();
	}

    pass Depth
    {
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DEPTH();
    }

    pass Weapon
    {
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_WEAPON();
    }
}