//+-------------------------------------------------------------------
//
//  PowerPrinter
//
//  Copyright (c) 1997, 1998, 1999, 2000, 2001
//  SecureWave. All rights reserved
//
//  See the file LICENSE.TXT for redistribution information.
//
//  File:        CWin95Prn.cpp
//
//  Contents:    CWin95Printer class implementation
//               Functions specific to Win 95
//
//--------------------------------------------------------------------

#include "stdafx.h"
#include "cwin95prn.h"


//************************************************************
//	Object:	CWin95Printer
//	Method:	CWin95Printer (constructor)
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	
//	Return:	
//
//	Desc  :	
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
CWin95Printer::CWin95Printer()
{   
	// Do nothing
}


//************************************************************
//	Object:	CWin95Printer
//	Method:	~CWin95Printer (destructor)
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	
//	Return:	
//
//	Desc  :	
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
CWin95Printer::~CWin95Printer()
{
	// Do nothing
}


//************************************************************
//	Object:	CWin95Printer
//	Method:	SetDefaultPrinter
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	Printer name
//	Return:	NO_ERR if Ok
//
//	Desc  :	Set default printer and broadcast
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin95Printer::SetDefaultPrinter(LPCSTR szPrinterName)
{
	char	Port[256], Driver[256], PrinterDriverPort[1000];


	// brute test
	if (CheckPrinterName(szPrinterName) == FALSE)
		return ERR_BAD_ARGUMENT;


	memset(Driver,				0, sizeof(Driver));
	memset(Port,				0, sizeof(Port));
	memset(PrinterDriverPort,	0, sizeof(PrinterDriverPort));

	// get other info
	GetDefaultPrinterPort(Port);
	GetDefaultPrinterDriver(Driver);

	// put them all together
	strcpy(PrinterDriverPort, szPrinterName);
	strcat(PrinterDriverPort, ",");
	strcat(PrinterDriverPort, Driver);
	strcat(PrinterDriverPort, ",");
	strcat(PrinterDriverPort, Port);
	strcat(PrinterDriverPort, "\0");

	// eg "My Printer,HPPCL5MS,lpt1:"
	WriteProfileString("windows", "device", PrinterDriverPort);
	SendMessage(HWND_BROADCAST, WM_WININICHANGE, 0L,(LPARAM)(LPCTSTR)"windows");

	return NO_ERR;
}


//************************************************************
//	Object:	CWin95Printer
//	Method:	SetDefaultPrinterEx
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	Printer name and port
//	Return:	NO_ERR if Ok
//
//	Desc  :	Set default printer given printer name and port
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin95Printer::SetDefaultPrinterEx(LPCSTR szPrinterName, LPCSTR szPrinterDriver, LPCSTR szPrinterPort)
{
	char	PrinterDriverPort[1000];
	BOOL	bRet;
	long		iRet;

	// brute test
	if (CheckPrinterName(szPrinterName) == FALSE)
		return ERR_BAD_ARGUMENT;

	// assume no errors
	iRet = NO_ERR;

	memset(PrinterDriverPort,	0, sizeof(PrinterDriverPort));

	// put them all together
	strcpy(PrinterDriverPort, szPrinterName);
	strcat(PrinterDriverPort, ",");
	strcat(PrinterDriverPort, szPrinterDriver);
	strcat(PrinterDriverPort, ",");
	strcat(PrinterDriverPort, szPrinterPort);
	strcat(PrinterDriverPort, "\0");

	// eg "My Printer,HPPCL5MS,lpt1:"
	bRet = ::WriteProfileString("windows", "device", PrinterDriverPort);

	if (bRet == TRUE)
		SendMessage(HWND_BROADCAST, WM_WININICHANGE, 0L,(LPARAM)(LPCTSTR)"windows");
	else 
		iRet = ERR_WIN95_SETDEFPRNEX;

	return iRet;

}


//************************************************************
//	Object:	CWin95Printer
//	Method:	AddAPrinterConnection
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	Server name, printer SHARE name
//	Return:	NO_ERR if Ok
//
//	Desc  :	!!! this API not implemented in Windows 95 !!!
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin95Printer::AddAPrinterConnection (LPCSTR szServerName, LPCSTR szPrinterShareName)
{
	return ERR_NOT_IMPLEMENTED;
}


//************************************************************
//	Object:	CWin95Printer
//	Method:	DeleteAPrinterConnection
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	Server name, printer SHARE name
//	Return:	NO_ERR if Ok
//
//	Desc  :	!!! this API not implemented in Windows 95 !!!
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin95Printer::DeleteAPrinterConnection (LPCSTR szServerName, LPCSTR szPrinterName)
{
	return ERR_NOT_IMPLEMENTED;
}


