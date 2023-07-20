echo Copying HLSL Default Headers to Script Project


:: Script 폴더로 기본 HLSL 헤더 파일들을 copy
xcopy /d /s /y /i /r  /exclude:exclude_list.txt ".\Project\HLSL\*.hlsli" ".\External\Include\HLSL\"

:: 1번 Argument에 아무것도 들어오지 않았을 경우(== 직접 실행했을 경우) 일시 정지
if "%1"=="" pause
