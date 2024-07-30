
#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float		g_fFar;

float       g_fLinearStep = 1.f;

vector      g_vCamPosition;
vector      g_vBaseColor;

matrix		g_CamViewMatrixInv, g_CamProjMatrixInv;
matrix		g_CamViewMatrix, g_CamProjMatrix;

texture2D	g_DepthTexture;
texture2D	g_NormalTexture;
texture2D   g_SSRflagTexture;
texture2D   g_DiffuseTexture;


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


PS_OUT PS_MAIN_SSR(PS_IN In)
{

	PS_OUT			Out = (PS_OUT)0;

	vector			vDepth = g_DepthTexture.Sample(LinearSampler, In.vTexUV);
	vector			vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	vector			vSSRflagDesc = g_SSRflagTexture.Sample(LinearSampler, In.vTexUV);
    
	float			fViewZ = vDepth.y * g_fFar;
    vector          vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

    if (0.f == vSSRflagDesc.a)
		discard;
    
	if (0 == fViewZ)
		discard;
   
	vector			vWorldPos;

	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepth.x;
	vWorldPos.w = 1.0f;

	vWorldPos *= fViewZ;
    
    vWorldPos = mul(vWorldPos, g_CamProjMatrixInv);
    vWorldPos = mul(vWorldPos, g_CamViewMatrixInv);
    
    vector vLook = normalize(vWorldPos - g_vCamPosition);
    vector vReflect = normalize(reflect(vLook, vNormal));
    
    float fDotAtt = dot(vReflect, normalize(vNormal));
    if ((vSSRflagDesc.r > fDotAtt)  ||
        (vSSRflagDesc.g < fDotAtt))
        discard;
 
    Out.vColor = g_vBaseColor;
    
    [loop]
    for (int i = 1; i < 50; ++i)
    {
        vector vTargetPos = vWorldPos + vReflect * g_fLinearStep * i;

        float2 vTargetUV;
        vTargetPos = mul(vTargetPos, g_CamViewMatrix);
        vTargetPos = mul(vTargetPos, g_CamProjMatrix);
        
        vTargetPos /= vTargetPos.w;
        float fTargetDepth = vTargetPos.z;
        
        vTargetUV.x = (vTargetPos.x + 1.f) * 0.5f;
        vTargetUV.y = (vTargetPos.y - 1.f) * -0.5f;

        float fBias = 0.003f;
        if (1.f - fBias < vTargetUV.x ||
		1.f - fBias < vTargetUV.y ||
		0.f + fBias > vTargetUV.x ||
		0.f + fBias > vTargetUV.y)
        {
            Out.vColor = g_vBaseColor;
            break;
        }
        
        //fTargetDepth -= fBias / g_fFar;
        vector vUVDepth = g_DepthTexture.Sample(LinearSampler, vTargetUV);
        
        // Sky Box
        if (1.f == vUVDepth.y)
        {
            Out.vColor = g_vBaseColor;
            break;
        }

        
        if (fTargetDepth > vUVDepth.x &&
            fTargetDepth > vDepth.x)
        {
            Out.vColor = g_DiffuseTexture.Sample(LinearSampler, vTargetUV);
            Out.vColor.a = vSSRflagDesc.a;
            break;
        }
    }
    
	return Out;
}



technique11 DefaultTechnique
{
    pass SSR
    {
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SSR();
    }
}