//************************************************************
//	Object:	CWin95Printer
//	Method:	AddAPrinter
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	Server name, printer name, printer processor
//	Return:	NO_ERR if Ok
//
//	Desc  :	Add a LOCAL printer
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin95Printer::AddAPrinter (LPCSTR szServerName, LPCSTR szPrinterName, LPCSTR szPortName, LPCSTR szDriverName, LPCSTR szPrintProcessor)
{
	long				iRet = NO_ERR;
	PRINTER_INFO_2	pi2;
	HANDLE			hPrinter;

	// reset error attribute
	m_iExtendedErrorAttribute  = 0;


	// Win95 does not accept Server Name
	// if server name was given then it is a net printer
	if ((szServerName != NULL) && (strlen(szServerName) > 0))
		return ERR_BAD_ARGUMENT;

	ZeroMemory(&pi2, sizeof(PRINTER_INFO_2));
	
	pi2.pPrinterName	= (char *)szPrinterName;
	pi2.pPortName		= (char *)szPortName;
	pi2.pDriverName		= (char *)szDriverName;
	pi2.pPrintProcessor	= (char *)szPrintProcessor;


	// pi2.pDataType = "EMF";
	hPrinter = AddPrinter (NULL, 2, (LPBYTE)&pi2);

	if (hPrinter == NULL) {
		iRet = ERR_ADD_PRINTER;
		m_iExtendedErrorAttribute  = GetLastError();
	}
	else
		ClosePrinter(hPrinter);

   return iRet;

}

//************************************************************
//	Object:	CWin95Printer
//	Method:	DeleteAPrinter
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	printer name
//	Return:	NO_ERR if Ok
//
//	Desc  :	Delete a local printer
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin95Printer::DeleteAPrinter (LPCSTR szPrinterName)
{
	HANDLE hPrinter = NULL;
	PRINTER_DEFAULTS pd;
	BOOL bFlag;
	long iRet = NO_ERR;

	// make sure it has been reset
	m_iExtendedErrorAttribute = 0;

	ZeroMemory(&pd, sizeof(pd));
	pd.DesiredAccess = m_PrinterAccess;

	bFlag = OpenPrinter((char *)szPrinterName, &hPrinter, &pd);
	if (!bFlag || (hPrinter== NULL)) {
		iRet = ERR_PRN_OPEN;
		m_iExtendedErrorAttribute  = GetLastError();
	}
	else {
		if (DeletePrinter(hPrinter) == FALSE) {
			iRet = ERR_DEL_PRINTER;
			m_iExtendedErrorAttribute  = GetLastError();
		}
	}

	return iRet;

}


//************************************************************
//	Object:	CWin95Printer
//	Method:	GetPrinterPort
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	printer name, port (by ref)
//	Return:	NO_ERR if Ok
//
//	Desc  :	Get the port given a printer name
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin95Printer::GetPrinterPort(LPCSTR szPrinterName, LPSTR szPortName)
{
	DWORD	Type;
	DWORD	cbData;
	DWORD	ulOptions=0;		
	REGSAM	samDesired;	
	LONG	llRet;
	char	Buf[200];
	HKEY	hKey;
	char	sKey[200];

	memset(sKey, 0, sizeof(sKey));

	strcpy(sKey, "System\\CurrentControlSet\\control\\Print\\Printers\\");
	strcat(sKey, szPrinterName);

	samDesired = KEY_QUERY_VALUE;
	llRet = RegOpenKeyEx (HKEY_LOCAL_MACHINE, sKey, ulOptions, samDesired, &hKey);

	if (llRet == ERROR_SUCCESS) {

		Type = REG_SZ;
		cbData = 200;

		llRet = RegQueryValueEx(hKey, "Port", 0, &Type, (unsigned char*)Buf, &cbData);
		if (llRet == ERROR_SUCCESS) {
			OutputDebugString(Buf);
			strcpy(szPortName, Buf);
		}
		RegCloseKey(hKey);
	}
	
	return llRet;

}

long CWin95Printer::GetPrinterDriver(LPCSTR szPrinterName, LPSTR szDriverName)
{
	DWORD	Type;
	DWORD	cbData;
	DWORD	ulOptions = 0;		
	REGSAM	samDesired;	
	LONG	llRet;
	char	Buf[200];
	HKEY	hKey;
	char	sKey[200];

	memset(sKey, 0, sizeof(sKey));

	strcpy(sKey, "System\\CurrentControlSet\\control\\Print\\Printers\\");
	strcat(sKey, szPrinterName);

	samDesired = KEY_QUERY_VALUE;
	llRet = RegOpenKeyEx (HKEY_LOCAL_MACHINE, sKey, ulOptions, samDesired, &hKey);

	if (llRet == ERROR_SUCCESS) {

		Type = REG_SZ;
		cbData = 200;

		llRet = RegQueryValueEx(hKey, "Driver", 0, &Type, (unsigned char*)Buf, &cbData);
		if (llRet == ERROR_SUCCESS) {
			OutputDebugString(Buf);
			strcpy(szDriverName, Buf);
		}
		RegCloseKey(hKey);
	}
	
	return llRet;

}

