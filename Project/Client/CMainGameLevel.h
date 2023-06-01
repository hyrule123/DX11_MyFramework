#pragma once

#include <Engine/strKey_Default.h>

void CreateMainGame();

//멀티스레드를 통해서 로드하기
void LoadRes(eRES_TYPE _eResType);

//void LoadAllTexture();
//
//void LoadUserMtrl();


bool CreateUserGraphicsShader();
bool CreateUserComputeShader();

