
#include "Engine_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector g_vColor;

struct VS_IN
{
    float3 vPosition : POSITION;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vWorldPos : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	
    vector vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix); // �������� ���� ��ǥ.
    Out.vWorldPos = vWorldPos;
	
    return Out;
}

// w�����⿬���� �����Ѵ�. (In ���������̽�)
// ����Ʈ ��ȯ. (In ����Ʈ(��������ǥ))

// �����Ͷ�����(�ȼ������� �����Ѵ�. )

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vWorldPos : TEXCOORD0;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	//PS_OUT		Out = (PS_OUT)0;

	//Out.vColor = g_vColor;

	//return Out;
	
	
    PS_OUT Out = (PS_OUT) 0;
	
    vector vMtrlDiffuse = { 0.f, 1.f, 0.f, 1.f };
    float iMaxHeight = 100.f;
    float value = min((In.vWorldPos.y) / iMaxHeight, 1.f);
    // 0 ~ 50 ������ ���̸� 0~1�� ġȯ.
    if (1 == g_vColor.a &&
		1 == g_vColor.g &&
		1 == g_vColor.b)
    {
        Out.vColor = g_vColor;
    }
    else
    {
        if (0 <= In.vWorldPos.y)
        {
            vMtrlDiffuse.r += value;
            vMtrlDiffuse.g -= value;
        }
        else
        {
            vMtrlDiffuse.b -= value;
            vMtrlDiffuse.g += value;
        }
    
        Out.vColor = vMtrlDiffuse;
    }

    return Out;
}

technique11 DefaultTechnique
{
    pass Default
    {
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}