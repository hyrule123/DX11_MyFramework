:: *.h 파일 복사(라이브러리 참조용)
xcopy /d /s /y /i /r /exclude:exclude_list.txt ".\Project\UtilLib_DLL\json-cpp\*.h" ".\External\Include\UtilLib_DLL\"

if exist ".\External\Library\UtilLib_DLL\UtilLib_DLL_Debug.dll" (
xcopy /d /s /y /i ".\External\Library\UtilLib_DLL\UtilLib_DLL_Debug.dll" ".\OutputFile\Bin_Debug\"
)

if exist ".\External\Library\UtilLib_DLL\UtilLib_DLL.dll" (
xcopy /d /s /y /i ".\External\Library\UtilLib_DLL\UtilLib_DLL.dll" ".\OutputFile\Bin\"
)


:: *.h 읽기 전용으로 변경
attrib +r ".\External\Include\UtilLib_DLL\*.h"