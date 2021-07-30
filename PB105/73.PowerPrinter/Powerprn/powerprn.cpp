//+-------------------------------------------------------------------
//
//  PowerPrinter
//
//  Copyright (c) 1997, 1998, 1999, 2000, 2001
//  SecureWave. All rights reserved
//
//  See the file LICENSE.TXT for redistribution information.
//
//  File:        PowerPrn.cpp
//
//  Contents:    DLL interface file. Allows us to call C++ member
//               functions
//				 Define as well initialization routines for the DLL.
//--------------------------------------------------------------------+

// NB: Check out CPP functions. All other utility functions utilize
// The pointer returned by CWin32Prn_CPP_CONSTRUCTOR is used to access
// Member functions via C functions. The pointer is called this_hdl

#include "stdafx.h"
#include "powerprn.h"

#include "cwin32prn.h"
#include "cwinntprn.h"
#include "cwin95prn.h"
#include "about.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPowerprnApp

BEGIN_MESSAGE_MAP(CPowerprnApp, CWinApp)
	//{{AFX_MSG_MAP(CPowerprnApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPowerprnApp construction

CPowerprnApp::CPowerprnApp()
{

}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPowerprnApp object

CPowerprnApp theApp;


//************************************************************
//	Object:	PowerPrn
//	Method:	GetWinVersion
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	
//	Return:	
//
//	Desc  :	Return Windows Platform. version is not used for now
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
DWORD dwGetWinVersion()
{
	OSVERSIONINFO osvi;
	
   memset(&osvi, 0, sizeof(OSVERSIONINFO));
   osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
   GetVersionEx (&osvi);
 
   /*
   char  szVersion [80];
 
   if (osvi.dwPlatformId == VER_PLATFORM_WIN32s)
      wsprintf (szVersion, "Microsoft Win32s %d.%d (Build %d)",
                osvi.dwMajorVersion,
                osvi.dwMinorVersion,
                osvi.dwBuildNumber & 0xFFFF);
 
   else if (osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	  
      wsprintf (szVersion, "Microsoft Windows 95 %d.%d (Build %d)",
                osvi.dwMajorVersion,
                osvi.dwMinorVersion,
                osvi.dwBuildNumber & 0xFFFF);
	
   else if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)

	
      wsprintf (szVersion, "Microsoft Windows NT %d.%d (Build %d)",
                osvi.dwMajorVersion,
                osvi.dwMinorVersion,
                osvi.dwBuildNumber & 0xFFFF);
	
   */

	return osvi.dwPlatformId;
}


//************************************************************
//	Object:	PowerPrn
//	Method:	CWin32Prn_CPP_CONSTRUCTOR
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	
//	Return:	pointer to Printer class
//
//	Desc  :	Create an instance of either CWin95 or CWinNT
//			according to OS. 
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
STDAPI_ (CWin32Printer*) CWin32Prn_CPP_CONSTRUCTOR() 
{

	if (dwGetWinVersion() == VER_PLATFORM_WIN32_NT)
		return (new CWinNTPrinter);
	else
		return (new CWin95Printer);
}                             

//************************************************************
//	Object:	PowerPrn
//	Method:	CWin32Prn_CPP_DESTRUCTOR
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	
//	Return:	
//
//	Desc  :	Free mem 
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
void API32 CWin32Prn_CPP_DESTRUCTOR(CWin32Printer *this_hdl)
{
	if (this_hdl != NULL)
		delete this_hdl;
}                                 


//************************************************************
//	Object:	PowerPrn
//	Method:	dwAbout
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	
//	Return:	
//
//	Desc  :	Show 'About PowerPrinter'
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
void API32 dwAbout()
{
	CAbout	CAboutDlg;

	CAboutDlg.DoModal();
}


//************************************************************
//	Object:	PowerPrn
//	Method:	dwUnlock
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	User name and Key
//	Return:	1 if successful -- message box otherwise
//
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long API32 dwUnlock(LPCSTR szUserName, DWORD dwUserKey)
{
	// do nothing
	return (long)TRUE;
}


// Free MFc mem
BOOL CPowerprnApp::OnIdle(LONG lCount) 
{
	return CWinApp::OnIdle(lCount);
}

