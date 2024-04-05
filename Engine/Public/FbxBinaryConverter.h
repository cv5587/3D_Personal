#pragma once
#include "Base.h"
class CFbxBinaryConverter final:
    public CBase
{
    typedef struct{
        _uint NumMesh;
           
               wstring MeshName;//메시이름
               _uint    MaterialIdx;//메테리얼
               _uint    Numbones;//영향주는 뼈갯수
               _uint* m_BoneIdxces;//뼈들의 인덱스
               _float4x4* OffsetMat;//뼈별 최초 오프셋
               _uint    PrimitiveTopology;
               _uint    IndexFormat;
               _uint    NumVertexBuffers;

               _uint    NumVertices;
               _uint    VertexStride;
               D3D11_BUFFER_DESC VBuffer_Desc;
               VTXMESH*  VerticesInfo;

               _uint    NumIndeces;
               _uint    IndexStride;
               D3D11_BUFFER_DESC IBuffer_Desc;
               _uint*      Indices;


        _uint NumMaterial;
            //메테리얼 하나에 21개의 정보 필요 (1메테리얼의 
            _uint*   NumTexture;
            wstring  TextureFilePath;

        _uint NumBone;
                //bone 갯수만큼
              wstring szName;
              _int*    ParentBoneIdx;
              _float4x4*   TransformMat;


    }NONANIM_DATA;

private:
    CFbxBinaryConverter();
    virtual ~CFbxBinaryConverter() = default;

public:
    HRESULT Initialize();
    HRESULT Save_Binary(void* pArg);
    HRESULT Load_Binary();
public:
    static CFbxBinaryConverter* Create();
    virtual void Free()override;
};

