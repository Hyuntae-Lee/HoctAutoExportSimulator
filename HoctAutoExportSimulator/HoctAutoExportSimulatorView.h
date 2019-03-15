
// HoctAutoExportSimulatorView.h : interface of the CHoctAutoExportSimulatorView class
//

#pragma once

#include "GlobalMeasureDef.h"
#include "Patient.h"
#include "OctScanDescript.h"
#include <iostream>
#include <vector>
#include <cpprest/json.h>

using namespace PatientDB;
using namespace OctGlobal;

class CHoctAutoExportSimulatorView : public CHtmlView
{
	const std::wstring kImageExt = L"jpg";
	const std::wstring kImageExtOld = L"png";

	const CString kMainUrl = _T("main.html");

protected: // create from serialization only
	CHoctAutoExportSimulatorView();
	DECLARE_DYNCREATE(CHoctAutoExportSimulatorView)

// Attributes
public:
	CHoctAutoExportSimulatorDoc* GetDocument() const;
	virtual HRESULT OnGetExternal(LPDISPATCH *lppDispatch);

// Operations
public:
	void changePage(CString url);
	CString getFundusSamplePath();
	auto exportOctEMNET(CString szRootPath, EyeSide eyeSide)->std::vector<std::wstring>;
	auto exportFundusEMNET(CString szRootPath, Patient patient, EyeSide eyeSide)->std::wstring;
	auto composeJsonEMNET(Patient patient, std::wstring fundusFileName,
		std::vector<std::wstring> octFileList, EyeSide side, CString szPatternName)
			->std::pair<bool, web::json::value>;
	auto getRaceStringEMNET(long raceId)->std::wstring;

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CHoctAutoExportSimulatorView();

protected:
	void OnBtnExportClicked(long mode, CComBSTR exportDir, long side, CComBSTR strPatternName);

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
};

inline CHoctAutoExportSimulatorDoc* CHoctAutoExportSimulatorView::GetDocument() const
   { return reinterpret_cast<CHoctAutoExportSimulatorDoc*>(m_pDocument); }