//************************************************************
//	Object:	CPowerprnApp
//	Method:	InitInstance
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	
//	Return:	TRUE if Ok
//
//	Desc  :	Initiliaze MFC dll.
//			
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
BOOL CPowerprnApp::InitInstance() 
{
	NotifyPrinterChanged((m_hDevNames == NULL));

	// Change colors to red and green.
    SetDialogBkColor(RGB(0, 0, 0), RGB(19, 192, 0));
 
	return CWinApp::InitInstance();
}


//************************************************************
//	Object:	CPowerprnApp
//	Method:	ExitInstance
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	
//	Return:	TRUE if Ok
//
//	Desc  :	Show registration reminder if user did not unlock
//			PowerPrinter with dwUnlock
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
int CPowerprnApp::ExitInstance() 
{
	return CWinApp::ExitInstance();
}


//************************************************************
//	Object:	CPowerprnApp
//	Method:	InitApplication
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	
//	Return:	TRUE if Ok
//
//	Desc  :	Not much, call default InitApplication
//			
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
BOOL CPowerprnApp::InitApplication() 
{
	return CWinApp::InitApplication();
}

//************************************************************
//	Object:	CPowerprnApp
//	Method:	NotifyPrinterChanged
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	
//	Return:	TRUE if Ok
//
//	Desc  :	Keep MFC Happy by updating its internal state
//			Not really used -- more a test than anything useful
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
void CPowerprnApp::NotifyPrinterChanged(BOOL bUpdate)
{
	if (bUpdate)
		UpdatePrinterSelection(FALSE);
}

//////////////////////////////////////////////////////////////////////
//																	//
//					PowerPrinter APIs								//
//																	//
//////////////////////////////////////////////////////////////////////

// extended APIs
long API32 dwGetDefaultPrinterEx (CWin32Printer *this_hdl, LPSTR szPrinterName, LPSTR szPrinterDriver, LPSTR szPrinterPort)
{
	return this_hdl->GetDefaultPrinterEx(szPrinterName, szPrinterDriver, szPrinterPort);
}

long API32 dwSetDefaultPrinterEx (CWin32Printer *this_hdl, LPCSTR szPrinterName, LPCSTR szPrinterDriver, LPCSTR szPrinterPort)
{
	return this_hdl->SetDefaultPrinterEx(szPrinterName, szPrinterDriver, szPrinterPort);
}

// Get default printer (friendly) name
long API32 dwGetDefaultPrinterName (CWin32Printer *this_hdl, LPSTR szPrtr)
{
	return this_hdl->GetDefaultPrinterName(szPrtr);
}

// Get default printer port
long API32 dwGetDefaultPrinterPort (CWin32Printer *this_hdl, LPSTR szPrtr)
{
	return this_hdl->GetDefaultPrinterPort(szPrtr);
}

// Get default printer driver
long API32 dwGetDefaultPrinterDriver (CWin32Printer *this_hdl, LPSTR szPrtr)
{
	return this_hdl->GetDefaultPrinterDriver(szPrtr);
}

// Get printer list
long API32 dwGetPrinterList (CWin32Printer *this_hdl, LPSTR szPrtr)
{
	return this_hdl->GetPrinterList(szPrtr);
}

// Set default printer
long API32 dwSetDefaultPrinter (CWin32Printer *this_hdl, LPCSTR szPrtr)
{
	return this_hdl->SetDefaultPrinter(szPrtr);
}

// short dmOrientation;
long API32 dwSetPrinterOrientation(CWinPrinter *this_hdl, short iPrinterOrientation)
{
	return this_hdl->SetPrinterOrientation(iPrinterOrientation);
}

long API32 dwGetPrinterOrientation(CWinPrinter *this_hdl, short &iPrinterOrientation)
{
	return this_hdl->GetPrinterOrientation(iPrinterOrientation);
}

// short dmPaperSize;
long API32 dwSetPaperSize(CWinPrinter *this_hdl, short iPaperSize)
{
	return this_hdl->SetPaperSize(iPaperSize);
}

long API32 dwGetPaperSize(CWinPrinter *this_hdl, short &iPaperSize)
{
	return this_hdl->GetPaperSize(iPaperSize);
}

