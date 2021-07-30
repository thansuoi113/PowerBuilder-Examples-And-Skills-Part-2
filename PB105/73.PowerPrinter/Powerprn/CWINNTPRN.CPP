//+-------------------------------------------------------------------
//
//  PowerPrinter
//
//  Copyright (c) 1997, 1998, 1999, 2000, 2001
//  SecureWave. All rights reserved
//
//  See the file LICENSE.TXT for redistribution information.
//
//  File:        CWinNTPrn.h
//
//  Contents:    CWinNTPrinter class implementation
//               NT specific functions
//
//--------------------------------------------------------------------

#include "stdafx.h"
#include "cwinntprn.h"

//************************************************************
//	Object:	CWinNTPrinter
//	Method:	CWinNTPrinter (constructor)
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
CWinNTPrinter::CWinNTPrinter()
{   
	// Do nothing
}


//************************************************************
//	Object:	CWinNTPrinter
//	Method:	~CWinNTPrinter (destructor)
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
CWinNTPrinter::~CWinNTPrinter()
{
	// Do nothing
}


//************************************************************
//	Object:	CWinNTPrinter
//	Method:	SetDefaultPrinter
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	Printer name
//	Return:	NO_ERR if Ok
//
//	Desc  :	Set default printer
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinNTPrinter::SetDefaultPrinter(LPCSTR szPrinterName)
{
	// To set the default printer for Windows NT use:
	/* Note printer driver is usually WINSPOOL under Windows NT */
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

	//eg "My Printer,WINSPOOL,lpt1:"
	WriteProfileString("windows", "device", PrinterDriverPort);
	SendMessage(HWND_BROADCAST, WM_WININICHANGE, 0L, 0L);
	return NO_ERR;
}


//************************************************************
//	Object:	CWinNTPrinter
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
long CWinNTPrinter::SetDefaultPrinterEx(LPCSTR szPrinterName, LPCSTR szPrinterDriver, LPCSTR szPrinterPort)
{
	// To set the default printer for Windows NT use:
	/* Note printer driver is usually WINSPOOL under Windows NT */
	char	PrinterDriverPort[1000];

	// brute test
	if (CheckPrinterName(szPrinterName) == FALSE)
		return ERR_BAD_ARGUMENT;

	memset(PrinterDriverPort,	0, sizeof(PrinterDriverPort));

	// put them all together
	strcpy(PrinterDriverPort, szPrinterName);
	strcat(PrinterDriverPort, ",");
	strcat(PrinterDriverPort, szPrinterDriver);
	strcat(PrinterDriverPort, ",");
	strcat(PrinterDriverPort, szPrinterPort);
	strcat(PrinterDriverPort, "\0");

	//eg "My Printer,WINSPOOL,lpt1:"
	WriteProfileString("windows", "device", PrinterDriverPort);
	SendMessage(HWND_BROADCAST, WM_WININICHANGE, 0L, 0L);
	return NO_ERR;

}


//************************************************************
//	Object:	CWinNTPrinter
//	Method:	AddAPrinterConnection
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	Server name, printer SHARE name
//	Return:	NO_ERR if Ok
//
//	Desc  :	Connect to a NET printer (NT only)
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinNTPrinter::AddAPrinterConnection (LPCSTR szServerName, LPCSTR szPrinterShareName)
{
	char tmpBuf[300];
	long iRet = NO_ERR;;

	memset(tmpBuf,	0, sizeof(tmpBuf));

	strcpy(tmpBuf, "\\\\");
	strcat(tmpBuf, szServerName);
	strcat(tmpBuf, "\\");
	strcat(tmpBuf, szPrinterShareName);
	strcat(tmpBuf, "\0");

	if (AddPrinterConnection(tmpBuf) != TRUE) {
		m_iExtendedErrorAttribute  = GetLastError();
		iRet = ERR_ADD_PRNCONN;
	}
	else
		m_iExtendedErrorAttribute  = 0;

	return iRet;
}


