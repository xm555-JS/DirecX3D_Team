
#include "Engine_Shader_Defines.hpp"

float g_fWinCX = 1280.f;
float g_fWinCY = 720.f;

struct tagDissolve_In
{
    vector vDiffuse;
    vector vMask;
    vector vFlagColor;
    float fPassedTime;
    float fTotalTime;
};

vector Get_Dissolve(tagDissolve_In In)
{
    vector Out = 0.f;

   
    vector vColor = In.vMask * In.vFlagColor;
    vector vStartPoint = { 0.f, 0.f, 0.f, 0.f };
    
    //vColor = (vColor - vStartPoint) * In.fPassedTime / In.fTotalTime + vStartPoint;
    vColor *= In.fPassedTime / In.fTotalTime;
    
    
    Out = saturate(In.vDiffuse - vColor);
    
    //vector Test = { 0.f, 0.f, 0.f, 0.f };
    //Out = In.vMask;
    
    return Out;
}

struct tagRimLight_In
{
    float3 vNormal;
    float3 vLook;
    //vector vCamPos;
    //vector vTargetPos;
    vector vFlagColor;
    float  fInterval;
};

vector Get_RimLight(tagRimLight_In In)
{
    vector Out = 0.f;

    float fAtt = 1.f - dot(In.vLook, In.vNormal);
    fAtt = pow(fAtt, In.fInterval);
    Out = saturate(In.vFlagColor * fAtt);
    
    return Out;
}

// Sigma의 경우 Power * 0.25f 로 넣어준다. 그게 무난
float Get_Gaussian(float2 vUV, float fSigma)
{
    return exp(-0.5f * dot(vUV /= fSigma, vUV)) / (6.28f * fSigma * fSigma);
}

// 주의해서 사용할 것 iPower 의 값을 적당한 값으로 넣어주거나 min 키워드를 통해 최소한의 값을 만들어 주는 것이 좋음
float4 Get_Blur(texture2D BlurTexture, float2 vUV, int2 iResolution, int iPower)
{
#if defined(_BLUR)
    
    float4 vColor = 0;
    //float2 vInUV = vUV / iResolution;
    float2 vScale = 1.f / iResolution;
    float fSigma = float(iPower) * 0.25f;
    
    //[unroll(1000)] 
    [loop]
    for (int i = 0; i < iPower * iPower; i++)
    {
        float2 vTargetUV = float2(i % iPower, i / iPower) - float(iPower) / 2.f;
        float fGaussian = Get_Gaussian(vTargetUV, fSigma);
        vector vBlurDesc = BlurTexture.Sample(LinearClampSampler, vUV + vTargetUV * vScale);
        
        vColor += fGaussian * vBlurDesc;
    }
    
    return vColor /* / vColor.a*/;
    
#endif
}

// 주의해서 사용할 것 iPower 의 값을 적당한 값으로 넣어주거나 min 키워드를 통해 최소한의 값을 만들어 주는 것이 좋음
float4 Get_RadialBlur(texture2D BlurTexture, float2 vUV, int iPower, float2 vPointUV, float blurStart, float blurWidth)
{
#if defined(_BLUR)
    
    //float2 vPointDir = vPointUV - vUV;
    
    //float4 vColor = 0;
    //float2 vScale = 1.f / iResolution;
    //float fSigma = float(iPower) * 0.25f;
    
    //[loop]
    //for (int i = 0; i < iPower; i++)
    //{
    //    float2 vTargetUV = vPointDir * i;
    //    float fGaussian = Get_Gaussian(vTargetUV, fSigma);
    //    vector vBlurDesc = BlurTexture.Sample(LinearClampSampler, vUV + vTargetUV * vScale);
        
    //    vColor += fGaussian * vBlurDesc;
    //}
    
    //return vColor /* / vColor.a*/;
    
    float4 vColor = 0;
    //float blurStart = 1.0f;
    //float blurWidth = 0.1f;
    
    vUV -= vPointUV;
    float fPrecompute = blurWidth * (1.0f / float(iPower - 1.f));
    
    for (int i = 0; i < iPower; i++)
    {
        float scale = blurStart + (float(i) * fPrecompute);
        vColor += BlurTexture.Sample(LinearClampSampler, vUV * scale + vPointUV);
    }
    
    vColor /= float(iPower);
    
    return vColor;
#endif
}

