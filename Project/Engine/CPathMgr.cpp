#include "pch.h"
#include "CPathMgr.h"

#include "CEngine.h"



CPathMgr::CPathMgr()
{
}

CPathMgr::~CPathMgr()
{
}


void CPathMgr::init()
{
	m_u8ProgramPath = std::filesystem::current_path().wstring();

	m_u8ContentPath = m_u8ProgramPath + L"\\Content\\";


	//CurrentDirectory는 비주얼스튜디오에서 디버그할때와 프로그램으로 실행시킬때 주소가 다르게 나옴
	//GetCurrentDirectory(256, m_szContentPath);

	////현재 프로그램의 주소를 받아오는게 더 나음.(고정된 주소를 받아올 수 있음)
	//GetModuleFileNameW(0, m_szProgramPath, 256);

	//// 2단계 상위폴더로 감
	//int iLen = (int)wcslen(m_szProgramPath);

	////Bin 바깥으로 나가서 OutputFile 까지의 주소만 남긴다
	//for (int i = iLen - 1; i >= 0; --i)
	//{
	//	if (L'\\' == m_szProgramPath[i])
	//	{
	//		m_szProgramPath[i] = 0;
	//		break;
	//	}
	//}

	//// + \\content\\
	//
	//lstrcpyW(m_szContentPath, m_szProgramPath);
	//wcscat_s(m_szContentPath, L"\\Content\\");

	//lstrcpyW(m_szShaderPath, m_szContentPath);
	//wcscat_s(m_szShaderPath, L"Shader\\");




    {
        //wstring MapPath = CPathMgr::GetInst()->GetContentPath();

        wstring MapPath = L"./";
        MapPath += L"Maps/(4)Fighting Sprit 1.3.scx";
        



        HANDLE hMpq = NULL;          // Open archive handle
        HANDLE hFile = NULL;          // Archived file handle
        //HANDLE handle = NULL;          // Disk file handle

        //DLL 로드
        //std::filesystem::current_path().wstring();
        HMODULE DLLModule = LoadLibraryW(L"./StormLib_DLL_Debug.dll");


        //함수 등록
        //MPQ 파일 로드
        typedef bool(WINAPI* pSFileOpenArchive)(const TCHAR*, DWORD, DWORD, HANDLE*);
        pSFileOpenArchive MpqOpenFunc = (pSFileOpenArchive)GetProcAddress(DLLModule, "SFileOpenArchive");

        typedef bool(WINAPI* pSFileOpenFileEx)(HANDLE, const char*, DWORD, HANDLE*);
        pSFileOpenFileEx funcOpenScenarioChk = (pSFileOpenFileEx)GetProcAddress(DLLModule, "SFileOpenFileEx");

        //DWORD WINAPI SFileGetFileSize(HANDLE hFile, LPDWORD pdwFileSizeHigh);
        typedef DWORD(WINAPI* pSFileGetFileSize)(HANDLE, LPDWORD);
        pSFileGetFileSize funcGetFileSize = (pSFileGetFileSize)GetProcAddress(DLLModule, "SFileGetFileSize");

        //bool WINAPI SFileReadFile(HANDLE hFile, void* pvBuffer, DWORD dwToRead, LPDWORD pdwRead, LPOVERLAPPED lpOverlapped);
        typedef bool (*pSFileReadFile)(HANDLE, void*, DWORD, LPDWORD, LPOVERLAPPED);
        pSFileReadFile funcReadFile = (pSFileReadFile)GetProcAddress(DLLModule, "SFileReadFile");

        //bool WINAPI SFileCloseFile(HANDLE hFile)
        typedef bool (WINAPI* pSFileCloseFile)(HANDLE);
        pSFileCloseFile funcCloseFile = (pSFileCloseFile)GetProcAddress(DLLModule, "SFileCloseFile");

        //bool WINAPI SFileCloseArchive(HANDLE hMpq)
        typedef bool (WINAPI* pSFileCloseArchive)(HANDLE);
        pSFileCloseArchive funcCloseArchive = (pSFileCloseArchive)GetProcAddress(DLLModule, "SFileCloseArchive");


        //DLL로부터 로드 실패 또는 함수에서 로드를 실패했을 경우 false 반환
        if (nullptr == MpqOpenFunc || false == MpqOpenFunc(MapPath.c_str(), 0, 0, &hMpq))
        {
            int nError = GetLastError();
            
        }

        //MPQ 파일 내부의 scenario.chk 파일 탐색.
        if (nullptr == funcOpenScenarioChk || false == funcOpenScenarioChk(hMpq, "staredit\\scenario.chk", 0, &hFile))
        {
            int nError = GetLastError();
            
        }


        // Read the file from the archive
        char* szBuffer;
        DWORD dwBytes = 1;

        //파일의 사이즈를 받아온 후 해당 파일을 받아올 공간을 동적할당 및 초기화한다.
        DWORD FileSize = funcGetFileSize(hFile, NULL);
        szBuffer = new char[FileSize];
        memset(szBuffer, 0, (size_t)FileSize);


        //데이터를 읽어온다.
        funcReadFile(hFile, szBuffer, FileSize, &dwBytes, NULL);


        //파일 연결 해제
        funcCloseFile(hFile);
        funcCloseArchive(hMpq);

        //라이브러리 연결 해제
        FreeLibrary(DLLModule);



        



        //암시적 링킹 코드
    //        // Open an archive, e.g. "d2music.mpq"
    //    if (nError == ERROR_SUCCESS)
    //    {
    //        if (!SFileOpenArchive(MapPath.c_str(), 0, 0, &hMpq))
    //            nError = GetLastError();
    //    }
    //
    //    typedef  bool (WINAPI* SFileOpenIndex)(HANDLE, const char*, DWORD, HANDLE);
    //
    //    // Open a file in the archive, e.g. "data\global\music\Act1\tristram.wav"
    //    if (nError == ERROR_SUCCESS)
    //    {
    //        //맵 파일의 scenario.chk 데이터를 찾는다.
    //        if (!SFileOpenFileEx(hMpq, "staredit\\scenario.chk", 0, &hFile))
    //            nError = GetLastError();
    //    }
    //
    //    if (nError != ERROR_SUCCESS)
    //        return false;
    //
    //    // Read the file from the archive
    //    char* szBuffer;
    //    DWORD dwBytes = 1;
    //
    //    //찾은 scenario.chk 파일로부터 정보를 읽어온다.
    //    DWORD FileSize = SFileGetFileSize(hFile, NULL);
    //
    //    szBuffer = new char[FileSize];
    //
    //    SFileReadFile(hFile, szBuffer, FileSize, &dwBytes, NULL);
    //
    //    // Cleanup and exit
    ////if (handle != NULL)
    ////    CloseHandle(handle);
    //    if (hFile != NULL)
    //        SFileCloseFile(hFile);
    //    if (hMpq != NULL)
    //        SFileCloseArchive(hMpq);
    }
}