long CWin95Printer::GetForms(LPCSTR szPrinterName, LPSTR szForms, long nFormType)
{
	// NT Only
	return ERR_NOT_IMPLEMENTED;
}

long CWin95Printer::SetPrinterAttribute(short dwField, short iAttribute)
{

	HGLOBAL hGlobal = NULL;
	HANDLE hPrinter = NULL;
	DWORD dwNeeded = 0;
	PRINTER_INFO_2 *pi2 = NULL;
	PRINTER_DEFAULTS pd;
	BOOL bFlag, bChanged = FALSE;
	LONG lFlag;
	long iRet = NO_ERR;
	char	szPrinterPort[256], 
			szPrinterDriver[256], 
			szPrinterName[256];

	// make sure they're clean
	memset(szPrinterName,	0, sizeof(szPrinterName));
	memset(szPrinterDriver,	0, sizeof(szPrinterDriver));
	memset(szPrinterPort,	0, sizeof(szPrinterPort));
	

	// get other info
	GetDefaultPrinterName(szPrinterName);
	GetDefaultPrinterPort(szPrinterPort);
	GetDefaultPrinterDriver(szPrinterDriver);
 
	ZeroMemory(&pd, sizeof(pd));
	pd.DesiredAccess = m_PrinterAccess;

	bFlag = OpenPrinter(szPrinterName, &hPrinter, &pd);
	if (!bFlag || (hPrinter== NULL)) {
		iRet = ERR_PRN_OPEN;
		m_iExtendedErrorAttribute  = GetLastError();
		goto ABORT;
	}

	/* The first GetPrinter() tells you how big the buffer should be in order
	to hold all of PRINTER_INFO_2. Note that this will usually return FALSE,
	which only means that the buffer (the third parameter) was not filled
	in. You don't want it filled in here. */
 
	GetPrinter(hPrinter, 2, 0, 0, &dwNeeded);
	if (dwNeeded == 0) {
		iRet = ERR_PRN_GET;
		m_iExtendedErrorAttribute  = GetLastError();
		goto ABORT;
	}
 
	/* Allocate enough space for PRINTER_INFO_2. */
 
	hGlobal = GlobalAlloc(GHND, dwNeeded);
	if (hGlobal == NULL) {
		iRet = ERR_GLOBAL_ALLOC;
		m_iExtendedErrorAttribute  = GetLastError();
		goto ABORT;
	}

	pi2 = (PRINTER_INFO_2 *)GlobalLock(hGlobal);
	if (pi2 == NULL) {
		iRet = ERR_GLOBAL_LOCK;
		m_iExtendedErrorAttribute  = GetLastError();
		goto ABORT;
	}
 
	/* The second GetPrinter() fills in all the current settings, so all you
	need to do is modify what you're interested in. */
 
	bFlag = GetPrinter(hPrinter, 2, (LPBYTE)pi2, dwNeeded, &dwNeeded);
	if (!bFlag) {
		iRet = ERR_PRINT_INFO2;
		m_iExtendedErrorAttribute  = GetLastError();
		goto ABORT;
	}

 
	/* Set orientation to Landscape mode if the driver supports it. */
 
	if (pi2->pDevMode != NULL) {

		switch (dwField) {

			case DW_ORIENTATION:
				if (pi2->pDevMode->dmFields & DM_ORIENTATION) {
					/* Change the devmode. */					
					pi2->pDevMode->dmOrientation = iAttribute;
					bChanged = TRUE;
					iRet = NO_ERR;
				}
				 else
					iRet = ERR_PRN_NOCHANGE;
				break;

			case DW_PAPERSIZE:
				if (pi2->pDevMode->dmFields & DM_PAPERSIZE) {
					/* Change the devmode. */					
					pi2->pDevMode->dmPaperSize = iAttribute;
					bChanged = TRUE;
					iRet = NO_ERR;
				}
				 else
					iRet = ERR_PRN_NOCHANGE;
				break;

			case DW_PAPERLENGTH:
				if (pi2->pDevMode->dmFields & DM_PAPERLENGTH) {
					/* Change the devmode. */					
					pi2->pDevMode->dmPaperLength = iAttribute;
					bChanged = TRUE;
					iRet = NO_ERR;
				}
				 else
					iRet = ERR_PRN_NOCHANGE;
				 break;

			case DW_PAPERWIDTH:
				if (pi2->pDevMode->dmFields & DM_PAPERWIDTH) {
					/* Change the devmode. */					
					pi2->pDevMode->dmPaperWidth = iAttribute;
					bChanged = TRUE;
					iRet = NO_ERR;
				}
				 else
					iRet = ERR_PRN_NOCHANGE;
				 break;

			case DW_SCALE:
				if (pi2->pDevMode->dmFields & DM_SCALE) {
					/* Change the devmode. */					
					pi2->pDevMode->dmScale = iAttribute;
					bChanged = TRUE;
					iRet = NO_ERR;
				}
				 else
					iRet = ERR_PRN_NOCHANGE;
				 break;

			case DW_COPIES:
				if (pi2->pDevMode->dmFields & DM_COPIES) {
					/* Change the devmode. */					
					pi2->pDevMode->dmCopies = iAttribute;
					bChanged = TRUE;
					iRet = NO_ERR;
				}
				 else
					iRet = ERR_PRN_NOCHANGE;
				 break;

			case DW_DEFAULTSOURCE:
				if (pi2->pDevMode->dmFields & DM_DEFAULTSOURCE) {
					/* Change the devmode. */					
					pi2->pDevMode->dmDefaultSource = iAttribute;
					bChanged = TRUE;
					iRet = NO_ERR;
				}
				 else
					iRet = ERR_PRN_NOCHANGE;
				 break;

			case DW_PRINTQUALITY:
				if (pi2->pDevMode->dmFields & DM_PRINTQUALITY) {
					/* Change the devmode. */					
					pi2->pDevMode->dmPrintQuality = iAttribute;
					bChanged = TRUE;
					iRet = NO_ERR;
				}
				 else
					iRet = ERR_PRN_NOCHANGE;
				 break;

			case DW_COLOR:
				if (pi2->pDevMode->dmFields & DM_COLOR) {
					/* Change the devmode. */					
					pi2->pDevMode->dmColor = iAttribute;
					bChanged = TRUE;
					iRet = NO_ERR;
				}
				 else
					iRet = ERR_PRN_NOCHANGE;
				 break;

			case DW_YRESOLUTION:
				if (pi2->pDevMode->dmFields & DM_YRESOLUTION) {
					/* Change the devmode. */					
					pi2->pDevMode->dmYResolution = iAttribute;
					bChanged = TRUE;
					iRet = NO_ERR;
				}
				 else
					iRet = ERR_PRN_NOCHANGE;
				 break;

			case DW_TTOPTION:
				if (pi2->pDevMode->dmFields & DM_TTOPTION) {
					/* Change the devmode. */					
					pi2->pDevMode->dmTTOption = iAttribute;
					bChanged = TRUE;
					iRet = NO_ERR;
				}
				 else
					iRet = ERR_PRN_NOCHANGE;
				 break;

			case DW_COLLATE:
				if (pi2->pDevMode->dmFields & DM_COLLATE) {
					/* Change the devmode. */					
					pi2->pDevMode->dmCollate = iAttribute;
					bChanged = TRUE;
					iRet = NO_ERR;
				}
				 else
					iRet = ERR_PRN_NOCHANGE;
				 break;

			default:
					iRet = ERR_ATTR_UNKNOWN;
		}	// end switch

	}		//	end pDevMode != NULL
	else {
		iRet = ERR_DEVMODE_ISNULL; // Aaargh !
		m_iExtendedErrorAttribute  = GetLastError();
	}

	// has the attribute been changed ?
	if (bChanged) {

		/* Make sure the driver-dependent part of devmode is updated as
			necessary. */
		lFlag = DocumentProperties(NULL, hPrinter, szPrinterName,
            pi2->pDevMode, pi2->pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER | DM_UPDATE);

		if (lFlag != IDOK) {
			iRet = ERR_UPDT_DEVMODE;
			m_iExtendedErrorAttribute  = GetLastError();
			goto ABORT;
		}
 
		/* Update printer information. */
		pi2->pSecurityDescriptor = NULL;

		bFlag = SetPrinter(hPrinter, 2, (LPBYTE)pi2, 0);
		if (!bFlag) {
		/* The driver supported the change but wasn't allowed due to some
			other reason (probably lack of permission). */
			iRet = ERR_PRN_SET;
			m_iExtendedErrorAttribute  = GetLastError();
			goto ABORT;
		}
		else // Test for Covance: some printers do not change orientation
			 // under win98
			SendMessage(HWND_BROADCAST, WM_WININICHANGE, 0L,(LPARAM)(LPCTSTR)"windows");


	}	// end bChanged

 
/* Clean up. */
ABORT:   

	if (pi2 != NULL)
      GlobalUnlock(hGlobal);
   if (hGlobal != NULL)
      GlobalFree(hGlobal);
   if (hPrinter != NULL)
      ClosePrinter(hPrinter);

   return iRet;

}
