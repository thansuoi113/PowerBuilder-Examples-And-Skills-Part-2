//+-------------------------------------------------------------------
//
//  PowerPrinter
//
//  Copyright (c) 1997, 1998, 1999, 2000, 2001
//  SecureWave. All rights reserved
//
//  See the file LICENSE.TXT for redistribution information.
//
//  File:        CWin32Prn.h
//
//  Contents:    Header file for CWin32Printer class
//               32Bit Windows specific methods
//
//--------------------------------------------------------------------

#include <afxwin.h>   

#ifdef WIN32
#include "winspool.h"
#endif

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#ifndef __CWIN32PRINTER_H__
#define __CWIN32PRINTER_H__

#include "resource.h"		// main symbols
#include "winprn.h"

//OLE Stuff for SAFEARRAY
#include <oleauto.h>


typedef SAFEARRAY * FPSAFEARRAY;

// Function *prototypes* for GetProcAddress

typedef HBITMAP		(FAR PASCAL *LPCSTB)	(LPRECT);				// CopyScreenToBitmap
typedef HPALETTE	(FAR PASCAL *LPGSP)		(void);					// GetSystemPalette
typedef HDIB		(FAR PASCAL *LPBTDIB)	(HBITMAP , HPALETTE);	// BitmapToDIB
typedef WORD		(FAR PASCAL *LPPDIB)	(HDIB, WORD, WORD, WORD, LPSTR);	// PrintDIB 
typedef WORD		(FAR PASCAL *LPDDIB)	(HDIB);					// DestroyDIB 
typedef void		(FAR PASCAL *LPDIBE)	(int);	// DIBError
typedef HBITMAP		(FAR PASCAL *LPCWBMP)	(HWND, WORD); // CopyWindowToBitmap

    
class CWin32Printer : public CWinPrinter {

	
//  Attributes

protected:

	long m_PrinterAccess;
	HINSTANCE m_hPowerDIB;
	HPALETTE m_hPal;
	HBITMAP m_hBitmap;
	DWORD m_dUnits;
	DWORD m_iExtendedErrorAttribute;

//	implementation

protected:
	long SetFormName();

	BOOL CheckPrinterName(LPCSTR szPrnName);

	// short attributes
	long GetPrinterAttribute(short dwField, short &iAttribute);
	long SetPrinterAttribute(short dwField, short iAttribute);

	// WORD attributes
	long GetPrinterAttribute(short dwField, WORD &iAttribute);
	long SetPrinterAttribute(short dwField, WORD iAttribute);

	// DWORD attributes
	long GetPrinterAttribute(short dwField, DWORD &iAttribute);
	long SetPrinterAttribute(short dwField, DWORD iAttribute);

	//v.12
	virtual long GetPrinterDriver(LPCSTR szPrinterName, LPSTR szDriverName);
	BOOL GetJobs(HANDLE hPrinter, JOB_INFO_2 **ppJobInfo, int *pcJobs, DWORD *pStatus);

public:
	long SetPrinterAttributes (DWORD Attributes);
	long Close(HDC hdc);
	long EndPage (HDC hdc);
	long NewPage(HDC hdc);
	HDC Open(LPCSTR szDevice, LPCSTR szDriver, LPCSTR szDoc, BOOL bShow, HWND hWnd);
	long RawPrinterProperties(HWND hWnd, HANDLE hPrinter);
	long RawWrite (HANDLE hPrinter, LPBYTE lpData, DWORD dwCount);
	long RawClose (HANDLE hPrinter);
	long RawEndPage (HANDLE hPrinter);
	long RawNewPage(HANDLE hPrinter);
	HANDLE RawOpen ( LPSTR szPrinterName, LPSTR DocName);
	long PrintRawData(LPSTR szPrinterName, LPSTR DocName, LPBYTE lpData, DWORD dwCount);
	long GetPaperExtents(long &MinX, long &MinY, long &MaxX, long &MaxY);
	long SetDocumentProperties (HWND hWnd);

	// constructor and destructor
	CWin32Printer();
	~CWin32Printer();

	long GetPrinterListEx(LPSTR szPrtr);
	long SetDefaultPrinterPort(LPCSTR szPort);
	long SetPrinterAttribute(short dwField, LPCSTR szAttribute);
	virtual long GetPrinterPort (LPCSTR szPrinterName, LPSTR szPortName);
	long GetNamedPaperBinListEx(LPCSTR szPrinterName, LPSTR szPaperBins);
	long GetPaperBinListEx(LPCSTR szPrinterName, LPSTR szPaperBins);

