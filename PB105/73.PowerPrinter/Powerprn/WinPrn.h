//+-------------------------------------------------------------------
//
//  PowerPrinter
//
//  Copyright (c) 1997, 1998, 1999, 2000, 2001
//  SecureWave. All rights reserved
//
//  See the file LICENSE.TXT for redistribution information.
//
//  File:        WinPrn.h
//
//  Contents:    Header file for CWinPrinter class (base class)
//               Contains error #define for all errors
//
//--------------------------------------------------------------------

#if !defined(AFX_WINPRN_H__527506B3_D5E7_11D0_A89C_00AA00AA9DA2__INCLUDED_)
#define AFX_WINPRN_H__527506B3_D5E7_11D0_A89C_00AA00AA9DA2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// For PowerDIB Only
// NB: PowerDIB APIs are loaded dynamically using LoadLibrary and
// GetProcAddress. See CWin32Printer for implementation

/* Print Options selection */
#define PW_BESTFIT       1
#define PW_STRETCHTOPAGE 2
#define PW_SCALE         3

// from dialogs.h
#define IDC_BESTFIT                 119
#define IDC_STRETCHTOPAGE           120
#define IDC_SCALE                   121


#define HDIB HANDLE			// Handle to a DIB
#define BUFSIZE		 256	// Default Buffer size
#define SEPSIZE		 3		// Default Separator size (see later)
#define CCHBINNAME   24		// Bin name string size

#define MAX_AMOUNT   256
#define MAX_PAPERS   69
#define MAX_BINS     16		// Maximum number of bins supported


// PowerPrinter error codes
#define NO_ERR				1		// hey, no error !
#define ERR_BAD_ARGUMENT	-1		// bad arg given to function
#define ERR_BUFFER_2SMALL	-2		// not large enough
#define ERR_NO_DEF_PRN		-3		// no default printer
#define ERR_NO_DEF_PORT		-4		// no default printer port (v1.1)
#define ERR_NO_DEF_DRIVER	-5		// no default printer driver (v1.1)
#define ERR_BUFFER_EMPTY	-6		// given argument is empty ! (v1.1)
#define ERR_NULL_POINTER	-7		// null pointer eceived while expecting an argument (v1.1)
#define	ERR_PRN_OPEN		-100	// Cannot get the printer -- access problem ?
#define	ERR_PRN_GET			-101	// GetPrinter failed to return buffer size
#define	ERR_GLOBAL_ALLOC	-102	// GlobalAlloc failed
#define	ERR_GLOBAL_LOCK		-103	// GlobalLock failed 
#define	ERR_PRINT_INFO2		-104	// Cannot fill in PRINT_INFO_2 structure
#define	ERR_PRN_NOCHANGE	-105	// Printer does not support change
#define	ERR_UPDT_DEVMODE	-106	// Cannot update driver-dependent part of devmode
#define	ERR_PRN_SET			-107	// SetPrinter Failed
#define ERR_ATTR_UNKNOWN	-108	// Unknown Attribute
#define ERR_DEVMODE_ISNULL	-109	// GetPrinter API failed -- even if ret code was OK !
#define ERR_NOT_IMPLEMENTED	-110	// API not implmemented (v1.1)
#define ERR_ADD_PRINTER		-111	// AddPrinter API failed (v1.1)
#define	ERR_ADD_PRNCONN		-112	// AddPrinterConnection API failed (v1.1)
#define	ERR_DEL_PRNCONN		-113	// DeletePrinterConnection API failed (v1.1)
#define ERR_DEL_PRINTER		-114	// DeletePrinter API failed (v1.1)
#define ERR_ADD_DRIVER		-115	// AddAPrinterDriver failed (v1.1)
#define ERR_DEL_DRIVER		-116	// DelAPrinterDriver failed (v1.1)
#define ERR_LOCAL_ALLOC		-117	// LocalAlloc failed (v1.1)
#define ERR_ENUM_NO_DRV		-118	// EnumDrivers returned zero (v1.1)
#define ERR_NOTHING_2DO		-119	// Nothing to do (v1.1). Eg no print jobs
#define ERR_ENUM_JOBS1		-120	// EnumJobs (step 1) failed (v1.1)
#define ERR_ENUM_JOBS2		-121	// EnumJobs (step 2) failed (v1.1)
#define ERR_WIN95_SETDEFPRNEX -122	// SetDefaultPrinterEx failed under Win 95 (v1.1)
#define ERR_NOLOAD_POWERDIB	-123	// Cannot load PowerDIB
#define ERR_NOLOAD_FUNCTION -124	// Cannot Load function. This should never happen !
#define ERR_BAD_DC			-125	// Bad device context argument
#define ERR_CREATE_FONT		-126	// Could not create the font
#define ERR_COPY_WINDOW		-127	// Could not copy window
#define ERR_FATAL_ERROR		-128	// Fatal Error. Us dwGetExtendedErrorCode
#define	ERR_DOCPROPERTY		-129	
#define ERR_DRIVER_NOLOAD	-130	// could not load printer driver (16bit)
#define ERR_EXTDMODE_NOF	-131	// EXTDEVICEMODE not found in driver
#define ERR_BAD_BUFFER		-132	// EXTDEVICEMODE buffer problem. Could not get data
#define ERR_GET_JOBS		-133	// GetJobs failed
#define ERR_GET_JOB			-134	// GetJob failed
#define ERR_SET_JOB			-135	// SetJob failed
#define ERR_SAFEAALLOCDESC	-136	// SafeArrayAllocDescriptor failed
#define ERR_SAFEAALLOCDATA	-137	// SafeArrayAllocData Failed
#define ERR_SAFEAACCESDATA	-138	// SafeArrayAccessData Failed
#define ERR_STARTDOCPRN		-139	// StartDocPrinter
#define	ERR_STARTPAGEPRN	-140	// StartPagePrinter
#define ERR_WRITEPAGEPRN	-141	// WritePagePrinter
#define ERR_ENDPAGEPRN		-142	// EndPagePrinter
#define ERR_DATAMISSING		-143	// could not write all data (to printer)
#define ERR_PRN_CLOSE		-144	// Failed when calling ClosePrinter
#define ERR_NULLHPRINTER	-145	// hPrinter cannot be NULL		
#define ERR_PRINTERPROP		-146	// PrinterProperties failed
#define ERR_CREATEDC		-147	// CreateDC failed
#define	ERR_STARTPAGE		-148	// StartPage failed
#define ERR_ENDPAGE			-149	// EndPage failed
#define ERR_ENDDOC			-150	// EndDoc failed
#define ERR_ENUMFORMS1		-151	// first call to Enumforms failed
#define ERR_ENUMFORMS2		-152	// first call to Enumforms failed