//************************************************************
//	Object:	CWinNTPrinter
//	Method:	DeleteAPrinterConnection
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	Server name, printer name
//	Return:	NO_ERR if Ok
//
//	Desc  :	Delete a printer NET connection
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinNTPrinter::DeleteAPrinterConnection (LPCSTR szServerName, LPCSTR szPrinterName)
{
	long iRet = NO_ERR;
	char tmpBuf[300];

	// make sure it has been reset
	m_iExtendedErrorAttribute = 0;

	memset(tmpBuf,	0, sizeof(tmpBuf));

	strcpy(tmpBuf, "\\\\");
	strcat(tmpBuf, szServerName);
	strcat(tmpBuf, "\\");
	strcat(tmpBuf, szPrinterName);
	strcat(tmpBuf, "\0");


	if (DeletePrinterConnection(tmpBuf) != TRUE) {
		m_iExtendedErrorAttribute  = GetLastError();
		iRet = ERR_DEL_PRNCONN;
	}

	return iRet;
}


//************************************************************
//	Object:	CWinNTPrinter
//	Method:	AddAPrinter
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	Server name, printer name, print processor
//	Return:	NO_ERR if Ok
//
//	Desc  :	Add alocal printer. If server name is given then the
//			printer is added to the SERVER specified !
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinNTPrinter::AddAPrinter (LPCSTR szServerName, LPCSTR szPrinterName, LPCSTR szPortName, LPCSTR szDriverName, LPCSTR szPrintProcessor)
{
	long				iRet = NO_ERR;
	PRINTER_INFO_2	pi2;
	HANDLE			hPrinter;

	// make sure it has been reset
	m_iExtendedErrorAttribute = 0;

	ZeroMemory(&pi2, sizeof(PRINTER_INFO_2));
	
	pi2.pPrinterName	= (char *)szPrinterName;
	pi2.pPortName		= (char *)szPortName;
	pi2.pDriverName		= (char *)szDriverName;
	pi2.pPrintProcessor	= (char *)szPrintProcessor;

	// pi2.pDataType = "EMF";

	// if server name was given then it is a net printer
	if ((szServerName != NULL) && (strlen(szServerName) > 0))
		hPrinter = AddPrinter ((char *)szServerName, 2, (LPBYTE)&pi2);
	else
		hPrinter = AddPrinter (NULL, 2, (LPBYTE)&pi2);

	if (hPrinter == NULL) {
		iRet = ERR_ADD_PRINTER;
		m_iExtendedErrorAttribute  = GetLastError();
	}
	else 
	{
		ClosePrinter(hPrinter);
	}

   return iRet;
}


//************************************************************
//	Object:	CWinNTPrinter
//	Method:	DeleteAPrinter
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	Printer name
//	Return:	NO_ERR if Ok
//
//	Desc  :	Get rid of a local printer
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinNTPrinter::DeleteAPrinter (LPCSTR szPrinterName)
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
//	Object:	CWinNTPrinter
//	Method:	GetPrinterPort
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	printer name and port (by ref)
//	Return:	NO_ERR if Ok
//
//	Desc  :	Get the port given the printer name
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinNTPrinter::GetPrinterPort(LPCSTR szPrinterName, LPSTR szPortName)
{
	DWORD	Type;
	DWORD	cbData;
	DWORD	ulOptions=0;		
	REGSAM	samDesired;	
	LONG	llRet;
	char	Buf[200];
	HKEY	hKey;
	char *token;
	LPCSTR lpszSubKey = "Software\\Microsoft\\Windows NT\\CurrentVersion\\Devices";

	samDesired = KEY_QUERY_VALUE;
	llRet = RegOpenKeyEx (HKEY_CURRENT_USER, lpszSubKey, ulOptions, samDesired, &hKey);

	if (llRet == ERROR_SUCCESS) {

		Type = REG_SZ;
		cbData = 200;

		llRet = RegQueryValueEx(hKey, szPrinterName, 0, &Type, (unsigned char*)Buf, &cbData);
		if (llRet == ERROR_SUCCESS) {
			token = strtok(Buf, ",");
			if (token != NULL) {
				token = strtok( NULL, ";");
				OutputDebugString(token);
				strcpy(szPortName, token);
			}
		}
		RegCloseKey(hKey);
	}
	
	return llRet;
}

