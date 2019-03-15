#pragma once

#include "GlobalMeasureDef.h"
#include "OctScanDescript.h"

class HctMainUtil
{
public:
	HctMainUtil();
	~HctMainUtil();

	static CString getSerialNumber(void);
	static CString getEyeSideString(OctGlobal::EyeSide side);
	static CString getPatternNameString(OctGlobal::OctScanPattern pattern);
};

