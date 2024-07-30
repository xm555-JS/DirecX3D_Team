
#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float		g_fFar = 80.f;

float		g_fRadius = 0.001f;
//float		g_fFalloff = 0.000002f;
//float		g_fStrength = 0.0007f;
//float		g_fTotStrength = 1.38f;
//float		g_fInvSamples = 1.f / 16.f;
float		g_fPower = 1.f;



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
	vector		vColor : SV_TARGET0;
};


float3 g_vRandom[16] =
{
	float3(0.2024537f, 0.841204f, -0.9060141f),
	float3(-0.2200423f, 0.6282339f, -0.8275437f),
	float3(0.3677659f, 0.1086345f, -0.4466777f),
	float3(0.8775856f, 0.4617546f, -0.6427765f),
	float3(0.7867433f, -0.141479f, -0.1567597f),
	float3(0.4839356f, -0.8253108f, -0.1563844f),
	float3(0.4401554f, -0.4228428f, -0.3300118f),
	float3(0.0019193f, -0.8048455f, 0.0726584f),
	float3(-0.7578573f, -0.5583301f, 0.2347527f),
	float3(-0.4540417f, -0.252365f, 0.0694318f),
	float3(-0.0483353f, -0.2527294f, 0.5924745f),
	float3(-0.4192392f, 0.2084218f, -0.3672943f),
	float3(-0.8433938f, 0.1451271f, 0.2202872f),
	float3(-0.4037157f, -0.8263387f, 0.4698132f),
	float3(-0.6657394f, 0.6298575f, 0.6342437f),
	float3(-0.0001783f, 0.2834622f, 0.8343929f),
};


struct tagSSAO_In
{
	float2 vUV;
	float fDepth;
	float fViewZ;
	vector vNormal;
};

struct tagSSAO_Out
{
	vector vAmbient;
};

vector randomNormal(float2 tex)
{
	float noiseX = (frac(sin(dot(tex, float2(15.8989f, 76.132f) * 1.0f)) * 46336.23745f));
	float noiseY = (frac(sin(dot(tex, float2(11.9899f, 62.223f) * 2.0f)) * 34748.34744f));
	float noiseZ = (frac(sin(dot(tex, float2(13.3238f, 63.122f) * 3.0f)) * 59998.47362f));
	return normalize(vector(noiseX, noiseY, noiseZ, 0.f));
}

tagSSAO_Out Get_SSAO(tagSSAO_In In)
{
	tagSSAO_Out Out = (tagSSAO_Out) 0.f;

	vector vRay;
	vector vReflect;
	float2 vRandomUV;
	float fOccNorm;

	int iColor = 0;

    [loop]
    for (int i = 0; i < 32; ++i)
	{
		vRay = reflect(randomNormal(In.vUV), vector(g_vRandom[i / 2], 0.f));
		vReflect = normalize(reflect(vRay, In.vNormal)) * g_fRadius;
		//vReflect = normalize(vRay) * g_fRadius;
		//vReflect.x *= -1.f;
		vRandomUV = In.vUV + vReflect.xy;
		fOccNorm = g_DepthTexture.Sample(LinearSampler, vRandomUV).x * In.fViewZ;
		if (fOccNorm <= In.fDepth + 0.0003f)
			++iColor;

	}

	Out.vAmbient = abs((iColor / 32.f) - 1);
	return Out;

}

PS_OUT PS_MAIN_SSAO(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexUV);
	vector		vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	float		fViewZ = vDepthDesc.y * g_fFar;

	if (vNormalDesc.a != 0.f)
	{
		Out.vColor = vector(1.f, 1.f, 1.f, 1.f);
		return Out;
	}

	//vNormalDesc = normalize(vNormalDesc * 2.f - 1.f);


	tagSSAO_In SSAO_In = (tagSSAO_In)0;
	SSAO_In.vUV = In.vTexUV;
	SSAO_In.vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
	SSAO_In.fViewZ = fViewZ;
	SSAO_In.fDepth = vDepthDesc.x * fViewZ;

	tagSSAO_Out SSAO_Out = Get_SSAO(SSAO_In);

	Out.vColor = (1.f - vector(SSAO_Out.vAmbient.xyz, 0.f)) * g_fPower;

	return Out;
}

technique11 DefaultTechnique
{
	pass SSAO
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SSAO();
	}
}