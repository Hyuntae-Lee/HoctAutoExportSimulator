
// HoctAutoExportSimulatorView.cpp : implementation of the CHoctAutoExportSimulatorView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "HoctAutoExportSimulator.h"
#endif

#include "HoctAutoExportSimulatorDoc.h"
#include "HoctAutoExportSimulatorView.h"

#include "HctMainDef.h"

#include "Measure.h"
#include "HctMainUtil.h"

#include <codecvt>
#include <locale>

// CHoctAutoExportSimulatorView

IMPLEMENT_DYNCREATE(CHoctAutoExportSimulatorView, CHtmlView)

BEGIN_MESSAGE_MAP(CHoctAutoExportSimulatorView, CHtmlView)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CHoctAutoExportSimulatorView, CHtmlView)
	DISP_FUNCTION(CHoctAutoExportSimulatorView, "OnBtnExportClicked", OnBtnExportClicked, VT_EMPTY, VTS_I4 VTS_BSTR VTS_I4 VTS_BSTR)
END_DISPATCH_MAP()

// CHoctAutoExportSimulatorView construction/destruction

CHoctAutoExportSimulatorView::CHoctAutoExportSimulatorView()
{
	EnableAutomation();
}

CHoctAutoExportSimulatorView::~CHoctAutoExportSimulatorView()
{
}

void CHoctAutoExportSimulatorView::changePage(CString url)
{
	// get html path
	TCHAR strCurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strCurDir);
	CString szCurDir(strCurDir);

	// go to the page
	Navigate(szCurDir + _T("/ui/") + url);
}

CString CHoctAutoExportSimulatorView::getFundusSamplePath()
{
	// get html path
	TCHAR strCurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strCurDir);
	CString szCurDir(strCurDir);

	return szCurDir + _T("/sample/fundus data sample");
}

auto CHoctAutoExportSimulatorView::exportOctEMNET(CString szRootPath, EyeSide eyeSide)
	->std::vector<std::wstring>
{
	if (szRootPath.IsEmpty()) {
		return std::vector<std::wstring>();
	}

	// 1. export files
	std::wstring strPrefix = L"scan_";
	std::wstring strRemove = L"remove_"; // files to remove
	std::wstring strDirRoot = CT2CW(szRootPath);
	if (Measure::exportScannerResults(strDirRoot, false, strPrefix, strRemove, strRemove,
		strRemove, strRemove) <= 0) {
		return std::vector<std::wstring>();
	}
	// 2. make a report for file list
	CFileFind finder;
	std::vector<std::wstring> pathList;
	// - build a string with wildcards
	CString strWildcard(strDirRoot.c_str());
	strWildcard += _T("\\*.*");
	// - start working for files
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking) {
		bWorking = finder.FindNextFile();
		// skip . and .. files; otherwise, we'd
		// recur infinitely!
		if (finder.IsDirectory() || finder.IsDots()) {
			continue;
		}

		CString szPath = finder.GetFilePath();
		std::wstring strPath = CT2CW(szPath);

		size_t pos = strPath.rfind(L"\\");
		// 경로가 없을 수는 없다.
		if (pos == std::wstring::npos) {
			pathList.clear();
			break;
		}
		pos += std::wstring(L"\\").length();
		auto strFileName = strPath.substr(pos);
		// scan file 만 필요
		// - preview 등 기타 파일들은 지우고,
		if (strFileName.substr(0, strRemove.length()) == strRemove) {
			_wremove(strPath.c_str());
			continue;
		}
		// - prefix 를 확인해 리스트에 포함 시킨다.
		if (strFileName.substr(0, strPrefix.length()) == strPrefix) {
			pathList.push_back(strFileName);
		}
	}

	return pathList;
}

auto CHoctAutoExportSimulatorView::exportFundusEMNET(CString szRootPath, Patient patient,
	EyeSide eyeSide)->std::wstring
{
	if (szRootPath.IsEmpty()) {
		return L"";
	}

	std::wstring strDirRoot = CT2CW(szRootPath);

	std::wstring strFileName = L"fundus." + kImageExt;

	// custom image 가 필요하다.
	// - get path
	wstring strPathOrigin = getFundusSamplePath() + L"/" + L"fundus_custom.jpg";
	wstring strPathExport = strDirRoot + L"/" + strFileName;

	// export
	auto exportRet = CopyFile(CString(strPathOrigin.c_str()), CString(strPathExport.c_str()), FALSE);
	if (exportRet) {
		return (strFileName);
	}
	else {
		return L"";
	}
}

