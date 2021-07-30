//+-------------------------------------------------------------------
//
//  PowerPrinter
//
//  Copyright (c) 1997, 1998, 1999, 2000, 2001
//  SecureWave. All rights reserved
//
//  See the file LICENSE.TXT for redistribution information.
//
//  File:        CWin32Prn.cpp
//
//  Contents:    CWin32Printer class implementation
//               Implement functions common to NT & 95
//				 Other functions implemented in CWin95 and CWinNT
//
//--------------------------------------------------------------------

#include "stdafx.h"         
#include "powerprn.h"  

#ifdef WIN32
#include "cwin32prn.h"
#else
#include "cwin32~1.h
#endif

long giFontCount = 0;
 
#ifndef WIN32
#define __TEXT(x)	x
#endif

// paper selections 
char  *szPaperList[] ={
                        __TEXT("Custom paper size"),
                        __TEXT("Letter 8 1/2 x 11 in"),
                        __TEXT("Letter Small 8 1/2 x 11 in"),
                        __TEXT("Tabloid 11 x 17 in"),
                        __TEXT("Ledger 17 x 11 in"),
                        __TEXT("Legal 8 1/2 x 14 in"),
                        __TEXT("Statement 5 1/2 x 8 1/2 in"),
                        __TEXT("Executive 7 1/2 x 10 in"),
                        __TEXT("A3 297 x 420 mm"),
                        __TEXT("A4 210 x 297 mm"),
                        __TEXT("A4 Small 210 x 297 mm"),
                        __TEXT("A5 148 x 210 mm"),
                        __TEXT("B4 250 x 354"),
                        __TEXT("B5 182 x 257 mm"),
                        __TEXT("Folio 8 1/2 x 13 in"),
                        __TEXT("Quarto 215 x 275 mm"),
                        __TEXT("10x14 in"),
                        __TEXT("11x17 in"),
                        __TEXT("Note 8 1/2 x 11 in"),
                        __TEXT("Envelope #9 3 7/8 x 8 7/8"),
                        __TEXT("Envelope #10 4 1/8 x 9 1/2"),
                        __TEXT("Envelope #11 4 1/2 x 10 3/8"),
                        __TEXT("Envelope #12 4 \276 x 11"),
                        __TEXT("Envelope #14 5 x 11 1/2"),
                        __TEXT("C size sheet"),
                        __TEXT("D size sheet"),
                        __TEXT("E size sheet"),
                        __TEXT("Envelope DL 110 x 220mm"),
                        __TEXT("Envelope C5 162 x 229 mm"),
                        __TEXT("Envelope C3  324 x 458 mm"),
                        __TEXT("Envelope C4  229 x 324 mm"),
                        __TEXT("Envelope C6  114 x 162 mm"),
                        __TEXT("Envelope C65 114 x 229 mm"),
                        __TEXT("Envelope B4  250 x 353 mm"),
                        __TEXT("Envelope B5  176 x 250 mm"),
                        __TEXT("Envelope B6  176 x 125 mm"),
                        __TEXT("Envelope 110 x 230 mm"),
                        __TEXT("Envelope Monarch 3.875 x 7.5 in"),
                        __TEXT("6 3/4 Envelope 3 5/8 x 6 1/2 in"),
                        __TEXT("US Std Fanfold 14 7/8 x 11 in"),
                        __TEXT("German Std Fanfold 8 1/2 x 12 in"),
                        __TEXT("German Legal Fanfold 8 1/2 x 13 in"),
                        __TEXT("B4 (ISO) 250 x 353 mm"),
                        __TEXT("Japanese Postcard 100 x 148 mm"),
                        __TEXT("9 x 11 in"),
                        __TEXT("10 x 11 in"),
                        __TEXT("15 x 11 in"),
                        __TEXT("Envelope Invite 220 x 220 mm"),
                        __TEXT("RESERVED--DO NOT USE"),
                        __TEXT("RESERVED--DO NOT USE"),
                        __TEXT("Letter Extra 9 \275 x 12 in"),
                        __TEXT("Legal Extra 9 \275 x 15 in"),
                        __TEXT("Tabloid Extra 11.69 x 18 in"),
                        __TEXT("A4 Extra 9.27 x 12.69 in"),
                        __TEXT("Letter Transverse 8 \275 x 11 in"),
                        __TEXT("A4 Transverse 210 x 297 mm"),
                        __TEXT("Letter Extra Transverse 9\275 x 12"),
                        __TEXT("SuperA/SuperA/A4 227 x 356 mm"),
                        __TEXT("SuperB/SuperB/A3 305 x 487 mm"),
                        __TEXT("Letter Plus 8.5 x 12.69 in"),
                        __TEXT("A4 Plus 210 x 330 mm"),
                        __TEXT("A5 Transverse 148 x 210 mm"),
                        __TEXT("B5 (JIS) Transverse 182 x 257 mm"),
                        __TEXT("A3 Extra 322 x 445 mm"),
                        __TEXT("A5 Extra 174 x 235 mm"),
                        __TEXT("B5 (ISO) Extra 201 x 276 mm"),
                        __TEXT("A2 420 x 594 mm"),
                        __TEXT("A3 Transverse 297 x 420 mm"),
                        __TEXT("A3 Extra Transverse 322 x 445 mm")
                        };
                        
                        
                       
 
// bin selections 
char *szBinList[] = {
                      __TEXT("UNKNOWN"),
                      __TEXT("UPPER/ONLYONE"),		    
                      __TEXT("LOWER"),	   
                      __TEXT("MIDDLE"),	   
                      __TEXT("MANUAL"),	   
                      __TEXT("ENVELOPE"),	   
                      __TEXT("ENVMANUAL"),   
                      __TEXT("AUTO"),	    
                      __TEXT("TRACTOR"),	   
                      __TEXT("SMALLFMT"),	   
                      __TEXT("LARGEFMT"),	   
                      __TEXT("LARGE CAPACITY"),
                      __TEXT("UNKNOWN"),
                      __TEXT("UNKNOWN"),
                      __TEXT("CASSETTE"),
                      __TEXT("USER DEFINED")
                      }; 

//************************************************************
//	Object:	CWin32Printer
//	Method:	GetDefaultPrinterDC
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	
//	Return:	Handle to DC.
//
//	Desc  :	Obtain printer device context
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
HDC GetDefaultPrinterDC(void)
{
    PRINTDLG pdlg;
 
    // Initialize the PRINTDLG structure
    memset( &pdlg, 0, sizeof( PRINTDLG ) );
    pdlg.lStructSize = sizeof( PRINTDLG );
    // Set the flag to return printer DC
    pdlg.Flags = PD_RETURNDEFAULT | PD_RETURNDC;
 
    // Invoke the printer dialog box
    PrintDlg( &pdlg );
    // hDC member of the PRINTDLG structure contains
    // the printer DC
    return pdlg.hDC;
}

//************************************************************
//	Object:	CWin32Printer
//	Method:	CWin32Printer (constructor)
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	
//	Return:	
//
//	Desc  :	Initialize member variables
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
CWin32Printer::CWin32Printer()
{
	// PowerDIB handle
	m_hPowerDIB = NULL;

	// set default printer access 
	m_PrinterAccess = PRINTER_ALL_ACCESS;
	m_iExtendedErrorAttribute = 0;

}

//************************************************************
//	Object:	CWin32Printer
//	Method:	~CWin32Printer (destructor)
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	
//	Return:	
//
//	Desc  :	If PowerDIB dll was loaded then free it 
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
CWin32Printer::~CWin32Printer()
{
	if (m_hPowerDIB != NULL)
		FreeLibrary(m_hPowerDIB);
}


/////////////////////////////////////////////////////
// Public

//************************************************************
//	Object:	CWin32Printer
//	Method:	GetDefaultPrinterName
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Default printer name string pointer (by ref)
//	Return:	NO_ERR if Ok
//
//	Desc  :	Get printer name from win.ini
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::GetDefaultPrinterName(LPSTR szDefaultPrinterName)
{

	char	Buffer[256], tmpBuf[256];
	long	iDefaultIsThere = ERR_NO_DEF_PRN;
	long	iColPos;
	size_t	iBufLen;

  GetProfileString("windows", "device", ",,,", Buffer, sizeof(Buffer));
  
  iBufLen = strlen(Buffer);

  if (iBufLen > 0) {
	iDefaultIsThere = NO_ERR;	// there is a default printer

	// make sure that temp buffer is clean
	memset (tmpBuf, 0, sizeof(tmpBuf));

	// get comma position
	for (iColPos = 0; Buffer[iColPos] != ','; iColPos++)
		; // do nothing

	// copy iColPos + 1 bytes
	strncpy(tmpBuf, Buffer, iColPos);
	strcat (tmpBuf, "\0");	// terminate string

	strcpy(szDefaultPrinterName, tmpBuf);

  }

  return iDefaultIsThere;
}

//************************************************************
//	Object:	CWin32Printer
//	Method:	SetDefaultPrinter
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	
//	Return:	
//
//	Desc  :	Implemented in descendant classes
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::SetDefaultPrinter(LPCSTR szPrinterName)
{
	// Virtual function implemented in descendant classes
	// return any value to avoid warning
	return ERR_NOT_IMPLEMENTED;
}


//************************************************************
//	Object:	CWin32Printer
//	Method:	GetPrinterList
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Printer list string pointer (by ref)
//	Return:	Number of printers found or negative if an 
//			error occurred
//
//	Desc  :	Get all printers available. Printer names are 
//			separated using list separator (m_szListSep)
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//	11 of May 1997	Marco Peretti		Make sure that buffer 
//										is large enough
//************************************************************
long CWin32Printer::GetPrinterList(LPSTR szPrtr)
{

  DWORD            dwFlags = PRINTER_ENUM_FAVORITE | PRINTER_ENUM_LOCAL;
  LPPRINTER_INFO_2 pPrinters;
  DWORD            cbPrinters;
  DWORD            cReturned, i;
  const int		   iBufSize = 4048;
  char             Buf[iBufSize];
  size_t		   iListSepLen, iPrnNameLen, iByteCount = 0;

  //
  // get byte count needed for buffer, alloc buffer, the enum the printers
  //

  // init string
  strcpy (Buf, "\0");

  EnumPrinters (dwFlags, NULL, 2, NULL, 0, &cbPrinters, &cReturned);

  if (!(pPrinters = (LPPRINTER_INFO_2) LocalAlloc (LPTR, cbPrinters + 4)))
	goto done_refreshing;
  
  if (!EnumPrinters (dwFlags, NULL, 2, (LPBYTE) pPrinters,
                     cbPrinters, &cbPrinters, &cReturned))
	goto done_refreshing;
  
  if (cReturned > 0) {	// suceeded

	// Get length of list separator
	iListSepLen = strlen(m_szListSep);

	for (i = 0; i < cReturned; i++) {

		// how many bytes are needed ?.
		iPrnNameLen = strlen((pPrinters + i)->pPrinterName);

		if ((iByteCount +  iPrnNameLen + iListSepLen) < iBufSize) {
			strcat (Buf, (pPrinters + i)->pPrinterName);
			strcat (Buf, m_szListSep);

			// calculate the bytes used so far
			iByteCount = strlen(Buf);	// iPrnNameLen + iListSepLen;
		} else
			cReturned = ERR_BUFFER_2SMALL;
	}
	strcat (Buf, "\0");	// make sure that it is NULL terminated
		
  }						// end test cReturned > 0
	
		
done_refreshing:

 LocalFree (LocalHandle (pPrinters));

 strcpy (szPrtr, Buf);
 
 return cReturned;
}

//************************************************************
//	Object:	CWin32Printer
//	Method:	GetDefaultPrinterPort
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Printer port string pointer (by ref)
//	Return:	NO_ERR if Ok
//
//	Desc  :	Get default printer's port
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::GetDefaultPrinterPort(LPSTR szPort)
{
	char	Buffer[256], tmpBuf[256];
	long	iDefaultIsThere = ERR_NO_DEF_PORT;
	long	iColPos, iSecondColPos = 0;
	size_t	iBufLen;

  GetProfileString("windows", "device", ",,,", Buffer, sizeof(Buffer));
  
  iBufLen = strlen(Buffer);

  if (iBufLen > 0) {

	OutputDebugString(Buffer);

	iDefaultIsThere = NO_ERR;	// there is a default driver

	memset(tmpBuf, 0, sizeof(tmpBuf));
	// find first column
	for (iColPos = 0; Buffer[iColPos] != ','; iColPos++)
		iSecondColPos = iColPos;	// save column position

	++iSecondColPos;
	++iSecondColPos;

	// find second column
	for (iColPos = iSecondColPos; Buffer[iColPos] != ','; iColPos++)
		iSecondColPos = iColPos;	// save column position

	++iSecondColPos;
	++iSecondColPos;

	strncpy(tmpBuf, Buffer + iSecondColPos, (iBufLen -iSecondColPos));
	strcat (tmpBuf, "\0");

	strcpy(szPort, tmpBuf);
  }


  return iDefaultIsThere;
}


//************************************************************
//	Object:	CWin32Printer
//	Method:	GetDefaultPrinterDriver
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	printer driver string pointer (by ref)
//	Return:	NO_ERR if Ok
//
//	Desc  :	Get the default printer's driver
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::GetDefaultPrinterDriver(LPSTR szDriver)
{
	char	Buffer[256], tmpBuf[256];
	long	iDefaultIsThere = ERR_NO_DEF_DRIVER;
	long	iColPos, iFirstColPos = 0;
	size_t	iBufLen;

  GetProfileString("windows", "device", ",,,", Buffer, sizeof(Buffer));
  
  iBufLen = strlen(Buffer);

  if (iBufLen > 0) {

	OutputDebugString(Buffer);

	iDefaultIsThere = NO_ERR;	// there is a default driver

	memset(szDriver,	0, sizeof(szDriver));
	memset(tmpBuf,		0, sizeof(tmpBuf));

	// find first column
	for (iColPos = 0; Buffer[iColPos] != ','; iColPos++)
		iFirstColPos = iColPos;	// save column position

	++iFirstColPos;
	++iFirstColPos;

	// find second column
	for (iColPos = iFirstColPos; Buffer[iColPos] != ','; iColPos++)
		;	// do nothing

	strncpy(tmpBuf, Buffer + iFirstColPos, (iColPos -iFirstColPos));
	strcat (tmpBuf, "\0");

	strcpy(szDriver, tmpBuf);
	
  }


  return iDefaultIsThere;
}

