
#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float		g_fWinCX = 1280.f;
float		g_fWinCY = 720.f;

texture2D	g_TargetTexture;
texture2D   g_LumninanceTexture;


static const float3 LUMINANCE = float3(0.2125f, 0.7154f, 0.0721f);
static const float  DELTA = 0.0001f;


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



PS_OUT PS_LUMNINANCE(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

    //float fLogLumSum = 0.f;
    
    //fLogLumSum += log(dot(g_TargetTexture.Sample(LinearClampSampler, In.vTexUV + float2(-1.f * g_fWinCX, -1.f * g_fWinCY)).rgb, LUMINANCE) + DELTA);
    //fLogLumSum += log(dot(g_TargetTexture.Sample(LinearClampSampler, In.vTexUV + float2(-1.f * g_fWinCX, 0.f * g_fWinCY)).rgb, LUMINANCE) + DELTA);
    //fLogLumSum += log(dot(g_TargetTexture.Sample(LinearClampSampler, In.vTexUV + float2(-1.f * g_fWinCX, 1.f * g_fWinCY)).rgb, LUMINANCE) + DELTA);
    //fLogLumSum += log(dot(g_TargetTexture.Sample(LinearClampSampler, In.vTexUV + float2(0.f * g_fWinCX, -1.f * g_fWinCY)).rgb, LUMINANCE) + DELTA);
    //fLogLumSum += log(dot(g_TargetTexture.Sample(LinearClampSampler, In.vTexUV + float2(0.f * g_fWinCX, 0.f * g_fWinCY)).rgb, LUMINANCE) + DELTA);
    //fLogLumSum += log(dot(g_TargetTexture.Sample(LinearClampSampler, In.vTexUV + float2(0.f * g_fWinCX, 1.f * g_fWinCY)).rgb, LUMINANCE) + DELTA);
    //fLogLumSum += log(dot(g_TargetTexture.Sample(LinearClampSampler, In.vTexUV + float2(1.f * g_fWinCX, -1.f * g_fWinCY)).rgb, LUMINANCE) + DELTA);
    //fLogLumSum += log(dot(g_TargetTexture.Sample(LinearClampSampler, In.vTexUV + float2(1.f * g_fWinCX, 0.f * g_fWinCY)).rgb, LUMINANCE) + DELTA);
    //fLogLumSum += log(dot(g_TargetTexture.Sample(LinearClampSampler, In.vTexUV + float2(1.f * g_fWinCX, 1.f * g_fWinCY)).rgb, LUMINANCE) + DELTA);
    
    //fLogLumSum /= 9.f;
    
    //Out.vColor = exp(fLogLumSum);

    Out.vColor = dot(g_TargetTexture.Sample(LinearClampSampler, In.vTexUV).rgb, LUMINANCE);
    
	return Out;
}

PS_OUT PS_DOWNSAMPLING(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float fSum = 0.f;
    
    //for (int i = 0; i < 4; ++i)
    //{
    //    for (int j = 0; j < 4; ++j)
    //    {
    //        float2 vTargetUV = { i - 1.5f, j - 1.5f };
    //        fSum += log(g_LumninanceTexture.Sample(LinearClampSampler, In.vTexUV + float2(vTargetUV.x / g_fWinCX, vTargetUV.y / g_fWinCY)).x + DELTA);
    //    }
    //}

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            float2 vTargetUV = { i - 0.5f, j - 0.5f };
            fSum += log(g_LumninanceTexture.Sample(LinearClampSampler, In.vTexUV + float2(vTargetUV.x / g_fWinCX, vTargetUV.y / g_fWinCY)).x + DELTA);
        }
    }
    
    
    Out.vColor = exp(fSum / 4.f);
    return Out;
}

PS_OUT PS_DOWNSAMPLING_END(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float fSum = 0.f;
    
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            float2 vTargetUV = { i - 1.5f, j - 1.5f };
            fSum += g_LumninanceTexture.Sample(LinearClampSampler, In.vTexUV + float2(vTargetUV.x * g_fWinCX, vTargetUV.y * g_fWinCY)).x;
        }
    }

    Out.vColor = exp(fSum / 16.0f);
    return Out;
}


technique11 DefaultTechnique
{
    pass Lumninance
    {
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_LUMNINANCE();
	}

    pass DOWNSAMPLING
    {
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DOWNSAMPLING();
    }

    pass DOWNSAMPLING_END
    {
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DOWNSAMPLING_END();
    }
}