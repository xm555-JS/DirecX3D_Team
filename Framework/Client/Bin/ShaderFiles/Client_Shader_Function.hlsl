


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

    float fAtt = 1.f - dot(-In.vLook, In.vNormal);
    fAtt = pow(fAtt, In.fInterval);
    Out = saturate(In.vFlagColor * fAtt);
    
    return Out;
}

vector Get_RimLight(float3 vNormal, float3 vLook, vector vFlagColor, float fInterval)
{
    vector Out = 0.f;

    float fAtt = 1.f - dot(-vLook, vNormal);
    fAtt = pow(fAtt, fInterval);
    Out = saturate(vFlagColor * fAtt);
    
    return Out;
}

float3 Gamma_LinearSpace(float3 _rgb)
{
    return pow(_rgb, 2.2);
}