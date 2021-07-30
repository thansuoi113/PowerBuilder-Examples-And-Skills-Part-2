//+-------------------------------------------------------------------
//
//  PowerPrinter
//
//  Copyright (c) 1997, 1998, 1999, 2000, 2001
//  SecureWave. All rights reserved
//
//  See the file LICENSE.TXT for redistribution information.
//
//  File:        PowerPrn.h
//
//  Contents:    Main header file for POWERPRN.DLL
//               
//
//--------------------------------------------------------------------

// Define API32 according if we're compiling under NT/95 
// or Win 16. Perhaps not the best name but PowerPrinter was
// born as a 32Bit (only) dll...
                 
#ifdef WIN32                 
#define API32 __declspec(dllexport) pascal
#else
#define API32 _export FAR PASCAL
#endif

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "cwin32prn.h"

/////////////////////////////////////////////////////////////////////////////
// CPowerprnApp
// See powerprn.cpp for the implementation of this class
// 

class CPowerprnApp : public CWinApp
{
public:
	void NotifyPrinterChanged(BOOL bUpdate);
	CPowerprnApp();

private:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPowerprnApp)
	public:
	virtual BOOL OnIdle(LONG lCount);
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL InitApplication();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CPowerprnApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// Define C functions callable from any 4GL. C++ methods cannot
// be used because of name mangling

// This two functions allows us to create an instance of a class
// The correct class (NT or 95) is determined at run-time

class CWin32Printer;


STDAPI_ (CWin32Printer*) CWin32Prn_CPP_CONSTRUCTOR();

