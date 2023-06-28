echo Copying HLSL Default Headers to Script Project

:: Script 폴더로 기본 HLSL 헤더 파일들을 copy
set source_dir=.\Project\Engine
set destination_dir=.\Project\Script
for /f %%f in (HLSL_Copy_To_Script_Proj.txt) do (
    xcopy "%source_dir%\%%f" "%destination_dir%" /d /s /y /i

    :: 참조용 파일 읽기 전용으로 변경
    attrib +r "%destination_dir%\%%f"
)

:: 1번 Argument에 아무것도 들어오지 않았을 경우(== 직접 실행했을 경우) 일시 정지
if "%1"=="" pause
