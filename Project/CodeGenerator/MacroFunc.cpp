#include "pch.h"

#include "MacroFunc.h"

std::vector<stdfs::path>& MacroFunc::ConvertPathToLowerCase(std::vector<stdfs::path>& _vecPath)
{
    size_t size = _vecPath.size();
    for (size_t i = 0; i < size; ++i)
    {
        _vecPath[i] = MacroFunc::LowerCase<wchar_t>(_vecPath[i].wstring());
    }

    return _vecPath;
}