// short Length
long API32 dwGetPaperLength(CWin32Printer *this_hdl, short &iPaperLength)
{
	return this_hdl->GetPaperLength(iPaperLength);
}

long API32 dwSetPaperLength(CWin32Printer *this_hdl, short iPaperLength)
{
	return this_hdl->SetPaperLength(iPaperLength);
}

// short Width
long API32 dwGetPaperWidth(CWin32Printer *this_hdl, short &iPaperWidth)
{
	return this_hdl->GetPaperWidth(iPaperWidth);
}

long API32 dwSetPaperWidth(CWin32Printer *this_hdl, short iPaperWidth)
{
	return this_hdl->SetPaperWidth(iPaperWidth);
}

// short  dmPrintQuality;
long API32 dwGetPrintQuality(CWin32Printer *this_hdl, short &iPaperQuality)
{
	return this_hdl->GetPrintQuality(iPaperQuality);
}

long API32 dwSetPrintQuality(CWin32Printer *this_hdl, short iPrintQuality)
{
	return this_hdl->SetPrintQuality(iPrintQuality);
}

// Scale
long API32 dwGetScale(CWin32Printer *this_hdl, short &iScale)
{
	return this_hdl->GetScale(iScale);
}

long API32 dwSetScale(CWin32Printer *this_hdl, short iScale)
{
	return this_hdl->SetScale(iScale);
}


// Copies
long API32 dwGetCopies(CWin32Printer *this_hdl, short &iCopies)
{
	return this_hdl->GetCopies(iCopies);
}

long API32 dwSetCopies(CWin32Printer *this_hdl, short iCopies)
{
	return this_hdl->SetCopies(iCopies);
}

// Default Source
long API32 dwGetDefaultSource(CWin32Printer *this_hdl, short &iDefaultSource)
{
	return this_hdl->GetDefaultSource(iDefaultSource);
}

long API32 dwSetDefaultSource(CWin32Printer *this_hdl, short iSource)
{
	return this_hdl->SetDefaultSource(iSource);
}


// Color
long API32 dwGetColor(CWin32Printer *this_hdl, short &iColor)
{
	return this_hdl->GetColor(iColor);
}

long API32 dwSetColor(CWin32Printer *this_hdl, short iColor)
{
	return this_hdl->SetColor(iColor);
}


// Duplex
long API32 dwGetDuplex(CWinPrinter *this_hdl, short &iDuplex)
{
	return this_hdl->GetDuplex(iDuplex);
}

long API32 dwSetDuplex(CWinPrinter *this_hdl, short iDuplex)
{
	return this_hdl->SetDuplex(iDuplex);
}

// YResolution
long API32 dwGetYResolution(CWin32Printer *this_hdl, short &iYResolution)
{
	return this_hdl->GetYResolution(iYResolution);
}

long API32 dwSetYResolution(CWin32Printer *this_hdl, short iYResolution)
{
	return this_hdl->SetYResolution(iYResolution);
}

// TTOption
long API32 dwGetTTOption(CWin32Printer *this_hdl, short &iTTOption)
{
	return this_hdl->GetTTOption(iTTOption);
}

long API32 dwSetTTOption(CWin32Printer *this_hdl, short iTTOption)
{
	return this_hdl->SetTTOption(iTTOption);
}

// Collate
long API32 dwGetCollate(CWin32Printer *this_hdl, short &iCollate)
{
	return this_hdl->GetCollate(iCollate);
}

long API32 dwSetCollate(CWin32Printer *this_hdl, short iCollate)
{
	return this_hdl->SetCollate(iCollate);
}


//LogPixels
long API32 dwGetLogPixels(CWin32Printer *this_hdl, WORD &wLogPixels)
{
	return this_hdl->GetLogPixels(wLogPixels);
}

long API32 dwSetLogPixels(CWin32Printer *this_hdl, WORD wLogPixels)		
{
	return this_hdl->SetLogPixels(wLogPixels);
}

//BitsPerPel
long API32 dwGetBitsPerPel(CWin32Printer *this_hdl, DWORD &dwBitsPerPel)
{
	return this_hdl->GetBitsPerPel(dwBitsPerPel);
}

long API32 dwSetBitsPerPel(CWin32Printer *this_hdl, DWORD dwBitsPerPel)		
{
	return this_hdl->SetBitsPerPel(dwBitsPerPel);
}

