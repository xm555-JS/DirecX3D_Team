
#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float		g_fWinCX = 1280.f;
float		g_fWinCY = 720.f;
float		g_fFar;

float		g_fPower = 1.f;
float		g_fThickness = 2.0f;
float       g_fLineBrightness = -0.2f;


float		g_fMaxDepthDiff = 1.0f;
float       g_fDepthDiff = 1.0f;

texture2D	g_DepthTexture;
texture2D	g_FlagTexture;

float	mask[9] = {
	+1, +1, +1,
	+1, +1, +1,
	+1, +1, +1 };
float coord[3] = { -1, 0, 1 };


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

	float	fViewZ = 0.f;
	float	fView = 0.f;
	float	fDeviation = 0.2f;
	float	fPositionViewZ = g_DepthTexture.Sample(LinearSampler, In.vTexUV).y * g_fFar;
	int		iCount = 9;

    [loop]
    for (int i = 0; i < 9; i++)
	{
		fView = mask[i] * g_DepthTexture.Sample(LinearSampler, In.vTexUV + float2(coord[i % 3] / g_fWinCX * 7.f, coord[i / 3] / g_fWinCY * 7.f)).y * g_fFar;
		if (fDeviation < (fPositionViewZ - fView))
			iCount -= 1;
		else
			fViewZ += fView;
	}

	fViewZ /= iCount;

	float	fAtt = (fPositionViewZ - fViewZ) * g_fPower;

	Out.vColor.rgb = 1.f - fAtt;
	Out.vColor.a = 1.f;

	return Out;
}


PS_OUT PS_MAIN_TARGET(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vColor = g_FlagTexture.Sample(LinearSampler, In.vTexUV);
    vector vFinalColor = vector(0.f, 0.f, 0.f, 0.f);

    [loop]
    for (int i = 0; i < 9; i++)
    {
        vector vTargetColor = g_FlagTexture.Sample(LinearSampler, In.vTexUV + float2(coord[i % 3] / g_fWinCX * g_fThickness, coord[i / 3] / g_fWinCY * g_fThickness)).y;
	
        if (0.f == vColor.a && 0.f != vTargetColor.a)
            vFinalColor = vTargetColor;
    }

    Out.vColor = vFinalColor;

    return Out;
}

PS_OUT PS_MAIN_UNIT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vColor = g_FlagTexture.Sample(LinearSampler, In.vTexUV);
    vector vDepth = g_DepthTexture.Sample(LinearSampler, In.vTexUV);
    vector vFinalColor = vector(0.f, 0.f, 0.f, 0.f);
    float fViewZ = vDepth.y * g_fFar;

    [loop]
    for (int i = 0; i < 9; i++)
    {
        float2 vTargetUV = In.vTexUV + float2(coord[i % 3] / g_fWinCX * g_fThickness, coord[i / 3] / g_fWinCY * g_fThickness);
        vector vTargetDepth = g_DepthTexture.Sample(LinearSampler, vTargetUV).y;
        float fTargetViewZ = vTargetDepth.y * g_fFar;
        float fTargetDepthDiff = fViewZ - fTargetViewZ;
        
        if (g_fMaxDepthDiff < fTargetDepthDiff)
            continue;
        
        if (g_fDepthDiff < fTargetDepthDiff)
        {
            vector vTargetColor = g_FlagTexture.Sample(LinearSampler, vTargetUV);
            
            if (0.f == vTargetColor.a)
                continue;
            
            vFinalColor = vTargetColor/* * g_fLineBrightness*/;
            break;
        }
        //if (0.f == vColor.a && 0.f != vTargetColor.a)
            
    }

    Out.vColor = vFinalColor;

    return Out;
}


technique11 DefaultTechnique
{
	pass Outline
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

	pass Target_Outline
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TARGET();
	}

    pass Unit_Outline
    {
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_UNIT();
    }
}