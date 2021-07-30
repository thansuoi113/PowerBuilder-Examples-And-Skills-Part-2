//+-------------------------------------------------------------------
//
//  PowerPrinter
//
//  Copyright (c) 1997, 1998, 1999, 2000, 2001
//  SecureWave. All rights reserved
//
//  See the file LICENSE.TXT for redistribution information.
//
//  File:        WinPrn.cpp
//
//  Contents:    CWinPrinter class implementation file
//               Virtual functions implemented in descendant
//				 classes (CWin16, CWin32, CWinNT, CWin95)
//
//--------------------------------------------------------------------

#include "stdafx.h"   
#ifdef WIN32
#include "powerprn.h"
#else
#include "pwrprn16.h"
#endif
#include "WinPrn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//************************************************************
//	Object:	CWinPrinter
//	Method:	CWinPrinter (constructor)
//	Author:	Marco Peretti - SecureWave
//	Date  :	27 Sep 1997
//
//	Arg   :	
//	Return:	
//
//	Desc  :	set default Item and List separators
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
CWinPrinter::CWinPrinter()
{
	// set default separators
	strcpy(m_szItemSep, "#");
	strcpy(m_szListSep, ";");
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	~CWinPrinter (destructor)
//	Author:	Marco Peretti - SecureWave
//	Date  :	27 Sep 1997
//
//	Arg   :	
//	Return:	
//
//	Desc  :	Default destructor
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
CWinPrinter::~CWinPrinter()
{

}

//************************************************************
//	Object:	CWinPrinter
//	Method:	GetDefaultPrinterName
//	Author:	Marco Peretti - SecureWave
//	Date  :	27 Sep 1997
//
//	Arg   :	printer name string pointer
//	Return:	NO_ERR if Ok
//
//	Desc  :	Get default printer name from win.ini
//			Been told that strtok is buggy -- doing it manually
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::GetDefaultPrinterName(LPSTR szDefaultPrinterName)
{
	char		Buffer[256], tmpBuf[256];
	long		iDefaultIsThere = ERR_NO_DEF_PRN;
	int			iColPos;
	size_t		iBufLen;

  // Get it !
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
//	Object:	CWinPrinter
//	Method:	GetDefaultPrinterDriver
//	Author:	Marco Peretti - SecureWave
//	Date  :	27 Sep 1997
//
//	Arg   :	printer driver string pointer
//	Return:	NO_ERR if Ok
//
//	Desc  :	Get driver name from win.ini
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::GetDefaultPrinterDriver(LPSTR szDriver)
{
	char	Buffer[256], tmpBuf[256];
	long	iDefaultIsThere = ERR_NO_DEF_DRIVER;
	int		iColPos, iFirstColPos = 0;
	size_t	iBufLen;

  GetProfileString("windows", "device", ",,,", Buffer, sizeof(Buffer));
  
  iBufLen = strlen(Buffer);

  if (iBufLen > 0) {
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
//	Object:	CWinPrinter
//	Method:	GetDefaultPrinterPort
//	Author:	Marco Peretti - SecureWave
//	Date  :	27 Sep 1997
//
//	Arg   :	port name string pointer
//	Return:	NO_ERR if Ok
//
//	Desc  :	Get port name from win.ini
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::GetDefaultPrinterPort(LPSTR szPort)
{
	char	Buffer[256], tmpBuf[256];
	long	iDefaultIsThere = ERR_NO_DEF_PORT;
	int		iColPos, iSecondColPos = 0;
	size_t	iBufLen;

  GetProfileString("windows", "device", ",,,", Buffer, sizeof(Buffer));
  
  iBufLen = strlen(Buffer);

  if (iBufLen > 0) {
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
//	Object:	CWinPrinter
//	Method:	GetDefaultPrinterEx
//	Author:	Marco Peretti - SecureWave
//	Date  :	27 Sep 1997
//
//	Arg   :	Printer name, port, driver string pointers
//	Return:	NO_ERR if Ok
//
//	Desc  :	get default printer name, driver, port in one call
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::GetDefaultPrinterEx(LPSTR szPrinterName, LPSTR szPrinterDriver, LPSTR szPrinterPort)
{
	char	Buffer[256], tmpBuf[256];
	long	iDefaultIsThere = ERR_NO_DEF_PRN;
	int		iColPos, iNextItemColPos;
	size_t	iBufLen;

  GetProfileString("windows", "device", ",,,", Buffer, sizeof(Buffer));
  
  iBufLen = strlen(Buffer);

  if (iBufLen > 0) {
	iDefaultIsThere = NO_ERR;	// there is a default printer

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
//	Object:	CWinPrinter
//	Method:	SetDefaultPrinter
//	Author:	Marco Peretti - SecureWave
//	Date  :	27 Sep 1997
//
//	Arg   :	Default printer name
//	Return:	NO_ERR if Ok
//
//	Desc  :	Implemented in descendant classes
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::SetDefaultPrinter(LPCSTR szPrinterName)
{
	// Virtual function implemented in descendant classes
	// return any value to avoid warning
	return ERR_NOT_IMPLEMENTED;
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	SetDefaultPrinterEx
//	Author:	Marco Peretti - SecureWave
//	Date  :	27 Sep 1997
//
//	Arg   :	Default printer name, port
//	Return:	NO_ERR if Ok
//
//	Desc  :	Implemented in descendant classes
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::SetDefaultPrinterEx(LPCSTR szPrinterName, LPCSTR szPrinterDriver, LPCSTR szPrinterPort)
{
	// Virtual function implemented in descendant classes
	// return any value to avoid warning
	return ERR_NOT_IMPLEMENTED;
}

//************************************************************
//	Object:	CWinPrinter
//	Method:	GetPrinterList
//	Author:	Marco Peretti - SecureWave
//	Date  :	27 Sep 1997
//
//	Arg   :	printer list string pointer
//	Return:	NO_ERR if Ok
//
//	Desc  :	Implemented in descendant classes
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::GetPrinterList(LPSTR szPrtr)
{
	return	ERR_NOT_IMPLEMENTED;
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	GetPrinterOrientation
//	Author:	Marco Peretti - SecureWave
//	Date  :	27 Sep 1997
//
//	Arg   :	printer orientation (by ref)
//	Return:	NO_ERR if Ok
//
//	Desc  :	Implemented in descendant classes
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::GetPrinterOrientation(short &iPrinterOrientation)
{
	return GetPrinterAttribute(DW_ORIENTATION, iPrinterOrientation);
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	SetPrinterOrientation
//	Author:	Marco Peretti - SecureWave
//	Date  :	27 Sep 1997
//
//	Arg   :	printer orientation 
//	Return:	NO_ERR if Ok
//
//	Desc  :	Implemented in descendant classes
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::SetPrinterOrientation(short iPrinterOrientation)
{
	return SetPrinterAttribute(DW_ORIENTATION, iPrinterOrientation);
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	GetItemSep
//	Author:	Marco Peretti - SecureWave
//	Date  :	27 Sep 1997
//
//	Arg   :	Item separator string pointer
//	Return:	NO_ERR if Ok
//
//	Desc  :	Get Item separator character
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::GetItemSep(LPSTR szItemSep)
{
	long lRet = NO_ERR;

	if (strlen(szItemSep) >= SEPSIZE)
		strcpy(szItemSep, m_szItemSep);
	else
		lRet = ERR_BUFFER_2SMALL;

	return lRet;
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	SetItemSep
//	Author:	Marco Peretti - SecureWave
//	Date  :	27 Sep 1997
//
//	Arg   :	item separator string
//	Return:	NO_ERR if ok
//
//	Desc  :	Set item separator
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::SetItemSep(LPCSTR szItemSep)
{
	long lRet = NO_ERR;

	if (szItemSep == 0)
		return ERR_NULL_POINTER;

	if (strlen(szItemSep) > 0)
		strcpy(m_szItemSep, szItemSep);
	else
		lRet = ERR_BUFFER_EMPTY;

	return lRet;
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	GetListSep
//	Author:	Marco Peretti - SecureWave
//	Date  :	27 Sep 1997
//
//	Arg   :	List separator string pointer
//	Return:	NO_ERR if ok
//
//	Desc  :	Get list separator
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::GetListSep(LPSTR szListSep)
{
	long lRet = NO_ERR;

	if (strlen(szListSep) >= SEPSIZE)
		strcpy(szListSep, m_szItemSep);
	else
		lRet = ERR_BUFFER_2SMALL;

	return lRet;
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	SetListSep
//	Author:	Marco Peretti - SecureWave
//	Date  :	27 Sep 1997
//
//	Arg   :	List separator to be set
//	Return:	NO_ERR if ok
//
//	Desc  :	Change default list separator
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::SetListSep(LPCSTR szListSep)
{
	long lRet = NO_ERR;

	if (szListSep == 0)
		return ERR_NULL_POINTER;

	if (strlen(szListSep) > 0)
		strcpy(m_szListSep, szListSep);
	else
		lRet = ERR_BUFFER_EMPTY;

	return lRet;

}


//************************************************************
//	Object:	CWinPrinter
//	Method:	CheckPrinterName
//	Author:	Marco Peretti - SecureWave
//	Date  :	27 Sep 1997
//
//	Arg   :	String with printer name
//	Return:	TRUE of Ok
//
//	Desc  :	Check if printer name is valid 
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
BOOL CWinPrinter::CheckPrinterName(LPCSTR szPrnName)
{
	// implemented in descendant
	return FALSE;
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	GetPrinterListEx
//	Author:	Marco Peretti - SecureWave
//	Date  :	27 Sep 1997
//
//	Arg   :	printer list string pointer (by ref)
//	Return:	NO_ERR if Ok
//
//	Desc  :	Get printer list, with port and driver too
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::GetPrinterListEx(LPSTR szPrtr)
{
	return ERR_NOT_IMPLEMENTED;
}

// Virtual functions to get/set printer attributes. Implemented in
// CWin32Printer or CWin16Printer. See files for description

long CWinPrinter::GetPrinterAttribute(short dwField, short & iAttribute)
{
	return ERR_NOT_IMPLEMENTED;
}

long CWinPrinter::GetPrinterAttribute(short dwField, DWORD & iAttribute)
{
	return ERR_NOT_IMPLEMENTED;
}

long CWinPrinter::GetPrinterAttribute(short dwField, WORD & iAttribute)
{
	return ERR_NOT_IMPLEMENTED;
}

long CWinPrinter::SetPrinterAttribute(short dwField, WORD wAttribute)
{
	return ERR_NOT_IMPLEMENTED;
}

long CWinPrinter::SetPrinterAttribute(short dwField, DWORD dwAttribute)
{
	return ERR_NOT_IMPLEMENTED;
}


long CWinPrinter::SetPrinterAttribute(short dwField, short iAttribute)
{
	return ERR_NOT_IMPLEMENTED;
}


//************************************************************
//	Object:	CWin32Printer
//	Method:	GetPaperSize
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Paper size, by ref
//	Return:	NO_ERR if Ok
//
//	Desc  :	Get default printer paper size
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::GetPaperSize(short &iPaperSize)
{
 	return GetPrinterAttribute(DW_PAPERSIZE, iPaperSize);
}


//************************************************************
//	Object:	CWin32Printer
//	Method:	SetPaperSize
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Paper size
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Set default printer paper size
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::SetPaperSize(short iPaperSize)
{
	return SetPrinterAttribute(DW_PAPERSIZE, iPaperSize);
}


////////////////////////////////////////////////////////////////
// DEVMODE GET SET Methods. The following functions call 
// Get/Set PrinterAttribute (SHORT, WORD, DWORD)

//************************************************************
//	Object:	CWinPrinter
//	Method:	GetPaperLength
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Paper length by ref
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Get default printer paper length
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::GetPaperLength(short &iPaperLength)
{
	return GetPrinterAttribute(DW_PAPERLENGTH, iPaperLength);
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	SetPaperLength
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Paper length
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Set default printer paper length
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::SetPaperLength(short iPaperLength)
{
	return SetPrinterAttribute(DW_PAPERLENGTH, iPaperLength);
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	GetPaperWidth
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Paper width
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Get default printer paper width
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::GetPaperWidth(	short &iPaperWidth)
{
	return GetPrinterAttribute(DW_PAPERWIDTH, iPaperWidth);
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	SetPaperWidth
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Paper width
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Set default printer paper width
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::SetPaperWidth(short iPaperWidth)
{
	return SetPrinterAttribute(DW_PAPERWIDTH, iPaperWidth);
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	GetScale
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Paper scale
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Get default printer paper scale
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::GetScale(short &iScale)
{
	return GetPrinterAttribute(DW_SCALE, iScale);
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	SetScale
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Paper scale
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Set default printer paper scale
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::SetScale(short iScale)
{
	return SetPrinterAttribute(DW_SCALE, iScale);
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	GetCopies
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Number of copies
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Get number of copies to be printed
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::GetCopies(short &iCopies)
{
	return GetPrinterAttribute(DW_COPIES, iCopies);
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	SetCopies
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Number of copies
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Set number of copies to be printed
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::SetCopies(short iCopies)
{
	return SetPrinterAttribute(DW_COPIES, iCopies);
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	GetDefaultSource
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Default source,by ref
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Get printer source (or BIN): where should it print ?
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::GetDefaultSource(short &iDefaultSource)
{
	return GetPrinterAttribute(DW_DEFAULTSOURCE, iDefaultSource);
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	SetDefaultSource
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Printer Source to be set as default
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Set printer default sheet source
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::SetDefaultSource(short iDefaultSource)
{
	return SetPrinterAttribute(DW_DEFAULTSOURCE, iDefaultSource);
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	GetPrintQuality
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Current paper quality, by ref
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Returns current paper quality. There are 4 
//			device independent values (HIGH, MEDIUM, LOW, 
//			DRAFT, as described in the help file. Can accept
//			as well device dependent values (dots x inch or DPI)
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::GetPrintQuality(short &iPrintQuality)
{
	return GetPrinterAttribute(DW_PRINTQUALITY, iPrintQuality);
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	SetPrintQuality
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Printer esolution to be used
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Either a negative value that represnts a device-
//			independent setting or a positive to specofy
//			dots x inch (DPI) resolution.
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::SetPrintQuality(short iPrintQuality)
{
	return SetPrinterAttribute(DW_PRINTQUALITY, iPrintQuality);
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	GetDuplex
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Current Duplex settings
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Retrieve current duplex settings. Possible values
//			are DMDUP_SIMPLEX, DMDUP_HORIZONTAL, DMDUP_VERTICAL
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::GetDuplex(short &iDuplex)
{
	return GetPrinterAttribute(DW_DUPLEX, iDuplex);
}


//************************************************************
//	Object:	CWinPrinter
//	Method:	SetDuplex
//	Author:	Marco Peretti - SecureWave
//	Date  :	10 of Feb 1997
//
//	Arg   :	Duplex settings (se help for values)
//	Return:	NO_ERR if Ok	
//
//	Desc  :	Selects duplex or double-sided printing for 
//			printers capable of duplex printing
//
//************************************************************
//	Modifications:
//	Date            Author              Comments
//------------------------------------------------------------
//
//************************************************************
long CWinPrinter::SetDuplex(short iDuplex)
{
	return SetPrinterAttribute(DW_DUPLEX, iDuplex);
}
     
long CWinPrinter::SetDocumentProperties(HWND hWnd)
{
	return ERR_NOT_IMPLEMENTED;
}

long CWinPrinter::SetFormName(LPCSTR szFormName)
{
	return ERR_NOT_IMPLEMENTED;
}

	//V1.2
long CWinPrinter::GetNamedPaperBinListEx(LPCSTR szPrinterName, LPSTR szPaperBins)
{
	return ERR_NOT_IMPLEMENTED;
}


long CWinPrinter::GetPaperBinListEx(LPCSTR szPrinterName, LPSTR szPaperBins)
{
	return ERR_NOT_IMPLEMENTED;
}


long CWinPrinter::GetSupportedPaperSizeList(LPSTR szPageSizeList)
{
	return ERR_NOT_IMPLEMENTED;
}


long CWinPrinter::GetNamedPaperBinList(LPSTR szPaperBins)
{
	return ERR_NOT_IMPLEMENTED;
}


long CWinPrinter::GetPaperBinList(LPSTR szPaperBins)
{
	return ERR_NOT_IMPLEMENTED;
}

long CWinPrinter::GetPaperExtents(long &MinX, long &MinY, long &MaxX, long &MaxY)
{
	return ERR_NOT_IMPLEMENTED;
}