float3 Gamma_LinearSpace(float3 _rgb)
{
    return pow(_rgb, 2.2);
    //return _rgb;
}

float3 Gamma_LinearSpace(float3 _rgb, float _f)
{
    return pow(_rgb, _f);
    //return _rgb;
}

float3 Gamma_ForMonitor(float3 _rgb, float _f = 0.45454545)
{
    return pow(_rgb, _f); // 0.45454545 = 1/2.2
    //return _rgb;
}

float3 ToneMapACES(float3 hdr)
{
    const float A = 2.51, B = 0.03, C = 2.43, D = 0.59, E = 0.14;
    return saturate((hdr * (A * hdr + B)) / (hdr * (C * hdr + D) + E));
}

static const float3 LUM_FACTOR = float3(0.2125f, 0.7154f, 0.0721f);
//static const float g_fMiddleGrey = float(0.5f);
//static const float g_fLumWhiteSqr = float(1.f);

//float3 ToneMapping(float3 vHDRColor)
//{
//    // 현재 픽셀에 대한 휘도 스케일 계산
//    float fLScale = dot(vHDRColor, LUM_FACTOR);
//    fLScale *= g_fMiddleGrey / 0.5f;
//    fLScale = (fLScale + fLScale * fLScale / g_fLumWhiteSqr) / (1.f + fLScale);

//    // 휘도 스케일을 픽셀 색상에 적용
//    return vHDRColor * fLScale;
//}


const float3 ToneMap_Reinhard(float3 hdr)
{
    return hdr / (hdr + 1);
}

static const float3 LUMINANCE_VECTOR = float3(0.2125f, 0.7154f, 0.0721f);

float3 Luminance(float3 hdr, float fBright, float fOldLumi, float fNewLumi)
{
    float3 Out = (float3) hdr;
    
    //fSceneLumi 의 범위내로 밝기를 맞추어주는 코드
    float fSceneLumi = (fOldLumi * 0.5 + fNewLumi * 0.5);
    float fMiddleGrayOfSceneLumi = 1.03 - (2 / (2 + log10(fSceneLumi + 1)));
    float fRgbLumi = dot(Out.rgb, LUMINANCE_VECTOR) + 0.0001f;

    //rgb lumi 를 fSceneLumi 의 미들그레이로 감쇄하여,
    //fSceneLumi 값범위내로 조절해준다
    float fLumiScaled = (fRgbLumi * fMiddleGrayOfSceneLumi) / (fSceneLumi + 0.0001f);

    float fWhite = 1;
    //scale all luminance within a displayable range of 0 to 1
    fLumiScaled = (fLumiScaled * (1.0f + fLumiScaled / (fWhite))) / (1.0f + fLumiScaled);

    //
    Out.rgb *= (fLumiScaled) * fBright; //밝기 조정
    
    return Out;

}

const float3 ToneMap(float3 hdr, float fOldLumi, float fNewLumi, float fMiddleGrey, float fLumWhiteSqr)
{
    float fSceneLumi = (fOldLumi * 0.5f + fNewLumi * 0.5f) + 0.0001f;
    
    // 현재 픽셀에 대한 휘도 스케일 계산
    float fLScale = dot(hdr, LUM_FACTOR);
    fLScale *= fMiddleGrey / fSceneLumi;
    fLScale = (fLScale + fLScale * fLScale / fLumWhiteSqr) / (1.f + fLScale);

    return hdr * fLScale;
}