//************************************************************
//	Object:	CWin32Printer
//	Method:	SetPrinterAttribute
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Fiedl ID, attribute value
//	Return:	NO_ERR if Ok
//
//	Desc  :	Set SHORT printer (or DEVMODE) attributes
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::SetPrinterAttribute(short dwField, short iAttribute)
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


//************************************************************
//	Object:	CWin32Printer
//	Method:	GetPrinterAttribute
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Field ID, attribute (by ref)
//	Return:	NO_ERR if Ok
//
//	Desc  :	Get SHORT printer (or DEVMODE) attribute
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::GetPrinterAttribute(short	dwField, short &iAttribute)
{

	HGLOBAL hGlobal = NULL;
	HANDLE hPrinter = NULL;
	DWORD dwNeeded = 0;
	PRINTER_INFO_2 *pi2 = NULL;
	PRINTER_DEFAULTS pd;
	BOOL bFlag;
	long iRet = 0;
	char	szPrinterPort[256], 
			szPrinterDriver[256], 
			szPrinterName[256];

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
	need to do is modify what you'r interested in. */
 
	bFlag = GetPrinter(hPrinter, 2, (LPBYTE)pi2, dwNeeded, &dwNeeded);
	if (!bFlag) {
		iRet = ERR_PRINT_INFO2;
		m_iExtendedErrorAttribute  = GetLastError();
		goto ABORT;
	}
 
	if (pi2->pDevMode != NULL) {

		switch (dwField) {

			case DW_ORIENTATION:
				iAttribute =pi2->pDevMode->dmOrientation;
				iRet = NO_ERR;
				break;

			case DW_PAPERSIZE:
				iAttribute =pi2->pDevMode->dmPaperSize;
				iRet = NO_ERR;
				break;

			case DW_PAPERLENGTH:
				iAttribute =pi2->pDevMode->dmPaperLength;
				iRet = NO_ERR;
				break;

			case DW_PAPERWIDTH:
				iAttribute =pi2->pDevMode->dmPaperWidth;
				iRet = NO_ERR;
				break;

			case DW_SCALE:
				iAttribute =pi2->pDevMode->dmScale;
				iRet = NO_ERR;
				break;

			case DW_COPIES:
				iAttribute =pi2->pDevMode->dmCopies;
				iRet = NO_ERR;
				break;

			case DW_DEFAULTSOURCE:
				iAttribute =pi2->pDevMode->dmDefaultSource;
				iRet = NO_ERR;
				break;

			case DW_PRINTQUALITY:
				iAttribute =pi2->pDevMode->dmPrintQuality;
				iRet = NO_ERR;
				break;

			case DW_COLOR:
				iAttribute =pi2->pDevMode->dmColor;
				iRet = NO_ERR;
				break;

			case DW_DUPLEX:
				iAttribute =pi2->pDevMode->dmDuplex;
				iRet = NO_ERR;
				break;

			case DW_YRESOLUTION:
				iAttribute =pi2->pDevMode->dmYResolution;
				iRet = NO_ERR;
				break;

			case DW_TTOPTION:
				iAttribute =pi2->pDevMode->dmTTOption;
				iRet = NO_ERR;
				break;

			case DW_COLLATE:
				iAttribute =pi2->pDevMode->dmCollate;
				iRet = NO_ERR;
				break;

			default:
				iAttribute = -1;
				iRet = ERR_ATTR_UNKNOWN;
				
		}	// end switch

	}		//	end pDevMode != NULL
	else
		iRet = ERR_DEVMODE_ISNULL; // Aaargh !

 
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

//************************************************************
//	Object:	CWin32Printer
//	Method:	SetPrinterAttribute
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Field ID, value
//	Return:	NO_ERR if OK
//
//	Desc  :	Set WORD printer (or DEVMODE) attributes
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::SetPrinterAttribute(short dwField, WORD wAttribute)
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
	need to do is modify what you'r interested in. */
 
	bFlag = GetPrinter(hPrinter, 2, (LPBYTE)pi2, dwNeeded, &dwNeeded);
	if (!bFlag) {
		iRet = ERR_PRINT_INFO2;
		m_iExtendedErrorAttribute  = GetLastError();
		goto ABORT;
	}
 
	/* Set orientation to Landscape mode if the driver supports it. */
 
	if (pi2->pDevMode != NULL) {

		switch (dwField) {

			case DW_LOGPIXELS:
				if (pi2->pDevMode->dmFields & DM_LOGPIXELS) {
					/* Change the devmode. */					
					pi2->pDevMode->dmLogPixels = wAttribute;
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
	else
		iRet = ERR_DEVMODE_ISNULL; // Aaargh !


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


//************************************************************
//	Object:	CWin32Printer
//	Method:	GetPrinterAttribute
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Field ID, value (by ref)
//	Return:	NO_ERR if Ok
//
//	Desc  :	Get WORD printer (or DEVMODE) attribute
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::GetPrinterAttribute(short	dwField, WORD &wAttribute)
{

	HGLOBAL hGlobal = NULL;
	HANDLE hPrinter = NULL;
	DWORD dwNeeded = 0;
	PRINTER_INFO_2 *pi2 = NULL;
	PRINTER_DEFAULTS pd;
	BOOL bFlag;
	long iRet = 0;
	char	szPrinterPort[256], 
			szPrinterDriver[256], 
			szPrinterName[256];

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
	need to do is modify what you'r interested in. */
 
	bFlag = GetPrinter(hPrinter, 2, (LPBYTE)pi2, dwNeeded, &dwNeeded);
	if (!bFlag) {
		iRet = ERR_PRINT_INFO2;
		m_iExtendedErrorAttribute  = GetLastError();
		goto ABORT;
	}
 
	/* Set orientation to Landscape mode if the driver supports it. */
 
	if (pi2->pDevMode != NULL) {

		switch (dwField) {

			case DW_LOGPIXELS:
				wAttribute =pi2->pDevMode->dmLogPixels;
				iRet = NO_ERR;
				break;

			default:
				wAttribute = 0;	// cannot set to -1
				iRet = ERR_ATTR_UNKNOWN;
				
		}	// end switch

	}		//	end pDevMode != NULL
	else
		iRet = ERR_DEVMODE_ISNULL; // Aaargh !

 
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

//************************************************************
//	Object:	CWin32Printer
//	Method:	SetPrinterAttribute
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Field ID, value
//	Return:	NO_ERR if Ok
//
//	Desc  :	Set DWORD Printer attributes
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::SetPrinterAttribute(short dwField, DWORD dwAttribute)
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
	need to do is modify what you'r interested in. */
 
	bFlag = GetPrinter(hPrinter, 2, (LPBYTE)pi2, dwNeeded, &dwNeeded);
	if (!bFlag) {
		iRet = ERR_PRINT_INFO2;
		m_iExtendedErrorAttribute  = GetLastError();
		goto ABORT;
	}
 
	if (pi2->pDevMode != NULL) {

		switch (dwField) {

			case DW_BITSPERPEL:
				if (pi2->pDevMode->dmFields & DM_BITSPERPEL) {
					/* Change the devmode. */					
					pi2->pDevMode->dmBitsPerPel = dwAttribute;
					bChanged = TRUE;
					iRet = NO_ERR;
				}
				 else
					iRet = ERR_PRN_NOCHANGE;
				break;

			case DW_PELSWIDTH:
				if (pi2->pDevMode->dmFields & DM_PELSWIDTH) {
					/* Change the devmode. */					
					pi2->pDevMode->dmPelsWidth = dwAttribute;
					bChanged = TRUE;
					iRet = NO_ERR;
				}
				 else
					iRet = ERR_PRN_NOCHANGE;
				break;

			case DW_PELSHEIGHT:
				if (pi2->pDevMode->dmFields & DM_PELSHEIGHT) {
					/* Change the devmode. */					
					pi2->pDevMode->dmPelsHeight = dwAttribute;
					bChanged = TRUE;
					iRet = NO_ERR;
				}
				 else
					iRet = ERR_PRN_NOCHANGE;
				break;

			case DW_DISPLAYFLAGS:
				if (pi2->pDevMode->dmFields & DM_DISPLAYFLAGS) {
					/* Change the devmode. */					
					pi2->pDevMode->dmDisplayFlags = dwAttribute;
					bChanged = TRUE;
					iRet = NO_ERR;
				}
				 else
					iRet = ERR_PRN_NOCHANGE;
				break;

			case DW_DISPLAYFREQUENCY:
				if (pi2->pDevMode->dmFields & DM_DISPLAYFREQUENCY) {
					/* Change the devmode. */					
					pi2->pDevMode->dmDisplayFrequency = dwAttribute;
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
	else
		iRet = ERR_DEVMODE_ISNULL; // Aaargh !


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

//************************************************************
//	Object:	CWin32Printer
//	Method:	GetPrinterAttribute
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Field ID, value (by ref)
//	Return:	NO_ERR if Ok
//
//	Desc  :	Get DWORD Printer attributes
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::GetPrinterAttribute(short	dwField, DWORD &dwAttribute)
{

	HGLOBAL hGlobal = NULL;
	HANDLE hPrinter = NULL;
	DWORD dwNeeded = 0;
	PRINTER_INFO_2 *pi2 = NULL;
	PRINTER_DEFAULTS pd;
	BOOL bFlag;
	long iRet = 0;
	char	szPrinterPort[256], 
			szPrinterDriver[256], 
			szPrinterName[256];

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
	need to do is modify what you'r interested in. */
 
	bFlag = GetPrinter(hPrinter, 2, (LPBYTE)pi2, dwNeeded, &dwNeeded);
	if (!bFlag) {
		iRet = ERR_PRINT_INFO2;
		m_iExtendedErrorAttribute  = GetLastError();
		goto ABORT;
	}
 
	if (pi2->pDevMode != NULL) {

		switch (dwField) {

			case DW_BITSPERPEL:
				dwAttribute =pi2->pDevMode->dmBitsPerPel;
				iRet = NO_ERR;
				break;

			case DW_PELSWIDTH:
				dwAttribute =pi2->pDevMode->dmPelsWidth;
				iRet = NO_ERR;
				break;

			case DW_PELSHEIGHT:
				dwAttribute =pi2->pDevMode->dmPelsHeight;
				iRet = NO_ERR;
				break;

			case DW_DISPLAYFLAGS:
				dwAttribute =pi2->pDevMode->dmDisplayFlags;
				iRet = NO_ERR;
				break;

			case DW_DISPLAYFREQUENCY:
				dwAttribute =pi2->pDevMode->dmDisplayFrequency;
				iRet = NO_ERR;
				break;
				
			default:
				dwAttribute = 0; // cannot set to -1
				iRet = ERR_ATTR_UNKNOWN;
				
		}	// end switch

	}		//	end pDevMode != NULL
	else
		iRet = ERR_DEVMODE_ISNULL; // Aaargh !

 
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


//************************************************************
//	Object:	CWin32Printer
//	Method:	GetDefaultPrinterEx
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Printer name, driver, port - all by ref
//	Return:	NO_ERR if Ok
//
//	Desc  :	Get printer name, driver, and port in one go
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::GetDefaultPrinterEx(LPSTR szPrinterName, LPSTR szPrinterDriver, LPSTR szPrinterPort)
{
	char	Buffer[256], tmpBuf[256];
	long	iDefaultIsThere = ERR_NO_DEF_PRN;
	long	iColPos, iNextItemColPos;
	size_t	iBufLen;

  GetProfileString("windows", "device", ",,,", Buffer, sizeof(Buffer));
  
  iBufLen = strlen(Buffer);

  if (iBufLen > 0) {
	iDefaultIsThere = NO_ERR;	// there is a default printer

	OutputDebugString(Buffer);	
	
	memset (tmpBuf, 0, sizeof(tmpBuf));

	// printer name
	for (iColPos = 0; Buffer[iColPos] != ','; iColPos++)
		; // do nothing
	
	strncpy(tmpBuf, Buffer, iColPos);
	strcat (tmpBuf, "\0");
	strcpy(szPrinterName, tmpBuf);


	// printer driver
	iNextItemColPos = iColPos;
	++iNextItemColPos;
	memset(tmpBuf, 0, sizeof(tmpBuf));

	// find second column
	for (iColPos = iNextItemColPos; Buffer[iColPos] != ','; iColPos++)
		;	// do nothing

	strncpy(tmpBuf, Buffer + iNextItemColPos, (iColPos -iNextItemColPos));
	strcat (tmpBuf, "\0");
	strcpy(szPrinterDriver, tmpBuf);

	// printer port
	memset(tmpBuf, 0, sizeof(tmpBuf));
	iNextItemColPos = iColPos;	// save column position
	++iNextItemColPos;

	strncpy(tmpBuf, Buffer + iNextItemColPos, (iBufLen -iNextItemColPos));
	strcat (tmpBuf, "\0");
	strcpy(szPrinterPort, tmpBuf);
  }

  return iDefaultIsThere;
}

//************************************************************
//	Object:	CWin32Printer
//	Method:	SetDefaultPrinterEx
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Printer name and port
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Set default printer given its name and port
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::SetDefaultPrinterEx(LPCSTR szPrinterName, LPCSTR szPrinterDriver, LPCSTR szPrinterPort)
{
	// virtual, implemented in descendant classes
	return ERR_NOT_IMPLEMENTED;
}

long CWin32Printer::GetColor(short &iColor)
{
	return GetPrinterAttribute(DW_COLOR, iColor);
}

long CWin32Printer::SetColor(short iColor)
{
	return SetPrinterAttribute(DW_COLOR, iColor);
}


long CWin32Printer::GetYResolution(short &iYResolution)
{
	return GetPrinterAttribute(DW_YRESOLUTION, iYResolution);
}

long CWin32Printer::SetYResolution(short iYResolution)
{
	return SetPrinterAttribute(DW_YRESOLUTION, iYResolution);
}

long CWin32Printer::GetTTOption(short &iTTOption)
{
	return GetPrinterAttribute(DW_TTOPTION, iTTOption);
}

long CWin32Printer::SetTTOption(short iTTOption)
{
	return SetPrinterAttribute(DW_TTOPTION, iTTOption);
}

long CWin32Printer::GetCollate(short &iCollate)
{
	return GetPrinterAttribute(DW_COLLATE, iCollate);
}

long CWin32Printer::SetCollate(short iCollate)
{
	return SetPrinterAttribute(DW_COLLATE, iCollate);
}

long CWin32Printer::GetLogPixels(WORD &wLogPixels)
{
	return GetPrinterAttribute(DW_LOGPIXELS, wLogPixels);
}

long CWin32Printer::SetLogPixels(WORD wLogPixels)
{
	return SetPrinterAttribute(DW_LOGPIXELS, wLogPixels);
}

long CWin32Printer::GetBitsPerPel(DWORD &dwBitsPerPel)
{
	return GetPrinterAttribute(DW_BITSPERPEL, dwBitsPerPel);
}

long CWin32Printer::SetBitsPerPel(DWORD dwBitsPerPel)
{
	return SetPrinterAttribute(DW_BITSPERPEL, dwBitsPerPel);
}

long CWin32Printer::GetPelsWidth(DWORD &dwPelsWidth)
{
	return GetPrinterAttribute(DW_PELSWIDTH, dwPelsWidth);
}

long CWin32Printer::SetPelsWidth(DWORD dwPelsWidth)
{
	return SetPrinterAttribute(DW_PELSWIDTH, dwPelsWidth);
}

long CWin32Printer::GetPelsHeight(DWORD &dwPelsHeight)
{
	return GetPrinterAttribute(DW_PELSHEIGHT, dwPelsHeight);
}

long CWin32Printer::SetPelsHeight(DWORD dwPelsHeight)
{
	return SetPrinterAttribute(DW_PELSHEIGHT, dwPelsHeight);
}

long CWin32Printer::GetDisplayFlags(DWORD &dwDisplayFlags)
{
	return GetPrinterAttribute(DW_DISPLAYFLAGS, dwDisplayFlags);
}

long CWin32Printer::SetDisplayFlags(DWORD dwDisplayFlags)
{
	return SetPrinterAttribute(DW_DISPLAYFLAGS, dwDisplayFlags);
}

long CWin32Printer::GetDisplayFrequency(DWORD &dwDisplayFrequency)
{
	return GetPrinterAttribute(DW_DISPLAYFREQUENCY, dwDisplayFrequency);
}

long CWin32Printer::SetDisplayFrequency(DWORD dwDisplayFrequency)
{
	return SetPrinterAttribute(DW_DISPLAYFREQUENCY, dwDisplayFrequency);
}

//************************************************************
//	Object:	CWin32Printer
//	Method:	CheckPrinterName
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	printer name
//	Return:	TRUE if printer exists	
//
//	Desc  :	Check printer name in the list if available printers
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
BOOL CWin32Printer::CheckPrinterName(LPCSTR szPrnName)
{

  DWORD				dwFlags = PRINTER_ENUM_FAVORITE | PRINTER_ENUM_LOCAL;
  LPPRINTER_INFO_2	pPrinters;
  DWORD				cbPrinters;
  DWORD				cReturned, i;
  BOOL				bOk = FALSE;

  //
  // get byte count needed for buffer, alloc buffer, the enum the printers
  //

  EnumPrinters (dwFlags, NULL, 2, NULL, 0, &cbPrinters, &cReturned);

  if (!(pPrinters = (LPPRINTER_INFO_2) LocalAlloc (LPTR, cbPrinters + 4)))
	goto done_refreshing;
  
  if (!EnumPrinters (dwFlags, NULL, 2, (LPBYTE) pPrinters,
                     cbPrinters, &cbPrinters, &cReturned))
	goto done_refreshing;
  
  // if we have at least a printer
  if (cReturned > 0) {
	for (i = 0; i < cReturned; i++)	// check if names are ==
      if (strcmp (szPrnName, (pPrinters + i)->pPrinterName) == 0)
		  bOk = TRUE;
  }

		
done_refreshing:

 LocalFree (LocalHandle (pPrinters));

 return bOk;

}


//************************************************************
//	Object:	CWin32Printer
//	Method:	GetPaperBinList
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Paper bin string pointer (by ref)
//	Return:	Buffer size
//
//	Desc  :	Check out device capabilites and return list of
//			supported paper bins
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::GetPaperBinList(LPSTR szPaperBins)
{
	DWORD       dwBufSize = 0;
	long			i;
	char		PrnName[256], PrnPort[256], PrnDriver[256];
	char		szBuffer[64], tmpBuf[2000];
    WORD		*pawBinList;

	// Make sure there's no rubbish
	memset(PrnName,		0, sizeof(PrnName));
	memset(PrnDriver,	0, sizeof(PrnDriver));
	memset(PrnPort,		0, sizeof(PrnPort));
	
	// get default printer info
	GetDefaultPrinterName(PrnName);
	GetDefaultPrinterPort(PrnPort);
	GetDefaultPrinterDriver(PrnDriver);

	// clean temp buffer too
	memset (tmpBuf, 0, sizeof(tmpBuf));

	// get number of bins
    dwBufSize = DeviceCapabilities ((LPCSTR )PrnName, (LPCSTR )PrnPort, (WORD)DC_BINS,(LPSTR )NULL, (LPDEVMODE)NULL);

	pawBinList = (WORD *)VirtualAlloc(NULL, dwBufSize* (sizeof(WORD)), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
	
    // fill buffer with paper list
    DeviceCapabilities ((LPCSTR )PrnName, (LPSTR )PrnPort, (WORD)DC_BINS, (LPSTR)pawBinList, (LPDEVMODE)NULL);
	
	if ((dwBufSize > 0) && (dwBufSize < MAX_AMOUNT))
    {
		for (i=0; i < (long)dwBufSize; i++)
        {
			if (pawBinList[i] < MAX_AMOUNT)  // default bins
            {
               if (pawBinList[i] < MAX_BINS)
                  wsprintf((LPTSTR )szBuffer, "%u  %s;", pawBinList[i], (LPTSTR )szBinList[pawBinList[i]]);
               else
                  wsprintf((LPTSTR )szBuffer, "%u  %s;", pawBinList[i], (LPTSTR )szBinList[0]);
            }
            else  // device specific bins
               wsprintf((LPTSTR )szBuffer, "%u  %s;", pawBinList[i], (LPTSTR )szBinList[15]);

	   		strcat(tmpBuf, szBuffer);
        }
		strcpy(szPaperBins, tmpBuf);
	}
    else if (dwBufSize == 0)  // no bins
		strcpy(szPaperBins, "None");

    else
		strcpy(szPaperBins, "Driver gave bad info!");

	VirtualFree(pawBinList, 0, MEM_RELEASE);

	return (long)dwBufSize;
}


//************************************************************
//	Object:	CWin32Printer
//	Method:	GetNamedPaperBinList
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Paper bin list string pointer (by ref)
//	Return:	Buffer size
//
//	Desc  :	Examine device capabilities and return NAMED
//			paper bins
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::GetNamedPaperBinList(LPSTR szPaperBins)
{
	DWORD       dwBufSize = 0, dwMemSize;
	long		i;
	char		szBuffer[64], tmpBuf[2000];
	LPTSTR      lpstrBinNames;
	LPTSTR      lpstrBinNameList;
	char		PrnName[256], PrnPort[256], PrnDriver[256];

	memset(PrnName,		0, sizeof(PrnName));
	memset(PrnDriver,	0, sizeof(PrnDriver));
	memset(PrnPort,		0, sizeof(PrnPort));
	
	// get default printer info
	GetDefaultPrinterName(PrnName);
	GetDefaultPrinterPort(PrnPort);
	GetDefaultPrinterDriver(PrnDriver);

	memset (tmpBuf, 0, sizeof(tmpBuf));

	// get number of named bins
    dwBufSize = DeviceCapabilities ((LPCSTR )PrnName, (LPCSTR )PrnPort, (WORD)DC_BINNAMES,(LPSTR )NULL, (LPDEVMODE)NULL);

    if ((dwBufSize > 0) && (dwBufSize < MAX_AMOUNT))
    {
		// get buffer size
        dwMemSize = dwBufSize * (sizeof(short) + (CCHBINNAME*sizeof(char)));

		lpstrBinNames = (LPSTR )VirtualAlloc(NULL, dwBufSize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

        // fill buffer with bin names
		DeviceCapabilities ((LPCSTR )PrnName, (LPSTR )PrnPort, (WORD)DC_BINNAMES, (LPSTR)lpstrBinNames, (LPDEVMODE)NULL);
 
        // display bin identifier number and bin name
        for (i=0; i < (long)dwBufSize; i++)
        {
			lpstrBinNameList = lpstrBinNames;

            wsprintf((LPTSTR )szBuffer, "%s;", 
                     (LPTSTR )lpstrBinNameList + (CCHBINNAME*i));

  	   		strcat(tmpBuf, szBuffer);
         }

		 strcpy(szPaperBins, tmpBuf);
         // clean up
		 VirtualFree(lpstrBinNameList, 0, MEM_RELEASE);
	}		 
   	return (long)dwBufSize;

}

//************************************************************
//	Object:	CWin32Printer
//	Method:	GetSupportedPaperSizeList
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Paper size string pointer
//	Return:	Buffer size
//
//	Desc  :	Get the paper size the default printer can print to
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::GetSupportedPaperSizeList(LPSTR szPageSizeList)
{

    DWORD       dwBufSize = 0;
    BOOL        bResult = 1;
    WORD		*pawPaperList; 
    char        szBuffer[64], tmpBuf[4000];;
    long i;
	char		PrnName[256], PrnPort[256], PrnDriver[256];

	memset(PrnName,		0, sizeof(PrnName));
	memset(PrnDriver,	0, sizeof(PrnDriver));
	memset(PrnPort,		0, sizeof(PrnPort));
	
	// get default printer info
	GetDefaultPrinterName(PrnName);
	GetDefaultPrinterPort(PrnPort);
	GetDefaultPrinterDriver(PrnDriver);

	memset (tmpBuf, 0, sizeof(tmpBuf));

	// get paper dimensions
	dwBufSize = DeviceCapabilities ((LPSTR )PrnName, (LPSTR )PrnPort, (WORD)DC_PAPERS, (LPSTR )NULL, (LPDEVMODE)NULL);

	// allocate space for paper sizes
	pawPaperList = (WORD *)VirtualAlloc(NULL, dwBufSize* (sizeof(WORD)), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
   
	// fill buffer with paper list
	DeviceCapabilities ((LPSTR )PrnName, (LPSTR )PrnPort, (WORD)DC_PAPERS, (LPSTR )pawPaperList, (LPDEVMODE)NULL);

	// display results
    if (dwBufSize < MAX_AMOUNT)
    {
		for (i=0; i < (long)dwBufSize; i++)
        {
            if ((pawPaperList[i] < MAX_PAPERS) && (pawPaperList[i] > 0))
               wsprintf((LPTSTR )szBuffer, "%u %s;", pawPaperList[i],(LPTSTR )szPaperList[pawPaperList[i]]);

            else
               wsprintf((LPTSTR )szBuffer, "%u %s;", pawPaperList[i], (LPTSTR )szPaperList[0]);

			strcat(tmpBuf, szBuffer);
        }
		strcpy(szPageSizeList, tmpBuf);
    }
    else
		strcpy(szPageSizeList, "Driver gave bad info!");

	// clean up
	VirtualFree(pawPaperList, 0, MEM_RELEASE);
				
	return dwBufSize;
}

//************************************************************
//	Object:	CWin32Printer
//	Method:	GetPaperBinListEx
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	printer name, list by ref
//	Return:	Buffer size
//
//	Desc  :	Get paper bins given a printer name
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::GetPaperBinListEx(LPCSTR szPrinterName, LPSTR szPaperBins)
{
	DWORD       dwBufSize = 0;
	long			i;
	char		PrnPort[256];
	char		szBuffer[64], tmpBuf[2000];
    WORD		*pawBinList;

	memset(PrnPort,		0, sizeof(PrnPort));
	
	// get printer port
	GetPrinterPort(szPrinterName, PrnPort);

	memset (tmpBuf, 0, sizeof(tmpBuf));

	// get number of bins
    dwBufSize = DeviceCapabilities (szPrinterName, (LPCSTR )PrnPort, (WORD)DC_BINS,(LPSTR )NULL, (LPDEVMODE)NULL);

	pawBinList = (WORD *)VirtualAlloc(NULL, dwBufSize* (sizeof(WORD)), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
	
    // fill buffer with paper list
    DeviceCapabilities (szPrinterName, (LPSTR )PrnPort, (WORD)DC_BINS, (LPSTR)pawBinList, (LPDEVMODE)NULL);
	
	if ((dwBufSize > 0) && (dwBufSize < MAX_AMOUNT))
    {
		for (i=0; i < (long)dwBufSize; i++)
        {
			if (pawBinList[i] < MAX_AMOUNT)  // default bins
            {
               if (pawBinList[i] < MAX_BINS)
                  wsprintf((LPTSTR )szBuffer, "%u  %s;", pawBinList[i], (LPTSTR )szBinList[pawBinList[i]]);
               else
                  wsprintf((LPTSTR )szBuffer, "%u  %s;", pawBinList[i], (LPTSTR )szBinList[0]);
            }
            else  // device specific bins
               wsprintf((LPTSTR )szBuffer, "%u  %s;", pawBinList[i], (LPTSTR )szBinList[15]);

	   		strcat(tmpBuf, szBuffer);
        }
		strcpy(szPaperBins, tmpBuf);
	}
    else if (dwBufSize == 0)  // no bins
		strcpy(szPaperBins, "None");

    else
		strcpy(szPaperBins, "Driver gave bad info!");

	VirtualFree(pawBinList, 0, MEM_RELEASE);

	return (long)dwBufSize;
}



//************************************************************
//	Object:	CWin32Printer
//	Method:	GetNamedPaperBinListEx
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	Printer name, paper bin list string pointer (by ref)
//	Return:	Buffer size
//
//	Desc  :	
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::GetNamedPaperBinListEx(LPCSTR szPrinterName, LPSTR szPaperBins)
{
	DWORD       dwBufSize = 0, dwMemSize;
	long		i;
	char		szBuffer[64], tmpBuf[2000];
	LPTSTR      lpstrBinNames;
	LPTSTR      lpstrBinNameList;
	char		PrnPort[256];

	memset(PrnPort,		0, sizeof(PrnPort));
	
	// get printer info
	GetPrinterPort(szPrinterName, PrnPort);

	memset (tmpBuf, 0, sizeof(tmpBuf));

	// get number of named bins
    dwBufSize = DeviceCapabilities (szPrinterName, (LPCSTR )PrnPort, (WORD)DC_BINNAMES,(LPSTR )NULL, (LPDEVMODE)NULL);

    if ((dwBufSize > 0) && (dwBufSize < MAX_AMOUNT))
    {
		// get buffer size
        dwMemSize = dwBufSize * (sizeof(short) + (CCHBINNAME*sizeof(char)));

		lpstrBinNames = (LPSTR )VirtualAlloc(NULL, dwBufSize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

        // fill buffer with bin names
		DeviceCapabilities (szPrinterName, (LPSTR )PrnPort, (WORD)DC_BINNAMES, (LPSTR)lpstrBinNames, (LPDEVMODE)NULL);
 
        // display bin identifier number and bin name
        for (i=0; i < (long)dwBufSize; i++)
        {
			lpstrBinNameList = lpstrBinNames;

            wsprintf((LPTSTR )szBuffer, "%s;", 
                     (LPTSTR )lpstrBinNameList + (CCHBINNAME*i));

  	   		strcat(tmpBuf, szBuffer);
         }

		 strcpy(szPaperBins, tmpBuf);
         // clean up
		 VirtualFree(lpstrBinNameList, 0, MEM_RELEASE);
	}		 
   	return (long)dwBufSize;

}


//************************************************************
//	Object:	CWin32Printer
//	Method:	GetPrinterPort
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	Printer Name, port by ref
//	Return:	NO_ERR if Ok
//
//	Desc  :	
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::GetPrinterPort(LPCSTR szPrinterName, LPSTR szPortName)
{
	return ERR_NOT_IMPLEMENTED;
}


//************************************************************
//	Object:	CWin32Printer
//	Method:	SetPrinterAttribute
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	Field ID, string pointer attribute
//	Return:	NO_ERR if Ok
//
//	Desc  :	Set a printer attribute
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::SetPrinterAttribute(short dwField, LPCSTR szAttribute)
{
	HGLOBAL				hGlb, hGlobal = NULL;
	HANDLE				hPrinter = NULL;
	DWORD				dwNewPI2, dwNeeded = 0;
	PRINTER_INFO_2		*pi2 = NULL;
	PRINTER_INFO_2		*p2 = NULL;
	PRINTER_DEFAULTS	pd;
	BOOL				bFlag, bChanged = FALSE;
	LONG				lFlag;
	long				iRet = NO_ERR;
	char				szPrinterPort[256],
						szPrinterDriver[256],
						szPrinterName[256];
	size_t				dwDelta;

	// TEST
	HGLOBAL hPort;

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

	/* Allocate enough space for new PRINTER_INFO_2. */
	if (dwField == DW_PORT) {
		if (strlen(szAttribute) > strlen(szPrinterPort))
			dwDelta = strlen(szAttribute) - strlen(szPrinterPort);
		else
			dwDelta = 0;
	} else
		dwDelta = 0;

	dwNewPI2 = dwNeeded + dwDelta;

	hGlb = GlobalAlloc(GHND, dwNewPI2);
	if (hGlb == NULL) {
		iRet = ERR_GLOBAL_ALLOC;
		m_iExtendedErrorAttribute  = GetLastError();
		goto ABORT;
	}

	p2 = (PRINTER_INFO_2 *)GlobalLock(hGlb);
	if (p2 == NULL) {
		iRet = ERR_GLOBAL_LOCK;
		m_iExtendedErrorAttribute  = GetLastError();
		goto ABORT;
	}
	
	/* change the PORT NAME */
	if (pi2->pDevMode != NULL) {

		memcpy(p2, pi2, dwNeeded);

		switch (dwField) {
			
			case DW_PORT:
				if (dwDelta  > 0) {
			
					hPort = GlobalAlloc(GHND, strlen(szAttribute) +1);
					if (hPort == NULL) {
						iRet = ERR_GLOBAL_ALLOC;
						m_iExtendedErrorAttribute  = GetLastError();
						goto ABORT;
					}

					p2->pPortName = (LPSTR)GlobalLock(hPort);
					if (p2->pPortName == NULL) {
						iRet = ERR_GLOBAL_LOCK;
						m_iExtendedErrorAttribute  = GetLastError();
						goto ABORT;
					}
				}
				// new port
				strcpy(p2->pPortName, szAttribute);
				bChanged = TRUE;
				break;

			// this is just a test: it never worked :-(
			case DW_FORMNAME:
				strcpy((char*)p2->pDevMode->dmFormName, szAttribute);
				p2->pDevMode->dmFields |= DM_FORMNAME;
				//p2->pDevMode->dmFields & !(DM_PAPERSIZE | DM_PAPERLENGTH | DM_PAPERWIDTH);

				bChanged = TRUE;
				break;

			default:
				iRet = ERR_ATTR_UNKNOWN;
		}	// end switch
	}	
	else
		iRet = ERR_DEVMODE_ISNULL; // Aaargh !

	// has the attribute been changed ?
	if (bChanged) {

		/* Make sure the driver-dependent part of devmode is updated as
			necessary. */
		lFlag = DocumentProperties(NULL, hPrinter, szPrinterName,
            p2->pDevMode, p2->pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER | DM_UPDATE);

		if (lFlag != IDOK) {
			iRet = ERR_UPDT_DEVMODE;
			m_iExtendedErrorAttribute  = GetLastError();
			goto ABORT;
		}
 
	/* Update printer information. */
		p2->pSecurityDescriptor = NULL;

		bFlag = SetPrinter(hPrinter, 2, (LPBYTE)p2, 0);
		if (!bFlag) {
		/* The driver supported the change but wasn't allowed due to some
			other reason (probably lack of permission). */
			iRet = ERR_PRN_SET;
			m_iExtendedErrorAttribute  = GetLastError();
			goto ABORT;
		}
		else { // test due to NT bug ?

			if (dwGetWinVersion() == VER_PLATFORM_WIN32_NT)
				SetDefaultPrinterEx (szPrinterName, szPrinterDriver, szAttribute);

		}
	}	// end bChanged

 
/* Clean up. */
ABORT:   

	if (p2->pPortName != NULL)
		GlobalUnlock(hPort);

	if (hPort != NULL)
		GlobalFree(hPort);

	if (pi2 != NULL)
		GlobalUnlock(hGlobal);
	if (hGlobal != NULL)
		GlobalFree(hGlobal);

	if (p2 != NULL)
		GlobalUnlock(hGlb);
	if (hGlb != NULL)
		GlobalFree(hGlb);

	if (hPrinter != NULL)
		ClosePrinter(hPrinter);

   return iRet;

}


//************************************************************
//	Object:	CWin32Printer
//	Method:	SetDefaultPrinterPort
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	Printer port
//	Return:	NO_ERR if Ok
//
//	Desc  :	Set the port for the default printer
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::SetDefaultPrinterPort(LPCSTR szPort)
{
	return SetPrinterAttribute(DW_PORT, szPort);
}

//************************************************************
//	Object:	CWin32Printer
//	Method:	SetFormName
//	Author:	Marco Peretti - SecureWave
//	Date  :	15 March 1998
//
//	Arg   :	Form Name
//	Return:	NO_ERR if Ok
//
//	Desc  :	Set the form to be used by the default printer
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::SetFormName(LPCSTR szFormName)
{
	return SetPrinterAttribute(DW_FORMNAME, szFormName);
}

//************************************************************
//	Object:	CWin32Printer
//	Method:	GetPrinterListEx
//	Author:	Marco Peretti - SecureWave
//	Date  :	5 Jun 1997
//
//	Arg   :	Printer list string pointer (by ref)
//	Return:	NO_ERR if Ok
//
//	Desc  :	Get printer list (printer name, port, driver)
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::GetPrinterListEx(LPSTR szPrtr)
{
    DWORD            dwFlags = PRINTER_ENUM_FAVORITE | PRINTER_ENUM_LOCAL;
    LPPRINTER_INFO_2 pPrinters;
    DWORD            cbPrinters;
    DWORD            cReturned, i;
    size_t			 iPrnNameLen, iPortNameLen, iDriverNameLen; 
    size_t			 iListSepLen, iItemSepLen, iByteCount = 0;
    const int		 iBufSize = 4048;
    char             Buf[iBufSize];

    //
  // get byte count needed for buffer, alloc buffer, the enum the printers
  //

  // init string
  strcpy (Buf, "\0");

  EnumPrinters (dwFlags, NULL, 2, NULL, 0, &cbPrinters, &cReturned);

  if (!(pPrinters = (LPPRINTER_INFO_2) LocalAlloc (LPTR, cbPrinters + 4)))
	goto done_refreshing;
  
  if (!EnumPrinters (dwFlags, NULL, 2, (LPBYTE) pPrinters,
                     cbPrinters, &cbPrinters, &cReturned))
	goto done_refreshing;
  
  if (cReturned > 0) {	// suceeded

	// Get length of list & item separator
	iListSepLen = strlen(m_szListSep);
	iItemSepLen = strlen(m_szItemSep);

	for (i = 0; i < cReturned; i++) {

		// how many bytes are needed ?.
		iPrnNameLen		= strlen((pPrinters + i)->pPrinterName);
		iPortNameLen	= strlen((pPrinters + i)->pPortName);
		iDriverNameLen	= strlen((pPrinters + i)->pDriverName);

		if ((iByteCount +  iPrnNameLen + iItemSepLen + iPortNameLen + iItemSepLen + iDriverNameLen + iListSepLen) < iBufSize) {
			strcat (Buf, (pPrinters + i)->pPrinterName);
			strcat (Buf, m_szItemSep);
			strcat (Buf, (pPrinters + i)->pPortName);
			strcat (Buf, m_szItemSep);
			strcat (Buf, (pPrinters + i)->pDriverName);
			strcat (Buf, m_szListSep);

			// calculate the bytes used so far
			iByteCount = strlen(Buf);	


		} else
			cReturned = ERR_BUFFER_2SMALL;
	}
	strcat (Buf, "\0");	// make sure that it is NULL terminated
		
  }						// end test cReturned > 0
	
		
done_refreshing:

 LocalFree (LocalHandle (pPrinters));

 strcpy (szPrtr, Buf);
 
 return cReturned;
}


//************************************************************
//	Object:	CWin32Printer
//	Method:	SetPrinterAccess
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	access
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Set printer acces to be used when changing default
//			printer settings -- NT only
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::SetPrinterAccess(long iPrinterAccess)
{
	m_PrinterAccess = iPrinterAccess;

	// The printer access code is not checked
	return 1;
}


//************************************************************
//	Object:	CWin32Printer
//	Method:	GetPrinterAccess
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	printer access, by ref
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Get printer access as defined
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::GetPrinterAccess()
{
	return m_PrinterAccess;
}


//************************************************************
//	Object:	CWin32Printer
//	Method:	AddAPrinter
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Server name, printer name
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Add a local printer
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::AddAPrinter (LPCSTR szServerName, LPCSTR szPrinterName, LPCSTR szPortName, LPCSTR szDriverName, LPCSTR szPrintProcessor)
{
	return ERR_NOT_IMPLEMENTED;
}


//************************************************************
//	Object:	CWin32Printer
//	Method:	DeleteAPrinter
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Printer name
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
long CWin32Printer::DeleteAPrinter (LPCSTR szPrinterName)
{
	return ERR_NOT_IMPLEMENTED;
}


//************************************************************
//	Object:	CWin32Printer
//	Method:	AddAPrinterConnection
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Server name, printer share name
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
long CWin32Printer::AddAPrinterConnection (LPCSTR szServerName, LPCSTR szPrinterShareName)
{
	return ERR_NOT_IMPLEMENTED;
}


//************************************************************
//	Object:	CWin32Printer
//	Method:	DeleteAPrinterConnection
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Server name, printer name
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Delete a NET printer connection (NT only)
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::DeleteAPrinterConnection (LPCSTR szServerName, LPCSTR szPrinterName)
{
	return ERR_NOT_IMPLEMENTED;
}


//************************************************************
//	Object:	CWin32Printer
//	Method:	GetExtendedErrorCode
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	
//	Return:	Extended error code
//
//	Desc  :	Windows APIs set an error code when fail. We store
//			it in m_iExtendedErrorAttribute and usually return
//			a simple error code to indicate failure. This one
//			gives your more info about the error.
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
DWORD CWin32Printer::GetExtendedErrorCode()
{
	return m_iExtendedErrorAttribute;
}


//************************************************************
//	Object:	CWin32Printer
//	Method:	GetExtendedErrorMessage
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Get error message according to m_iExtendedErrorAttribute
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
void CWin32Printer::GetExtendedErrorMessage (LPSTR szErrorMsg)
{
	LPVOID lpMsgBuf;
 
	FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
					NULL,m_iExtendedErrorAttribute,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf, 0, NULL );

	// return it to caller
	strcpy(szErrorMsg, (const char *)lpMsgBuf);
	
	// Free the buffer.
	::LocalFree( lpMsgBuf );

}


//************************************************************
//	Object:	CWin32Printer
//	Method:	AddAPrinterDriver
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Server name, etc.
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Add a driver on the fly
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::AddAPrinterDriver (LPCSTR lpszServer, LPCSTR lpszPName, LPCSTR lpszEnvironment, LPCSTR lpszDriverPath, LPCSTR lpszDataFile, LPCSTR lpszConfigFile, DWORD nVersion)
{
	DRIVER_INFO_2	di2;
	long				lRet = NO_ERR;
	BOOL			bRet;

	ZeroMemory(&di2, sizeof(DRIVER_INFO_2));

	di2.cVersion		= nVersion;
	di2.pName			= (LPSTR)lpszPName;
	di2.pEnvironment	= (LPSTR)lpszEnvironment;
	di2.pDriverPath		= (LPSTR)lpszDriverPath;
	di2.pDataFile		= (LPSTR)lpszDataFile;
	di2.pConfigFile		= (LPSTR)lpszConfigFile;

	bRet = AddPrinterDriver((LPSTR)lpszServer, 2, (LPBYTE)&di2);
	if (bRet == FALSE) {
		m_iExtendedErrorAttribute  = GetLastError();
		lRet = ERR_ADD_DRIVER;
	}
	else
		m_iExtendedErrorAttribute  = 0;	// no error

	return lRet;
}


//************************************************************
//	Object:	CWin32Printer
//	Method:	DeleteAPrinterDriver
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Printer name, etc.
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Delete a printer driver -- NOT the files
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::DeleteAPrinterDriver (LPCSTR pName, LPCSTR pEnvironment, LPCSTR pDriverName)
{
	long				lRet = NO_ERR;
	BOOL			bRet;

	bRet = DeletePrinterDriver((LPSTR)pName, (LPSTR)pEnvironment, (LPSTR)pDriverName);
	if (bRet == FALSE) {
		m_iExtendedErrorAttribute  = GetLastError();
		lRet = ERR_DEL_DRIVER;
	}
	else
		m_iExtendedErrorAttribute  = 0;	// no error

	return lRet;

}


//************************************************************
//	Object:	CWin32Printer
//	Method:	GetPrinterDriver
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	printer name, port by ref
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Get the driver name given a printer name
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::GetPrinterDriver (LPCSTR szPrinterName, DRIVER_INFO_2* pDriverInfo)
{
  HANDLE        hPrinter;
  DWORD         dwBytesNeeded;
  DRIVER_INFO_1 *pDriverInfo1;
  DRIVER_INFO_2 *pDriverInfo2;
  char          pEnvironment[BUFSIZE] = "";
  long			lRet = NO_ERR;


  // assume no errors
   m_iExtendedErrorAttribute = 0;

  //
  // open selected printer & alloc buffers & get sundry info, close printer
  //
  ::OpenPrinter ((LPSTR)szPrinterName, &hPrinter, NULL);

  if (!hPrinter)
  {
	  m_iExtendedErrorAttribute  = ::GetLastError();	
    lRet = ERR_PRN_OPEN;
    goto display_prt_drv_info_done1;
  }

  ::GetPrinterDriver (hPrinter, pEnvironment, 1, NULL, 0, &dwBytesNeeded);

  //
  // simple error checking, if these work assume rest will too
  //

  if (!(pDriverInfo1 = (DRIVER_INFO_1 *) ::LocalAlloc (LPTR, dwBytesNeeded)))
  {
    m_iExtendedErrorAttribute  = ::GetLastError();	
    lRet = ERR_LOCAL_ALLOC;
    goto display_prt_drv_info_done1;
  }

  if (!::GetPrinterDriver (hPrinter, pEnvironment, 1, (LPBYTE) pDriverInfo1,
                         dwBytesNeeded, &dwBytesNeeded))
  {
	m_iExtendedErrorAttribute  = ::GetLastError();	
    lRet = ERR_PRN_GET;
    goto display_prt_drv_info_done2;
  }

  ::GetPrinterDriver (hPrinter, pEnvironment, 2, NULL, 0, &dwBytesNeeded);
  pDriverInfo2 = (DRIVER_INFO_2 *) ::LocalAlloc (LPTR, dwBytesNeeded);
  ::GetPrinterDriver (hPrinter, pEnvironment, 2, (LPBYTE) pDriverInfo2,
                    dwBytesNeeded, &dwBytesNeeded);

  ::ClosePrinter (hPrinter);

  pDriverInfo->cVersion = pDriverInfo2->cVersion;

  strcpy(pDriverInfo->pName,			pDriverInfo2->pName);
  strcpy(pDriverInfo->pEnvironment,		pDriverInfo2->pEnvironment);
  strcpy(pDriverInfo->pDriverPath,		pDriverInfo2->pDriverPath);
  strcpy(pDriverInfo->pDataFile,		pDriverInfo2->pDataFile);
  strcpy(pDriverInfo->pConfigFile,		pDriverInfo2->pConfigFile);
  
  ::LocalFree (LocalHandle (pDriverInfo2));

display_prt_drv_info_done2:

  ::LocalFree (LocalHandle (pDriverInfo1));

display_prt_drv_info_done1:

  return lRet;
}


//************************************************************
//	Object:	CWin32Printer
//	Method:	GetPrinterDrivers
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	DRIVER_INFO2 pointer
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Get all drivers installed
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::GetPrinterDrivers (DRIVER_INFO_2* pDriverInfo, DWORD iMaxDrivers, short &Drivers)
{
  DWORD         dwBytesNeeded, dwDrvRet, i;
  DRIVER_INFO_1 *pDriverInfo1;
  DRIVER_INFO_2 *pDriverInfo2;
  long			lRet = NO_ERR;

  Drivers = -1;
  //
  // get byte count needed for buffer, alloc buffer, the enum the drivers
  //

  ::EnumPrinterDrivers ((LPTSTR) NULL, (LPTSTR) NULL, 1, NULL,
                      0, &dwBytesNeeded, &dwDrvRet);

  //
  // simple error checking, if these work assume rest will too
  //

  if (!(pDriverInfo1 = (DRIVER_INFO_1 *) ::LocalAlloc (LPTR, dwBytesNeeded)))
  {
    m_iExtendedErrorAttribute  = ::GetLastError();	
    lRet = ERR_LOCAL_ALLOC;
    goto display_prt_drvs_info_done1;
  }

  if (!::EnumPrinterDrivers ((LPTSTR) NULL, (LPTSTR) NULL, 1,
                           (LPBYTE) pDriverInfo1, dwBytesNeeded, &dwBytesNeeded,
                           &dwDrvRet))
  {
    m_iExtendedErrorAttribute  = ::GetLastError();	
    lRet = ERR_ENUM_NO_DRV;
    goto display_prt_drvs_info_done2;
  }

  ::EnumPrinterDrivers ((LPTSTR) NULL,(LPTSTR) NULL, 2, NULL,
                      0, &dwBytesNeeded, &dwDrvRet);

  pDriverInfo2 = (DRIVER_INFO_2 *) ::LocalAlloc (LPTR, dwBytesNeeded);

  ::EnumPrinterDrivers ((LPTSTR) NULL, (LPTSTR) NULL, 2,
                      (LPBYTE) pDriverInfo2, dwBytesNeeded, &dwBytesNeeded,
                      &dwDrvRet);

  if (!dwDrvRet)
  {
    m_iExtendedErrorAttribute  = ::GetLastError();	
    lRet = ERR_ENUM_NO_DRV;
    goto display_prt_drvs_info_done3;
  }

  // return the number of Drivers found
  Drivers = (short)dwDrvRet;
  //
  // insert formatted info into listbox
  //

  for (i = 0; i < dwDrvRet; i++)
  {
	    
	(pDriverInfo + i)->cVersion = (pDriverInfo2 + i)->cVersion;

	strcpy((pDriverInfo + i)->pName,			(pDriverInfo2 + i)->pName);
	strcpy((pDriverInfo + i)->pEnvironment,	(pDriverInfo2 + i)->pEnvironment);
	strcpy((pDriverInfo + i)->pDriverPath,	(pDriverInfo2 + i)->pDriverPath);
	strcpy((pDriverInfo + i)->pDataFile,		(pDriverInfo2 + i)->pDataFile);
	strcpy((pDriverInfo + i)->pConfigFile,	(pDriverInfo2 + i)->pConfigFile);

  }

display_prt_drvs_info_done3:

  ::LocalFree (LocalHandle (pDriverInfo2));

display_prt_drvs_info_done2:

  ::LocalFree (LocalHandle (pDriverInfo1));

display_prt_drvs_info_done1:

  return lRet;
}

//************************************************************
//	Object:	CWin32Printer
//	Method:	EnumPrinterJobs
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Printer name, max # of Jobs, JOB_INFO1 pointer
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Enumerate print jobs for a printer
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::EnumPrinterJobs (LPCSTR szPrinterName, long NoJobs, JOB_INFO_1* pInfo1, long &Jobs)
{

	long			lRet = NO_ERR;
	HANDLE			hPrinter;
	PRINTER_DEFAULTS pd;
    DWORD			dwNeeded;		// addr. of variable with no. of bytes copied (or required) 
    DWORD			dwReturned; 	// addr. of variable with no. of job info. structures copied 
	DWORD			i;
	JOB_INFO_1      *pJobInfo;

	// assume no errors
	m_iExtendedErrorAttribute = 0;
	Jobs = -1;

	if (NoJobs <= 0) {
		lRet = ERR_NOTHING_2DO;
		goto EnumPrinterJobs_done1;
	}

	//
	// open selected printer & alloc buffers & get sundry info, close printer
	//
	ZeroMemory(&pd, sizeof(pd));
	pd.DesiredAccess = m_PrinterAccess;

	::OpenPrinter ((LPSTR)szPrinterName, &hPrinter, &pd);

	if (!hPrinter)
	{
		m_iExtendedErrorAttribute  = ::GetLastError();	
		lRet = ERR_PRN_OPEN;
		goto EnumPrinterJobs_done1;
	}

     // First you call EnumJobs() to find out how much memory you need
     if( ! EnumJobs( hPrinter, 0, (DWORD)NoJobs, 1, NULL, 0, &dwNeeded,
                     &dwReturned ) )
     {
       // It should have failed, but if it failed for any reason other
       // than "not enough memory", you should bail out
       if( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
       {
		 m_iExtendedErrorAttribute  = ::GetLastError();
         ClosePrinter( hPrinter );
         return ERR_FATAL_ERROR;
	   }
     }
     
     // Allocate enough memory for the JOB_INFO_1 structures plus
     // the extra data - dwNeeded from the previous call tells you
     // the total size needed
     if( (pJobInfo = (JOB_INFO_1 *)malloc( dwNeeded )) == NULL )
     {
	   m_iExtendedErrorAttribute  = ::GetLastError();
       ClosePrinter( hPrinter );
       return ERR_LOCAL_ALLOC;
     }
     // Call EnumJobs() again and let it fill out our structures
     if( ! EnumJobs( hPrinter, 0, 0xFFFFFFFF, 1, (LPBYTE)pJobInfo,
                     dwNeeded, &dwNeeded, &dwReturned ) )
     {
       m_iExtendedErrorAttribute  = ::GetLastError();
       ClosePrinter( hPrinter );
       free( pJobInfo );
       return ERR_ENUM_JOBS1;
     }

	// we made it here
	Jobs = dwReturned;

	// It could be that there is nothing in the queue
	if (dwNeeded == 0) {	
		lRet = NO_ERR;
		goto EnumPrinterJobs_done2;
	}

     // It's easy to loop through the jobs and access each one

	for(i=0; i<dwReturned; i++)
	{
		// up to NoJobs only !
		if (i < (DWORD)NoJobs) {
			pInfo1[i].JobId = pJobInfo[i].JobId;
			strcpy(pInfo1[i].pPrinterName,	pJobInfo[i].pPrinterName);
			strcpy(pInfo1[i].pMachineName,	pJobInfo[i].pMachineName);
			strcpy(pInfo1[i].pUserName,		pJobInfo[i].pUserName);
			strcpy(pInfo1[i].pDocument,		pJobInfo[i].pDocument);
			strcpy(pInfo1[i].pDatatype,		pJobInfo[i].pDatatype);

			// Get a GPF by PB when pStatus = 0 !
			if (pJobInfo[i].pStatus != 0)
				strcpy(pInfo1[i].pStatus, pJobInfo[i].pStatus);
			else
				strcpy(pInfo1[i].pStatus, "\0");

			pInfo1[i].Status		= pJobInfo[i].Status;
			pInfo1[i].Priority		= pJobInfo[i].Priority;
			pInfo1[i].Position		= pJobInfo[i].Position;
			pInfo1[i].TotalPages	= pJobInfo[i].TotalPages;
			pInfo1[i].PagesPrinted	= pJobInfo[i].PagesPrinted;
			pInfo1[i].Submitted		= pJobInfo[i].Submitted;

		}	// end if < NoJobs
	}		// End for


    // Clean up
	free( pJobInfo );

EnumPrinterJobs_done2:
	
	if (hPrinter != NULL)
		ClosePrinter(hPrinter);
 
EnumPrinterJobs_done1:

	return lRet;

}


// I am working on it !!

//************************************************************
//	Object:	CWin32Printer
//	Method:	SetPrinterJob
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	
//	Return:	NO_ERR if Ok	
//
//	Desc  :	
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::SetPrinterJob(LPCSTR szPrinterName, DWORD nJobId, DWORD nCommand)
{
	return ERR_NOT_IMPLEMENTED;
}


//************************************************************
//	Object:	CWin32Printer
//	Method:	SetPrinterFont
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Printer DC, font info
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Set a font at the printer level -- not perfect yet
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::SetPrinterFont(HDC pDC, LPCSTR szFontFaceName, short iPoint, short iBold, short iItalic, short iUnderline, short iStrike)
{
	long	lRet = NO_ERR;
	long		iWeight;
	LOGFONT	lf;
	HFONT	hFont;
	HGDIOBJ	hOldObject;

	// check pointer
	if (szFontFaceName == NULL)
		return ERR_NULL_POINTER;

	// Check buffer size
	if (strlen(szFontFaceName) > 31)
		return ERR_BUFFER_2SMALL;

    // Did you get a good DC?
    if( !pDC)
        return ERR_BAD_DC;
    
	// Make sure that there's no rubbish
	ZeroMemory(&lf, sizeof(LOGFONT));

	// Set up LOGFONT structure
	lf.lfHeight		= -MulDiv(iPoint, GetDeviceCaps(pDC, LOGPIXELSY), 72);
	lf.lfWidth		= 0;

	// Calculate weight
	if (iBold == 1)
		iWeight = 700;
	else
		iWeight = 400;

    lf.lfWeight		= (BYTE)iWeight;
    lf.lfItalic		= (BYTE)iItalic;
    lf.lfUnderline	= (BYTE)iUnderline;
    lf.lfStrikeOut	= (BYTE)iStrike;
	strcpy(lf.lfFaceName, szFontFaceName);

//Windows then matches the logical font with a real font. You can determine the name of the typeface by using the function:
//GetTextFace (hdc, sizeof szFaceName, szFaceName) ;
//where szFaceName is a character array to receive the name. 
//You can have Windows copy the various sizes of the font into a structure of type TEXTMETRIC using the familiar:
//GetTextMetrics (hdc, &tm) ;

//You can delete a logical font (but not while it is selected into a device context) with DeleteObject:
//DeleteObject (hFont) ;
//It's easiest to unselect the logical font from the device context by selecting a stock font into the device context.

	hFont = CreateFontIndirect(&lf);

//	if (hFont == (HFONT)NULL);
//		return ERR_CREATE_FONT;

	hOldObject = SelectObject(pDC, hFont);

	if ( (hOldObject == (HGDIOBJ)NULL) || (hOldObject ==  (HGDIOBJ)GDI_ERROR) )
		lRet = -11;	// temp TEST 
	else
		lRet = (long)hOldObject;
/*
	
	DOCINFO di = {sizeof (DOCINFO), "PowerPrinter test", NULL };
	char szText1[] = "Hello, Printer (1) !";
	char szText2[] = "Hello, Printer (2) !";

	if (StartDoc(pDC, &di) > 0) {
		StartPage(pDC);

		hOldObject = SelectObject(pDC, hFont);

		if ( (hOldObject == (HGDIOBJ)NULL) || (hOldObject ==  (HGDIOBJ)GDI_ERROR) )
			lRet = -11;	// temp TEST 
		else
			lRet = (long)hOldObject;

		TextOut(pDC, 50, 50, szText1, sizeof(szText1) -1);
		SelectObject(pDC, GetStockObject(SYSTEM_FONT));
		TextOut(pDC, 100, 100, szText2, sizeof(szText2) -1);
		EndPage(pDC);
		EndDoc(pDC);
	}
*/	
	return lRet;
}

// simple callback prototype to *please* the compiler
long CALLBACK GetNextFont(ENUMLOGFONT FAR *lpelf, NEWTEXTMETRIC FAR *lpntm, long FontType, LPARAM lParam);


//************************************************************
//	Object:	CWin32Printer
//	Method:	GetPrinterFont
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Font list string pointer, by ref
//	Return:	Number of fonts found	
//
//	Desc  :	Get all fonts installed. Some fonts that belong to 
//			more than one category may appear more than once
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::GetPrinterFonts(LPSTR szFonts)
{
	giFontCount = 0;

    HDC        pDC;
 
    // Need a printer DC to print to
    pDC = GetDefaultPrinterDC();
 
    // Did you get a good DC?
    if( !pDC)
    {
        MessageBox(NULL, "Error creating DC", "Error",
                                    MB_APPLMODAL | MB_OK );
        return -1;
    }
 
	ZeroMemory(szFonts, strlen(szFonts));

	// Use GetNextFont CALLBACK function
	while (EnumFontFamilies(pDC, NULL, (FONTENUMPROC)GetNextFont, (LPARAM)szFonts));
		
	DeleteDC(pDC);

	return giFontCount;
}

//************************************************************
//	Object:	CWin32Printer
//	Method:	GetNextFont
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Callback function. Called once per font. Just
//			Get the font face name
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CALLBACK GetNextFont(ENUMLOGFONT FAR *lpelf, NEWTEXTMETRIC FAR *lpntm, long FontType, LPARAM lParam)
{

      static char szFirstFont[LF_FACESIZE + 1];
      char szFaceName[LF_FACESIZE + 1];
	  LOGFONT	LogFont;
	  
      szFaceName[0] = '\0';
 
	  // get pointer to log font
	  LogFont = lpelf->elfLogFont;

	  // Get face name
      lstrcpyn((LPSTR) szFaceName, LogFont.lfFaceName, LF_FACESIZE - 1);
	  szFaceName[LF_FACESIZE] = '\0';
 
      
	  if (giFontCount == 0)
 
		//Store the first font retrieved.  If we see this font
        //again, we know we've enumerated all the fonts
        lstrcpy((LPSTR) szFirstFont, szFaceName);
 
      else if (!lstrcmp(szFirstFont, szFaceName))
         //If we see the same face name again, get out and stop
         //enumerating
      return FALSE;

	  // Add font to the fonts string
	  lstrcat((LPSTR) lParam, szFaceName);	
	  lstrcat((LPSTR) lParam, ";");
	  giFontCount++;
 
      return TRUE;
}


//////////////////////////////////////////////////////////////////
//																//
//					POWERDIB Interface							//
//																//
//////////////////////////////////////////////////////////////////
	
//************************************************************
//	Object:	CWin32Printer
//	Method:	CaptureDesktop
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Capture desktop to Device Independent Bitmap (DIB)
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::CaptureDesktop()
{
    HBITMAP     hBitmap;       // Handle to our temporary bitmap
    HPALETTE    hPal;          // Handle to our palette
    RECT		rScreen;       // Rect containing entire screen coordinates
    HDC			hDC;           // DC to screen
    MSG			msg;           // Message for the PeekMessage()
	LPCSTB		CopyScreenToBitmapPn;
	LPGSP		GetSystemPalettePn;
    long		iRet = NO_ERR;

    // Copy Entire screen to DIB
    hDC				= ::CreateDC("DISPLAY", NULL, NULL, NULL);
    rScreen.left	= rScreen.top = 0;
    rScreen.right	= ::GetDeviceCaps(hDC, HORZRES);
    rScreen.bottom	= ::GetDeviceCaps(hDC, VERTRES);

    // Delete our DC
    DeleteDC(hDC);

    // Wait until everybody repaints themselves

    while (::PeekMessage(&msg,NULL,0,0,PM_REMOVE) != 0)
    {
		::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }

	// try to load PowerDib if necessary
	if (m_hPowerDIB == NULL)
		m_hPowerDIB = LoadLibrary("powerdib.dll");

	// if PowerDIB has been successfully loaded
	if (m_hPowerDIB != NULL) {

		CopyScreenToBitmapPn	= (LPCSTB)GetProcAddress(m_hPowerDIB, "CopyScreenToBitmap");
		GetSystemPalettePn		= (LPGSP)GetProcAddress(m_hPowerDIB, "GetSystemPalette");

		// did we get the function address ?
		if ((CopyScreenToBitmapPn != NULL) && 
			(GetSystemPalettePn != NULL)) {		

			hBitmap = CopyScreenToBitmapPn(&rScreen);

			if (hBitmap) 
				hPal = GetSystemPalettePn();
			else
				hPal = NULL;
        
			if (hBitmap)
			{
				m_hBitmap = hBitmap;
				hBitmap = NULL;
			}

			if (hPal)
			{
				m_hPal = hPal;
				hPal = NULL;
			}
		}
		else {
			m_iExtendedErrorAttribute  = GetLastError();
			iRet = ERR_NOLOAD_FUNCTION;
		}
	
	}
	else {
		m_iExtendedErrorAttribute  = GetLastError();
		iRet = ERR_NOLOAD_POWERDIB;
	}

	return iRet;
}


//************************************************************
//	Object:	CWin32Printer
//	Method:	PrintTheDIB
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	print option, job title
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Print DIB in memory (captured) 
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::PrintTheDIB(long iOption, LPSTR szText)
{

	HDIB        hDIB;
    WORD        wReturn;
	LPBTDIB		BitmapToDIBPn;
	LPPDIB		PrintDIBPn;
	LPDDIB		DestroyDIBPn;
	LPDIBE		DIBErrorPn;
	long		iRet = NO_ERR;

	// try to load PowerDib if necessary
	if (m_hPowerDIB == NULL)
		m_hPowerDIB = LoadLibrary("powerdib.dll");

	// if PowerDIB has been successfully loaded
	if (m_hPowerDIB != NULL) {

		// Get function pointers
		BitmapToDIBPn	= (LPBTDIB)GetProcAddress(m_hPowerDIB, "BitmapToDIB");
		PrintDIBPn		= (LPPDIB)GetProcAddress(m_hPowerDIB, "PrintDIB");
		DestroyDIBPn	= (LPDDIB)GetProcAddress(m_hPowerDIB, "DestroyDIB");
		DIBErrorPn		= (LPDIBE)GetProcAddress(m_hPowerDIB, "DIBError");

		// must get all 3
		if ((BitmapToDIBPn != NULL) && (PrintDIBPn != NULL) &&
			(DestroyDIBPn != NULL) && (DIBErrorPn != NULL)) {

			// transform it to a DIB
			hDIB = BitmapToDIBPn(m_hBitmap, m_hPal);

			if (hDIB) {

				// Print the dib using PrintDIB() API
				if (iOption == IDC_STRETCHTOPAGE)
				wReturn = PrintDIBPn(hDIB, PW_STRETCHTOPAGE, 0, 0, szText);

				/*
				else if (iOption == IDC_SCALE)
					wReturn = PrintDIBPn(hDIB, (WORD)PW_SCALE, (WORD)opts.iXScale,
							(WORD)opts.iYScale, (LPSTR)gszWindowText);
				*/

				else
					wReturn = PrintDIBPn(hDIB, PW_BESTFIT, 0, 0, szText);
		
				if (wReturn)
					DIBErrorPn(wReturn);

				DestroyDIBPn(hDIB);
			}	// end if HDIB

		}	// end if Pn != NULL
		else {
			m_iExtendedErrorAttribute  = GetLastError();
			iRet = ERR_NOLOAD_FUNCTION;
		}
	
	}
	else {
		m_iExtendedErrorAttribute  = GetLastError();
		iRet = ERR_NOLOAD_POWERDIB;
	}

	return iRet;

}


//************************************************************
//	Object:	CWin32Printer
//	Method:	CaptureWindow
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Window HANDLE, capture option
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Capture a window given its handle
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWin32Printer::CaptureWindow(HWND hWin, long fPrintArea)
{
    HBITMAP     hBitmap;       // Handle to our temporary bitmap
    HPALETTE    hPal;          // Handle to our palette
	LPCWBMP		CaptureWindowPn;
	LPDIBE		DIBErrorPn;
	LPGSP		GetSystemPalettePn;
	long		iRet = NO_ERR;

	// try to load PowerDib if necessary
	if (m_hPowerDIB == NULL)
		m_hPowerDIB = LoadLibrary("powerdib.dll");

	// if PowerDIB has been successfully loaded
	if (m_hPowerDIB != NULL) {

		// Get function pointers
		CaptureWindowPn		= (LPCWBMP)GetProcAddress(m_hPowerDIB, "CopyWindowToBitmap");
		DIBErrorPn			= (LPDIBE)GetProcAddress(m_hPowerDIB, "DIBError");
		GetSystemPalettePn	= (LPGSP)GetProcAddress(m_hPowerDIB, "GetSystemPalette");

		// must get all 2
		if ((CaptureWindowPn != NULL) && (DIBErrorPn != NULL)) {

		
			hBitmap = CaptureWindowPn(hWin, (WORD)fPrintArea);

			if (hBitmap) 
				hPal = GetSystemPalettePn();
			else {
				hPal = NULL;
				iRet = ERR_COPY_WINDOW;
			}
        
			if (hBitmap)
			{
				m_hBitmap = hBitmap;
				hBitmap = NULL;
			}

			if (hPal)
			{
				m_hPal = hPal;
				hPal = NULL;
			}
			
				

		}	// end if Pn != NULL
		else {
			m_iExtendedErrorAttribute  = GetLastError();
			iRet = ERR_NOLOAD_FUNCTION;
		}
	
	}
	else {
		m_iExtendedErrorAttribute  = GetLastError();
		iRet = ERR_NOLOAD_POWERDIB;
	}

	return iRet;
}

long CWin32Printer::GetPrinterStatus(LPCSTR szPrinterName, DWORD &dwPrinterStatus)
{

	JOB_INFO_2  *pJobs;       
	int         cJobs;
	BOOL		bFlag;
	HANDLE		hPrinter = NULL;
	PRINTER_DEFAULTS pd;
	long	lRet = NO_ERR;
	char	szPrinterPort[256], 
			szPrinterDriver[256];

	memset(szPrinterDriver,	0, sizeof(szPrinterDriver));
	memset(szPrinterPort,	0, sizeof(szPrinterPort));
	

	// get other info
	GetPrinterPort(szPrinterName, szPrinterPort);
	GetPrinterDriver(szPrinterName, szPrinterDriver);
 
	ZeroMemory(&pd, sizeof(pd));
	pd.DesiredAccess = m_PrinterAccess;

	bFlag = OpenPrinter((LPSTR)szPrinterName, &hPrinter, &pd);
	if (!bFlag || (hPrinter== NULL)) {
		lRet = ERR_PRN_OPEN;
		m_iExtendedErrorAttribute  = GetLastError();
		return lRet;
	}


	/*
     *  Get the state information for the Printer 
	 */
	
	if (!GetJobs(hPrinter, &pJobs, &cJobs, &dwPrinterStatus)) {
		m_iExtendedErrorAttribute  = GetLastError();
		lRet = ERR_GET_JOBS;     
	}
	else
		lRet = NO_ERR;

	// Don't need it anymore
	if (hPrinter != NULL)
		ClosePrinter(hPrinter);

	return lRet;
}

BOOL CWin32Printer::GetJobs(HANDLE hPrinter, JOB_INFO_2 * * ppJobInfo, int * pcJobs, DWORD * pStatus)
{
	DWORD			cByteNeeded, nReturned, cByteUsed, cLastErr;
    JOB_INFO_2		*pJobStorage = NULL;
    PRINTER_INFO_2	*pPrinterInfo = NULL;

	/* Get the buffer size needed */
	if (!GetPrinter(hPrinter, 2, NULL, 0, &cByteNeeded)) {
		cLastErr = GetLastError();

		// this error is normal. Fail if another one occurs
		if (cLastErr != ERROR_INSUFFICIENT_BUFFER) {
			m_iExtendedErrorAttribute = cLastErr;
            return FALSE;       
		}
	}
    
	pPrinterInfo = (PRINTER_INFO_2 *)malloc(cByteNeeded);
    if (!(pPrinterInfo))           /* failure to allocate memory */
		return FALSE;       /* get the printer info */

    if (!GetPrinter(hPrinter, 2, (LPBYTE)pPrinterInfo, cByteNeeded, &cByteUsed)) {
		/* failure to access the printer */           
		free(pPrinterInfo);
        pPrinterInfo = NULL;           
		return FALSE;       
	}
    
    /* Get job storage space */       
	if (!EnumJobs(	hPrinter, 0, pPrinterInfo->cJobs, 2,
					NULL, 0, (LPDWORD)&cByteNeeded, (LPDWORD)&nReturned)) {

        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
			free(pPrinterInfo);               
			pPrinterInfo = NULL;
            return FALSE;           
		}       
	}
    
	pJobStorage = (JOB_INFO_2 *)malloc(cByteNeeded);       
	if (!pJobStorage) {           
		/* failure to allocate Job storage space */
        free(pPrinterInfo);           
		pPrinterInfo = NULL;
        return FALSE;       
	}       
	
	ZeroMemory(pJobStorage, cByteNeeded);
    
	/* get the list of jobs */       
	if (!EnumJobs(hPrinter, 0, pPrinterInfo->cJobs, 2,
               (LPBYTE)pJobStorage, cByteNeeded, (LPDWORD)&cByteUsed,
			   (LPDWORD)&nReturned))       {

		free(pPrinterInfo);           
		free(pJobStorage);
        pJobStorage = NULL;           
		pPrinterInfo = NULL;
        return FALSE;       
	}       
	
	/*        
	 *  Return the information
     */       
	*pcJobs = pPrinterInfo->cJobs;
    *pStatus = pPrinterInfo->Status;       
	*ppJobInfo = pJobStorage;
    free(pPrinterInfo);       

	return TRUE;
}

long CWin32Printer::GetPrinterDriver(LPCSTR szPrinterName, LPSTR szDriverName)
{
	return ERR_NOT_IMPLEMENTED;
}

long CWin32Printer::SetJobProperties(LPCSTR szPrinterName, DWORD dwJobID, DWORD dwCommand, DWORD dwPriority)
{
	JOB_INFO_1  *pJob;       
	DWORD		cBytesNeeded, nReturned, cLastErr;
	BOOL		bFlag;
	HANDLE		hPrinter = NULL;
	PRINTER_DEFAULTS pd;
	long	lRet = NO_ERR;
	char	szPrinterPort[256], 
			szPrinterDriver[256];

	memset(szPrinterDriver,	0, sizeof(szPrinterDriver));
	memset(szPrinterPort,	0, sizeof(szPrinterPort));
	

	// get other info
	GetPrinterPort(szPrinterName, szPrinterPort);
	GetPrinterDriver(szPrinterName, szPrinterDriver);
 
	ZeroMemory(&pd, sizeof(pd));
	pd.DesiredAccess = m_PrinterAccess;

	bFlag = OpenPrinter((LPSTR)szPrinterName, &hPrinter, &pd);
	if (!bFlag || (hPrinter== NULL)) {
		lRet = ERR_PRN_OPEN;
		m_iExtendedErrorAttribute  = GetLastError();
		return lRet;
	}

	// Get number of bytes needed to be allocated to hold JOB_INFO_1
	bFlag = GetJob(hPrinter, dwJobID, 1, 0, 0, &cBytesNeeded); 
	if (!bFlag) {
		cLastErr = GetLastError();

		// this error is normal. Fail if another one occurs
		if (cLastErr != ERROR_INSUFFICIENT_BUFFER) {
			m_iExtendedErrorAttribute = cLastErr;
            return ERR_GET_JOB;       
		}
	}

	pJob = (JOB_INFO_1 *)malloc(cBytesNeeded);
    if (!(pJob))						/* failure to allocate memory */
		return ERR_LOCAL_ALLOC;			/* get the printer info */


	// Now get the info
	bFlag = GetJob(hPrinter, dwJobID, 1, (LPBYTE)pJob, cBytesNeeded, &nReturned); 
	if (!bFlag) {
		m_iExtendedErrorAttribute = GetLastError();
		return ERR_GET_JOB;
	}

	// Set the priority
	pJob->Priority = dwPriority;

	// Make the call !
	bFlag = SetJob(hPrinter, dwJobID, 1, (LPBYTE)pJob, dwCommand);
	if (!bFlag) {
		m_iExtendedErrorAttribute = GetLastError();
		return ERR_SET_JOB;
	}

	// free mem
	if (pJob)
		free(pJob); 

	// Don't need it anymore
	if (hPrinter != NULL)
		ClosePrinter(hPrinter);

	return lRet;

}

typedef struct _JOB_INFO_1_BSTR {    // ji1     
	DWORD  JobId; 
    BSTR pPrinterName;     
	BSTR pMachineName;     
	BSTR pUserName; 
    BSTR pDocument;     
	BSTR pDatatype;     
	BSTR pStatus; 
    DWORD  Status;     
	DWORD  Priority;     
	DWORD  Position; 
    DWORD  TotalPages;     
	DWORD  PagesPrinted;     
	SYSTEMTIME Submitted; 
} JOB_INFO_1_BSTR; 

long CWin32Printer::EnumPrinterJobsVB(LPCSTR szPrinterName, FPSAFEARRAY * pInfo1, long & Jobs)
{
	long			lRet = NO_ERR;
	HANDLE			hPrinter;
	PRINTER_DEFAULTS pd;
    DWORD			dwNeeded;		// addr. of variable with no. of bytes copied (or required) 
    DWORD			dwReturned; 	// addr. of variable with no. of job info. structures copied 
	DWORD			i;
	JOB_INFO_1		*pJobInfo;		// result of EnumJobs
	JOB_INFO_1_BSTR	*pJobInfoBSTR;	
	SAFEARRAY		*psa;
	int				cchWideChar = 100;
	WCHAR			wcTemp[100];
	size_t			iBytesWritten, iStrLen;
	
	// assume no errors
	m_iExtendedErrorAttribute = 0;
	Jobs = -1;

	//
	// open selected printer & alloc buffers & get sundry info, close printer
	//
	ZeroMemory(&pd, sizeof(pd));
	pd.DesiredAccess = m_PrinterAccess;

	::OpenPrinter ((LPSTR)szPrinterName, &hPrinter, &pd);

	if (!hPrinter)
	{
		m_iExtendedErrorAttribute  = ::GetLastError();	
		lRet = ERR_PRN_OPEN;
		goto EnumPrinterJobs_done1;
	}

     // First you call EnumJobs() to find out how much memory you need
     if( ! EnumJobs( hPrinter, 0, (DWORD)100, 1, NULL, 0, &dwNeeded,
                     &dwReturned ) )
     {
       // It should have failed, but if it failed for any reason other
       // than "not enough memory", you should bail out
       if( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
       {
		 m_iExtendedErrorAttribute  = ::GetLastError();
         ClosePrinter( hPrinter );
         return ERR_FATAL_ERROR;
	   }
     }
     
     // Allocate enough memory for the JOB_INFO_1 structures plus
     // the extra data - dwNeeded from the previous call tells you
     // the total size needed
     if( (pJobInfo = (JOB_INFO_1 *)malloc( dwNeeded )) == NULL )
     {
	   m_iExtendedErrorAttribute  = ::GetLastError();
       ClosePrinter( hPrinter );
       return ERR_LOCAL_ALLOC;
     }
     // Call EnumJobs() again and let it fill out our structures
     if( ! EnumJobs( hPrinter, 0, 0xFFFFFFFF, 1, (LPBYTE)pJobInfo,
                     dwNeeded, &dwNeeded, &dwReturned ) )
     {
       m_iExtendedErrorAttribute  = ::GetLastError();
       ClosePrinter( hPrinter );
       free( pJobInfo );
       return ERR_ENUM_JOBS1;
     }

	// we made it here
	Jobs = dwReturned;

	// It could be that there is nothing in the queue
	if (dwNeeded == 0) {	
		lRet = NO_ERR;
		goto EnumPrinterJobs_done2;
	}

   	if (*pInfo1 == NULL) { // create a new array
  	
		// Create a 'One dimension' OLE array
		if (FAILED(SafeArrayAllocDescriptor(1, &psa))) {
	 		lRet = ERR_SAFEAALLOCDESC;
			goto EnumPrinterJobs_done2;
		}

    	// set up the SAFEARRAY structure
    	// and allocate data space
		psa->fFeatures = 0;
  		psa->cbElements = sizeof(JOB_INFO_1_BSTR);                
  		psa->rgsabound[0].cElements = dwReturned;
  		psa->rgsabound[0].lLbound = 0;
  
  		if (FAILED(SafeArrayAllocData(psa))) {
  			SafeArrayDestroyDescriptor(psa);
			lRet = ERR_SAFEAALLOCDATA; 
			goto EnumPrinterJobs_done2;
		}

		// get a pointer to the new data
        if (FAILED(SafeArrayAccessData(psa, (void **)&pJobInfoBSTR))) {
			SafeArrayDestroy(psa);
			lRet = ERR_SAFEAACCESDATA;
  		}            
	}

	for (i=0; i<dwReturned; i++)
	{
		// Get JOb ID (VIP = Very Important Parameter)
		pJobInfoBSTR[i].JobId = pJobInfo[i].JobId;

		// Convert PrinterName to UNICODE for VB
		memset(wcTemp, 0, cchWideChar);
		iStrLen = strlen(pJobInfo[i].pPrinterName);
		iBytesWritten = MultiByteToWideChar (CP_ACP, MB_PRECOMPOSED, pJobInfo[i].pPrinterName, (int)iStrLen, (LPWSTR )&wcTemp, cchWideChar);
		pJobInfoBSTR[i].pPrinterName = SysAllocStringByteLen((char *)wcTemp, ((UINT)iBytesWritten * 2));	

		// Convert MachineName to UNICODE for VB
		memset(wcTemp, 0, cchWideChar);
		iStrLen = strlen(pJobInfo[i].pMachineName);
		iBytesWritten = MultiByteToWideChar (CP_ACP, MB_PRECOMPOSED, pJobInfo[i].pMachineName, (int)iStrLen, (LPWSTR )&wcTemp, cchWideChar);
		pJobInfoBSTR[i].pMachineName = SysAllocStringByteLen((char *)wcTemp, ((UINT)iBytesWritten * 2));	

		// Convert User Name to UNICODE for VB
		memset(wcTemp, 0, cchWideChar);
		iStrLen = strlen(pJobInfo[i].pUserName);
		iBytesWritten = MultiByteToWideChar (CP_ACP, MB_PRECOMPOSED, pJobInfo[i].pUserName, (int)iStrLen, (LPWSTR )&wcTemp, cchWideChar);
		pJobInfoBSTR[i].pUserName =	SysAllocStringByteLen((char *)wcTemp, ((UINT)iBytesWritten * 2));	

		// Convert Document Name to UNICODE for VB
		memset(wcTemp, 0, cchWideChar);
		iStrLen = strlen(pJobInfo[i].pDocument);
		iBytesWritten = MultiByteToWideChar (CP_ACP, MB_PRECOMPOSED, pJobInfo[i].pDocument, (int)iStrLen, (LPWSTR )&wcTemp, cchWideChar);
		pJobInfoBSTR[i].pDocument =	SysAllocStringByteLen((char *)wcTemp, ((UINT)iBytesWritten * 2));	

		// Convert Datatype to UNICODE for VB
		memset(wcTemp, 0, cchWideChar);
		iStrLen = strlen(pJobInfo[i].pDatatype);
		iBytesWritten = MultiByteToWideChar (CP_ACP, MB_PRECOMPOSED, pJobInfo[i].pDatatype, (int)iStrLen, (LPWSTR )&wcTemp, cchWideChar);
		pJobInfoBSTR[i].pDatatype =	SysAllocStringByteLen((char *)wcTemp, ((UINT)iBytesWritten * 2));	

		// Convert Status to UNICODE for VB (OFTEN NULL !!)
		memset(wcTemp, 0, cchWideChar);
		if (pJobInfo[i].pStatus != 0) {

			iStrLen = strlen(pJobInfo[i].pStatus);
			iBytesWritten = MultiByteToWideChar (CP_ACP, MB_PRECOMPOSED, pJobInfo[i].pStatus, (int)iStrLen, (LPWSTR )&wcTemp, cchWideChar);

			if (iBytesWritten > 0)
				iBytesWritten *= 2;

			pJobInfoBSTR[i].pStatus = SysAllocStringByteLen((char *)wcTemp, (UINT)iBytesWritten);	
		}
		else
			pJobInfoBSTR[i].pStatus = SysAllocStringByteLen((char *)wcTemp, 0);	
	
		// Numeric attributes
		pJobInfoBSTR[i].Status			= pJobInfo[i].Status;
		pJobInfoBSTR[i].Priority		= pJobInfo[i].Priority;
		pJobInfoBSTR[i].Position		= pJobInfo[i].Position;
		pJobInfoBSTR[i].TotalPages		= pJobInfo[i].TotalPages;
		pJobInfoBSTR[i].PagesPrinted	= pJobInfo[i].PagesPrinted;
		pJobInfoBSTR[i].Submitted		= pJobInfo[i].Submitted;
	}		

	if (*pInfo1 == NULL) {
		SafeArrayUnaccessData(psa);
		*pInfo1 = psa;                         
	}
	else 
		SafeArrayUnaccessData(*pInfo1);

    // Clean up
	free( pJobInfo );

EnumPrinterJobs_done2:
	
	if (hPrinter != NULL)
		ClosePrinter(hPrinter);
 
EnumPrinterJobs_done1:

	return lRet;

}

long CWin32Printer::SetDocumentProperties(HWND hWnd)
{
//	HDC         hPrnDC;
	LPDEVMODE   lpDevMode = NULL;
	LPDEVNAMES  lpDevNames;
	LPSTR       lpszDriverName;
	LPSTR       lpszDeviceName;
	LPSTR       lpszPortName;
	PRINTDLG    pd;
	HANDLE      hPrinter;
	int         nDMSize;
	HANDLE      hDevMode;
	NPDEVMODE   npDevMode;
	DEVMODE     DevModeIn;
	long		lRet = NO_ERR, lAnswer, lFlag;

   // Get the defaults without displaying any dialog boxes.

	memset(&pd, 0, sizeof(PRINTDLG));

   pd.Flags = PD_RETURNDEFAULT;
   pd.hDevNames = NULL;
   pd.hDevMode = NULL;
   pd.lStructSize = sizeof(PRINTDLG);
   PrintDlg((LPPRINTDLG)&pd);

   lpDevNames = (LPDEVNAMES)GlobalLock(pd.hDevNames);
   lpszDriverName = (LPSTR)lpDevNames + lpDevNames->wDriverOffset;
   lpszDeviceName = (LPSTR)lpDevNames + lpDevNames->wDeviceOffset;
   lpszPortName   = (LPSTR)lpDevNames + lpDevNames->wOutputOffset;

   OpenPrinter(lpszDeviceName,&hPrinter,NULL);

   // A zero for last param returns the size of buffer needed.

   nDMSize = DocumentProperties(hWnd,hPrinter,lpszDeviceName,NULL,NULL,0);
   if ((nDMSize < 0) || !(hDevMode = LocalAlloc (LHND, nDMSize)))
      return NULL;

   npDevMode = (NPDEVMODE) LocalLock (hDevMode);

   // Fill in the rest of the structure.

   lstrcpy ((char*)DevModeIn.dmDeviceName, lpszDeviceName);
   DevModeIn.dmSpecVersion    = 0x300;
   DevModeIn.dmDriverVersion  = 0;
   DevModeIn.dmSize           = sizeof (DevModeIn);
   DevModeIn.dmDriverExtra    = 0;

   // Display the "Document Properties" dialog box.

   lAnswer = DocumentProperties(hWnd,hPrinter,lpszDeviceName,npDevMode,&DevModeIn,
      DM_IN_PROMPT|DM_OUT_BUFFER|DM_UPDATE );
	
   if (lAnswer == IDOK) {

		lFlag = DocumentProperties(hWnd, hPrinter, lpszDeviceName, npDevMode,
									npDevMode, DM_IN_BUFFER | DM_OUT_BUFFER |DM_UPDATE);      
		if (lFlag) {
		//	lFlag = SetPrinter(hPrinter, 2, (LPBYTE)pi2, 0);      
			if (!lFlag)
				lRet = -1;
		}
		else
			lRet = -2;	
   }

   // Get the printer DC.

/*
   hPrnDC = CreateDC
(lpszDriverName,lpszDeviceName,lpszPortName,(LPSTR)npDevMode); 

*/
   
   LocalUnlock (hDevMode);

   //  Use the printer DC.

   ClosePrinter(hPrinter);

	return lRet;
}



long CWin32Printer::GetPaperExtents(long & MinX, long & MinY, long & MaxX, long & MaxY)
{
    DWORD       dwBufSize = 0;
    BOOL        bResult = 1;
    char        tmpBuf[4000];;
	char		PrnName[256], PrnPort[256], PrnDriver[256];
	char        *szOutput=0;
    long 		lRet = NO_ERR;
    
	memset(PrnName,		0, sizeof(PrnName));
	memset(PrnDriver,	0, sizeof(PrnDriver));
	memset(PrnPort,		0, sizeof(PrnPort));
	
	// get default printer info
	GetDefaultPrinterName(PrnName);
	GetDefaultPrinterPort(PrnPort);
	GetDefaultPrinterDriver(PrnDriver);

	memset (tmpBuf, 0, sizeof(tmpBuf));

	// get min paper extent
    dwBufSize = DeviceCapabilities  ((LPTSTR)PrnName, (LPSTR)szOutput, (WORD)DC_MINEXTENT,
                                    (LPSTR)NULL, (LPDEVMODE)NULL);
                                                                             
	MinX = (long)LOWORD(dwBufSize);
	MinY = (long)HIWORD(dwBufSize);                                                                             
		

    // get max paper extent
    dwBufSize = DeviceCapabilities ((LPTSTR)PrnName, (LPSTR)szOutput, (WORD)DC_MAXEXTENT,
                                    (LPSTR)NULL, (LPDEVMODE)NULL);

	MaxX = (long)LOWORD(dwBufSize);
	MaxY = (long)HIWORD(dwBufSize);                                                                             

	return lRet;
}

long CWin32Printer::PrintRawData(LPSTR szPrinterName, LPSTR DocName, LPBYTE lpData, DWORD dwCount)
{
	 HANDLE     hPrinter;
     DOC_INFO_1 DocInfo;
     DWORD      dwJob;
     DWORD      dwBytesWritten;
	 long		lRet = NO_ERR;

     // Need a handle to the printer
     if( ! OpenPrinter( szPrinterName, &hPrinter, NULL ) )
       return ERR_PRN_OPEN;

     // Fill in the structure with info about this "document"
     DocInfo.pDocName = DocName;
     DocInfo.pOutputFile = NULL;
     DocInfo.pDatatype = "RAW";
     // Inform the spooler the document is beginning
     if( (dwJob = StartDocPrinter( hPrinter, 1, (unsigned char*)&DocInfo )) == 0 )
     {
       ClosePrinter( hPrinter );
       return ERR_STARTDOCPRN;
     }
     // Start a page
     if( ! StartPagePrinter( hPrinter ) )
     {
       EndDocPrinter( hPrinter );
       ClosePrinter( hPrinter );
       return ERR_STARTPAGEPRN;
     }
     // Send the data to the printer
     if( ! WritePrinter( hPrinter, lpData, dwCount, &dwBytesWritten ) )
     {
       EndPagePrinter( hPrinter );
       EndDocPrinter( hPrinter );
       ClosePrinter( hPrinter );
       return ERR_WRITEPAGEPRN;
     }
    // Inform the spooler that the document is ending
     if( ! EndDocPrinter( hPrinter ) )
     {
       ClosePrinter( hPrinter );
       return FALSE;
     }
     // Tidy up the printer handle
     ClosePrinter( hPrinter );
     // Check to see if correct number of bytes writen
     if( dwBytesWritten != dwCount )
       return ERR_DATAMISSING;

	 return lRet;
}

HANDLE CWin32Printer::RawOpen(LPSTR szPrinterName, LPSTR DocName)
{
	 HANDLE     hPrinter;
     DOC_INFO_1 DocInfo;
     DWORD      dwJob;
	 long		lRet = NO_ERR;

     // Need a handle to the printer
     if( ! OpenPrinter( szPrinterName, &hPrinter, NULL ) )
       return (HANDLE)ERR_PRN_OPEN;

     // Fill in the structure with info about this "document"
     DocInfo.pDocName = DocName;
     DocInfo.pOutputFile = NULL;
     DocInfo.pDatatype = "RAW";
     // Inform the spooler the document is beginning
     if( (dwJob = StartDocPrinter( hPrinter, 1, (unsigned char*)&DocInfo )) == 0 )
     {
       ClosePrinter( hPrinter );
       return (HANDLE)ERR_STARTDOCPRN;
     }

	 return hPrinter;
}

long CWin32Printer::RawNewPage(HANDLE hPrinter)
{
     // Start a page
     if( ! StartPagePrinter( hPrinter ) )
     {
       EndDocPrinter( hPrinter );
	   ClosePrinter( hPrinter );
       return ERR_STARTPAGEPRN;
     }

	 return NO_ERR;
 
}

long CWin32Printer::RawEndPage(HANDLE hPrinter)
{
     // End the page
     if( ! EndPagePrinter( hPrinter ) )
     {
       EndDocPrinter( hPrinter );
       ClosePrinter( hPrinter );
       return ERR_ENDPAGEPRN;
     }

	return NO_ERR;
}

long CWin32Printer::RawClose(HANDLE hPrinter)
{
	BOOL bOk = FALSE;

	if (hPrinter != NULL)
		bOk = ClosePrinter( hPrinter );

	if (bOk == FALSE)
		return ERR_PRN_CLOSE;
	else
		return NO_ERR;
 
}

long CWin32Printer::RawWrite(HANDLE hPrinter, LPBYTE lpData, DWORD dwCount)
{
	DWORD      dwBytesWritten;

	// Send the data to the printer
	if( ! WritePrinter( hPrinter, lpData, dwCount, &dwBytesWritten ) )
	{
		EndPagePrinter( hPrinter );
		EndDocPrinter( hPrinter );
		ClosePrinter( hPrinter );
		return ERR_WRITEPAGEPRN;
	}

	if( dwBytesWritten != dwCount )
		return ERR_DATAMISSING;

	return NO_ERR;
}

long CWin32Printer::RawPrinterProperties(HWND hWnd, HANDLE hPrinter)
{
	BOOL	bRet;
	long	lRet = NO_ERR;

	if (hPrinter == NULL)
		return ERR_NULLHPRINTER;

	bRet = PrinterProperties(hWnd, hPrinter);

	if (bRet == FALSE) {
		m_iExtendedErrorAttribute  = GetLastError();
		lRet = ERR_PRINTERPROP;
	}

	return lRet;
}

HDC CWin32Printer::Open(LPCSTR szDevice, LPCSTR szDriver, LPCSTR szDoc, BOOL bShow, HWND hWnd)
{
	DOCINFO		di;
	HDC			hdc;
	PRINTDLG	pd;

    if (!(hdc = CreateDC (szDriver, szDevice, NULL, NULL))) {
		m_iExtendedErrorAttribute  = GetLastError();
        return (HDC)ERR_CREATEDC;
	}

	if (bShow == TRUE) {

		memset ((void *) &pd, 0, sizeof(PRINTDLG));

		pd.lStructSize = sizeof(PRINTDLG);
		pd.hwndOwner   = hWnd;
		pd.Flags       = PD_RETURNDC;
		pd.hInstance   = NULL;

		PrintDlg(&pd);
		hdc = pd.hDC;

		if (pd.hDevMode)
			GlobalFree (pd.hDevMode);

		if (pd.hDevNames)
			GlobalFree (pd.hDevNames);

      if (!hdc){
        m_iExtendedErrorAttribute  = GetLastError();
        return (HDC)ERR_BAD_DC;
      }

    }

  di.cbSize      = sizeof(DOCINFO);
  di.lpszDocName = szDoc;
  di.lpszOutput  = NULL;

  StartDoc  (hdc, &di);

  return hdc;
}

long CWin32Printer::NewPage(HDC hdc)
{
	int		iRet;
	long	lRet = NO_ERR;

	if (hdc == NULL)
		return ERR_BAD_DC;

	iRet = ::StartPage(hdc);
	
	if (iRet <= 0) {	// failed
		m_iExtendedErrorAttribute  = GetLastError();
		lRet = ERR_STARTPAGE;
	}

	return lRet;
}

long CWin32Printer::EndPage(HDC hdc)
{
	int		iRet;
	long	lRet = NO_ERR;

	if (hdc == NULL)
		return ERR_BAD_DC;

	iRet = ::EndPage(hdc);
	if (iRet <= 0) {	// failed
		m_iExtendedErrorAttribute  = GetLastError();
		lRet = ERR_ENDPAGE;
	}

	return lRet;
}

long CWin32Printer::Close(HDC hdc)
{
	int	iRet;
	long	lRet = NO_ERR;

	if (hdc == NULL)
		return ERR_BAD_DC;

	iRet = ::EndDoc(hdc);
	if (iRet <= 0) {	// failed
		m_iExtendedErrorAttribute  = GetLastError();
		lRet = ERR_ENDDOC;
	}

	if (hdc != NULL)
		DeleteDC(hdc);

	return lRet;

}

long CWin32Printer::GetForms(LPCSTR szPrinterName, LPSTR szForms, long nFormType)
{
	// NT only
	return ERR_NOT_IMPLEMENTED;
}

long CWin32Printer::SetPrinterAttributes(DWORD Attributes)
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
	need to do is modify what you'r interested in. */
 
	bFlag = GetPrinter(hPrinter, 2, (LPBYTE)pi2, dwNeeded, &dwNeeded);
	if (!bFlag) {
		iRet = ERR_PRINT_INFO2;
		m_iExtendedErrorAttribute  = GetLastError();
		goto ABORT;
	}

	// make the change
	pi2->Attributes = pi2->Attributes | Attributes;

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
