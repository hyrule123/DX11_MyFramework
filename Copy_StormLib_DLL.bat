:: *.h 파일 복사(라이브러리 참조용)
xcopy /d /s /y /i /r /exclude:exclude_list_stormlib.txt ".\Project\StormLib_DLL\SRC\*.h" ".\External\Include\StormLib_DLL\"

if exist ".\External\Library\StormLib_DLL\StormLib_DLL_Debug.dll" (
xcopy /d /s /y /i ".\External\Library\StormLib_DLL\StormLib_DLL_Debug.dll" ".\OutputFile\Bin_Debug\"
)

if exist ".\External\Library\StormLib_DLL\StormLib_DLL.dll" (
xcopy /d /s /y /i ".\External\Library\StormLib_DLL\StormLib_DLL.dll" ".\OutputFile\Bin\"
)


:: *.h 읽기 전용으로 변경
attrib +r ".\External\Include\StormLib_DLL\*.h"