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
//  Contents:    Header file for CWinNTPrinter class
//               NT specific methods
//
//--------------------------------------------------------------------

#include <afxwin.h>
#include "cwin32prn.h"

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#ifndef __CWINNTPRINTER_H__
#define __CWINNTPRINTER_H__


class CWinNTPrinter : public CWin32Printer {
           
//  Implementation	                      
                      
public:
	long GetForms (LPCSTR szPrinterName, LPSTR szForms, long nFormType);
	long SetPrinterAttribute(short dwField, short iAttribute);

	long DeleteAPrinter (LPCSTR szPrinterName);
	long AddAPrinter (LPCSTR szServerName, LPCSTR szPrinterName, LPCSTR szPortName, LPCSTR szDriverName, LPCSTR szPrintProcessor);
	long DeleteAPrinterConnection (LPCSTR szServerName, LPCSTR szPrinterName);
	long AddAPrinterConnection (LPCSTR szServerName, LPCSTR szPrinterShareName);
	long SetDefaultPrinterEx(LPCSTR szPrinterName, LPCSTR szPrinterDriver, LPCSTR szPrinterPort);
	long SetDefaultPrinter(LPCSTR szPrinterName);
	long GetPrinterPort (LPCSTR szPrinterName, LPSTR szPortName);
        
	// constructor and destructor
	CWinNTPrinter();
	~CWinNTPrinter();

protected:
	long GetPrinterDriver(LPCSTR szPrinterName, LPSTR szDriverName);
};

#endif	// __CWINNTPRINTER_H__