
:: Copy *.h file(for library reference) 테스트
xcopy /d /s /y /i /r /exclude:exclude_list_stormlib.txt ".\Project\stormLib_DLL\SRC\*.h" ".\External\include\stormLib_DLL\"

if exist ".\External\Library/stormLib_DLL/stormLib_DLL_Debug.dll" (
xcopy /d /s /y /i ".\External\Library\stormLib_DLL\stormLib_DLL_Debug.dll" ".\OutputFile\Bin_Debug\"
)

if exist ".\External\Library/stormLib_DLL/stormLib_DLL_Release.dll" (
xcopy /d /s /y /i ".\External\Library\stormLib_DLL\stormLib_DLL_Release.dll" ".\OutputFile\Bin_Release\"
)


:: Add read-only attributes to *.h files
attrib +r ".\External\include\stormLib_DLL\*.h"
