#include "Shader_std2D_Light_0_header.hlsli"

void CalcLight2D(float3 _vWorldPos, inout tLightColor _Light);
void CalcLight2DNormal(float3 _vWorldPos, float3 _vNormalDir, inout tLightColor _Light);

float4 PS_std2D_Light(VS_OUT _in) : SV_TARGET
{
    float4 vOutColor = (float4)0.f;
    
    if(1== g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);

        //Alpha Check + Color Key Check
        if (0.f == vOutColor.a || all(vOutColor.rgb == COLOR_KEY.rgb))
        {
            discard;
        }
        
        
        //�븻���� ���� ��� �� ó���� ���ش�.
        //���� ���忡 ��ġ�Ǿ��ִ� ������ ������ tGlobal ������ۿ�,
        //���� ���� �����ʹ� ����ȭ ���ۿ� ��ġ�Ǿ� �ִ�.
        float3 vNormal = (float3) 0.f;
        if (1 == g_btex_1)
        {
            vNormal = g_tex_1.Sample(g_sam_0, _in.vUV).xyz;
            
            //��ָ��� ������ ������ '����'(-1 ~ 1), ���� ���̴��� ���� ������ R8G8B8A8 UNORM(0 ~ 1)
            vNormal = (vNormal * 2.f) - 1.f;
            
            //���� ��ָ��� DX ��ǥ��� y��� z���� �ٸ��� �Ǿ� ����.
            float Temp = vNormal.y;
            vNormal.y = vNormal.z;
            vNormal.z = Temp;

            //��ָ��� ���� ���⿡ �ȼ��� ���� ����� ���� �� normalize ���ش�.
            //�� �� ������ǥ�� 0�̹Ƿ� ������� ��ǥ���� �ƴ� ���Ͱ��� �ȴ�.
            //������� �ش� �ȼ��� ȸ�� ����� �ݿ��Ǿ� ������ �ٶ󺸰� �ִ� ������ �ȴ�.
            vNormal = normalize(mul(float4(vNormal, 0.f), g_matWorld).xyz);
            
        }
        

        tLightColor LightColor = (tLightColor) 0.f;
        //�븻���� �����Ͷ��, �� �븻�� �ؽ�ó�� ��� 0.f�� �ʱ�ȭ�� ���� �״�ζ��
        if (all(float3(0.f, 0.f, 0.f) == vNormal))
        {
            //�ڽ��� ��ְ��� ������� ���� ���� ����� ����
            CalcLight2D(_in.vWorldPos, LightColor);
        }
        else
        {     
            //��ְ����� �ջ�� ���� ����� ����.
            CalcLight2DNormal(_in.vWorldPos, vNormal, LightColor);
        }
            
        //���������� ���� ���� ������ �ȼ� ���� �����ش�.
        vOutColor.rgb *= (LightColor.vDiffuse.rgb + LightColor.vAmbient.rgb);
    }

	
    return vOutColor;
}

void CalcLight2D(float3 _vWorldPos, inout tLightColor _Light)
{
    for (uint i = 0; i < g_Light2DCount; ++i)
    {  
        if (eLIGHT_DIRECTIONAL == g_Light2DSBuffer[i].LightType)
        {
            _Light.vAmbient.rgb += g_Light2DSBuffer[i].LightColor.vAmbient.rgb;
            _Light.vDiffuse.rgb += g_Light2DSBuffer[i].LightColor.vDiffuse.rgb;
        }
        else if (eLIGHT_POINT == g_Light2DSBuffer[i].LightType)
        {
            //�ȼ��� ���� ������ ���̸� ����ϰ�, �װ� �������� ����(fRadius)�� �����ش�.
            float DistancePow = distance(_vWorldPos.xy, g_Light2DSBuffer[i].vLightWorldPos.xy) / g_Light2DSBuffer[i].fRadius;
            
            
            //�� ���� 1���� �� �ְ�(�־������� ���� �۾������� - ������ �������� ���� �۾�����.)
            //saturate �Լ��� ���ؼ� ������ 0~1 ���̷� �����Ѵ�.(���� ���ϴٰ� ���� �������� ���� �ƴϹǷ�)
            DistancePow = saturate(1.f - DistancePow);
            
            //�Ÿ��� ���� ���� ������ ������� �� �� ���� Diffuse ���� �־��ش�.
            //�������� ��� ȯ�汤(Ambient)�� ���ٰ� �����Ѵ�.
            _Light.vDiffuse.rgb += g_Light2DSBuffer[i].LightColor.vDiffuse.rgb * DistancePow;
        }
        else if (eLIGHT_SPOTLIGHT == g_Light2DSBuffer[i].LightType)
        {
            //����Ʈ����Ʈ�� ��쿡�� �߰������� ���� ����� ��ä���� ������ �����Ѵ�. �� ���� ������ acos�� ���ؼ� ���� �� �ִ�.
            //�ȼ��� ��ġ�� ������ ��ġ�� �� �� ����ȭ �Ͽ� �������κ��� �ȼ������� ���� ���͸� ���Ѵ�.
            float2 LightToPixelDir = normalize(_vWorldPos.xy - g_Light2DSBuffer[i].vLightWorldPos.xy);
            
            //������ ���� ���� ���Ϳ� ���� ���� ���͸� �����ϸ� ���հ��� �ڻ��ΰ��� ���� �� �ִ�.
            float Angle = acos(dot(LightToPixelDir, g_Light2DSBuffer[i].vLightDir.xy));
            

            //���� ������ ����Ʈ����Ʈ �������� ������ ��ä���� �������� ���� ��쿡�� ó�����ش�.
            if(Angle < g_Light2DSBuffer[i].fAngle)
            {
                //������ ���� ���� ���⸦ ����Ѵ�.
                float AnglePow = saturate(1.f - (Angle / g_Light2DSBuffer[i].fAngle));

                float DistancePow = distance(_vWorldPos.xy, g_Light2DSBuffer[i].vLightWorldPos.xy) / g_Light2DSBuffer[i].fRadius;
                DistancePow = saturate(1.f - DistancePow);
            
                _Light.vDiffuse.rgb += g_Light2DSBuffer[i].LightColor.vDiffuse.rgb * DistancePow * AnglePow;
            }

        }
        
    }

}

