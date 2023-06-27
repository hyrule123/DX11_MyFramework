#include "S_H_STD2DLight.hlsli"

void CalcLight2D(float3 _vWorldPos, inout tLightColor _Light);
void CalcLight2DNormal(float3 _vWorldPos, float3 _vNormalDir, inout tLightColor _Light);

float4 PS_STD2D_Light(VS_OUT_Light _in) : SV_TARGET
{
    float4 vOutColor = (float4)0.f;
    
	tMtrlScalarData Data = GetMtrlScalarData(_in.uInstID);
    
    //메인텍스처가 존재하지 않을경우는 무조건 마젠타 색상을 return;
    if(FALSE == g_CBuffer_Mtrl_Tex.bTEX_0)
    {
        vOutColor = float4(1.f, 1.f, 0.f, 1.f);
    }
    
    //애니메이션 사용중일경우
	else if (TRUE == Data.SHADER_STD2DMTRL_bAnimUse)
	{

		if (eMTRL_SCALAR_STD2D_FLAG::USE_ANIM & Data.MTRL_SCALAR_STD2D_FLAG)
		{
			float2 vUV = _in.vUV;
			if (eMTRL_SCALAR_STD2D_FLAG::NEED_FLIP_X & Data.MTRL_SCALAR_STD2D_FLAG)
				vUV.x = 1.f - vUV.x;
        
        //애니메이션의 Left Top부터 Slice에 자신의 UV값을 곱해서 실제 UV값을 구해준다.
			float2 RealUV = Data.MTRL_SCALAR_STD2D_ANIM_UV_LEFTTOP + Data.MTRL_SCALAR_STD2D_ANIM_UV_SLICE * vUV;

			int idx = Data.MTRL_SCALAR_STD2D_ANIM_TEXATLAS_IDX;
			vOutColor = SampleMtrlTex(idx, g_Sampler_0, RealUV);
		}
		else
		{
			vOutColor = g_tex_0.Sample(g_Sampler_0, _in.vUV);
		}
	}
    else
    {
        vOutColor = g_tex_0.Sample(g_Sampler_0, _in.vUV);
    }

	//Alpha Check 
	if (0.f == vOutColor.a)
	{
		discard;
	}
    
	//ColorKey Check
	if (eMTRL_SCALAR_STD2D_FLAG::USE_COLOR_KEY & Data.MTRL_SCALAR_STD2D_FLAG)
	{
		if (all(vOutColor.rgb == Data.MTRL_SCALAR_STD2D_COLORKEY.rgb))
			discard;
	}
        
        
    //노말맵이 있을 경우 빛 처리를 해준다. 노말맵은 Tex 1번에 저장되어 있다.
    //현재 월드에 배치되어있는 광원의 갯수는 tGlobal 상수버퍼에,
    //실제 광원 데이터는 구조화 버퍼에 배치되어 있다.
    float3 vNormal = (float3) 0.f;
    BOOL bNormalMapProcessed = FALSE;
    if (TRUE == g_CBuffer_Mtrl_Tex.bTEX_1)
    {
		bNormalMapProcessed = TRUE;
        
        vNormal = g_tex_1.Sample(g_Sampler_0, _in.vUV).xyz;
            
        //노멀맵은 방향을 가지는 '벡터'(-1 ~ 1), 현재 쉐이더의 색상 설정은 R8G8B8A8 UNORM(0 ~ 1)
        vNormal = (vNormal * 2.f) - 1.f;
            
        //현재 노멀맵은 DX 좌표축과 y축과 z축이 다르게 되어 있음.
        vNormal.y = -vNormal.y;

        //노멀맵의 벡터 방향에 픽셀의 월드 행렬을 곱한 뒤 normalize 해준다.
        //이 때 공차좌표가 0이므로 결과값은 좌표값이 아닌 벡터값이 된다.
        //결과값은 해당 픽셀의 회전 결과가 반영되어 실제로 바라보고 있는 방향이 된다.
		vNormal = normalize(mul(float4(vNormal, 0.f), Data.MTRL_SCALAR_MAT_WORLD).xyz);
    }
        

    tLightColor LightColor = (tLightColor) 0.f;
    //노말값이 영벡터라면, 즉 노말맵 텍스처가 없어서 0.f로 초기화된 상태 그대로라면
    if (FALSE == bNormalMapProcessed)
    {
        //자신의 노멀값을 고려하지 않은 광원 계산을 진행
        CalcLight2D(_in.vWorldPos, LightColor);
    }
    else
    {     
        //노멀값까지 합산된 광원 계산을 진행.
        CalcLight2DNormal(_in.vWorldPos, vNormal, LightColor);
    }
            
    //최종적으로 계산된 빛의 색상을 픽셀 색상에 곱해준다.
    vOutColor.rgb *= (LightColor.vDiffuse.rgb + LightColor.vAmbient.rgb);

	
    return vOutColor;
}

