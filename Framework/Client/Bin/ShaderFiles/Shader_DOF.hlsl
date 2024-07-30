
//#include "Engine_Shader_Defines.hpp"
#include "Engine_Shader_Function.hlsl"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float		g_fFar;

float		g_fPower = 1.f;


texture2D	g_BlurTexture;
texture2D	g_DepthTexture;
//texture2D	g_NormalTexture;

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


PS_OUT PS_MAIN_DOF(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

    
    
    //Out.vColor = Get_Blur();
    
	return Out;
}

technique11 DefaultTechnique
{
	pass DOF
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DOF();
    }
}