extern "C" {

DWORD dwGetWinVersion();

void API32 CWin32Prn_CPP_DESTRUCTOR(CWin32Printer *this_hdl);

// Extended Get Set
long API32 dwGetDefaultPrinterEx (CWin32Printer *this_hdl, LPSTR szPrinterName, LPSTR szPrinterDriver, LPSTR szPrinterPort);
long API32 dwSetDefaultPrinterEx (CWin32Printer *this_hdl, LPCSTR szPrinterName, LPCSTR szPrinterDriver, LPCSTR szPrinterPort);

long API32 dwGetDefaultPrinterName (CWin32Printer *this_hdl, LPSTR szPrtr);
long API32 dwGetPrinterList (CWin32Printer *this_hdl, LPSTR szPrtr);
long API32 dwGetDefaultPrinterPort (CWin32Printer *this_hdl, LPSTR szPrtr);
long API32 dwGetDefaultPrinterDriver (CWin32Printer *this_hdl, LPSTR szPrtr);
long API32 dwSetDefaultPrinter (CWin32Printer *this_hdl, LPCSTR szPrtr);

// short  dmOrientation;
long API32 dwSetPrinterOrientation(CWinPrinter *this_hdl, short iPrinterOrientation);
long API32 dwGetPrinterOrientation(CWinPrinter *this_hdl, short &iPrinterOrientation);

// short paper size
long API32 dwGetPaperSize(CWinPrinter *this_hdl, short &iPaperSize);
long API32 dwSetPaperSize(CWinPrinter *this_hdl, short iPaperSize);

// short paper Length
long API32 dwGetPaperLength(CWinPrinter *this_hdl, short &iPaperLength);
long API32 dwSetPaperLength(CWinPrinter *this_hdl, short iPaperLength);

// short paper Width
long API32 dwGetPaperWidth(CWinPrinter *this_hdl, short &iPaperWidth);
long API32 dwSetPaperWidth(CWinPrinter *this_hdl, short iPaperWidth);

// Scale
long API32 dwGetScale(CWinPrinter *this_hdl, short &iScale);
long API32 dwSetScale(CWinPrinter *this_hdl, short iScale);

// Copies
long API32 dwGetCopies(CWinPrinter *this_hdl, short &iCopies);
long API32 dwSetCopies(CWinPrinter *this_hdl, short iCopies);

// Default Source
long API32 dwGetDefaultSource(CWinPrinter *this_hdl, short &iSource);
long API32 dwSetDefaultSource(CWinPrinter *this_hdl, short iSource);

// short  dmPrintQuality;
long API32 dwGetPrintQuality(CWinPrinter *this_hdl, short &iPrintQuality);
long API32 dwSetPrintQuality(CWinPrinter *this_hdl, short iPrintQuality);

// Color
long API32 dwGetColor(CWin32Printer *this_hdl, short &iColor);
long API32 dwSetColor(CWin32Printer *this_hdl, short iColor);

// Duplex
long API32 dwGetDuplex(CWinPrinter *this_hdl, short &iDuplex);
long API32 dwSetDuplex(CWinPrinter *this_hdl, short iDuplex);

// YResolution
long API32 dwGetYResolution(CWin32Printer *this_hdl, short &iYResolution);
long API32 dwSetYResolution(CWin32Printer *this_hdl, short iYResolution);

// TTOption
long API32 dwGetTTOption(CWin32Printer *this_hdl, short &iTTOption);
long API32 dwSetTTOption(CWin32Printer *this_hdl, short iTTOption);

// Collate
long API32 dwGetCollate(CWin32Printer *this_hdl, short &iCollate);
long API32 dwSetCollate(CWin32Printer *this_hdl, short iCollate);

//LogPixels
long API32 dwGetLogPixels(CWin32Printer *this_hdl, DWORD &wLogPixels);
long API32 dwSetLogPixels(CWin32Printer *this_hdl, DWORD wLogPixels);		

//BitsPerPel
long API32 dwGetBitsPerPel(CWin32Printer *this_hdl, DWORD &dwBitsPerPel);
long API32 dwSetBitsPerPel(CWin32Printer *this_hdl, DWORD dwBitsPerPel);		

// PelsWidth
long API32 dwGetPelsWidth(CWin32Printer *this_hdl, DWORD &dwPelsWidth);
long API32 dwSetPelsWidth(CWin32Printer *this_hdl, DWORD dwPelsWidth);		

//PelsHeight
long API32 dwGetPelsHeight(CWin32Printer *this_hdl, DWORD &dwPelsHeight);
long API32 dwSetPelsHeight(CWin32Printer *this_hdl, DWORD dwPelsHeight);		

// DisplayFlags
long API32 dwGetDisplayFlags(CWin32Printer *this_hdl, DWORD &dwDisplayFlags);
long API32 dwSetDisplayFlags(CWin32Printer *this_hdl, DWORD dwDisplayFlags);		

// DisplayFrequency
long API32 dwGetDisplayFrequency(CWin32Printer *this_hdl, DWORD &dwDisplayFrequency);
long API32 dwSetDisplayFrequency(CWin32Printer *this_hdl, DWORD dwDisplayFrequency);		

// Page Size
long API32 dwGetPageSize(CWin32Printer *this_hdl, long *iX, long *iY, long *iHRes, long *iVRes, long *iPixelsInchX, long *iPixelsInchY);
long API32 dwSetPageSize(CWin32Printer *this_hdl, short);

// Filename
long API32 dwRemoveFileName(CWin32Printer *this_hdl, LPCSTR szPrinterName, LPSTR szFileName);
long API32 dwGetFileName(CWin32Printer *this_hdl, LPSTR szFileName);
long API32 dwSetFileName(CWin32Printer *this_hdl, LPCSTR szPrinterName, LPCSTR szFileName);

// Printer test
long API32 dwIsPrinterReady(CWin32Printer *this_hdl, short iPortNumber);
long API32 dwIsDefaultPrinterReady(CWin32Printer *this_hdl);

long API32 dwGetPort(CWin32Printer *this_hdl, LPSTR szPort);
long API32 dwGetDriver(CWin32Printer *this_hdl, LPSTR szDriver);

void API32 dwAbout();
long API32 dwUnlock(LPCSTR szUserName, DWORD dwUserKey);

long API32 dwGetPaperBinList(CWin32Printer *this_hdl, LPSTR szBinList);
long API32 dwGetNamedPaperBinList(CWin32Printer *this_hdl, LPSTR szBinList);

long API32 dwGetSupportedPaperSizeList(CWin32Printer *this_hdl, LPSTR szPageSizeList);

long API32 dwGetPrinterAccess(CWin32Printer *this_hdl);
long API32 dwSetPrinterAccess(CWin32Printer *this_hdl, long iPrinterAccess);

// v1.1 APIs

// NT ONLY !!
long API32 dwAddPrinterConnection(CWin32Printer *this_hdl, LPCSTR szServerName, LPCSTR szPrinterShareName);
long API32 dwDeletePrinterConnection(CWin32Printer *this_hdl, LPCSTR szServerName, LPCSTR szPrinterName);

long API32 dwGetExtendedErrorCode(CWin32Printer *this_hdl);
void API32 dwGetExtendedErrorMessage(CWin32Printer *this_hdl, LPSTR szErrorMessage);

long API32 dwAddPrinter(CWin32Printer *this_hdl,LPCSTR szServerName, LPCSTR szPrinterName, LPCSTR szPortName, LPCSTR szDriverName, LPCSTR szPrintProcessor);
long API32 dwDeletePrinter (CWin32Printer *this_hdl,LPCSTR szPrinterName);	

long API32 dwAddPrinterDriver(CWin32Printer *this_hdl,LPCSTR szServerName, LPCSTR szPrinterName, LPCSTR szEnvironment, LPCSTR szDriverPath, LPCSTR szDataFile, LPCSTR ConfigFile, DWORD nVersion);
long API32 dwDeletePrinterDriver (CWin32Printer *this_hdl,LPCSTR szPrinterName, LPCSTR szEnvironment, LPCSTR lpszDriverName);	

long API32 dwGetPrinterDriver (CWin32Printer *this_hdl, LPCSTR szPrinterName, DRIVER_INFO_2* pDriverInfo);
long API32 dwGetPrinterDrivers (CWin32Printer *this_hdl, DRIVER_INFO_2* pDriverInfo, DWORD iMaxDrivers, short &Drivers);

long API32 dwEnumPrinterJobs (CWin32Printer *this_hdl, LPCSTR szPrinterName, long NoJobs, JOB_INFO_1* pInfo1, long &Jobs);

long API32 dwCaptureDesktop(CWin32Printer *this_hdl);
long API32 dwPrintTheDIB(CWin32Printer *this_hdl, long iOption, LPSTR szText);
long API32 dwCaptureWindow(CWin32Printer *this_hdl, HWND hWin, long fPrintArea);

long API32 dwEnumPrinterFonts (CWin32Printer *this_hdl, LPSTR szFonts);
long API32 dwSetPrinterFont(CWin32Printer *this_hdl, HDC pDC, LPCSTR szFontFaceName, short iPoint, short iBold, short iItalic, short iUnderline, short iStrike);

long API32 dwSetDefaultPrinterPort(CWin32Printer *this_hdl, LPCSTR szPort);
long API32 dwGetNamedPaperBinListEx(CWin32Printer *this_hdl, LPCSTR szPrinterName, LPSTR szBinList);
long API32 dwGetPaperBinListEx(CWin32Printer *this_hdl, LPCSTR szPrinterName, LPSTR szBinList);

// v1.1.01
long API32 dwGetPrinterListEx (CWin32Printer *this_hdl, LPSTR szPrtr);

// v1.2
long API32 dwGetPrinterStatus (CWin32Printer *this_hdl, LPSTR szPrtr, DWORD &dwStatus);
long API32 dwSetJobProperties (CWin32Printer *this_hdl, LPCSTR szPrinterName, DWORD dwJobID, DWORD dwCommand, DWORD dwPriority);
long API32 dwEnumPrinterJobsVB (CWin32Printer *this_hdl, LPCSTR szPrinterName, FPSAFEARRAY * pInfo1, long & Jobs);
long API32 dwSetDocumentProperties (CWinPrinter *this_hdl, HWND hWnd);
long API32 dwSetFormName(CWin32Printer *this_hdl, LPCSTR szFormName);
long API32 dwGetPaperExtents(CWin32Printer *this_hdl, long &MinX, long &MinY, long &MaxX, long &MaxY);

long API32 dwSetPrinterAttributes(CWin32Printer *this_hdl, DWORD Attrbibutes);

long API32 dwGetPrinterForms (CWin32Printer *this_hdl, LPCSTR szPrinterName, LPSTR szForms, long nFormType);

long API32 dwPrintRawData(CWin32Printer *this_hdl, LPSTR szPrinterName, LPSTR DocName, LPBYTE lpData, DWORD dwCount);

// RAW Interface
HANDLE	API32 dwRawOpen(CWin32Printer *this_hdl, LPSTR szPrinterName, LPSTR DocName);

long API32 dwRawNewPage(CWin32Printer *this_hdl, HANDLE hPrinter);
long API32 dwRawWrite(CWin32Printer *this_hdl, HANDLE hPrinter, LPBYTE lpData, DWORD dwCount);
long API32 dwRawEndPage(CWin32Printer *this_hdl, HANDLE hPrinter);
long API32 dwRawClose(CWin32Printer *this_hdl, HANDLE hPrinter);
long API32 dwRawPrinterProperties(CWin32Printer *this_hdl, HWND hWnd, HANDLE hPrinter);

// Win GDI Interface
HDC API32 dwOpen(CWin32Printer *this_hdl, LPCSTR szDevice, LPCSTR szDriver, LPCSTR szDoc, BOOL bShow, HWND hWnd);
long API32 dwNewPage(CWin32Printer *this_hdl, HDC hdc);
long API32 dwEndPage(CWin32Printer *this_hdl, HDC hdc);
long API32 dwClose(CWin32Printer *this_hdl, HDC hdc);

}

extern CPowerprnApp theApp;
/////////////////////////////////////////////////////////////////////////////
