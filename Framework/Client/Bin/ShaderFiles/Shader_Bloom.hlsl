
#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float		g_fWinCX = 1280.f;
float		g_fWinCY = 720;
float		g_fPower = 1.f;

texture2D	g_BloomTexture;
texture2D	g_BloomBlurTexture;
texture2D	g_FinalTexture;



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


PS_OUT PS_MAIN_BLOOMABLE(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float4 vBrightColor = 0.f;
	float4 vFragColor = g_FinalTexture.Sample(LinearSampler, In.vTexUV);

	// light weight (0.2126f, 0.7152f, 0.0722f)
	float fBrightness = dot(vFragColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));
	if (fBrightness > 0.99)
		vBrightColor = float4(vFragColor.rgb, 0.f);

	Out.vColor = vBrightColor;
	return Out;
}


PS_OUT PS_MAIN_BLOOM(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float4 vFinalColor = g_FinalTexture.Sample(LinearSampler, In.vTexUV) * 1.f;
	float4 vBloomColor = g_BloomBlurTexture.Sample(LinearSampler, In.vTexUV) * g_fPower;
	float4 vBloomOriTex = g_BloomTexture.Sample(LinearSampler, In.vTexUV) * 1.f;


	//float4 vBloom = pow(pow(abs(vBloomColor), 2.2f) + pow(abs(vBloomOriTex), 2.2f), 1.f / 2.2f);
	float4 vBloom = pow(pow(abs(vBloomColor), 2.2f) + pow(abs(vBloomOriTex), 2.2f), 1.f / 2.2f);

	float4 vColor = (vFinalColor);

	vColor = pow(abs(vColor), 2.2f);
	vBloom = pow(abs(vBloom), 2.2f);

	vColor += vBloom * 2.2f;

	Out.vColor = pow(abs(vColor), 1 / 2.2f);


	//if (0.f == Out.vColor.a)
	//	discard;

	return Out;
}


technique11 DefaultTechnique
{
	pass Bloomable
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLOOMABLE();
	}

	pass Bloom
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLOOM();
	}
}