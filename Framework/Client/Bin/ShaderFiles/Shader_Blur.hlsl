
#include "Engine_Shader_Function.hlsl"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

//float		g_fWinCX = 1280.f;
//float		g_fWinCY = 720.f;
float		g_fPower = 1.f;
float		g_fRange = 1.f;

float       g_fBlurStart = 1.f;
float       g_fBlurWidth = 0.1f;

float2      g_vPoint;

texture2D	g_BlurTexture;

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
	vector		vColor : SV_TARGET0;
};


PS_OUT PS_MAINX(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float2	vTexUV = 0;
	float	UVDiff = 1.f / g_fWinCX * g_fRange;

    [loop]
    for (int i = -6; i < 6; ++i)
	{
		vTexUV = In.vTexUV + float2(UVDiff * i, 0);
		Out.vColor += fWeight[6 + i] * g_BlurTexture.Sample(LinearClampSampler, vTexUV);
	}

	Out.vColor = Out.vColor / fTotal * g_fPower;

	return Out;
}


PS_OUT PS_MAINY(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float2	vTexUV = 0;
	float	UVDiff = 1.f / g_fWinCY * g_fRange;

    [loop]
    for (int i = -6; i < 6; ++i)
	{
		vTexUV = In.vTexUV + float2(0, UVDiff * i);
		Out.vColor += fWeight[6 + i] * g_BlurTexture.Sample(LinearClampSampler, vTexUV);
	}

	Out.vColor = Out.vColor / fTotal * g_fPower;

	return Out;
}

int         g_iPower = 35;
//          Sigma 는 Power / 4 로 넣어준다 
float       g_fSigma = 35 * 0.25f;

float gaussian(float2 vUV)
{
    return exp(-0.5f * dot(vUV /= g_fSigma, vUV)) / (6.28f * g_fSigma * g_fSigma);
}

float4 blur(texture2D BlurTexture, float2 vUV, int2 iResolution)
{
    float4 vColor = 0;
    //float2 vInUV = vUV / iResolution;
    float2 vScale = 1.f / iResolution;
    
    [loop]
    for (int i = 0; i < g_iPower * g_iPower; i++)
    {
        float2 vTargetUV = float2(i % g_iPower, i / g_iPower) - float(g_iPower) / 2.f;
        float fGaussian = gaussian(vTargetUV);
        vector vBlurDesc = BlurTexture.Sample(LinearClampSampler, vUV + vTargetUV * vScale);
        
        vColor += fGaussian * vBlurDesc;
    }
    
    return vColor/* / vColor.a*/;
}

PS_OUT PS_MAINXY(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
#if defined(_BLUR)
    
    int2 iResolution = { g_fWinCX, g_fWinCY };
    
    Out.vColor = blur(g_BlurTexture, In.vTexUV, iResolution.xy);
    
#else
    
    Out.vColor = g_BlurTexture.Sample(LinearClampSampler, In.vTexUV);
    
#endif
    
    return Out;
}

PS_OUT PS_MAIN_RADIAL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
#if defined(_BLUR)
    
    //int2 iResolution = { g_fWinCX, g_fWinCY };
    if (1 < g_iPower)
        Out.vColor = Get_RadialBlur(g_BlurTexture, In.vTexUV, g_iPower, g_vPoint, g_fBlurStart, g_fBlurWidth);
    else
        Out.vColor = g_BlurTexture.Sample(LinearClampSampler, In.vTexUV);
    
#else
    Out.vColor = g_BlurTexture.Sample(LinearClampSampler, In.vTexUV);
#endif
    
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

    pass Blur1Pass
    {
        SetBlendState(BS_AllColorBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAINXY();
    }

    pass RadialBlur
    {
        SetBlendState(BS_AllColorBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RADIAL();
    }
}