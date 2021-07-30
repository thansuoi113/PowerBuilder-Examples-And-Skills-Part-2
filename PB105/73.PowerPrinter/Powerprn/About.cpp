//+-------------------------------------------------------------------
//
//  PowerPrinter
//
//  Copyright (c) 1997, 1998, 1999, 2000, 2001
//  SecureWave. All rights reserved
//
//  See the file LICENSE.TXT for redistribution information.
//
//  File:        About.cpp
//
//  Contents:    CAbout class implementation
//               Show simple about box
//
//--------------------------------------------------------------------

#include "stdafx.h"  
#ifdef WIN32 
#include "powerprn.h"
#else
#include "pwrprn16.h"
#endif

#include "About.h"

//#include "gl\gl.h"
//#include "gl\glu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAbout dialog


CAbout::CAbout(CWnd* pParent /*=NULL*/)
	: CDialog(CAbout::IDD, pParent)
{

	//{{AFX_DATA_INIT(CAbout)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}


void CAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAbout)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAbout, CDialog)
	//{{AFX_MSG_MAP(CAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAbout message handlers




BOOL CAbout::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
#ifndef WIN32
	CenterWindow();
#endif
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
