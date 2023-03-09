#include "pch.h"

#include "MacroFunc.h"

void WriteCode(std::wofstream& _pFile, const wstring& _strCode)
{
	static int s_iIndentation = 0;

	//���� �����ش�.
	_pFile << L"\n";

	//�������� ���� ��� ���� �����⸸ �ϰ� ��
	if (true == _strCode.empty())
		return;

	//�ݴ� �߰�ȣ�� ������ ���� ���� �����ش�.
	if (std::wstring::npos != _strCode.find(BracketEnd))
	{
		--s_iIndentation;
		if (s_iIndentation < 0)
			s_iIndentation = 0;
	}

	//���� �鿩���� �ܰ踸ŭ �鿩���⸦ ���ش�.
	for (int i = 0; i < s_iIndentation; ++i)
		_pFile << L"\t";

	//�߰�ȣ�� ���� ��� indentation ����
	if (std::wstring::npos != _strCode.find(BracketStart))
	{
		++s_iIndentation;
	}

	//�ڵ� �ۼ�
	_pFile << _strCode;
}