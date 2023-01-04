:: *.h 파일 복사(라이브러리 참조용)
xcopy /d /s /y /i  /exclude:exclude_list.txt ".\Project\Engine\*.h" ".\External\Include\Engine\"

:: *.inl 파일 복사(inline 파일)
xcopy /d /s /y /i  /exclude:exclude_list.txt ".\Project\Engine\SimpleMath.inl" ".\External\Include\Engine\"

:: *.fx 파일 복사(쉐이더 컴파일 코드)
:: xcopy /d /s /y /i  /exclude:exclude_list.txt ".\Project\Engine\*.fx" ".\OutputFile\bin\content\shader\"