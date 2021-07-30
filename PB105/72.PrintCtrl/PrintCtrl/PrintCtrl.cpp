// PrintCtrl.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PrintCtrl.h"
#include "Printer.h"
#include "AboutDlg.h"

#include <WINSPOOL.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CPrintCtrlApp

BEGIN_MESSAGE_MAP(CPrintCtrlApp, CWinApp)
	//{{AFX_MSG_MAP(CPrintCtrlApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintCtrlApp construction

CPrintCtrlApp::CPrintCtrlApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPrintCtrlApp object

CPrintCtrlApp theApp;

typedef TCHAR PAPERNAME[64]; //打印机纸张名称类型

/////////////////////////////////////////////////////////////////////////////
extern "C" long PASCAL EXPORT PrintOpen_(LPTSTR pDocName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	//获得默认打印机名称
	PRINTDLG   pd;
	BOOL rt = FALSE;
	HANDLE hPrinter;

	pd.lStructSize = (DWORD) sizeof(PRINTDLG);
	rt = AfxGetApp()->GetPrinterDeviceDefaults(&pd);
	LPTSTR lpszDriverName = NULL;
	LPTSTR lpszDeviceName = NULL;
	LPTSTR lpszPortName = NULL;
	char pPrintName[255];
	memset(pPrintName,0,255);
	DWORD docNum = 0;
	int docNum2 = 0;
	//DOCINFO docInfo;
	DOC_INFO_1 docInfo1;
	//DOC_INFO_2 docInfo2;
	//HDC dc;
	CString s;

	DWORD errNum = 0;
	if(rt)
	{
		DEVMODE FAR* pDevMode = (DEVMODE FAR*)::GlobalLock(pd.hDevMode);
        if (pDevMode)
        {
            LPDEVNAMES lpDevNames;
			lpDevNames = (LPDEVNAMES)GlobalLock(pd.hDevNames);
			lpszDriverName = (LPTSTR )lpDevNames + lpDevNames->wDriverOffset; //驱动
			lpszDeviceName = (LPTSTR )lpDevNames + lpDevNames->wDeviceOffset; //设备
			lpszPortName   = (LPTSTR )lpDevNames + lpDevNames->wOutputOffset; //端口
			
			//获得默认打印机的名称
			strncpy(pPrintName,lpszDeviceName,strlen(lpszDeviceName)); 

			::GlobalUnlock(pd.hDevNames);
			::GlobalUnlock(pd.hDevMode);
			//一下程序用来打开打印机并开始一个文档
			rt = OpenPrinter(pPrintName, &hPrinter, 0);
			if(rt)
			{
				memset(&docInfo1,0,sizeof(DOC_INFO_1));
				
				docInfo1.pDocName = pDocName;
				docInfo1.pOutputFile = (LPTSTR) NULL; 
				docInfo1.pDatatype = (LPTSTR) NULL;
				//docInfo1.pDatatype = "RAW";


				docNum = StartDocPrinter(hPrinter,1,(LPBYTE)&docInfo1);
				if(docNum<=0)
				{
					ClosePrinter(hPrinter);
					return FALSE;
				}

				rt = StartPagePrinter(hPrinter);
				if(!rt)
				{
					EndDocPrinter(hPrinter);
					ClosePrinter(hPrinter);
					return FALSE;
				}
				//rt = EndPagePrinter(hPrinter);
				//rt = EndDocPrinter(hPrinter);
				//rt = ClosePrinter(hPrinter);
				/*
				memset(&docInfo,0,sizeof(DOCINFO));
				
				docInfo.lpszDocName = pDocName;
				docInfo.lpszOutput = (LPTSTR) NULL; 
				docInfo.lpszDatatype = (LPTSTR) NULL;
				docInfo.fwType = 0;

				dc = CreateDC("WINSPOOL", pPrintName, NULL, NULL);
				docNum2 = StartDoc(dc,&docInfo);
				if (docNum2==SP_ERROR)
				{
					DeleteDC(dc);
					return 0;
				}
				*/
			}
			else
				return FALSE;
			
		}
		else
			return FALSE;
	}
	else
		return FALSE;
	//return (long)dc;
	return (long)hPrinter;
}
//////////////////////////////////////////////////////////////////////////////
extern "C" long PASCAL EXPORT PrintClose_(long handle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	BOOL rt = FALSE;
	rt = EndPagePrinter((HANDLE)handle);
	if(!rt)
	{
		AfxMessageBox("结束页面错误！");
		return 0;
	}
	//rt = EndDoc((HDC)handle);
	//AfxMessageBox("开始结束文档！");
	rt = EndDocPrinter((HANDLE)handle);
	if(rt)
	{
		//AfxMessageBox("结束文档成功！");
		rt = ClosePrinter((HANDLE)handle);
		//rt = DeleteDC((HDC)handle);
		if(rt)
		{
			//AfxMessageBox("关闭打印机成功！");
			return 1;
		}
		else 
			return 0;
	}
	else
		return 0;

}
/////////////////////////////////////////////////////////////////////////////
extern "C" long PASCAL EXPORT PrintSend_(long handle, LPTSTR data, char zeroChar=0)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	HANDLE hPrinter;
	hPrinter = (HANDLE)handle;
	char *pBuffer = NULL;
	int dataLen = 0;
	dataLen = strlen(data);
	BOOL rt = FALSE;
	DWORD writeed = 0;
	if (dataLen>0)
	{
		pBuffer = new char[dataLen]; //分配空间
		memset(pBuffer,0,dataLen);
		strncpy(pBuffer, data, dataLen);
		//将指定的字符中0x0000代替
		for(int i=0;i<dataLen;i++)
		{
			if (data[i]==zeroChar)
				pBuffer[i] = 0;
		}
		//rt = StartPagePrinter(hPrinter);
		//if(!rt)
		//	AfxMessageBox("开始页错误");
		//数据准备完毕，开始写数据
		rt = WritePrinter(hPrinter, pBuffer, dataLen, &writeed);
		//rt = EndPagePrinter(hPrinter);
		//if(!rt)
		//	AfxMessageBox("结束页错误");
		//rt = TextOut((HDC)handle,0,0,pBuffer,dataLen); 
		if(rt)
			return writeed;
			//return dataLen;
		else
			return 0;
	}
	else
		return 0;

	return 1;
}
////////////////////////////////////////////////////////////////////
//显示关于对话框
extern "C" void PASCAL EXPORT About() 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	CAboutDlg dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////
//判断操作系统是不是NT
extern "C" BOOL PASCAL EXPORT IsWindowsNT() 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	CPrinter p;
	return p.IsWindowsNT();
}
///////////////////////////////////////////////////////////////////
//获得默认打印机名称
extern "C" BOOL PASCAL EXPORT GetDefaultPrinter(LPTSTR szPrinterName, int nPrintNameBufferLen)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
    CPrinter p;
	return p.GetDefaultPrinter(szPrinterName, nPrintNameBufferLen);
    //return TRUE;
}