long CWinNTPrinter::GetPrinterDriver(LPCSTR szPrinterName, LPSTR szDriverName)
{
	DWORD	Type;
	DWORD	cbData;
	DWORD	ulOptions=0;		
	REGSAM	samDesired;	
	LONG	llRet;
	char	Buf[200];
	HKEY	hKey;
	char *token;
	LPCSTR lpszSubKey = "Software\\Microsoft\\Windows NT\\CurrentVersion\\Devices";

	samDesired = KEY_QUERY_VALUE;
	llRet = RegOpenKeyEx (HKEY_CURRENT_USER, lpszSubKey, ulOptions, samDesired, &hKey);

	if (llRet == ERROR_SUCCESS) {

		Type = REG_SZ;
		cbData = 200;

		llRet = RegQueryValueEx(hKey, szPrinterName, 0, &Type, (unsigned char*)Buf, &cbData);
		if (llRet == ERROR_SUCCESS) {
			token = strtok(Buf, ",");
			if (token != NULL) {
				OutputDebugString(token);
				strcpy(szDriverName, token);
			}
		}
		RegCloseKey(hKey);
	}
	
	return llRet;
}

long CWinNTPrinter::SetPrinterAttribute(short dwField, short iAttribute)
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
					pi2->pDevMode->dmFields |= DM_PAPERSIZE;

					bChanged = TRUE;
					iRet = NO_ERR;
				}
				 else
					iRet = ERR_PRN_NOCHANGE;
				break;


			case DW_PAPERLENGTH:	// changed test for NT !
				if (pi2->pDevMode->dmFields & DM_PAPERLENGTH) {
					/* Change the devmode. */					
					pi2->pDevMode->dmPaperLength = iAttribute;
					pi2->pDevMode->dmFields |= DM_PAPERLENGTH;
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
					pi2->pDevMode->dmFields |= DM_PAPERWIDTH;
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
		else {

			// win 9.x
			SendMessage ( HWND_BROADCAST, WM_DEVMODECHANGE, 0L,
						( LPARAM)(LPCSTR)szPrinterName);

			// winnt
			SendMessage(HWND_BROADCAST, WM_WININICHANGE, 0L, 0L);
		}
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

long CWinNTPrinter::GetForms(LPCSTR szPrinterName, LPSTR szForms, long nFormType)
{
	HANDLE			hPrinter;     
	DWORD			dwNeeded, dwReturned, i;
	FORM_INFO_1     *pFormInfo;
	char			buf[9000];
	long			iErr = NO_ERR;

	// You need a printer handle, open the printer
	if( ! OpenPrinter( (char *)szPrinterName, &hPrinter, NULL ) )       
		return ERR_PRN_OPEN;


	// First ind out how much memory you need
	if( ! EnumForms( hPrinter, 1, NULL, 0, &dwNeeded, &dwReturned ) ) {

		// It should have failed, but if it failed for any reason other
		// than "not enough memory", you should bail out
		if( GetLastError() != ERROR_INSUFFICIENT_BUFFER ) {
			ClosePrinter( hPrinter );         
			return ERR_ENUMFORMS1;       
		}     
	}
	

	// Allocate enough memory for the FORM_INFO_1 structures plus
	// the extra data - dwNeeded from the previous call tells you
	// the total size needed
	if( (pFormInfo = (FORM_INFO_1 *)malloc( dwNeeded )) == NULL ) {
		ClosePrinter( hPrinter );       
		return ERR_LOCAL_ALLOC;     
	}

	// Call EnumJobs() again and let it fill out our structures
	if( ! EnumForms( hPrinter, 1, (LPBYTE)pFormInfo, dwNeeded, &dwNeeded, &dwReturned ) ) {

		ClosePrinter( hPrinter );       
		free( pFormInfo );       
		return ERR_ENUMFORMS2;
	}   
	
	// You're done with the printer handle, close it
	ClosePrinter( hPrinter );     
	
	// #define FORM_USER       0x00000000
	// #define FORM_BUILTIN    0x00000001
	// #define FORM_PRINTER    0x00000002

	ZeroMemory(buf, 9000);

	for(i=0;i<dwReturned;i++) {    

		// check form type
		if ( pFormInfo[i].Flags && nFormType ) {
			strcat(buf, pFormInfo[i].pName );     
			strcat(buf, ";");
		}
    }

	// Clean up     
	free( pFormInfo );    
	
	strcpy(szForms, buf);

	return iErr;
}
