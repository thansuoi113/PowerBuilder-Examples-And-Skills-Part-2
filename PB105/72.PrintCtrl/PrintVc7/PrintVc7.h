// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
//��ı�׼�������� DLL �е������ļ��������������϶���� PRINTVC7_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
//�κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ 
// PRINTVC7_API ������Ϊ�ǴӴ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef PRINTVC7_EXPORTS
#define PRINTVC7_API __declspec(dllexport)
#else
#define PRINTVC7_API __declspec(dllimport)
#endif

// �����Ǵ� PrintVc7.dll ������
class PRINTVC7_API CPrintVc7 {
public:
	CPrintVc7(void);
	// TODO: �ڴ�������ķ�����
};

extern PRINTVC7_API int nPrintVc7;

PRINTVC7_API int fnPrintVc7(void);
