// SysInfo.cpp : 定义 DLL 应用程序的入口点。
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



//用于获得CPU的速度
SYSINFO_API int GetCPUSpeed(void)
{
	LARGE_INTEGER ulFreq,ulTicks,ulValue,ulStartCounter,ul_EAX_EDX,ulResult;
	//获得每秒的嘀嗒数
	QueryPerformanceFrequency(&ulFreq);
	//获得当前的性能计数器的值
	QueryPerformanceCounter(&ulTicks);
	ulValue.QuadPart=ulTicks.QuadPart+ulFreq.QuadPart;
	//rdtsc奔腾汇编指令会读取时间戳计数
    //用这个汇编指令会把寄存器的高32位赋值给EDX，低32位给EAX寄存器
	_asm{
		rdtsc
		mov ul_EAX_EDX.u.HighPart,edx
		mov ul_EAX_EDX.u.LowPart,eax
	}
	ulStartCounter.QuadPart=ul_EAX_EDX.QuadPart;
	do{
		QueryPerformanceCounter(&ulTicks);

	}while (ulTicks.QuadPart<=ulValue.QuadPart);
	//得到实际的嘀嗒数
	_asm{
		rdtsc
		mov ul_EAX_EDX.u.HighPart,edx
		mov ul_EAX_EDX.u.LowPart,eax
	}
	ulResult.QuadPart=ul_EAX_EDX.QuadPart-ulStartCounter.QuadPart;
	return (int)ulResult.QuadPart/1000000;

	
}


//用于获得CPU型号

SYSINFO_API char* GetCPUType(void){
	//必须是静态变量，否则无法返回信息
	static char pszCPUType[13];
	memset(pszCPUType,0,13);
	_asm{
		xor eax,eax
		cpuid
		//从EBX寄存器中获得信息
		mov pszCPUType[0],bl
		mov pszCPUType[1],bh
		ror ebx,16
		mov pszCPUType[2],bl
		mov pszCPUType[3],bh
		//从ECX寄存器中获得信息
		mov pszCPUType[4],dl
		mov pszCPUType[5],dh
		ror edx,16
		mov pszCPUType[6],dl
		mov pszCPUType[7],dh
		//从EDX寄存器中获得信息
		mov pszCPUType[8],cl
		mov pszCPUType[9],ch
		ror ecx,16
		mov pszCPUType[10],cl
		mov pszCPUType[11],ch
		
	}
	return pszCPUType;
}
//用于获得CPU的家族
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
//用于获得CPU的模型
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

//用于获得CPU的步长
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
