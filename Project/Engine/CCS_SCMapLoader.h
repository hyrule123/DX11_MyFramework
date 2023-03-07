#pragma once
#include "CComputeShader.h"

#include "Shader_SCMapLoader_0_Header.hlsli"

#include "SCMapDefine.h"


class CStructBuffer;
class CCS_SCMapLoader :
    public CComputeShader
{
public:
    CCS_SCMapLoader();
    virtual ~CCS_SCMapLoader();

public:
    virtual bool BindDataCS() override;
    virtual void UnBindCS() override;

    void Debug();

private:
    tpSBufferTileSet m_arrpSBufferTileSet[(int)eTILESET_INFO::END];

    //MXTM ����
    //Map�� tMapDataChunk ���� �Ʒ��� ��������
    CStructBuffer* m_pSBuffer_MXTM;

    //============�� ����=============
    tMapData m_tMapWorkSpace;

    //TODO : ���߿� �� �������� �ε��ɰ�� �Ʒ� ������ �����Ұ�
    CStructBuffer* m_pSBuffer_Debug;
    tMtrlScalarData* m_DebugData;

public:
    bool LoadMap(const wstring& _wstrMapName, __out tMapData& _tMapData);

private:
    bool ReadMapData(char* Data, DWORD Size);
    bool UploadMapDataToCS();
};

