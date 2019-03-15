#pragma once

#include "GlobalMeasureDef.h"
#include <iostream>
#include <memory>

using namespace OctGlobal;

struct MeasureImpl;
class Measure
{
public:
	Measure();
	~Measure();

public:
	static bool isScannerResult(EyeSide side = EyeSide::BOTH);
	static bool isFundusResult(EyeSide side = EyeSide::BOTH);
	static void setScanResult(bool value);
	static void setFundusResult(bool value);

	static int exportScannerResults(std::wstring dirPath = L"", bool subPath = true,
		std::wstring imagePrefix = L"",
		std::wstring enfaceName = L"enface",
		std::wstring previewName = L"preview",
		std::wstring retinaName = L"retina",
		std::wstring corneaName = L"cornea");
	static int exportFundusResults(unsigned int imgQuality,
		std::wstring dirPath = L"", bool subPath = true,
		std::wstring imageName = L"fundus",
		std::wstring thumbName = L"thumbnail",
		std::wstring retinaName = L"retina",
		std::wstring corneaName = L"cornea");

private:
	static bool createDefaultExportDirectory(void);
	
	struct MeasureImpl;
	static std::unique_ptr<MeasureImpl> d_ptr;
	static MeasureImpl& getImpl(void);
};

