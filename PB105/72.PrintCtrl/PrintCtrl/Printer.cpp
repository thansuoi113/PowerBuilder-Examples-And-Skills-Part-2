// Printer.cpp: implementation of the CPrinter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PrintCtrl.h"
#include "Printer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrinter::CPrinter()
{

}

CPrinter::~CPrinter()
{

}

//检测系统是否为Windows NT/2000/XP
BOOL CPrinter::IsWindowsNT()
{
    OSVERSIONINFO vi;
    vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&vi);
    return (vi.dwPlatformId == VER_PLATFORM_WIN32_NT);
}
int CPrinter::GetWindowsVer()
{
	OSVERSIONINFO vi;
    vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&vi);
	//isNT = (vi.dwPlatformId == VER_PLATFORM_WIN32_NT);
	return (int)vi.dwMajorVersion;
	
}
//获得默认打印机名称，端口和默认的纸张
BOOL CPrinter::GetDefaultPrinterAndPaper(LPTSTR szPrinterName, int nPrintNameBufferLen, LPTSTR szPrinterPort, int nPrinterPortBufferLen, LPTSTR szPaperName)
{
    *szPrinterName = 0;
    *szPaperName = 0;
    CPrintDialog pd(FALSE);

    if (pd.GetDefaults())
    {
        if (pd.m_pd.hDC) 
			DeleteDC(pd.m_pd.hDC);
        if (pd.m_pd.hDevMode)
        {
            LPDEVMODE pdm = (LPDEVMODE)GlobalLock(pd.m_pd.hDevMode);
            *(szPaperName + 63) = 0;
            _tcsncpy(szPaperName, (LPCTSTR)pdm->dmFormName, 63); //打印纸张名称
            GlobalUnlock(pd.m_pd.hDevMode);
            GlobalFree(pd.m_pd.hDevMode);
        }
        if (pd.m_pd.hDevNames)
        {
            LPDEVNAMES pdn = (LPDEVNAMES)GlobalLock(pd.m_pd.hDevNames);
            nPrintNameBufferLen--;
            *(szPrinterName + nPrintNameBufferLen) = 0;
            _tcsncpy(szPrinterName, (LPTSTR)pdn + pdn->wDeviceOffset, nPrintNameBufferLen); //打印机名称
			_tcsncpy(szPrinterPort, (LPTSTR)pdn + pdn->wOutputOffset, nPrinterPortBufferLen); //打印机端口
            GlobalUnlock(pd.m_pd.hDevNames);
            GlobalFree(pd.m_pd.hDevNames);
        }
    }
    return (*szPrinterName && *szPaperName);
}
//增加规格自定义纸张
//szPaperName: 自定义纸张名称
//PaperSize: 纸张的大小，以0.1mm为单位
//rcPrintableMargin: 打印机的最小可打印边界，以0.1mm为单位。
// 可参见GetDeviceCaps函数说明中的PHYSICALOFFSETX及PHYSICALOFFSETY
BOOL CPrinter::AddCustomPaper(LPTSTR szPrinterName, PAPERNAME szPaperName, SIZE PaperSize, RECT rcPrintableMargin)
{
    BOOL bOk = FALSE;
    if (IsWindowsNT()) //Windows NT4/2000/XP才支持
    {
        FORM_INFO_1 fi1;
        fi1.Flags = FORM_USER;
        fi1.pName = (LPTSTR)szPaperName;
        fi1.Size.cx = PaperSize.cx * 100;
        fi1.Size.cy = PaperSize.cy * 100;
        fi1.ImageableArea.left = rcPrintableMargin.left * 100;
        fi1.ImageableArea.top = rcPrintableMargin.top * 100;
        fi1.ImageableArea.right = fi1.Size.cx - rcPrintableMargin.right * 100;
        fi1.ImageableArea.bottom = fi1.Size.cy - rcPrintableMargin.bottom * 100;
        HANDLE hPrinter = GetPrinterHandle(szPrinterName);
        if (hPrinter)
        {
            bOk = (SetForm(hPrinter, (LPSTR)szPaperName, 1, (LPBYTE)&fi1) || //已存在该类型纸张则更改
                   AddForm(hPrinter, 1, (LPBYTE)&fi1)); //否则添加此自定义纸张
            ClosePrinter(hPrinter);
        }
    }
    return bOk;
}
//删除自定义规格纸张
BOOL CPrinter::DeleteCustomPaper(LPTSTR szPrinterName, LPTSTR szPaperName)
{
    BOOL bOk = FALSE;
    if (IsWindowsNT()) //Windows NT4/2000/XP才支持
    {
        HANDLE hPrinter = GetPrinterHandle(szPrinterName);
        if (hPrinter)
        {
            bOk = DeleteForm(hPrinter, (LPSTR)szPaperName);
            ClosePrinter(hPrinter);
        }
    }
    return bOk;
}
//设置自定义纸张的大小，仅对Win95/98有效
BOOL CPrinter::SetUserDefinePaper(LPTSTR szPrinterName, long & nWidth, long & nHeight)
{
    BOOL bOk = FALSE;
    if (!IsWindowsNT()) //Windows 95/98才支持
    {
        PRINTER_INFO_2 *ppi2 = GetInfo2(szPrinterName);
		if (ppi2)
		{
			//CString s;
			//s.Format("ppi2->pDevMode->dmPaperSize = %d,ppi2->pDevMode->dmPaperWidth = %d,ppi2->pDevMode->dmPaperLength = %d",ppi2->pDevMode->dmPaperSize,ppi2->pDevMode->dmPaperWidth,ppi2->pDevMode->dmPaperLength);
			//AfxMessageBox(s);
			ppi2->pDevMode->dmFields = DM_PAPERSIZE|DM_PAPERWIDTH|DM_PAPERLENGTH;
			ppi2->pDevMode->dmPaperSize = (WORD)DMPAPER_USER;
			ppi2->pDevMode->dmPaperWidth = (WORD)nWidth;
			ppi2->pDevMode->dmPaperLength = (WORD)nHeight;
			bOk = SetInfo2(ppi2);
			GlobalFree((HGLOBAL)ppi2);
			if(!bOk)
				return bOk;
		}
		ppi2 = GetInfo2(szPrinterName);
		if (ppi2)
		{
			nWidth = ppi2->pDevMode->dmPaperWidth;
			nHeight = ppi2->pDevMode->dmPaperLength;
			GlobalFree((HGLOBAL)ppi2);
			bOk = TRUE;
		}
    }
    return bOk;
}
//设置自定义纸张的大小，仅对Win95/98有效
//该函数有问题，暂时不能使用
BOOL CPrinter::GetUserDefinePaperMinMax(LPTSTR szPrinterName, long & nMinWidth, long & nMinHeight, long & nMaxWidth, long & nMaxHeight)
{
    BOOL bOk = FALSE;
	long nOldWidth = 0; 
	long nOldHeight = 0;
	long nOldPaperSize = 0;
	CString s;
    if (!IsWindowsNT()) //Windows 95/98才支持
    {
        PRINTER_INFO_2 *ppi2 = GetInfo2(szPrinterName);
		//获得老的纸张信息
		if (ppi2)
		{
			nOldPaperSize = ppi2->pDevMode->dmPaperSize; 
			nOldWidth = ppi2->pDevMode->dmPaperWidth;
			nOldHeight = ppi2->pDevMode->dmPaperLength;
			s.Format("nOldPaperSize = %d, nOldWidth = %d, nOldHeight = %d",nOldPaperSize, nOldWidth, nOldHeight);
			AfxMessageBox(s);
			GlobalFree((HGLOBAL)ppi2);
		}
		////////////////////////////////////
		//取最小
		ppi2 = GetInfo2(szPrinterName);
		if (ppi2)
		{
			ppi2->pDevMode->dmFields = DM_PAPERWIDTH|DM_PAPERLENGTH;
			ppi2->pDevMode->dmPaperWidth = (WORD)1;
			ppi2->pDevMode->dmPaperLength = (WORD)1;
			bOk = SetInfo2(ppi2);
			GlobalFree((HGLOBAL)ppi2);
		}
		ppi2 = GetInfo2(szPrinterName);
		if (ppi2)
		{
			nMinWidth = ppi2->pDevMode->dmPaperWidth;
			nMinHeight = ppi2->pDevMode->dmPaperLength;
			s.Format("nMinWidth = %d, nMinHeight = %d",nMinWidth, nMinHeight);
			AfxMessageBox(s);
			GlobalFree((HGLOBAL)ppi2);
		}
		////////////////////////////////////
		//取最大
		ppi2 = GetInfo2(szPrinterName);
		if (ppi2)
		{
			ppi2->pDevMode->dmFields = DM_PAPERWIDTH|DM_PAPERLENGTH;
			ppi2->pDevMode->dmPaperWidth = (WORD)10000000;
			ppi2->pDevMode->dmPaperLength = (WORD)10000000;
			bOk = SetInfo2(ppi2);
			GlobalFree((HGLOBAL)ppi2);
		}
		ppi2 = GetInfo2(szPrinterName);
		if (ppi2)
		{
			nMaxWidth = ppi2->pDevMode->dmPaperWidth;
			nMaxHeight = ppi2->pDevMode->dmPaperLength;
			s.Format("nMaxWidth = %d, nMaxHeight = %d",nMaxWidth, nMaxHeight);
			AfxMessageBox(s);
			GlobalFree((HGLOBAL)ppi2);
		}
		////恢复原来的
		ppi2 = GetInfo2(szPrinterName);
		if (ppi2)
		{
			ppi2->pDevMode->dmFields = DM_PAPERSIZE|DM_PAPERWIDTH|DM_PAPERLENGTH;
			ppi2->pDevMode->dmPaperSize = (WORD)nOldPaperSize;
			ppi2->pDevMode->dmPaperWidth = (WORD)nOldWidth;
			ppi2->pDevMode->dmPaperLength = (WORD)nOldHeight;
			bOk = SetInfo2(ppi2);
			GlobalFree((HGLOBAL)ppi2);
			bOk = TRUE;
		}
		////

    }
    return bOk;
}
//获取打印机句柄
HANDLE CPrinter::GetPrinterHandle(LPTSTR szPrinterName)
{
    PRINTER_DEFAULTS pds;
    HANDLE hPrinter = NULL;
    ZeroMemory(&pds, sizeof(PRINTER_DEFAULTS));
    pds.DesiredAccess = PRINTER_ALL_ACCESS;
    OpenPrinter(szPrinterName, &hPrinter, &pds);
    return hPrinter;
}
//由纸张名称得到对应的DEVMODE中的那个dmPaperSize值，返回-1表示有错误
short CPrinter::GetPaperSize(LPTSTR szPrinterName, LPTSTR szPortName, PAPERNAME szPaperName)
{
    short nPaperSize = -1;
    //获得可用打印机纸张类型数目
    int nNeeded = DeviceCapabilities(szPrinterName, szPortName, DC_PAPERNAMES, NULL, NULL);
    if (nNeeded)
    {
        PAPERNAME *pszPaperNames = new PAPERNAME[nNeeded]; //分配纸张名称数组
        //获得可用打印机纸张名称数组
        if (DeviceCapabilities(szPrinterName, szPortName, DC_PAPERNAMES, (LPTSTR)pszPaperNames, NULL) != -1)
        {
            int i;
            //查找纸张类型szPaperName在数组中的索引
            for (i = 0; i < nNeeded && _tcscmp(pszPaperNames[i], szPaperName); i++);
            if (i < nNeeded)
            {
                //获得可用打印机纸张尺寸号数目(应该等于打印机纸张类型数目)
                nNeeded = DeviceCapabilities(szPrinterName, szPortName, DC_PAPERS, NULL, NULL);
                if (nNeeded)
                {
                    LPWORD pPapers = new WORD[nNeeded]; //分配纸张尺寸号数组
                    //获得可用打印机纸张尺寸号数组
                    if (DeviceCapabilities(szPrinterName, szPortName, DC_PAPERS, (LPSTR)pPapers, NULL) != -1)
					{
                        nPaperSize = pPapers[i]; //获得纸张类型szPaperName对应的尺寸号
					}
                    delete []pPapers;
                }
            }
        }
        delete []pszPaperNames;
    }
    return nPaperSize;
}
//由纸张编号得到对应的纸张名称，返回FALSE表示有错误,TRUE表示成功，纸张名称在参数szPaperName中返回
BOOL CPrinter::GetPaperName(LPTSTR szPrinterName, LPTSTR szPortName, short nPaperSize, PAPERNAME szPaperName)
{
	BOOL rt = FALSE;
    //获得可用打印机纸张类型数目
    int nNeeded = DeviceCapabilities(szPrinterName, szPortName, DC_PAPERNAMES, NULL, NULL);
    if (nNeeded)
    {
        PAPERNAME *pszPaperNames = new PAPERNAME[nNeeded]; //分配纸张名称数组
        //获得可用打印机纸张名称数组
        if (DeviceCapabilities(szPrinterName, szPortName, DC_PAPERNAMES, (LPTSTR)pszPaperNames, NULL) != -1)
        {
			//获得可用打印机纸张尺寸号数目(应该等于打印机纸张类型数目)
            nNeeded = DeviceCapabilities(szPrinterName, szPortName, DC_PAPERS, NULL, NULL);
            if (nNeeded)
            {
                LPWORD pPapers = new WORD[nNeeded]; //分配纸张尺寸号数组
                //获得可用打印机纸张尺寸号数组
                if (DeviceCapabilities(szPrinterName, szPortName, DC_PAPERS, (LPSTR)pPapers, NULL) != -1)
				{
					int i = 0;
					//查找纸张尺寸nPaperSize在数组中的索引
					BOOL bFind = FALSE;
					int k = 0;
					for(k=0;k<nNeeded;k++)
					{
						if(pPapers[k]==nPaperSize)
						{
							bFind = TRUE;
							break;
						}
					}
					if(bFind)
					{
						//找到指定的纸张Size，返回对应的纸张名称
						_tcsncpy(szPaperName, pszPaperNames[k], 64);
						rt = TRUE;
					}
                    //nPaperSize = pPapers[i]; //获得纸张类型szPaperName对应的尺寸号
				}
                delete []pPapers;
            }
        }
        delete []pszPaperNames;
    }
    //return nPaperSize;
	return rt;
}
BOOL CPrinter::GetDefaultPaper(LPTSTR szPrinterName, LPTSTR szPaperName, long & nPaperSize, long & nOrientation)
{
	BOOL bOk = FALSE;
    PRINTER_INFO_2 *ppi2 = GetInfo2(szPrinterName);
    if (ppi2)
    {
		LPDEVMODE pdm = (ppi2->pDevMode);
		if(pdm)
		{
			//*(szPaperName + 63) = 0;
			memset(szPaperName, 0, 64);
			_tcsncpy(szPaperName, (LPCTSTR)pdm->dmFormName, 63); //打印纸张名称
			nOrientation = (long)pdm->dmOrientation; //打印方向
			nPaperSize = (long)pdm->dmPaperSize;
			//CString s;
			//s.Format("width = %d, height = %d",pdm->dmPaperWidth, pdm->dmPaperLength);
			//AfxMessageBox(s);
			//GlobalUnlock(ppi2->pDevMode);
			bOk = TRUE;
		}
		GlobalFree((HGLOBAL)ppi2);
	}
	return bOk;
}
//设置打印机的默认纸张和方向
BOOL CPrinter::SetPaper(LPTSTR szPrinterName, PAPERNAME szPaperName, short nOrientation)
{
    BOOL bOk = FALSE;
    PRINTER_INFO_2 *ppi2 = GetInfo2(szPrinterName);
    if (ppi2)
    {
        short nPaperSize = GetPaperSize(szPrinterName, ppi2->pPortName, szPaperName);
        if (nPaperSize != -1)
        {
            ppi2->pDevMode->dmFields = DM_PAPERSIZE|DM_PAPERWIDTH|DM_PAPERLENGTH|DM_ORIENTATION;
            ppi2->pDevMode->dmPaperSize = nPaperSize;
            ppi2->pDevMode->dmPaperWidth = 0;
            ppi2->pDevMode->dmPaperLength = 0;
            ppi2->pDevMode->dmOrientation = nOrientation;
            bOk = SetInfo2(ppi2);
        }
        GlobalFree((HGLOBAL)ppi2);
    }
    return bOk;
}
//获取打印机详细信息，返回的指针用后必须以GlobalFree释放
PRINTER_INFO_2 * CPrinter::GetInfo2(LPTSTR szPrinterName)
{
    HANDLE hPrinter = GetPrinterHandle(szPrinterName);
    PRINTER_INFO_2 *ppi2 = NULL;
    DWORD cbNeeded = 0;
    if (hPrinter)
    {
        GetPrinter(hPrinter, 2, 0, 0, &cbNeeded);
        if (cbNeeded)
        {
            ppi2 = (PRINTER_INFO_2 *)GlobalAlloc(GPTR, cbNeeded);
            if (ppi2)
            {
                if (!GetPrinter(hPrinter, 2, (LPBYTE)ppi2, cbNeeded, &cbNeeded))
                {
                    GlobalFree((HGLOBAL)ppi2);
                    ppi2 = NULL;
                }
            }
        }
        ClosePrinter(hPrinter);
    }
    return ppi2;
}
//打印机设置
BOOL CPrinter::SetInfo2(PRINTER_INFO_2 *ppi2)
{
    HANDLE hPrinter = GetPrinterHandle(ppi2->pPrinterName);
    BOOL bOk = FALSE;
    DWORD fMode;
    if (hPrinter)
    {
        fMode = DM_IN_BUFFER | DM_OUT_BUFFER;
        bOk = (DocumentProperties(NULL, hPrinter,
                                  ppi2->pPrinterName,
                                  ppi2->pDevMode,
                                  ppi2->pDevMode,
                                  fMode) == IDOK &&
              ::SetPrinter(hPrinter, 2, (LPBYTE)ppi2, 0));
        ClosePrinter(hPrinter);
    }
    return bOk;
}
////////////////////
BOOL CPrinter::GetPrinterPort(LPTSTR szPrinterName, LPTSTR szPrinterPort, int nPortBufferLen, LPTSTR szPrinterDriver, int nDriverBufferLen)
{
	BOOL bOk = FALSE;
    PRINTER_INFO_2 *ppi2 = GetInfo2(szPrinterName);
    if (ppi2)
    {
		_tcsncpy(szPrinterPort, ppi2->pPortName, nPortBufferLen); //打印机端口
		_tcsncpy(szPrinterDriver, ppi2->pDriverName, nDriverBufferLen); //打印机驱动名称

		bOk = TRUE;
        GlobalFree((HGLOBAL)ppi2);
    }
    return bOk;
}
/////////////////////////
BOOL CPrinter::GetPaperList(LPTSTR szPrinterName, LPTSTR szPaperList, long & nBufferLen)
{
	//DMORIENT_PORTRAIT   横向
	//DMORIENT_LANDSCAPE  纵向
	BOOL bOk = FALSE;
	CString s,temp;
    PRINTER_INFO_2 *ppi2 = GetInfo2(szPrinterName);
    if (ppi2)
    {
		//获得可用打印机纸张类型数目
		int nNeeded = DeviceCapabilities(szPrinterName, ppi2->pPortName, DC_PAPERNAMES, NULL, NULL);
		if (nNeeded)
		{
			PAPERNAME *pszPaperNames = new PAPERNAME[nNeeded]; //分配纸张名称数组
			//获得可用打印机纸张名称数组
			if (DeviceCapabilities(szPrinterName, ppi2->pPortName, DC_PAPERNAMES, (LPTSTR)pszPaperNames, NULL) != -1)
			{
				for(int i=0;i<nNeeded;i++)
				{
					temp = pszPaperNames[i];
					s = s+temp+"\t";
				}
				
				if (s.GetLength() > nBufferLen)
				{
					nBufferLen = s.GetLength()+1;
				}
				else
				{
					strncpy(szPaperList, s, s.GetLength());
					bOk = TRUE;
				}
			}
			delete []pszPaperNames;
		}
		GlobalFree((HGLOBAL)ppi2);
	}
    //return nPaperSize;
	return bOk;
}
//////////
BOOL CPrinter::GetDefaultPrinter(LPTSTR szPrinterName, int nPrintNameBufferLen)
{
	*szPrinterName = 0;
    CPrintDialog pd(FALSE);
    if (pd.GetDefaults())
    {
        if (pd.m_pd.hDC) 
			DeleteDC(pd.m_pd.hDC);
        
        if (pd.m_pd.hDevNames)
        {
            LPDEVNAMES pdn = (LPDEVNAMES)GlobalLock(pd.m_pd.hDevNames);
            nPrintNameBufferLen--;
            *(szPrinterName + nPrintNameBufferLen) = 0;
            _tcsncpy(szPrinterName, (LPTSTR)pdn + pdn->wDeviceOffset, nPrintNameBufferLen); //打印机名称
            GlobalUnlock(pd.m_pd.hDevNames);
            GlobalFree(pd.m_pd.hDevNames);
        }
    }
    return (*szPrinterName);
}
////////////////////////////////////////
BOOL CPrinter::GetPrinterList(LPTSTR szPrinterNameList, long & nPrintNameBufferLen)
{
	PRINTER_INFO_2 *pPrinterInfo = NULL;
	DWORD dFlags = 0;
	DWORD cbNeeded;
	DWORD cbReturned;
	BOOL rt = FALSE;
	LPBYTE buff = NULL;

	CString s,temp;

	if (IsWindowsNT())
	{
		dFlags = PRINTER_ENUM_CONNECTIONS | PRINTER_ENUM_LOCAL;
	}
	else
	{
		dFlags = PRINTER_ENUM_LOCAL;
	}
	rt = EnumPrinters(dFlags, NULL, 2, NULL, NULL, &cbNeeded, &cbReturned);
	if (!rt)
	{
		if (cbNeeded>0)
		{
			buff = new BYTE[cbNeeded];
			rt = EnumPrinters(dFlags, NULL, 2, buff, cbNeeded, &cbNeeded, &cbReturned);
			if(rt)
			{
				pPrinterInfo = (PRINTER_INFO_2 *)buff;
				for(DWORD i=0;i<cbReturned;i++)
				{
					temp = pPrinterInfo[i].pPrinterName;
					s = s+temp+"\t";
				}
				memset(szPrinterNameList,0,nPrintNameBufferLen);
				if (s.GetLength() > nPrintNameBufferLen)
				{
					nPrintNameBufferLen = s.GetLength();
					rt = FALSE;
				}
				else
				{
					strncpy(szPrinterNameList, s, s.GetLength());
					rt = TRUE;
				}
				
			}
		}
	}
	return rt;
}
////////////////////////////////////////////////////////////////////
BOOL CPrinter::GetPrinterStatus(LPTSTR szPrinterName, long & status)
{
	BOOL bOk = FALSE;
	
    PRINTER_INFO_2 *ppi2 = GetInfo2(szPrinterName);
	if (ppi2)
    {
		status = (long)ppi2->Status;
		GlobalFree((HGLOBAL)ppi2);
		bOk = TRUE;
	}
	return bOk;
}
BOOL CPrinter::SetPrinterDefault(LPTSTR szPrinterName)
{
	BOOL rt = FALSE;
	if (IsWindowsNT())
	{
		if(GetWindowsVer()>=5) //win2000/2003/xp
		{
			rt = SetDefaultPrinter(szPrinterName);
		}
		else //win nt 4 要写win.ini文件
		{
			LPTSTR pBuffer = NULL;
			PRINTER_INFO_2 *ppi2 = GetInfo2(szPrinterName);
			if(ppi2)
			{
				pBuffer = new CHAR[lstrlen(szPrinterName) + lstrlen(ppi2->pDriverName) + lstrlen(ppi2->pPortName) + 3];
				lstrcpy(pBuffer, szPrinterName);  
				lstrcat(pBuffer, ",");
				lstrcat(pBuffer, ppi2->pDriverName);  
				lstrcat(pBuffer, ",");
				lstrcat(pBuffer, ppi2->pPortName);
				//写到win.ini文件中
				rt = WriteProfileString("windows", "device", pBuffer);
				delete [] pBuffer;
				GlobalFree((HGLOBAL)ppi2);
				SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0L, 0L, SMTO_NORMAL, 1000, NULL);
			}
		}
	}
	else //win98,win95使用SetPrinter函数，修改成员Attributes
	{
		PRINTER_INFO_2 *ppi2 = GetInfo2(szPrinterName);
		if(ppi2)
		{
			ppi2->Attributes |= PRINTER_ATTRIBUTE_DEFAULT;
			rt = SetInfo2(ppi2);
			GlobalFree((HGLOBAL)ppi2);
			SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0L,
							(LPARAM)(LPCTSTR)"windows", SMTO_NORMAL, 1000, NULL);
		}
	}
	
	return rt;
}
//获得打印机的送纸器名称列表
BOOL CPrinter::GetBinNameList(LPTSTR szPrinterName, LPTSTR szBinNameList)
{
	BOOL rt = FALSE;
	LPTSTR pPortName = NULL;
	LPTSTR pDriverName = NULL;
	LPTSTR pBinNameList = NULL;
	pPortName = new CHAR[64];
	pDriverName = new CHAR[64];
	long binCount = 0;
	rt = GetPrinterPort(szPrinterName, pPortName, 64, pDriverName, 64);
	if (rt)
	{
        binCount = DeviceCapabilities(szPrinterName, pPortName, DC_BINNAMES, pBinNameList, NULL);
		if(binCount)
		{
			pBinNameList = new CHAR[binCount * 24];
			memset(pBinNameList,0, binCount * 24);
			CHAR BinName[24];
			binCount = DeviceCapabilities(szPrinterName, pPortName, DC_BINNAMES, pBinNameList, NULL);
			if(binCount)
			{
				CString s;
				CString t;
				for(int i=0;i<binCount;i++)
				{
					memset(BinName, 0, 24);
					memcpy(BinName, pBinNameList+(i*24),24);
					t = BinName;
					s = s+t+"\t";
				}
				lstrcpy(szBinNameList, s);
				rt = TRUE;
			}
			delete [] pBinNameList;
		}
              
        
    }
	delete [] pPortName;
	delete [] pDriverName;
	return rt;
}
//由指定的送纸器的名称，获得送纸器的代码
BOOL CPrinter::GetBin(LPTSTR szPrinterName, LPTSTR szBinName, short & nBin)
{
	BOOL rt = FALSE;
	LPTSTR pPortName = NULL;
	LPTSTR pDriverName = NULL;
	LPTSTR pBinNameList = NULL; //送纸器的名称列表
	WORD *pBinList = NULL; ////送纸器的代吗列表
	BOOL bGetBinNameList = FALSE;
	BOOL bGetBinList = FALSE;
	pPortName = new CHAR[64];
	pDriverName = new CHAR[64];
	long binCount = 0;
	rt = GetPrinterPort(szPrinterName, pPortName, 64, pDriverName, 64);
	if (rt)
	{
        binCount = DeviceCapabilities(szPrinterName, pPortName, DC_BINNAMES, pBinNameList, NULL);
		if(binCount)
		{
			//获得送纸器的名称列表
			pBinNameList = new CHAR[binCount * 24];
			memset(pBinNameList,0, binCount * 24);
			CHAR BinName[24];
			binCount = DeviceCapabilities(szPrinterName, pPortName, DC_BINNAMES, pBinNameList, NULL);
			if(binCount)
			{
				bGetBinNameList = TRUE;
			}
			pBinList = new WORD[sizeof(WORD) * 24 * binCount];
			binCount = DeviceCapabilities(szPrinterName, pPortName, DC_BINS, (LPTSTR)pBinList, NULL);
			if(binCount)
			{
				bGetBinList = TRUE;
			}
			if (bGetBinNameList && bGetBinList)
			{
				long k = -1;
				for(long i=0;i<binCount;i++)
				{
					memset(BinName, 0, 24);
					memcpy(BinName, pBinNameList + (i * 24), 24);
					if(lstrcmp(szBinName, BinName)==0)
					{
						k = i;
						break;
					}
				}
				if(k>=0)
				{
					nBin = pBinList[k];
					rt = TRUE;
				}
				else
				{
					nBin = -1;
				}	
			}
			delete [] pBinNameList;
			delete [] pBinList;
		}
    }
	delete [] pPortName;
	delete [] pDriverName;
	return rt;
}
//由指定的送纸器的代码，返回送纸器的名称
BOOL CPrinter::GetBinName(LPTSTR szPrinterName, LPTSTR szBinName, short nBin)
{
	BOOL rt = FALSE;
	LPTSTR pPortName = NULL;
	LPTSTR pDriverName = NULL;
	LPTSTR pBinNameList = NULL; //送纸器的名称列表
	WORD *pBinList = NULL; ////送纸器的代吗列表
	BOOL bGetBinNameList = FALSE;
	BOOL bGetBinList = FALSE;
	pPortName = new CHAR[64];
	pDriverName = new CHAR[64];
	long binCount = 0;

	rt = GetPrinterPort(szPrinterName, pPortName, 64, pDriverName, 64);
	if (rt)
	{
        binCount = DeviceCapabilities(szPrinterName, pPortName, DC_BINNAMES, pBinNameList, NULL);
		if(binCount)
		{
			//获得送纸器的名称列表
			pBinNameList = new CHAR[binCount * 24];
			memset(pBinNameList,0, binCount * 24);
			//CHAR BinName[24];
			binCount = DeviceCapabilities(szPrinterName, pPortName, DC_BINNAMES, pBinNameList, NULL);
			if(binCount)
			{
				bGetBinNameList = TRUE;
			}
			pBinList = new WORD[sizeof(WORD) * 24 * binCount];
			binCount = DeviceCapabilities(szPrinterName, pPortName, DC_BINS, (LPTSTR)pBinList, NULL);
			if(binCount)
			{
				bGetBinList = TRUE;
			}
			if (bGetBinNameList && bGetBinList)
			{
				long k = -1;
				for(long i=0;i<binCount;i++)
				{
					if(pBinList[i]==nBin)
					{
						k = i;
						break;
					}
				}
				if(k>=0)
				{
					memcpy(szBinName, pBinNameList + (k * 24), 24);
					rt = TRUE;
				}
			}
			delete [] pBinNameList;
			delete [] pBinList;
		}
    }
	delete [] pPortName;
	delete [] pDriverName;
	return rt;
}