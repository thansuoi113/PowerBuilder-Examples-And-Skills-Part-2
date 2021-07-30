//+-------------------------------------------------------------------
//
//  PowerPrinter
//
//  Copyright (c) 1997, 1998, 1999, 2000, 2001
//  SecureWave. All rights reserved
//
//  See the file LICENSE.TXT for redistribution information.
//
//  File:        Win16Prn.h
//
//  Contents:    Header file for CWin16Printer class
//               
//
//--------------------------------------------------------------------

#if !defined(AFX_WIN16PRN_H__527506B4_D5E7_11D0_A89C_00AA00AA9DA2__INCLUDED_)
#define AFX_WIN16PRN_H__527506B4_D5E7_11D0_A89C_00AA00AA9DA2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Base class
#include "WinPrn.h"

class CWin16Printer : public CWinPrinter  
{

// Member variables
protected:

	HANDLE m_hDriver;	// Handle to driver library

// Member functions
protected:
	long GetPrinterAttribute(short dwField, short &iAttribute);
	long SetPrinterAttribute(short dwField, short iAttribute);

	// 16Bit version functions are in manifacturer driver
	long FreeDriverLibrary();
	long LoadDriverLibrary (LPSTR szDriverFile);
	long LoadDefaultDriverLibrary();
	
public:

	// CWin16 Methods
	BOOL CheckPrinterName(LPCSTR szPrnName);
	long GetPrinterList(LPSTR szPrtr);
	long SetDefaultPrinterEx(LPCSTR szPrinterName, LPCSTR szPrinterDriver, LPCSTR szPrinterPort);
	long SetDefaultPrinter(LPCSTR szPrinterName);   

	// Constructor/Destructor
	CWin16Printer();
	virtual ~CWin16Printer();               
	
	//V1.2
//	long GetNamedPaperBinListEx(LPCSTR szPrinterName, LPSTR szPaperBins);
//	long GetPaperBinListEx(LPCSTR szPrinterName, LPSTR szPaperBins);
	long GetSupportedPaperSizeList(LPSTR szPageSizeList);
	long GetNamedPaperBinList(LPSTR szPaperBins);
	long GetPaperBinList(LPSTR szPaperBins);
	long SetDocumentProperties(long hWnd);
	long GetPaperExtents(long &MinX, long &MinY, long &MaxX, long &MaxY);
	
};

#endif // !defined(AFX_WIN16PRN_H__527506B4_D5E7_11D0_A89C_00AA00AA9DA2__INCLUDED_)
