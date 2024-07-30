
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D	g_DiffuseTexture;
texture2D	g_NormalTexture;

struct		tagBoneMatrix
{
	matrix		BoneMatrices[256];
};

tagBoneMatrix		g_Bones;
vector		g_vCamPosition;
float		g_fFar;
float       g_fAlphaRange;


vector		g_vRimLightColor = vector(0.6f, 0.f, 0.f, 1.f);

float		g_fDissolveAmount = 0.f;
float4		g_vDissolveColorMain = float4(1.f, 0.95f, 1.f, 1.f);
float4		g_vDissolveColorSub = float4(0.9f, 0.05f, 0.f, 1.f);

Texture2D	g_DissolveTexture;

texture2D	g_MaskTexture;
texture2D   g_NoiseTexture;

float		g_fDistortionStrength = 0.f;
float2		g_vUVSpeed_Mask;


sampler DefaultSampler = sampler_state 
{		
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

struct VS_IN_NONANIM
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;
};

struct VS_OUT_NONANIM
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

VS_OUT_NONANIM VS_MAIN_NONANIM(VS_IN_NONANIM In)
{
	VS_OUT_NONANIM	Out = (VS_OUT_NONANIM)0;

	matrix			TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector			vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);
	vector			vNormal = mul(vector(In.vNormal, 0.f), TransformMatrix);

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
	Out.vWorldPos = mul(vPosition, g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;	
}

// w나누기연산을 수행하낟. (In 투영스페이스)
// 뷰포트 변환. (In 뷰포트(윈도우좌표))

// 래스터라이즈(픽셀정볼르 생성한다. )

struct PS_IN_NONANIM
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;

};

struct PS_OUT_NONANIM
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
	vector		vGlow : SV_TARGET3;
	vector		vToonFlag :SV_TARGET4;
	vector		vShadeFlag : SV_TARGET5;
};

PS_OUT_NONANIM PS_MAIN_NONANIM(PS_IN_NONANIM In)
{
	PS_OUT_NONANIM		Out = (PS_OUT_NONANIM)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);

    if (g_NoiseTexture.Sample(DefaultSampler, In.vTexUV * 10.f).r > clamp(In.vProjPos.z * 0.4f, 0.f, 1.f))
        discard;
    
    if (Out.vDiffuse.a < 0.1f)
        discard;

	return Out;
}

PS_OUT_NONANIM PS_MAIN_GRASS(PS_IN_NONANIM In)
{
	PS_OUT_NONANIM		Out = (PS_OUT_NONANIM)0;

	float			fDistortionPower = 0.f;
	float2			vTexUV_Mask;

	vTexUV_Mask.x = In.vTexUV.x + g_vUVSpeed_Mask.x;
	vTexUV_Mask.y = In.vTexUV.y + g_vUVSpeed_Mask.y;

	vector			vDistortion = g_MaskTexture.Sample(DefaultSampler, vTexUV_Mask);

	if (0.f != vDistortion.g)
		fDistortionPower = g_fDistortionStrength / 1000.f;
	/*
	vector vDiffuse = g_DiffuseTexture.Sample(LinearClampSampler, In.vTexUV + vDistortion.r * fDistortionPower) * 1.f;
	*/

    if (g_NoiseTexture.Sample(DefaultSampler, In.vTexUV * 10.f).r > clamp(In.vProjPos.z * 0.4f, 0.f, 1.f))
        discard;
    
	vector			vDiffuseColor = g_DiffuseTexture.Sample(LinearClampSampler, In.vTexUV + vDistortion.r * fDistortionPower) /* * vNoise*/;

	Out.vDiffuse = vDiffuseColor;
	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vNormal = vector(float3(0.f, 1.f, 0.f) * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);
    Out.vShadeFlag.r = 1.f;
    
	if (Out.vDiffuse.a < 0.1f )
		discard;

	return Out;
}

struct PS_OUT_DEPTH
{
    vector vDepth : SV_TARGET0;
};

PS_OUT_DEPTH PS_DEPTH(PS_IN_NONANIM In)
{
    PS_OUT_DEPTH Out = (PS_OUT_DEPTH) 0;

    if (0.1f > g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV).a)
        discard;
    
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);
    
	
    ////(x - 0.9f) * 10.f
    //Out.vDepth.r = (In.vProjPos.z / In.vProjPos.w - 0.9f) * 10.f;
    //Out.vDepth.g = (In.vProjPos.w / g_fFar - 0.9f) * 10.f;
    //Out.vDepth.b = 0.f;
    //Out.vDepth.a = 1.f;
    

    return Out;
}

technique11 DefaultTechnique
{
	pass NonAnim
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_NONANIM();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NONANIM();
	}

    pass Depth
    {
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN_NONANIM();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DEPTH();
    }

	pass CullModeNone	//7
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_CullModeNone);

		VertexShader = compile vs_5_0 VS_MAIN_NONANIM();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NONANIM();
	}

	pass Grass
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_NONANIM();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_GRASS();
	}
}