float3 lensflare(float2 uv, float2 pos)
{
    float intensity = 1.5f;
    float2 main = uv - pos;
    float2 uvd = uv * (length(uv));
	
    float dist = length(main);
    dist = pow(dist, 0.1f);
	
	
    float f1 = max(0.01f - pow(length(uv + 1.2f * pos), 1.9f), 0.f) * 7.f;

    float f2 = max(1.f / (1.f + 32.f * pow(length(uvd + 0.8f * pos), 2.f)), 0.f) * 0.1f;
    float f22 = max(1.f / (1.f + 32.f * pow(length(uvd + 0.85f * pos), 2.f)), 0.f) * 0.08f;
    float f23 = max(1.f / (1.f + 32.f * pow(length(uvd + 0.9f * pos), 2.f)), 0.f) * 0.06f;
	
    float2 uvx = lerp(uv, uvd, -0.5);
	
    float f4 = max(0.01f - pow(length(uvx + 0.4f * pos), 2.4f), 0.f) * 6.0f;
    float f42 = max(0.01f - pow(length(uvx + 0.45f * pos), 2.4f), 0.f) * 5.0f;
    float f43 = max(0.01f - pow(length(uvx + 0.5f * pos), 2.4f), 0.f) * 3.0f;
	
    uvx = lerp(uv, uvd, -0.4f);
	
    float f5 = max(0.01f - pow(length(uvx + 0.2f * pos), 5.5f), 0.f) * 2.f;
    float f52 = max(0.01f - pow(length(uvx + 0.4f * pos), 5.5f), 0.f) * 2.f;
    float f53 = max(0.01f - pow(length(uvx + 0.6f * pos), 5.5f), 0.f) * 2.f;
	
    uvx = lerp(uv, uvd, -0.5);
	
    float f6 = max(0.01f - pow(length(uvx - 0.3f * pos), 1.6), 0.f) * 6.f;
    float f62 = max(0.01f - pow(length(uvx - 0.325f * pos), 1.6), 0.f) * 3.f;
    float f63 = max(0.01f - pow(length(uvx - 0.35f * pos), 1.6), 0.f) * 5.f;
	
    float3 c = 0.f;
	
    c.r += f2 + f4 + f5 + f6;
    c.g += f22 + f42 + f52 + f62;
    c.b += f23 + f43 + f53 + f63;
    c = c * 1.3f - float3(length(uvd) * 0.05f, length(uvd) * 0.05f, length(uvd) * 0.05f);
	
    return c * intensity;
}

float3 cc(float3 color, float factor, float factor2) // color modifier
{
    float w = color.x + color.y + color.z;
    return lerp(color, float3(w, w, w) * factor, w * factor2);
}

float4 Get_lensflare(float2 vUV, float2 vSunPos)
{
    // x -0.5 0.5
    // y -
    float2 iResolution = { g_fWinCX, g_fWinCY };
    //vUV.x = (vUV.x + 1.f) * 0.5f;
    //vUV.y = (vUV.y - 1.f) * -0.5f;
    vUV.xy = vUV.xy - 0.5f;
    vUV.xy *= 2.f;
    vUV.x *= iResolution.x / iResolution.y; //fix aspect ratio
    
    
    // 0.5  0, 0.5f  0
    //vSunPos.x = (vSunPos.x + 1.f) * 0.5f;
    //vSunPos.y = (vSunPos.y - 1.f) * -0.5f;
    vSunPos.xy = vSunPos.xy - 0.5f;
    vSunPos.xy *= 2.0f;
    vSunPos.x *= iResolution.x / iResolution.y; //fix aspect ratio

    float3 color = float3(1.5, 1.2, 1.2) * lensflare(vUV, vSunPos);
    color = cc(color, .5, .1);

    return clamp(float4(color, (color.r + color.g + color.b) / 3.0), 0.f, 1.f);
}



#define SAMPLES 16
#define INTENSITY 1.f
#define SCALE 2.5f
#define BIAS 0.05f
//#define SAMPLE_RAD 0.02f
#define SAMPLE_RAD 0.02f
//#define MAX_DISTANCE 0.07f
//float   g_fMaxDistance = 0.07f;
float   g_fMaxDistance = 5.92f;

#define MOD3 float3(.1031,.11369,.13787)

float hash12(float2 p)
{
    float3 p3 = frac(float3(p.xyx) * MOD3);
    p3 += dot(p3, p3.yzx + 19.19);
    return frac((p3.x + p3.y) * p3.z);
}