extern "C" BOOL PASCAL EXPORT GetPrinterPort(LPTSTR szPrinterName, LPTSTR szPrinterPort, int nPortBufferLen, LPTSTR szPrinterDrvier, int nDriverBufferLen)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	CPrinter p;
	//CString s;
	//s = szPrinterName;
	//AfxMessageBox(s);
	return p.GetPrinterPort(szPrinterName, szPrinterPort, nPortBufferLen, szPrinterDrvier, nDriverBufferLen);
}
//////////////////////////////////////////////////////////////////////
extern "C" BOOL PASCAL EXPORT GetPrinterList(LPTSTR szPrinterNameList, long & nPrintNameBufferLen)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	CPrinter p;
	return p.GetPrinterList(szPrinterNameList, nPrintNameBufferLen);
}

extern "C" BOOL PASCAL EXPORT GetPrinterStatus(LPTSTR szPrinterName, long & status)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	CPrinter p;
	return p.GetPrinterStatus(szPrinterName, status);
}

extern "C" BOOL PASCAL EXPORT GetPaperList(LPTSTR szPrinterName, LPTSTR szPaperList, long & BufferLen)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	CPrinter p;
	return p.GetPaperList(szPrinterName, szPaperList, BufferLen);
}

extern "C" BOOL PASCAL EXPORT SetPrinterDefault(LPTSTR szPrinterName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	CPrinter p;
	return p.SetPrinterDefault(szPrinterName);
}

