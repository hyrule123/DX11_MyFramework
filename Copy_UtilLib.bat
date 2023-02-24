:: *.h 파일 복사(라이브러리 참조용)
xcopy /d /s /y /i ".\Project\UtilLib\json-cpp\*.h" ".\External\Include\UtilLib\"


:: *.h 읽기 전용으로 변경
attrib +r ".\External\Include\UtilLib\*.h"