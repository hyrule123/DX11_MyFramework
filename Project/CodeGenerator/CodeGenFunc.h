#pragma once

#include <Engine/define.h>
#include <Engine/strKey_Default.h>





void CreateScriptCode();

//정규화된 이름 규칙을 가진 쉐이더들의 코드를 자동 생성
//여기서 자동 생성하지 않아도 직접 생성할 수 있음.
void CreateShaderCode();
void CreateGraphicsShaderCode(std::ofstream& _outFile_StrKey);
void CreateComputeShaderCode(std::ofstream& _outFile_StrKey);


//_PathFromContent : Content 폴더로부터의 경로
//ex) 텍스처 폴더를 순회돌면서 파일명을 추가하고 싶을 경우 "Texture"만 넣어주면 됨.
//_HeaderFileName : 헤더 파일의 이름만 작성
//_vecExtension : 가져올 확장자 종류
void CreateStrKey_EnumArray(const std::filesystem::path& _PathFromContent, const std::filesystem::path& _HeaderFilename, const vector<string>& _vecExtension);

void CreateStrKey_Hierarchy(const std::filesystem::path& _PathFromContent, const std::filesystem::path& _HeaderFilename, const vector<string>& _vecExtension);

void CreateStrKey(const std::filesystem::path& _PathFromContent, const std::filesystem::path& _HeaderFilename, const vector<string>& _vecExtension);

