
#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D	g_CopyTexture;

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


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector vCopyColor = g_CopyTexture.Sample(LinearClampSampler, In.vTexUV);

	Out.vColor = vCopyColor;

	return Out;
}


technique11 DefaultTechnique
{
	pass Copy
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}









///*
//simple.HDR | main file
//________________________________________________________________________________
//*/
//
///* Includes */
//
//// Include ReShade library
//#include "ReShade.fxh"
//
//// Turn off full ACES support for now, opting for BakingLab's approx.
//// DON'T TOUCH THIS, FULL ACES SUPPORT IS NOT IMPLEMENTED
//#ifndef _SIMPLE_HDR_ACES_APPROX
//#define _SIMPLE_HDR_ACES_APPROX 1
//#endif
//
//// Include ACES (thx to Unity FPSSample & BakingLab by MJP)
//#include "ACES.fxh"
//
//// Include UI file.
//#include "simple.HDR.UI.fxh"
//
///* HDR */
//
//#ifndef SIMPLE_HDR_SRGB
//#   if (BUFFER_BIT_COLOR_DEPTH < 10)
//#       define SIMPLE_HDR_SRGB 1
//#   else
//#       define SIMPLE_HDR_SRGB 0
//#   endif
//#endif
//
//static const float3x3 ACESInputMat = float3x3(
//	0.59719, 0.35458, 0.04823,
//	0.07600, 0.90834, 0.01566,
//	0.02840, 0.13383, 0.83777
//	);
//
//// RRT => ODT
//float3 RRTAndODTFit(float3 v)
//{
//	float3 a = v * (v + 0.0245786f) - 0.000090537f;
//	float3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
//	return a / b;
//}
//
//
//// sRGB => XYZ => D65_2_D60 => AP1 => RRT_SAT
//static const float3x3 ACESInputMat = float3x3(
//	0.59719, 0.35458, 0.04823,
//	0.07600, 0.90834, 0.01566,
//	0.02840, 0.13383, 0.83777
//	);
//
//
//sampler2D BackBuffer
//{
//	// Set our backbuffer to be ReShade's supply.
//	Texture = ReShade::BackBufferTex;
//
//// We don't need scaling, so set to POINT to ensure SGSSAA compatibility.
//MagFilter = POINT;
//MinFilter = POINT;
//MipFilter = POINT;
//
//// Convert sRGB backbuffer to linear sRGB
//SRGBTexture = SIMPLE_HDR_SRGB;
//};
//
//float3 HDRPS
//(
//	in float4 vpos     : SV_POSITION,
//	in float2 texcoord : TEXCOORD0
//) : SV_TARGET
//{
//	// Don't process if ACES mix is zero.
//	if (ACESMix == 0.0) discard;
//
//// Sample backbuffer texture
//float3 res = tex2D(BackBuffer, texcoord.xy).rgb;
//
//// Copy backbuffer texture
//float3 color = res;
//
//// Convert linear sRGB to RRT
//color = mul(ACESInputMat, color);
//
//// Convert RRT to ODT
//color = RRTAndODTFit(color);
//
//// Convert ODT back to linear sRGB
//color = mul(ACESOutputMat, color);
//
//// Clamp to [0, 1]
//color = saturate(color);
//
//// Apply multiplier to color to correct exposure
//color *= 1.8f;
//
//// Copy back color to backbuffer
//res = lerp(res, color, ACESMix);
//
//// Output result
//return res;
//}
//
//technique simpleHDR < ui_label = "simple.HDR"; >
//{
//	pass
//	{
//		VertexShader = PostProcessVS;
//		PixelShader = HDRPS;
//
//		// Convert linear texture to sRGB
//		SRGBWriteEnable = SIMPLE_HDR_SRGB;
//	}
//}