
// HoctAutoExportSimulator.h : main header file for the HoctAutoExportSimulator application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CHoctAutoExportSimulatorApp:
// See HoctAutoExportSimulator.cpp for the implementation of this class
//

class CHoctAutoExportSimulatorApp : public CWinApp
{
public:
	CHoctAutoExportSimulatorApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CHoctAutoExportSimulatorApp theApp;
