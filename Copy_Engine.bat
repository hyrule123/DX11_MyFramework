set Mode=%1
if "%Mode%"=="" set Mode=Debug
echo Running %0... 
echo Mode: %Mode%

:: *.h 파일 복사(라이브러리 참조용)
xcopy /d /s /y /i /r /exclude:exclude_list.txt ".\Project\Engine\*.h" ".\External\Include\Engine\"

:: C++와 공유하는 HLSL 헤더파일 복사
xcopy /d /s /y /i /r /exclude:exclude_list.txt ".\Project\Engine\*.hlsli" ".\External\Include\Engine\"

:: 참조용 파일 읽기 전용으로 변경
attrib +r ".\External\Include\Engine\*"

:: *.inl 파일 복사(inline 파일)
xcopy /d /s /y /i  /exclude:exclude_list.txt ".\Project\Engine\SimpleMath.inl" ".\External\Include\Engine\"

:: *.fx 파일 복사(쉐이더 컴파일 코드) - 헤더 파일 사용하므로 복사 X
:: xcopy /d /s /y /i  /exclude:exclude_list.txt ".\Project\Engine\*.fx" ".\OutputFile\Bin\Content\Shader\"

:: 릴리즈 모드일 경우 Bin_Debug/Content/ 폴더를 Bin_Release/Content/ 폴더로 복사
if "%Mode%"=="Release" (
xcopy /d /s /y /i  /exclude:exclude_list.txt ".\OutputFile\Bin_Debug\Content\*" ".\OutputFile\Bin_Release\Content\"
)

:: 파일 저장용 디렉토리가 없을 경우 새로 생성
if not exist .\OutputFile\Bin_%Mode%\Content\SavedSetting ( mkdir .\OutputFile\Bin_%Mode%\Content\SavedSetting )
::if not exist .\OutputFile\Bin_Release\Content\SavedSetting ( mkdir .\OutputFile\Bin_Release\Content\SavedSetting )

:: json 파일 백업
if exist .\OutputFile\Bin_%Mode%\Content\Shader\Graphics\*.json (
xcopy /d /s /y /i .\OutputFile\Bin_%Mode%\Content\Shader\Graphics\*.json .\JsonBackup\%Mode%\
)

if "%1"=="" pause