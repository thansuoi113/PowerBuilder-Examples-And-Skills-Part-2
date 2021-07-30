// SysInfo.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "SysInfo.h"
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



//���ڻ��CPU���ٶ�
SYSINFO_API int GetCPUSpeed(void)
{
	LARGE_INTEGER ulFreq,ulTicks,ulValue,ulStartCounter,ul_EAX_EDX,ulResult;
	//���ÿ��������
	QueryPerformanceFrequency(&ulFreq);
	//��õ�ǰ�����ܼ�������ֵ
	QueryPerformanceCounter(&ulTicks);
	ulValue.QuadPart=ulTicks.QuadPart+ulFreq.QuadPart;
	//rdtsc���ڻ��ָ����ȡʱ�������
    //��������ָ���ѼĴ����ĸ�32λ��ֵ��EDX����32λ��EAX�Ĵ���
	_asm{
		rdtsc
		mov ul_EAX_EDX.u.HighPart,edx
		mov ul_EAX_EDX.u.LowPart,eax
	}
	ulStartCounter.QuadPart=ul_EAX_EDX.QuadPart;
	do{
		QueryPerformanceCounter(&ulTicks);

	}while (ulTicks.QuadPart<=ulValue.QuadPart);
	//�õ�ʵ�ʵ������
	_asm{
		rdtsc
		mov ul_EAX_EDX.u.HighPart,edx
		mov ul_EAX_EDX.u.LowPart,eax
	}
	ulResult.QuadPart=ul_EAX_EDX.QuadPart-ulStartCounter.QuadPart;
	return (int)ulResult.QuadPart/1000000;

	
}


//���ڻ��CPU�ͺ�

SYSINFO_API char* GetCPUType(void){
	//�����Ǿ�̬�����������޷�������Ϣ
	static char pszCPUType[13];
	memset(pszCPUType,0,13);
	_asm{
		xor eax,eax
		cpuid
		//��EBX�Ĵ����л����Ϣ
		mov pszCPUType[0],bl
		mov pszCPUType[1],bh
		ror ebx,16
		mov pszCPUType[2],bl
		mov pszCPUType[3],bh
		//��ECX�Ĵ����л����Ϣ
		mov pszCPUType[4],dl
		mov pszCPUType[5],dh
		ror edx,16
		mov pszCPUType[6],dl
		mov pszCPUType[7],dh
		//��EDX�Ĵ����л����Ϣ
		mov pszCPUType[8],cl
		mov pszCPUType[9],ch
		ror ecx,16
		mov pszCPUType[10],cl
		mov pszCPUType[11],ch
		
	}
	return pszCPUType;
}
//���ڻ��CPU�ļ���
SYSINFO_API int GetCPUFamlily(void)
{
	int retval;
	_asm{
		mov eax,1
		cpuid
		mov retval,eax
	}
	return retval>>8;
}
//���ڻ��CPU��ģ��
SYSINFO_API int GetCPUModel(void)
{
	int retval;
	_asm{
		mov eax,1
			cpuid
			mov retval,eax
	}
	return retval>>4;
}

//���ڻ��CPU�Ĳ���
SYSINFO_API int GetCPUStepping(void)
{
	int retval;
	_asm{
		mov eax,1
			cpuid
			mov retval,eax
	}
	return retval & 0x0000000f;
}