// PelsWidth
long API32 dwGetPelsWidth(CWin32Printer *this_hdl, DWORD &dwPelsWidth)
{
	return this_hdl->GetPelsWidth(dwPelsWidth);
}

long API32 dwSetPelsWidth(CWin32Printer *this_hdl, DWORD dwPelsWidth)		
{
	return this_hdl->SetPelsWidth(dwPelsWidth);
}

//PelsHeight
long API32 dwGetPelsHeight(CWin32Printer *this_hdl, DWORD &dwPelsHeight)
{
	return this_hdl->GetPelsHeight(dwPelsHeight);
}

long API32 dwSetPelsHeight(CWin32Printer *this_hdl, DWORD dwPelsHeight)		
{
	return this_hdl->SetPelsHeight(dwPelsHeight);
}

// DisplayFlags
long API32 dwGetDisplayFlags(CWin32Printer *this_hdl, DWORD &dwDisplayFlags)
{
	return this_hdl->GetDisplayFlags(dwDisplayFlags);
}

long API32 dwSetDisplayFlags(CWin32Printer *this_hdl, DWORD dwDisplayFlags)		
{
	return this_hdl->SetDisplayFlags(dwDisplayFlags);
}

// DisplayFrequency
long API32 dwGetDisplayFrequency(CWin32Printer *this_hdl, DWORD &dwDisplayFrequency)
{
	return this_hdl->GetDisplayFrequency(dwDisplayFrequency);
}

long API32 dwSetDisplayFrequency(CWin32Printer *this_hdl, DWORD dwDisplayFrequency)		
{
	return this_hdl->SetDisplayFrequency(dwDisplayFrequency);
}

long API32 dwGetPaperBinList(CWin32Printer *this_hdl, LPSTR szBinList)
{
	return this_hdl->GetPaperBinList(szBinList);
}

long API32 dwGetNamedPaperBinList(CWin32Printer *this_hdl, LPSTR szBinList)
{
	return this_hdl->GetNamedPaperBinList(szBinList);
}

long API32 dwGetSupportedPaperSizeList(CWin32Printer *this_hdl, LPSTR szPageSizeList)
{
	return this_hdl->GetSupportedPaperSizeList(szPageSizeList);
}

long API32 dwGetPrinterAccess(CWin32Printer *this_hdl)
{
	return this_hdl->GetPrinterAccess();
}

long API32 dwSetPrinterAccess(CWin32Printer *this_hdl, long iPrinterAccess)
{
	return this_hdl->SetPrinterAccess(iPrinterAccess);
}

// New APIs for v1.1

long API32 dwAddPrinterConnection(CWin32Printer *this_hdl, LPCSTR szServerName, LPCSTR szPrinterShareName)
{
	return this_hdl->AddAPrinterConnection(szServerName, szPrinterShareName);
}

long API32 dwDeletePrinterConnection(CWin32Printer *this_hdl, LPCSTR szServerName, LPCSTR szPrinterName)
{
	return this_hdl->DeleteAPrinterConnection(szServerName, szPrinterName);
}

long API32 dwGetExtendedErrorCode(CWin32Printer *this_hdl)
{
	return (long) this_hdl->GetExtendedErrorCode();
}

void API32 dwGetExtendedErrorMessage(CWin32Printer *this_hdl, LPSTR szErrorMessage)
{
	this_hdl->GetExtendedErrorMessage(szErrorMessage);
}

long API32 dwAddPrinter(CWin32Printer *this_hdl,LPCSTR szServerName, LPCSTR szPrinterName, LPCSTR szPortName, LPCSTR szDriverName, LPCSTR szPrintProcessor)
{
	return this_hdl->AddAPrinter(szServerName, szPrinterName, szPortName, szDriverName, szPrintProcessor);	
}

long API32 dwDeletePrinter (CWin32Printer *this_hdl,LPCSTR szPrinterName)
{
	return this_hdl->DeleteAPrinter(szPrinterName);	
}

long API32 dwAddPrinterDriver(CWin32Printer *this_hdl,LPCSTR szServerName, LPCSTR szPrinterName, LPCSTR szEnvironment, LPCSTR szDriverPath, LPCSTR szDataFile, LPCSTR ConfigFile, DWORD nVersion)
{
	return this_hdl->AddAPrinterDriver(szServerName, szPrinterName, szEnvironment, szDriverPath, szDataFile, ConfigFile, nVersion);
}