extern "C" BOOL PASCAL EXPORT AddCustomPaper(LPTSTR szPrinterName, LPTSTR szPaperName, long nWidth, long nHeight, long nLeft, long nRight, long nTop, long nBottom)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	CPrinter p;
	SIZE s;
	RECT r;
	s.cx = nWidth;
	s.cy = nHeight;
	r.left = nLeft;
	r.right = nRight;
	r.top = nTop;
	r.bottom = nTop;
	return p.AddCustomPaper(szPrinterName, szPaperName, s, r);
	
}

extern "C" BOOL PASCAL EXPORT DeleteCustomPaper(LPTSTR szPrinterName, LPTSTR szPaperName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	CPrinter p;
	return p.DeleteCustomPaper(szPrinterName, szPaperName);
}
//只有win95/98支持
extern "C" BOOL PASCAL EXPORT SetUserDefinePaper(LPTSTR szPrinterName, long & nWidth, long & nHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	CPrinter p;
	return p.SetUserDefinePaper(szPrinterName, nWidth, nHeight);
}
////
extern "C" BOOL PASCAL EXPORT GetUserDefinePaperMinMax(LPTSTR szPrinterName, long & nMinWidth, long & nMinHeight, long & nMaxWidth, long & nMaxHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	CPrinter p;
	return FALSE;
	//return p.GetUserDefinePaperMinMax(szPrinterName, nMinWidth, nMinHeight, nMaxWidth, nMaxHeight);
}
extern "C" BOOL PASCAL EXPORT GetDefaultPaper(LPTSTR szPrinterName, LPTSTR szPaperName,long & nPaperSize, long & nOrientation)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	CPrinter p;
	return p.GetDefaultPaper(szPrinterName, szPaperName, nPaperSize, nOrientation);
	//return TRUE;
}

extern "C" BOOL PASCAL EXPORT SetDefaultPaper(LPTSTR szPrinterName, LPTSTR szPaperName, short nOrientation)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	CPrinter p;
	return p.SetPaper(szPrinterName, szPaperName, nOrientation);
	//return TRUE;
}

extern "C" BOOL PASCAL EXPORT GetPaperName(LPTSTR szPrinterName, LPTSTR szPaperName, short nPaperSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	CPrinter p;
	LPTSTR pPortName = NULL;
	LPTSTR pDriverName = NULL;
	BOOL rt = FALSE;
	pPortName = new CHAR[64];
	pDriverName = new CHAR[64];
	rt = p.GetPrinterPort(szPrinterName, pPortName, 64, pDriverName, 64);
	if(rt)
	{
		rt = p.GetPaperName(szPrinterName, pPortName, nPaperSize, szPaperName);
		delete [] pPortName;
		delete [] pDriverName;
	}
	return rt;
}

extern "C" BOOL PASCAL EXPORT GetPaperSize(LPTSTR szPrinterName, LPTSTR szPaperName, short & nPaperSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	CPrinter p;
	LPTSTR pPortName = NULL;
	LPTSTR pDriverName = NULL;
	BOOL rt = FALSE;
	pPortName = new CHAR[64];
	pDriverName = new CHAR[64];
	rt = p.GetPrinterPort(szPrinterName, pPortName, 64, pDriverName, 64);
	if(rt)
	{
		nPaperSize = p.GetPaperSize(szPrinterName, pPortName, szPaperName);		
		if (nPaperSize>=0)
			rt = TRUE;
		delete [] pPortName;
		delete [] pDriverName;
	}
	return rt;
}
//获得送纸器的名称
extern "C" BOOL PASCAL EXPORT GetBinNameList(LPTSTR szPrinterName, LPTSTR szBinNameList)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	CPrinter p;
	return p.GetBinNameList(szPrinterName, szBinNameList);
	//return TRUE;
}
//
extern "C" BOOL PASCAL EXPORT GetBin(LPTSTR szPrinterName, LPTSTR szBinName, short & nBin)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	CPrinter p;
	return p.GetBin(szPrinterName, szBinName, nBin);
}
//
extern "C" BOOL PASCAL EXPORT GetBinName(LPTSTR szPrinterName, LPTSTR szBinName, short nBin)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// normal function body here
	CPrinter p;
	return p.GetBinName(szPrinterName, szBinName, nBin);
}