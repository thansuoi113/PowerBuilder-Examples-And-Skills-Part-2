// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
//��ı�׼�������� DLL �е������ļ��������������϶���� SYSINFO_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
//�κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ 
// SYSINFO_API ������Ϊ�ǴӴ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef SYSINFO_EXPORTS
#define SYSINFO_API __declspec(dllexport)
#else
#define SYSINFO_API __declspec(dllimport)
#endif



SYSINFO_API int GetCPUSpeed(void);
SYSINFO_API char* GetCPUType(void);
SYSINFO_API int GetCPUFamlily(void);
SYSINFO_API int GetCPUModel(void);
SYSINFO_API int GetCPUStepping(void);