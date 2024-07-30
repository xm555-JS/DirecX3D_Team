
#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float		g_fWinCX = 1280.f;
float		g_fWinCY = 720;
float		g_fPower = 1.f;
float		g_fRange = 1.f;
float		g_fFar = 300.f;


texture2D	g_BlurTexture;
texture2D	g_DepthTexture;



float fWeight[13] =
{
	0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231,
	1, 0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561
};

float fTotal = 6.2108;

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
	vector		vGlow_Blur : SV_TARGET0;
	vector		vGlow_Depth : SV_TARGET1;
};


PS_OUT PS_MAINX(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float2	vTexUV = 0;
	float	UVDiff = 1.f / g_fWinCX * g_fRange;


	float	fMaxDepth = 1.f;
	float	fTargetAlpha = 0.f;
    
    [loop]
    for (int i = -6; i < 6; ++i)
	{
		vTexUV = In.vTexUV + float2(UVDiff * i, 0);
		vector vTargetBlurColor = fWeight[6 + i] * g_BlurTexture.Sample(LinearClampSampler, vTexUV);
		Out.vGlow_Blur += vTargetBlurColor;

		vector	vDepth = g_DepthTexture.Sample(LinearClampSampler, vTexUV);
		if (0.f == vDepth.y)
			continue;

		if (fMaxDepth > vDepth.y)
		{
			fMaxDepth = vDepth.y;
			fTargetAlpha = vTargetBlurColor.a;
		}

		//fMaxDepth = min(fMaxDepth, vDepth.y);
		
	}

	Out.vGlow_Blur = Out.vGlow_Blur / fTotal * g_fPower;
	Out.vGlow_Depth.y = fMaxDepth;
	Out.vGlow_Depth.a = fTargetAlpha;

		
	return Out;
}


PS_OUT PS_MAINY(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float2	vTexUV = 0;
	float	UVDiff = 1.f / g_fWinCY * g_fRange;

	float	fMaxDepth = 1.f;
	float	fTargetAlpha = 0.f;
    
    [loop]
    for (int i = -6; i < 6; ++i)
	{
		vTexUV = In.vTexUV + float2(0, UVDiff * i);
		vector vTargetBlurColor = fWeight[6 + i] * g_BlurTexture.Sample(LinearClampSampler, vTexUV);
		Out.vGlow_Blur += vTargetBlurColor;

		vector	vDepth = g_DepthTexture.Sample(LinearClampSampler, vTexUV);
		if (0.f == vDepth.y)
			continue;

		if (fMaxDepth > vDepth.y)
		{
			fMaxDepth = vDepth.y;
			fTargetAlpha = vTargetBlurColor.a;
		}

		//fMaxDepth = min(fMaxDepth, vDepth.y);
	}

	Out.vGlow_Blur = Out.vGlow_Blur / fTotal * g_fPower;
	Out.vGlow_Depth.y = fMaxDepth;
	Out.vGlow_Depth.a = fTargetAlpha;

	return Out;
}


technique11 DefaultTechnique
{
	pass BlurX
	{
		SetBlendState(BS_AllColorBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAINX();
	}

	pass BlurY
	{
		SetBlendState(BS_AllColorBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAINY();
	}
}