
#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

matrix		g_ViewMatrixInv;
matrix		g_ProjMatrixInv;

vector		g_vCamPosition;

vector		g_vLightDir;
vector		g_vLightPos;
vector		g_vLightLook = vector(0.f, -1.f, 0.f, 0.f);
vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

float		g_fInLightRange;
float		g_fOutLightRange;
float		g_fRange;


float		g_fFar = 80.f;



texture2D	g_DepthTexture;
texture2D	g_NormalTexture;

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
	vector		vShade : SV_TARGET0;
	vector		vSpecular : SV_TARGET1;
};

PS_OUT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)1;

	vector			vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	vector			vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexUV);
	float			fViewZ = vDepthDesc.y * g_fFar;
	/* 0 -> -1, 1 -> 1*/
	vector			vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	Out.vShade = g_vLightDiffuse * saturate(saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient));
	Out.vShade.a = 1.f;

	vector			vReflect = reflect(normalize(g_vLightDir), vNormal);

	vector			vWorldPos;

	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.0f;

	vWorldPos *= fViewZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector			vLook = normalize(vWorldPos - g_vCamPosition);

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 30.f);
	//Out.vSpecular.a = 0.f;


	return Out;
}


PS_OUT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)1;

	vector			vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	vector			vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexUV);
	float			fViewZ = vDepthDesc.y * g_fFar;
	/* 0 -> -1, 1 -> 1*/
	vector			vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	vector			vWorldPos;

	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.0f;

	vWorldPos *= fViewZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector			vLightDir = vWorldPos - g_vLightPos;

	float			fDistance = length(vLightDir);

	float			fAtt = saturate((g_fRange - fDistance) / g_fRange);

	Out.vShade = g_vLightDiffuse * saturate(saturate(dot(normalize(vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;
	Out.vShade.a = 1.f;

	vector			vReflect = reflect(normalize(vLightDir), vNormal);
	vector			vLook = normalize(vWorldPos - g_vCamPosition);

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 30.f) * fAtt * 0.3f;
	Out.vSpecular.a = 0.f;


	return Out;
}


PS_OUT PS_MAIN_SPOT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)1;

	vector			vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	vector			vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexUV);
	float			fViewZ = vDepthDesc.y * g_fFar;
	/* 0 -> -1, 1 -> 1*/
	vector			vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	vector			vWorldPos;


	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.0f;

	vWorldPos *= fViewZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);



	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector			vLightDir = normalize(vWorldPos - g_vLightPos);
	float			fCurLightRange = dot(vLightDir, g_vLightLook);


	float			fAtt = saturate((fCurLightRange - g_fOutLightRange) / (g_fInLightRange - g_fOutLightRange));
	fAtt = saturate(-0.5f * (cos(3.14f * fAtt) - 1.f));


	Out.vShade = g_vLightDiffuse * saturate(saturate(dot(normalize(vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt * 0.5f;
	Out.vShade.a = 1.f;

	vector			vReflect = reflect(normalize(vLightDir), vNormal);
	vector			vLook = normalize(vWorldPos - g_vCamPosition);

	//Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 30.f) * fAtt * 0.3f;
	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 5.f) * fAtt * 0.3f;
	Out.vSpecular.a = 0.f;


	return Out;
}


technique11 DefaultTechnique
{
	pass Light_Directional
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
	}

	pass Light_Point
	{
		SetBlendState(BS_AllColorBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_POINT();
	}

	pass Light_Spot
	{
		SetBlendState(BS_AllColorBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SPOT();
	}
}