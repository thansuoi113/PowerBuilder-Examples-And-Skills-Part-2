// PrintVc7.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "PrintVc7.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

// ���ǵ���������һ��ʾ��
PRINTVC7_API int nPrintVc7=0;

// ���ǵ���������һ��ʾ����
PRINTVC7_API int fnPrintVc7(void)
{
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� PrintVc7.h
CPrintVc7::CPrintVc7()
{ 
	return; 
}
