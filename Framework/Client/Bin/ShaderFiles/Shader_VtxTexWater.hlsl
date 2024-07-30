
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float	g_fFar;

vector	g_vBrushPos = vector(15.f, 0.f, 15.f, 1.f);
float	g_fBrushRange = 10.f;

float	g_fDistortionStrength = 10.f;
float   g_fDistortionPower = 10.f;



float2	g_vOffset;

float2 g_vUVSpeed_0;
float2 g_vUVSpeed_1;
float2 g_vUVSpeed_2;
float2 g_vUVSpeed_3;

float g_fUVScale_0;
float g_fUVScale_1;
float g_fUVScale_2;
float g_fUVScale_3;

//texture2D   g_DiffuseTexture;
texture2D   g_NormalTexture0;
texture2D   g_NormalTexture1;
texture2D   g_NormalTexture2;
texture2D   g_NormalTexture3;

//texture2D   g_DistortionTexture0;
//texture2D   g_DistortionTexture1;

//texture2D   g_DepthTexture;

float3  g_vNormal;
float3  g_vTangent;
float3  g_vBinormal;


// vector	g_vMtrlDiffuse;
//texture2D	g_SourDiffTexture;
//texture2D	g_DestDiffTexture;
//texture2D	g_BrushTexture;




//sampler DefaultSampler = sampler_state
//{
//    filter = min_mag_mip_linear;
//    AddressU = wrap;
//    AddressV = wrap;
//};



struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT_WATER
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};


VS_OUT_WATER VS_MAIN_WATER_SURFACE(VS_IN In)
{
    VS_OUT_WATER Out = (VS_OUT_WATER) 0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN_WATER
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

struct PS_OUT
{
	//vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET0;
	//vector		vDepth : SV_TARGET2;
    vector      vFlag : SV_TARGET1;
    vector      vDepth : SV_TARGET2;
    //vector      vDistortion : SV_TARGET3;
};

PS_OUT PS_MAIN_WATER_SURFACE(PS_IN_WATER In)
{
	PS_OUT		Out = (PS_OUT)0;

	//vector		vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV * 200.f);
    //vector      vNormal_0 = g_NormalTexture0.Sample(DefaultSampler, In.vTexUV * 200.f + g_vUVSpeed_0);
    float3 vNormal_0 = g_NormalTexture0.Sample(LinearSampler, (In.vTexUV + g_vUVSpeed_0) * g_fUVScale_0);
    float3 vNormal_1 = g_NormalTexture1.Sample(LinearSampler, (In.vTexUV + g_vUVSpeed_1) * g_fUVScale_1);
    float3 vNormal_2 = g_NormalTexture2.Sample(LinearSampler, (In.vTexUV + g_vUVSpeed_2) * g_fUVScale_2);
    float3 vNormal_3 = g_NormalTexture3.Sample(LinearSampler, (In.vTexUV + g_vUVSpeed_3) * g_fUVScale_3);
   // Out.vDiffuse = vDiffuse;
    
    //float4 vDistortion_0 = g_DistortionTexture0.Sample(LinearSampler, (In.vTexUV + g_vUVSpeed_0) * g_fUVScale_0);
    //float4 vDistortion_1 = g_DistortionTexture1.Sample(LinearSampler, (In.vTexUV + g_vUVSpeed_1) * g_fUVScale_1);
    
    
	//vector		vRDiffuse = g_RDiffTexture.Sample(DefaultSampler, In.vTexUV * 200.f);
	//vector		vGDiffuse = g_GDiffTexture.Sample(DefaultSampler, In.vTexUV * 200.f);
	//vector		vBDiffuse = g_BDiffTexture.Sample(DefaultSampler, In.vTexUV * 200.f);
	//vector		vBaseDiffuse = g_BaseDiffTexture.Sample(DefaultSampler, In.vTexUV * 200.f);
	
    float3 vNormalDesc = (vNormal_0 + vNormal_1 + vNormal_2 + vNormal_3) / 4.f;
    float3 vNormal = vNormalDesc;
    vNormal = vNormal * 2.f - 1.f;
    
    float3x3 WorldMatrix = float3x3(g_vTangent, g_vBinormal, g_vNormal);
    vNormal = mul(vNormal, WorldMatrix);
    Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);

    
    
    //Out.vDiffuse.r = 0.0f;
    //Out.vDiffuse.g = 0.0f;
    //Out.vDiffuse.b = 0.2f;
    //Out.vDiffuse.a = 0.1f;
    
    //float4 vDistortionDesc = (vDistortion_0 + vDistortion_1) / 2.f;
    
    
    
    
    
    // 반사율
    Out.vFlag.a = 0.2f;
    
    // 반사각
    Out.vFlag.r = 0.05f;
    Out.vFlag.g = 0.35f;
    
	/* -1 -> 0, 1 -> 1*/
	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
    //Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 1.f);
    //Out.vNormal = vector(vNormal_0.xyz * 0.5f + 0.5f, 1.f);
    //Out.vNormal = vector(vNormal.xyz, 1.f);
    //Out.vNormal = vNormal_0;

	/* x : 0 ~ 1 */
	/* y : n ~ f 정규화하여 */
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);

    //vector vDistortion = Out.vNormal;
    //vDistortion.r = 0.5f + (vDistortion.r - 0.5f) * g_fDistortionPower;
    
    

    //Out.vDistortion = vDistortionDesc;
    //Out.vDistortion.a = 1.f;
    //Out.vDistortion.g = 1.f / g_fDistortionStrength;
    
    
	return Out;
}

struct PS_OUT_BASE
{
    vector vDiffuse : SV_TARGET0;
    vector vNormal : SV_TARGET1;
    vector vDepth : SV_TARGET2;
    vector vGlow : SV_TARGET3;
    vector vBloom : SV_TARGET4;
};

PS_OUT_BASE PS_MAIN_WATER_BASE(PS_IN_WATER In)
{
    PS_OUT_BASE Out = (PS_OUT_BASE) 0;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);

	return Out;
}

technique11 DefaultTechnique
{
	pass Water
	{
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		//SetRasterizerState(RS_Wireframe);
		SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN_WATER_SURFACE();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_WATER_SURFACE();
    }

    pass Water_Base
    {
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN_WATER_SURFACE();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_WATER_BASE();
    }
}