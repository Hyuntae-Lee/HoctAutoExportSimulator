#include "stdafx.h"
#include "Measure.h"
#include <vector>

struct Measure::MeasureImpl {
	bool scanResult = false;
	bool fundusResult = false;

	std::wstring exportPath;
};

std::unique_ptr<Measure::MeasureImpl> Measure::d_ptr(new MeasureImpl());

Measure::Measure()
{
}

Measure::~Measure()
{
}

bool Measure::isScannerResult(EyeSide side)
{
	return getImpl().scanResult;
}

bool Measure::isFundusResult(EyeSide side)
{
	return getImpl().fundusResult;
}

void Measure::setScanResult(bool value)
{
	getImpl().scanResult = value;
}

void Measure::setFundusResult(bool value)
{
	getImpl().fundusResult = value;
}

int Measure::exportScannerResults(std::wstring dirPath, bool subPath,
	std::wstring imagePrefix, std::wstring enfaceName,
	std::wstring previewName, std::wstring retinaName, std::wstring corneaName)
{
	if (dirPath.empty()) {
		if (!createDefaultExportDirectory()) {
			return 0;
		}
	}
	else {
		d_ptr->exportPath = dirPath;
	}

	// get html path
	TCHAR strCurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strCurDir);
	CString szSampleDir = CString(strCurDir) + _T("/sample/oct data sample (512x96)");

	// export
	CFileFind finder;
	std::vector<std::wstring> pathList;
	// - build a string with wildcards
	CString strWildcard = szSampleDir + _T("/*.*");
	// - start working for files
	int count = 0;
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking) {
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd
		// recur infinitely!
		if (finder.IsDirectory() || finder.IsDots()) {
			continue;
		}

		CString szFileName = finder.GetFileName();
		CString szPathSrc = finder.GetFilePath();
		std::wstring strPathDst = dirPath;
		CString szDirDst(strPathDst.c_str());

		CString szPathDst;
		if (szFileName == CString(_T("cornea.jpg"))) {
			szPathDst = szDirDst + _T("/") + CString(corneaName.c_str()) + szFileName;
		}
		else if (szFileName == CString(_T("enface.jpg"))) {
			szPathDst = szDirDst + _T("/") + CString(enfaceName.c_str()) + szFileName;
		}
		else if (szFileName == CString(_T("preview.jpg"))) {
			szPathDst = szDirDst + _T("/") + CString(previewName.c_str()) + szFileName;
		}
		else if (szFileName == CString(_T("preview_1.jpg"))) {
			szPathDst = szDirDst + _T("/") + CString(previewName.c_str()) + szFileName;
		}
		else if (szFileName == CString(_T("retina.jpg"))) {
			szPathDst = szDirDst + _T("/") + CString(retinaName.c_str()) + szFileName;
		}
		else {
			szPathDst = szDirDst + _T("/") + szFileName;
		}

		CopyFile(szPathSrc, szPathDst, false);

		count++;
	}

	return count;
}


int Measure::exportFundusResults(unsigned int imgQuality,
	std::wstring dirPath, bool subPath,
	std::wstring imageName, std::wstring thumbName,
	std::wstring retinaName, std::wstring corneaName)
{
	if (dirPath.empty()) {
		if (!createDefaultExportDirectory()) {
			return 0;
		}
	}
	else {
		d_ptr->exportPath = dirPath;
	}

	// get html path
	TCHAR strCurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strCurDir);
	CString szSampleDir = CString(strCurDir) + _T("/sample/fundus data sample");

	// export
	CFileFind finder;
	std::vector<std::wstring> pathList;
	// - build a string with wildcards
	CString strWildcard = szSampleDir + _T("/*.*");
	// - start working for files
	int count = 0;
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking) {
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd
		// recur infinitely!
		if (finder.IsDirectory() || finder.IsDots()) {
			continue;
		}

		CString szPathSrc = finder.GetFilePath();
		std::wstring strPathDst = dirPath + L"/";
		CString szPathDst(strPathDst.c_str());

		CopyFile(szPathSrc, szPathDst, false);

		count++;
	}

	return count;
}

bool Measure::createDefaultExportDirectory(void)
{
	getImpl().exportPath = _T(".//export");
	if (CreateDirectory(getImpl().exportPath.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
		return true;
	}
	return false;
}

Measure::MeasureImpl & Measure::getImpl(void)
{
	return *d_ptr;
}