auto CHoctAutoExportSimulatorView::composeJsonEMNET(Patient patient, std::wstring fundusFileName,
	std::vector<std::wstring> octFileList, EyeSide side, CString szPatternName)
	->std::pair<bool, web::json::value>
{
	if (fundusFileName.empty()) {
		return make_pair(false, web::json::value());
	}

	web::json::value jsonValue;

	// patient info.
	// - birth date
	auto szBirthDate = patient.getBirthDate().Format(_T("%Y.%m.%d"));
	std::wstring strBirthDate = CT2CW(szBirthDate);
	jsonValue[L"dayOfBirth"] = web::json::value::string((utility::string_t)szBirthDate, false);
	// - gender
	auto strGender = patient.getGender() == PatientDB::Gender::FEMALE ? L"F" : L"M";
	jsonValue[L"gender"] = web::json::value::string((utility::string_t)strGender, false);
	// - race
	auto strRace = getRaceStringEMNET(patient.getRaceID());
	if (!strRace.empty()) {
		jsonValue[L"race"] = web::json::value::string((utility::string_t)strRace, false);
	}
	// - name
	auto strFirstName = patient.getFirstName();
	if (!strFirstName.empty()) {
		jsonValue[L"name"] = web::json::value::string((utility::string_t)strFirstName, false);
	}
	// - name affix
	auto strMiddleName = patient.getMiddleName();
	if (!strMiddleName.empty()) {
		jsonValue[L"nameAffix"] = web::json::value::string((utility::string_t)strMiddleName, false);
	}
	// - last name
	auto strLasttName = patient.getLastName();
	if (!strLasttName.empty()) {
		jsonValue[L"lastName"] = web::json::value::string((utility::string_t)strLasttName, false);
	}
	// - patient id
	auto strPatientId = patient.getPatientId();
	jsonValue[L"internalPatientId"] = web::json::value::string((utility::string_t)strPatientId, false);
	// - device id
	auto szSN = HctMainUtil::getSerialNumber();
	std::wstring strSN = CT2CW(szSN);
	jsonValue[L"deviceId"] = web::json::value::string((utility::string_t)strSN, false);

	// images
	std::vector<web::json::value> arrayImages;
	auto strEyeSide = HctMainUtil::getEyeSideString(side);
	// - fundus
	web::json::value fundusFileDesc;
	// : eye
	fundusFileDesc[L"eye"] = web::json::value::string((utility::string_t)strEyeSide);
	// : type
	fundusFileDesc[L"type"] = web::json::value::string((utility::string_t)L"Fundus");
	// - fileName
	fundusFileDesc[L"fileName"] = web::json::value::string((utility::string_t)fundusFileName);
	arrayImages.push_back(fundusFileDesc);
	// - oct
	for (auto octFile : octFileList) {
		web::json::value octFileDesc;
		// : eye
		fundusFileDesc[L"eye"] = web::json::value::string((utility::string_t)strEyeSide);
		// : type
		std::wstring strPatternName = CT2CW(szPatternName);
		fundusFileDesc[L"type"] = web::json::value::string((utility::string_t)strPatternName);
		// - fileName
		fundusFileDesc[L"fileName"] = web::json::value::string((utility::string_t)octFile);
		arrayImages.push_back(fundusFileDesc);
	}
	// - add to images
	jsonValue[L"images"] = web::json::value::array(arrayImages);

	return make_pair(true, jsonValue);
}

auto CHoctAutoExportSimulatorView::getRaceStringEMNET(long raceId)->std::wstring
{
	std::wstring strRace;

	switch (raceId) {
	case 1:
		strRace = L"Asian";
		break;
	case 2:
		strRace = L"Caucasian";
		break;
	case 3:
		strRace = L"Latino";
		break;
	case 5:
		strRace = L"Indian";
		break;
	case 6:
		strRace = L"Arab";
		break;
	default:
		strRace = L"";
		break;
	}

	return strRace;
}

BOOL CHoctAutoExportSimulatorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CHtmlView::PreCreateWindow(cs);
}

void CHoctAutoExportSimulatorView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();

	ModifyStyleEx(WS_EX_CLIENTEDGE, WS_EX_COMPOSITED, SWP_FRAMECHANGED);

	//
	changePage(kMainUrl);
}

// CHoctAutoExportSimulatorView diagnostics

HRESULT CHoctAutoExportSimulatorView::OnGetExternal(LPDISPATCH *lppDispatch)
{
	*lppDispatch = GetIDispatch(TRUE);
	return S_OK;
}

// CHoctAutoExportSimulatorView message handlers
void CHoctAutoExportSimulatorView::OnBtnExportClicked(long mode, CComBSTR exportDir, long side,
	CComBSTR strPatternName)
{
	// measMode
	MeasMode measMode;
	if (mode == 0) {
		measMode = MeasMode::fundus;
	}
	else if (mode == 1) {
		measMode = MeasMode::oct;
	}
	else if (mode == 2) {
		measMode = MeasMode::octFundus;
	}
	else {
		return;
	}

	// eye side
	EyeSide eyeSide;
	if (side == 0) {
		eyeSide = EyeSide::OD;
	}
	else if (side == 0) {
		eyeSide = EyeSide::OS;
	}
	else {
		return;
	}

	Patient patient;

	// fundus
	std::wstring fundusFileName;
	if (measMode == MeasMode::fundus || measMode == MeasMode::octFundus) {
		fundusFileName = exportFundusEMNET(CString(exportDir),
			patient, eyeSide);
	}
	// oct
	std::vector<std::wstring> octList;
	if (measMode == MeasMode::octFundus) {
		octList = exportOctEMNET(CString(exportDir), eyeSide);
	}

	// compose json
	auto retJsonValue = composeJsonEMNET(patient, fundusFileName, octList, eyeSide, CString(strPatternName));
	if (get<0>(retJsonValue)) {
		// export json file
		// - convert to stream
		utility::stringstream_t stream;
		get<1>(retJsonValue).serialize(stream);
		// - write to file
		std::wstring strPath = CString(exportDir) + L"\\patient.json";
		wofstream fileStream(strPath, std::ios::out | std::wofstream::binary);
		fileStream.imbue(std::locale(fileStream.getloc(), new std::codecvt_utf8<wchar_t>));

		std::wstring content;
		content.push_back(0xFEFF); // BOM
		content.append(stream.str());

		fileStream << content;
		fileStream.close();
	}
}
