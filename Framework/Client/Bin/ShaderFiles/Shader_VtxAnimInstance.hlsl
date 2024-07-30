
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
float		g_fFar = 300.f;

vector		g_vRimLightColor = vector(0.6f, 0.f, 0.f, 1.f);

float		g_fDissolveAmount = 0.f;
float4		g_vDissolveColorMain = float4(1.f, 0.95f, 1.f, 1.f);
float4		g_vDissolveColorSub = float4(0.9f, 0.05f, 0.f, 1.f);

Texture2D	g_DissolveTexture;

sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

struct VS_IN_ANIM
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
	uint4		vBlendIndex : BLENDINDEX;
	float4		vBlendWeight : BLENDWEIGHT;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;
};

struct VS_OUT_ANIM
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

VS_OUT_ANIM VS_MAIN_ANIM(VS_IN_ANIM In)
{
	VS_OUT_ANIM	Out = (VS_OUT_ANIM)0;

	matrix			TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float			fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix			BoneMatrix = g_Bones.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_Bones.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_Bones.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_Bones.BoneMatrices[In.vBlendIndex.w] * fWeightW;

	vector		vBonPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	vector		vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);

	vector			vPosition = mul(vBonPosition, TransformMatrix);

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

struct PS_IN_ANIM
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

struct PS_OUT_ANIM
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
};

struct PS_OUT_RIMLIGHT
{
	float4		vDiffuse	: SV_TARGET0;
	float4		vNormal		: SV_TARGET1;
	float4		vDepth		: SV_TARGET2;
	float4		vRimLight	: SV_TARGET3;
};

PS_OUT_RIMLIGHT PS_MAIN_ANIM(PS_IN_ANIM In)
{
	PS_OUT_RIMLIGHT		Out = (PS_OUT_RIMLIGHT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);
	Out.vRimLight = vector(0.f, 0.f, 0.f, 0.f);

	if (Out.vDiffuse.a < 0.1f)
		discard;

	return Out;
}

PS_OUT_RIMLIGHT PS_MAIN_RIMLIGHT_NO_NORTEX(PS_IN_ANIM In)
{
	PS_OUT_RIMLIGHT Out = (PS_OUT_RIMLIGHT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse = saturate(Out.vDiffuse * 1.5f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vPosition.w / g_fFar, 0.f, 0.f);

	float3	vNormal = In.vNormal.xyz;

	float3	vCamDir = normalize(g_vCamPosition.xyz - In.vWorldPos.xyz);
	float	fRim = saturate(dot(vNormal, vCamDir));

	if (fRim > 0.3)
	{
		fRim = 1.f;
	}
	else
	{
		fRim = -1.f;
	}

	float	fRimLight = pow(1 - fRim, 5.f);
	//float	fRimLight = smoothstep(1.f - g_fRimLightWidth, 1.f, 1 - max(0.f, dot(vNormal, vCamDir)));
	vector	vRimLightColor = g_vRimLightColor * fRimLight;

	Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vRimLight = vRimLightColor;

	if (Out.vDiffuse.a < 0.1f)
		discard;

	return Out;
}

technique11 DefaultTechnique
{
	pass Anim
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_ANIM();
	}
	pass RimLight
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_ANIM();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_RIMLIGHT_NO_NORTEX();
	}
}