long API32 dwDeletePrinterDriver (CWin32Printer *this_hdl,LPCSTR szPrinterName, LPCSTR szEnvironment, LPCSTR lpszDriverName)
{
	return this_hdl->DeleteAPrinterDriver (szPrinterName, szEnvironment, lpszDriverName);
}

long API32 dwGetPrinterDriver (CWin32Printer *this_hdl, LPCSTR szPrinterName, DRIVER_INFO_2* pDriverInfo)
{
	return this_hdl->GetPrinterDriver (szPrinterName, pDriverInfo);
}

long API32 dwGetPrinterDrivers (CWin32Printer *this_hdl, DRIVER_INFO_2* pDriverInfo, DWORD iMaxDrivers, short &Drivers)
{
	return this_hdl->GetPrinterDrivers (pDriverInfo, iMaxDrivers, Drivers);
}

long API32 dwEnumPrinterJobs (CWin32Printer *this_hdl, LPCSTR szPrinterName, long NoJobs, JOB_INFO_1* pInfo1, long &Jobs)
{
	return this_hdl->EnumPrinterJobs (szPrinterName, NoJobs, pInfo1, Jobs);
}

long API32 dwCaptureDesktop(CWin32Printer *this_hdl)
{
	return this_hdl->CaptureDesktop();
}

long API32 dwPrintTheDIB(CWin32Printer *this_hdl, long iOption, LPSTR szText)
{
	return this_hdl->PrintTheDIB(iOption, szText);
}

long API32 dwCaptureWindow(CWin32Printer *this_hdl, HWND hWin, long fPrintArea)
{
	return this_hdl->CaptureWindow(hWin, fPrintArea);
}

long API32 dwEnumPrinterFonts (CWin32Printer *this_hdl, LPSTR szFonts)
{
	return this_hdl->GetPrinterFonts (szFonts);
}

long API32 dwSetPrinterFont(CWin32Printer *this_hdl, HDC pDC, LPCSTR szFontFaceName, short iPoint, short iBold, short iItalic, short iUnderline, short iStrike)
{
	return this_hdl->SetPrinterFont(pDC, szFontFaceName, iPoint, iBold, iItalic, iUnderline, iStrike);
}

long API32 dwSetDefaultPrinterPort(CWin32Printer *this_hdl, LPCSTR szPort)
{
	return this_hdl->SetDefaultPrinterPort(szPort);
}

long API32 dwGetNamedPaperBinListEx(CWin32Printer *this_hdl, LPCSTR szPrinterName, LPSTR szBinList)
{
	return this_hdl->GetNamedPaperBinListEx(szPrinterName, szBinList);
}

long API32 dwGetPaperBinListEx(CWin32Printer *this_hdl, LPCSTR szPrinterName, LPSTR szBinList)
{
	return this_hdl->GetPaperBinListEx(szPrinterName, szBinList);
}

//v1.1.01
// Get printer list
long API32 dwGetPrinterListEx (CWin32Printer *this_hdl, LPSTR szPrtr)
{
	return this_hdl->GetPrinterListEx(szPrtr);
}

// v1.2
long API32 dwGetPrinterStatus (CWin32Printer *this_hdl, LPSTR szPrtr, DWORD &dwStatus)
{
	return this_hdl->GetPrinterStatus(szPrtr, dwStatus);
}

long API32 dwSetJobProperties (CWin32Printer *this_hdl, LPCSTR szPrinterName, DWORD dwJobID, DWORD dwCommand, DWORD dwPriority)
{
	return this_hdl->SetJobProperties (szPrinterName, dwJobID, dwCommand, dwPriority);
}

long API32 dwEnumPrinterJobsVB (CWin32Printer *this_hdl, LPCSTR szPrinterName, FPSAFEARRAY * pInfo1, long & Jobs)
{
	return this_hdl->EnumPrinterJobsVB (szPrinterName, pInfo1, Jobs);
}

long API32 dwSetDocumentProperties (CWinPrinter *this_hdl, HWND hWnd)
{
	return this_hdl->SetDocumentProperties(hWnd);
}