void CalcLight2D(float3 _vWorldPos, inout tLightColor _Light)
{
	for (uint i = 0; i < g_CBuffer_SBuffer_ShareData[eCBUFFER_SBUFFER_SHAREDATA_IDX::LIGHT2D].uSBufferCount; ++i)
    {  
        if (eLIGHT_TYPE::DIRECTIONAL == g_SBuffer_Light2D[i].LightType)
        {
            _Light.vAmbient.rgb += g_SBuffer_Light2D[i].LightColor.vAmbient.rgb;
            _Light.vDiffuse.rgb += g_SBuffer_Light2D[i].LightColor.vDiffuse.rgb;
        }
        else if (eLIGHT_TYPE::POINT == g_SBuffer_Light2D[i].LightType)
        {
            //픽셀과 광원 사이의 길이를 계산하고, 그걸 점광원의 범위(fRadius)로 나눠준다.
            float DistancePow = distance(_vWorldPos.xy, g_SBuffer_Light2D[i].vLightWorldPos.xy) / g_SBuffer_Light2D[i].fRadius;
            
            //이 값을 1에서 ?주고(멀어질수록 값이 작아지도록 - 지금은 가까울수록 값이 작아진다.)
            //saturate 함수를 통해서 범위를 0~1 사이로 한정한다.(빛이 강하다고 색이 진해지는 것은 아니므로)
            DistancePow = saturate(1.f - DistancePow);
            
            //거리에 따른 빛의 강도를 계산해준 뒤 이 값을 Diffuse 값에 넣어준다.
            //점광원의 경우 환경광(Ambient)은 없다고 가정한다.
            _Light.vDiffuse.rgb += g_SBuffer_Light2D[i].LightColor.vDiffuse.rgb * DistancePow;
        }
        else if (eLIGHT_TYPE::SPOTLIGHT == g_SBuffer_Light2D[i].LightType)
        {
            //스포트라이트의 경우에는 추가적으로 빛의 방향과 부채꼴의 각도가 존재한다. 이 값은 내적과 acos를 통해서 구할 수 있다.
            //픽셀의 위치와 광원의 위치를 뺀 후 정규화 하여 광원으로부터 픽셀까지의 방향 벡터를 구한다.
            float2 LightToPixelDir = normalize(_vWorldPos.xy - g_SBuffer_Light2D[i].vLightWorldPos.xy);
            
            //위에서 구한 방향 벡터와 빛의 방향 벡터를 내적하면 사잇각의 코사인값을 구할 수 있다.
            float Angle = acos(dot(LightToPixelDir, g_SBuffer_Light2D[i].vLightDir.xy));
            

            //구한 각도가 스포트라이트 광원에서 설정한 부채꼴의 각도보다 작을 경우에만 처리해준다.
            if(Angle < g_SBuffer_Light2D[i].fAngle)
            {
                //각도에 따른 빛의 세기를 계산한다.
                float AnglePow = saturate(1.f - (Angle / g_SBuffer_Light2D[i].fAngle));

                float DistancePow = distance(_vWorldPos.xy, g_SBuffer_Light2D[i].vLightWorldPos.xy) / g_SBuffer_Light2D[i].fRadius;
                DistancePow = saturate(1.f - DistancePow);
            
                _Light.vDiffuse.rgb += g_SBuffer_Light2D[i].LightColor.vDiffuse.rgb * DistancePow * AnglePow;
            }

        }
        
    }

}