// short attributes
#define	DW_ORIENTATION		1
#define	DW_PAPERSIZE		2
#define	DW_PAPERLENGTH		3
#define	DW_PAPERWIDTH		4
#define	DW_SCALE			5
#define	DW_COPIES			6
#define	DW_DEFAULTSOURCE	7
#define	DW_PRINTQUALITY		8
#define	DW_COLOR			9
#define	DW_DUPLEX			10
#define	DW_YRESOLUTION		11
#define	DW_TTOPTION			12
#define	DW_COLLATE			13

// WORD attributes
#define	DW_LOGPIXELS		14

// DWORD attributes
#define	DW_BITSPERPEL		15
#define	DW_PELSWIDTH		16
#define	DW_PELSHEIGHT		17
#define	DW_DISPLAYFLAGS		18
#define	DW_DISPLAYFREQUENCY	19


// string
#define	DW_PORT				30
#define	DW_FORMNAME			31

// Base class for the printer classes. Never instantiated directly
// Must create an instance of either CWin16Printer or NT, 95

class CWinPrinter  
{

// Member variables
protected:

	char m_szListSep[SEPSIZE];
	char m_szItemSep[SEPSIZE];

// Member functions
public:
	virtual long SetFormName(LPCSTR szFormName);
	virtual long SetDocumentProperties (HWND hWnd);

	// Get/Set List/Item separators. User can customize how items
	// are put together. Default values set in constructor

	long SetListSep (LPCSTR szListSep);
	long GetListSep (LPSTR szListSep);
	long SetItemSep (LPCSTR szItemSep);
	long GetItemSep (LPSTR szItemSep);

	// Base member functions available to both 16/32 bit 
	// If memeber function not implemented it returns ERR_NOT_IMPLEMENTED

	virtual long GetPrinterList(LPSTR szPrtr);
	virtual long SetDefaultPrinterEx(LPCSTR szPrinterName, LPCSTR szPrinterDriver, LPCSTR szPrinterPort);
	virtual long SetDefaultPrinter(LPCSTR szPrinterName);     
	virtual BOOL CheckPrinterName(LPCSTR szPrnName);

	long GetPrinterOrientation(short &iPrinterOrientation);
	long SetPrinterOrientation(short iPrinterOrientation);

	long SetDefaultSource(short iDefaultSource);
	long GetDefaultSource(short &iDefaultSource);

	long SetPaperSize(short iPaperSize);
	long GetPaperSize(short &iPaperSize);

	long SetCopies(short iCopies);
	long GetCopies(short &iCopies);

	long SetScale(short iScale);
	long GetScale(short &iScale);

	long SetPrintQuality(short iPrintQuality);
	long GetPrintQuality(short &iPrintQuality);

	long SetPaperWidth(short iPaperWidth);
	long GetPaperWidth(short &iPaperWidth);

	long SetPaperLength(short iPaperLength);
	long GetPaperLength(short &iPaperLength);

	long SetDuplex(short iDuplex);
	long GetDuplex(short &iDuplex);

	long GetDefaultPrinterEx(LPSTR szPrinterName, LPSTR szPrinterDriver, LPSTR szPrinterPort);
	long GetDefaultPrinterPort(LPSTR szPort);
	long GetDefaultPrinterDriver(LPSTR szDriver);
	long GetDefaultPrinterName(LPSTR szDefaultPrinterName);
	long GetPrinterListEx(LPSTR szPrtr);
	
	// Constructor/Destructor
	CWinPrinter();
	virtual ~CWinPrinter();                    

	//V1.2
	virtual long GetNamedPaperBinListEx(LPCSTR szPrinterName, LPSTR szPaperBins);
	virtual long GetPaperBinListEx(LPCSTR szPrinterName, LPSTR szPaperBins);
	virtual long GetSupportedPaperSizeList(LPSTR szPageSizeList);
	virtual long GetNamedPaperBinList(LPSTR szPaperBins);
	virtual long GetPaperBinList(LPSTR szPaperBins);	

protected:

	virtual long SetPrinterAttribute(short dwField, short iAttribute);
	virtual long GetPrinterAttribute(short dwField, short &iAttribute);
	virtual long SetPrinterAttribute(short dwField, DWORD wAttribute);
	virtual long GetPrinterAttribute(short dwField, DWORD &iAttribute);
	virtual long SetPrinterAttribute(short dwField, WORD wAttribute);
	virtual long GetPrinterAttribute(short dwField, WORD &iAttribute);
	virtual long GetPaperExtents(long &MinX, long &MinY, long &MaxX, long &MaxY);	
};

#endif // !defined(AFX_WINPRN_H__527506B3_D5E7_11D0_A89C_00AA00AA9DA2__INCLUDED_)