long API32 dwSetFormName(CWin32Printer *this_hdl, LPCSTR szFormName)
{
	return this_hdl->SetFormName(szFormName);
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//																	//
//						NOT IMPLEMENTED								//
//																	//
//////////////////////////////////////////////////////////////////////

long API32 dwSetPageSize(CWin32Printer *this_hdl, short)
{
	return 1;
}


// Filename
long API32 dwRemoveFileName(CWin32Printer *this_hdl, LPCSTR szPrinterName, LPSTR szFileName)
{
	return 1;
}

long API32 dwSetFileName(CWin32Printer *this_hdl, LPCSTR szPrinterName, LPCSTR szFileName)
{
	return 1;
}


// test Printer
long API32 dwIsPrinterReady(CWin32Printer *this_hdl, short iPortNumber)
{
	return 1;
}

long API32 dwIsDefaultPrinterReady(CWin32Printer *this_hdl)
{
	return 1;
}


long API32 dwGetPort(CWin32Printer *this_hdl, LPSTR szPort)
{
	return 1;
}

long API32 dwGetDriver(CWin32Printer *this_hdl, LPSTR szDriver)
{
	return 1;
}


long API32 dwGetFileName(CWin32Printer *this_hdl, LPSTR szFileName)
{
	return 1;
}

long API32 dwSetFileName(CWin32Printer *this_hdl, LPCSTR szFileName)
{
	return 1;
}

long API32 dwGetPaperExtents(CWin32Printer *this_hdl, long &MinX, long &MinY, long &MaxX, long &MaxY)
{
	return this_hdl->GetPaperExtents(MinX, MinY, MaxX, MaxY);
}

// Raw Printer Interface
long API32 dwPrintRawData(CWin32Printer *this_hdl, LPSTR szPrinterName, LPSTR DocName, LPBYTE lpData, DWORD dwCount)
{
	return this_hdl->PrintRawData(szPrinterName, DocName, lpData, dwCount);
}

HANDLE	API32 dwRawOpen(CWin32Printer *this_hdl, LPSTR szPrinterName, LPSTR DocName)
{
	return this_hdl->RawOpen(szPrinterName, DocName);
}

long API32 dwRawNewPage(CWin32Printer *this_hdl, HANDLE hPrinter)
{
	return this_hdl->RawNewPage(hPrinter);
}

long API32 dwRawWrite(CWin32Printer *this_hdl, HANDLE hPrinter, LPBYTE lpData, DWORD dwCount)
{
	return this_hdl->RawWrite(hPrinter, lpData, dwCount);
}

long API32 dwRawEndPage(CWin32Printer *this_hdl, HANDLE hPrinter)
{
	return this_hdl->RawEndPage(hPrinter);
}

long API32 dwRawClose(CWin32Printer *this_hdl, HANDLE hPrinter)
{
	return this_hdl->RawClose(hPrinter);
}

long API32 dwRawPrinterProperties(CWin32Printer *this_hdl, HWND hWnd, HANDLE hPrinter)
{
	return this_hdl->RawPrinterProperties(hWnd, hPrinter);
}

HDC API32 dwOpen(CWin32Printer *this_hdl, LPCSTR szDevice, LPCSTR szDriver, LPCSTR szDoc, BOOL bShow, HWND hWnd)
{
	return this_hdl->Open(szDevice, szDriver, szDoc, bShow, hWnd);
}

long API32 dwNewPage(CWin32Printer *this_hdl, HDC hdc)
{
	return this_hdl->NewPage(hdc);
}

long API32 dwEndPage(CWin32Printer *this_hdl, HDC hdc)
{
	return this_hdl->EndPage(hdc);
}

long API32 dwClose(CWin32Printer *this_hdl, HDC hdc)
{
	return this_hdl->Close(hdc);
}

long API32 dwGetPrinterForms (CWin32Printer *this_hdl, LPCSTR szPrinterName, LPSTR szForms, long nFormType)
{
	return this_hdl->GetForms (szPrinterName, szForms, nFormType);
}

long API32 dwSetPrinterAttributes(CWin32Printer *this_hdl, DWORD Attrbibutes)
{
	return this_hdl->SetPrinterAttributes(Attrbibutes);
}
