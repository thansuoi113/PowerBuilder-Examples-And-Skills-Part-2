//+-------------------------------------------------------------------
//
//  PowerPrinter
//
//  Copyright (c) 1997, 1998, 1999, 2000, 2001
//  SecureWave. All rights reserved
//
//  See the file LICENSE.TXT for redistribution information.
//
//  File:        CWin95Prn.h
//
//  Contents:    Header file for CWin95Printer class
//               95 specific methods
//
//--------------------------------------------------------------------

#include <afxwin.h>
#include "cwin32prn.h"

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#ifndef __CWIN95PRINTER_H__
#define __CWIN95PRINTER_H__


class CWin95Printer : public CWin32Printer {
           
//  Implementation
                      
public:
	long SetPrinterAttribute (short dwField, short iAttribute);
	long GetForms(LPCSTR szPrinterName, LPSTR szForms, long nFormType);

	long DeleteAPrinter (LPCSTR szPrinterName);
	long AddAPrinter (LPCSTR szServerName, LPCSTR szPrinterName, LPCSTR szPortName, LPCSTR szDriverName, LPCSTR szPrintProcessor);
	long DeleteAPrinterConnection (LPCSTR szServerName, LPCSTR szPrinterName);
	long AddAPrinterConnection (LPCSTR szServerName, LPCSTR szPrinterShareName);
	long SetDefaultPrinterEx(LPCSTR szPrinterName, LPCSTR szPrinterDriver, LPCSTR szPrinterPort);
	long SetDefaultPrinter(LPCSTR szPrinterName);
	long GetPrinterPort (LPCSTR szPrinterName, LPSTR szPortName);
        
	// constructor and destructor
	CWin95Printer();
	~CWin95Printer();

protected:
	long GetPrinterDriver(LPCSTR szPrinterName, LPSTR szDriverName);
};

#endif	// __CWIN95PRINTER_H__