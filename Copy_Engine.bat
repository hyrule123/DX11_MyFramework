
:: 형태는 main()의 argc, argv과 동일함. 
:: 0번 파라미터에는 파일 경로가 들어옴
:: 1번 파라미터에는 실행 메시지가 들어옴
:: 1번 메시지에는 빌드 이벤트에서 빌드 모드에 따라 Debug/Release 메시지가 들어오도록 해놓음
:: -> cd $(SolutionDir) "$(SolutionDir)Copy_Engine.bat" $(Configuration)
:: 아래 코드는 해당 메시지를 참조해서 경로를 생성하는 코드임. 
:: %(변수)% = 문자열
set Mode=%1
if "%Mode%"=="" set Mode=Debug
echo Running %0... 
echo Mode: %Mode%

:: *.h 파일 복사(라이브러리 참조용)
xcopy /d /s /y /i /r /exclude:exclude_list.txt ".\Project\Engine\*.h" ".\External\Include\Engine\"

:: C++와 공유하는 HLSL 헤더파일 복사
xcopy /d /s /y /i /r /exclude:exclude_list.txt ".\Project\Engine\*.hlsli" ".\External\Include\Engine\"

:: *.inl 파일 복사(inline 파일)
xcopy /d /s /y /i /r  /exclude:exclude_list.txt ".\Project\Engine\SimpleMath.inl" ".\External\Include\Engine\"

:: 참조용 파일 읽기 전용으로 변경
attrib +r ".\External\Include\Engine\*"

:: *.fx 파일 복사(쉐이더 컴파일 코드) - 헤더 파일 사용하므로 복사 X(현재 주석 처리 하였음)
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

:: 1번 Argument에 아무것도 들어오지 않았을 경우(== 직접 실행했을 경우) 일시 정지
if "%1"=="" pause
