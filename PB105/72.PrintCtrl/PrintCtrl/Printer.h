// Printer.h: interface for the CPrinter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINTER_H__42D2C2BD_1E42_45C8_AEDF_563679CA440A__INCLUDED_)
#define AFX_PRINTER_H__42D2C2BD_1E42_45C8_AEDF_563679CA440A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winspool.h>
typedef TCHAR PAPERNAME[64]; //打印机纸张名称类型

class CPrinter  
{
public:
	CPrinter();
	virtual ~CPrinter();
	
	BOOL IsWindowsNT();
	int GetWindowsVer();
	BOOL GetDefaultPrinterAndPaper(LPTSTR szPrinterName, int nPrintNameBufferLen, LPTSTR szPrinterPort, int nPrinterPortBufferLen, LPTSTR szPaperName);
	HANDLE GetPrinterHandle(LPTSTR szPrinterName);
	short GetPaperSize(LPTSTR szPrinterName, LPTSTR szPortName, PAPERNAME szPaperName);
	BOOL GetPaperName(LPTSTR szPrinterName, LPTSTR szPortName, short nPaperSize, PAPERNAME szPaperName);
	PRINTER_INFO_2 *GetInfo2(LPTSTR szPrinterName);
	BOOL SetInfo2(PRINTER_INFO_2 *ppi2);
	BOOL SetPaper(LPTSTR szPrinterName, PAPERNAME szPaperName, short nOrientation);
	BOOL GetDefaultPaper(LPTSTR szPrinterName, LPTSTR szPaperName, long & nPaperSize, long & nOrientation);
	BOOL AddCustomPaper(LPTSTR szPrinterName, PAPERNAME szPaperName, SIZE PaperSize, RECT rcPrintableMargin);
	BOOL DeleteCustomPaper(LPTSTR szPrinterName, LPTSTR szPaperName);
	BOOL SetUserDefinePaper(LPTSTR szPrinterName, long & nWidth, long & nHeight);
	BOOL GetUserDefinePaperMinMax(LPTSTR szPrinterName, long & nMinWidth, long & MinHeight, long & nMaxWidth, long & nMaxHeight);
	BOOL GetPrinterPort(LPTSTR szPrinterName, LPTSTR szPrinterPort, int nPortBufferLen, LPTSTR szPrinterDriver, int nDriverBufferLen);
	BOOL GetPaperList(LPTSTR szPrinterName, LPTSTR szPaperList, long & nBufferLen);
	BOOL GetDefaultPrinter(LPTSTR szPrinterName, int nPrintNameBufferLen);
	BOOL GetPrinterList(LPTSTR szPrinterNameList, long & nPrintNameBufferLen);
	BOOL GetPrinterStatus(LPTSTR szPrinterNameList, long & status);
	BOOL SetPrinterDefault(LPTSTR szPrinterName);
	BOOL GetBinNameList(LPTSTR szPrinterName, LPTSTR szBinNameList);
	BOOL GetBin(LPTSTR szPrinterName, LPTSTR szBinName, short & nBin);
	BOOL GetBinName(LPTSTR szPrinterName, LPTSTR szBinName, short nBin);
};

#endif // !defined(AFX_PRINTER_H__42D2C2BD_1E42_45C8_AEDF_563679CA440A__INCLUDED_)