float2 hash22(float2 p)
{
    float3 p3 = frac(float3(p.xyx) * MOD3);
    p3 += dot(p3, p3.yzx + 19.19);
    return frac(float2((p3.x + p3.y) * p3.z, (p3.x + p3.z) * p3.y));
}

float3 getPosition(float2 vUV, texture2D DepthTex, float fFar, matrix CamViewMatrixInv, matrix CamProjMatrixInv)
{
    vector vDepth = DepthTex.Sample(LinearSampler, vUV);

    float fViewZ = vDepth.y * fFar;

    if (0 == fViewZ)
        return float3(0.f, 0.f, 0.f);
    
    vector vWorldPos;
    vWorldPos.x = vUV.x * 2.f - 1.f;
    vWorldPos.y = vUV.y * -2.f + 1.f;
    vWorldPos.z = vDepth.x;
    vWorldPos.w = 1.0f;

    vWorldPos *= fViewZ;
    vWorldPos = mul(vWorldPos, CamProjMatrixInv);
    vWorldPos = mul(vWorldPos, CamViewMatrixInv);
    
    return vWorldPos.xyz;

}

//float3 getNormal(float2 uv)
//{
//    return textureLod(iChannel0, uv, 0.).xyz;
//}

//float2 getRandom(float2 uv)
//{
//    return normalize(hash22(uv * 126.1231) * 2. - 1.);
//}


float doAmbientOcclusion(float2 tcoord, float2 uv, float3 p, float3 cnorm, texture2D DepthTex, float fFar, matrix CamViewMatrixInv, matrix CamProjMatrixInv)
{
    float3 diff = getPosition(tcoord + uv, DepthTex, fFar, CamViewMatrixInv, CamProjMatrixInv) - p;
    float l = length(diff);
    float3 v = diff / l;
    float d = l * SCALE;
    float ao = max(0.0, dot(cnorm, v) - BIAS) * (1.0 / (1.0 + d));
    ao *= smoothstep(g_fMaxDistance, g_fMaxDistance * 0.5, l);
    return ao;

}

float spiralAO(float2 uv, float3 p, float3 n, float rad, texture2D DepthTex, float fFar, matrix CamViewMatrixInv, matrix CamProjMatrixInv)
{
    float goldenAngle = 2.4;
    float ao = 0.;
    float inv = 1. / float(SAMPLES);
    float radius = 0.;

    float rotatePhase = hash12(uv * 100.) * 6.28;
    float rStep = inv * rad;
    float2 spiralUV;

    [loop]
    for (int i = 0; i < SAMPLES; i++)
    {
        spiralUV.x = sin(rotatePhase);
        spiralUV.y = cos(rotatePhase);
        radius += rStep;
        ao += doAmbientOcclusion(uv, spiralUV * radius, p, n, DepthTex, fFar, CamViewMatrixInv, CamProjMatrixInv);
        rotatePhase += goldenAngle;
    }
    ao *= inv;
    return ao;
}

float Get_SSAO(float2 vUV, texture2D NormalTex, texture2D DepthTex, float fFar, matrix CamViewMatrixInv, matrix CamProjMatrixInv, float fPower)
{       
    float ao = 0.f;
    
#if defined(_SSAO)
    float3 p = getPosition(vUV, DepthTex, fFar, CamViewMatrixInv, CamProjMatrixInv);
    float3 n = NormalTex.Sample(LinearSampler, vUV).xyz * 2.f - 1.f;

    vector vDepth = DepthTex.Sample(LinearSampler, vUV);
    float fViewZ = vDepth.y * fFar;
    
    
    //float rad = SAMPLE_RAD / p.z;
    //float rad = SAMPLE_RAD / fViewZ;
    float rad = fPower / fViewZ;

    ao = spiralAO(vUV, p, n, rad, DepthTex, fFar, CamViewMatrixInv, CamProjMatrixInv);

    //ao = 1.f - ao * INTENSITY;
    
#endif
    
    return ao;
}