void CalcLight2DNormal(float3 _vWorldPos, float3 _vNormalDir, inout tLightColor _Light)
{
    //���⼱ �븻������ ��������� ��.    
    
    for (uint i = 0; i < g_Light2DCount; ++i)
    {
        if (eLIGHT_DIRECTIONAL == g_Light2DSBuffer[i].LightType)
        {
            //����Ʈ �ڻ��� ��Ģ�� ���� ���� ����(=�ڻ��ΰ�)�� ���Ѵ�.
            float DiffusePow = saturate(dot(-g_Light2DSBuffer[i].vLightDir.xyz, _vNormalDir));
            
            //���籤���� ��� ���⸸ ���
            _Light.vDiffuse.rgb += g_Light2DSBuffer[i].LightColor.vDiffuse.rgb * DiffusePow;
            _Light.vAmbient.rgb += g_Light2DSBuffer[i].LightColor.vAmbient.rgb;

        }
        else if (eLIGHT_POINT == g_Light2DSBuffer[i].LightType)
        {
            //�������� ���� ���� ��ġ�κ��� ���� �������� ���������ٰ� �����Ѵ�. �¾��� ���̶�� �����ϸ� �ɵ�.
            
            //�������� ���� �������� ���� �����Ƿ� ������ �ȼ� ������ ���⺤�Ͱ� �� ���� ���� �����̴�.
            float3 LightToPixelVector = _vWorldPos - g_Light2DSBuffer[i].vLightWorldPos.xyz;
            
            //�켱 �Ÿ��� ���� ���� ������ ����Ѵ�.
            float DistancePow = saturate(
            1.f - (length(LightToPixelVector.xy) / g_Light2DSBuffer[i].fRadius));
            
            //���� ���� �ʴ� �ȼ� ��� continue
            if(DistancePow <= 0.f)
                continue;
            
            //�븻 ���Ϳ� ���� ���� ���� ������ �ڻ��ΰ��� ���Ѵ�.
            float DiffusePow = saturate(dot(-LightToPixelVector, _vNormalDir));
            
            _Light.vDiffuse.rgb += g_Light2DSBuffer[i].LightColor.vDiffuse.rgb * DiffusePow * DistancePow;

        }
        else if (eLIGHT_SPOTLIGHT == g_Light2DSBuffer[i].LightType)
        {
            
            float DiffusePow = saturate(dot(-g_Light2DSBuffer[i].vLightDir.xyz, _vNormalDir));
            
            //�������� �ȼ������� ���� ����
            float2 LightToPixelDir = _vWorldPos.xy - g_Light2DSBuffer[i].vLightWorldPos.xy;
            
            float2 LightToPixelDirNorm = normalize(LightToPixelDir);
            

            
            //������ ����� �������� �ȼ������� ���⺤�͸� �����ϰ�, ��ũ�ڻ����� ���ؼ� ���� ����
            //�� �� ������ ��ũ�ڻ��� ���� ������ ����̴�.(0 ~ PI) ��ȣ�� ���� ������ ��ȯ��.
            float Angle = acos(dot(LightToPixelDir, g_Light2DSBuffer[i].vLightDir.xy));
            
            //�ݰ� 90�� ���� ��쿡�� ó��
            if(Angle < g_Light2DSBuffer[i].fAngle)
            {
                //�Ʊ� ���� �������� �ȼ������� ���͸� ���� �Ÿ� ���
                float DistancePow = saturate(1.f - (length(LightToPixelDir) / g_Light2DSBuffer[i].fRadius));
                
                //saturate �� �ʿ� ����. Angle�� �� �� ������ �����. ���� ���� ���ǹ� ������ Angle / Light.fAngle ���� 1�� �Ѿ �� ����.
                float AnglePow = 1.f - (Angle / g_Light2DSBuffer[i].fAngle);

                _Light.vDiffuse.rgb += g_Light2DSBuffer[i].LightColor.vDiffuse.rgb * DiffusePow * DistancePow * AnglePow;
            }
            
        }

    }

}