void CalcLight2DNormal(float3 _vWorldPos, float3 _vNormalDir, inout tLightColor _Light)
{
    //여기선 노말값까지 감안해줘야 함.    
    for (uint i = 0; i < g_CBuffer_SBuffer_ShareData[eCBUFFER_SBUFFER_SHAREDATA_IDX::LIGHT2D].uSBufferCount; ++i)
    {
        if (eLIGHT_TYPE::DIRECTIONAL == g_SBuffer_Light2D[i].LightType)
        {
        //램버트 코사인 법칙을 통한 빛의 강도(=코사인값)를 구한다.
            float DiffusePow = saturate(dot(-g_SBuffer_Light2D[i].vLightDir.xyz, _vNormalDir));
            
        //직사광선의 경우 방향만 고려
            _Light.vDiffuse.rgb += g_SBuffer_Light2D[i].LightColor.vDiffuse.rgb * DiffusePow;
            _Light.vAmbient.rgb += g_SBuffer_Light2D[i].LightColor.vAmbient.rgb;
        }
        else if (eLIGHT_TYPE::POINT == g_SBuffer_Light2D[i].LightType)
        {
        //점광원일 때는 광원 위치로부터 빛이 원형으로 퍼져나간다고 가정한다. 태양을 점이라고 생각하면 될듯.
            
        //점광원은 빛이 원형으로 퍼져 나가므로 광원과 픽셀 사이의 방향벡터가 곧 빛의 방향 벡터이다.
            float3 LightToPixelVector = _vWorldPos - g_SBuffer_Light2D[i].vLightWorldPos.xyz;
            
        //우선 거리에 따른 빛의 강도를 계산한다.
            float DistancePow = saturate(
        1.f - (length(LightToPixelVector.xy) / g_SBuffer_Light2D[i].fRadius));
            
        //빛이 닿지 않는 픽셀 경우 continue
            if (DistancePow <= 0.f)
                continue;
            
        //노말 벡터와 빛의 방향 벡터 사이의 코사인값을 구한다.
            float DiffusePow = saturate(dot(-LightToPixelVector, _vNormalDir));
            
            _Light.vDiffuse.rgb += g_SBuffer_Light2D[i].LightColor.vDiffuse.rgb * DiffusePow * DistancePow;

        }
        else if (eLIGHT_TYPE::SPOTLIGHT == g_SBuffer_Light2D[i].LightType)
        {
            
            float DiffusePow = saturate(dot(-g_SBuffer_Light2D[i].vLightDir.xyz, _vNormalDir));
            
        //광원에서 픽셀까지의 벡터 구함
            float2 LightToPixelDir = _vWorldPos.xy - g_SBuffer_Light2D[i].vLightWorldPos.xy;
            
            float2 LightToPixelDirNorm = normalize(LightToPixelDir);
           
            
        //광원의 방향과 광원에서 픽셀까지의 방향벡터를 내적하고, 아크코사인을 통해서 각도 도출
        //이 때 나오는 아크코사인 값은 무조건 양수이다.(0 ~ PI) 부호가 없는 각도를 반환함.
            float Angle = acos(dot(LightToPixelDirNorm, g_SBuffer_Light2D[i].vLightDir.xy));
            
        //반경 90도 내일 경우에만 처리
            if (Angle < g_SBuffer_Light2D[i].fAngle)
            {
            //아까 구한 광원에서 픽셀까지의 벡터를 통해 거리 계산
                float DistancePow = saturate(1.f - (length(LightToPixelDir) / g_SBuffer_Light2D[i].fRadius));
                
            //saturate 할 필요 없음. Angle은 둘 다 무조건 양수임. 또한 위의 조건문 문에 Angle / Light.fAngle 값이 1을 넘어설 수 없음.
                float AnglePow = 1.f - (Angle / g_SBuffer_Light2D[i].fAngle);

                _Light.vDiffuse.rgb += g_SBuffer_Light2D[i].LightColor.vDiffuse.rgb * DiffusePow * DistancePow * AnglePow;
            }
            
        }

    }

}