	long PrintTheDIB(long iOptions, LPSTR szText);
	long GetPrinterFonts (LPSTR szFonts);
	long CaptureDesktop();

	long SetPrinterJob(LPCSTR szPrinterName, DWORD nJobId, DWORD nCommand);
	long EnumPrinterJobs (LPCSTR szPrinterName, long NoJobs, JOB_INFO_1* pInfo1, long &Jobs);
	long GetPrinterDrivers (DRIVER_INFO_2* pDriverInfo, DWORD iMaxDrivers, short &Drivers);
	long GetPrinterDriver (LPCSTR szPrinterName, DRIVER_INFO_2* pDriverInfo);
	long DeleteAPrinterDriver (LPCSTR pName, LPCSTR pEnvironment, LPCSTR pDriverName);
	long AddAPrinterDriver (LPCSTR lpszServer, LPCSTR lpszPName, LPCSTR lpszEnvironment, LPCSTR lpszDriverPath, LPCSTR lpszDataFile, LPCSTR lpszConfigFile, DWORD nVersion);
	void GetExtendedErrorMessage (LPSTR szErrorMsg);
	DWORD GetExtendedErrorCode();

	long GetPrinterAccess();
	long SetPrinterAccess(long iAccessType);
	long GetSupportedPaperSizeList(LPSTR szPageSizeList);
	long GetNamedPaperBinList(LPSTR szPaperBins);
	long GetPaperBinList(LPSTR szPaperBins);
	long SetDisplayFrequency(DWORD dwDisplayFrequency);
	long GetDisplayFrequency(DWORD &dwDisplayFrequency);

	long SetDisplayFlags(DWORD dwDisplayFlags);
	long GetDisplayFlags(DWORD &dwDisplayFlags);

	long SetPelsHeight(DWORD dwPelsHeight);
	long GetPelsHeight(DWORD &dwPelsHeight);

	long SetPelsWidth(DWORD dwPelsWidth);
	long GetPelsWidth(DWORD &dwPelsWidth);

	long SetBitsPerPel(DWORD dwBitsPerPel);
	long GetBitsPerPel(DWORD &dwBitsPerPel);

	long SetLogPixels(WORD wLogPixels);
	long GetLogPixels(WORD &wLogPixels);
	
	long GetDefaultPrinterEx(LPSTR szPrinterName, LPSTR szPrinterDriver, LPSTR szPrinterPort);

	long SetCollate(short iCollate);
	long GetCollate(short &iCollate);

	long SetTTOption(short iTTOption);
	long GetTTOption(short &iTTOption);

	long SetYResolution(short iYResolution);
	long GetYResolution(short &iYResolution);

	long SetColor(short iColor);
	long GetColor(short &iColor);

	long GetDefaultPrinterDriver(LPSTR szDriver);
	long GetDefaultPrinterPort(LPSTR szPort);
	long GetPrinterList(LPSTR szPrtr);
	long GetDefaultPrinterName(LPSTR szDefaultPrinterName);
	
	// implemented in descendant
	virtual long SetDefaultPrinter(LPCSTR szPrinterName);
	virtual long SetDefaultPrinterEx(LPCSTR szPrinterName, LPCSTR szPrinterDriver, LPCSTR szPrinterPort);

	// NT Only !
	virtual long DeleteAPrinterConnection (LPCSTR szServerName, LPCSTR szPrinterName);
	virtual long AddAPrinterConnection (LPCSTR szServerName, LPCSTR szPrinterShareName);
	virtual long GetForms(LPCSTR szPrinterName, LPSTR szForms, long nFormType);
	long SetFormName(LPCSTR szFormName);

	// All OS
	virtual long AddAPrinter (LPCSTR szServerName, LPCSTR szPrinterName, LPCSTR szPortName, LPCSTR szDriverName, LPCSTR szPrintProcessor);
	virtual long DeleteAPrinter (LPCSTR szPrinterName);

	long CaptureWindow (HWND hWin, long fPrintArea);
	long SetPrinterFont (HDC pDC, LPCSTR szFontFaceName, short iPoint, short iBold, short iItalic, short iUnderline, short iStrike);

	//v1.2
	long SetJobProperties (LPCSTR szPrinterName, DWORD dwJobID, DWORD dwCommand, DWORD dwPriority);
	long GetPrinterStatus (LPCSTR szPrinterName, DWORD &dwPrinterStatus);
	long EnumPrinterJobsVB (LPCSTR szPrinterName, FPSAFEARRAY* pInfo1, long &Jobs);
};

#endif	// __CWIN32PRINTER_H__