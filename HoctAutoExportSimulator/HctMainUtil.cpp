#include "stdafx.h"
#include "HctMainUtil.h"


HctMainUtil::HctMainUtil()
{
}


HctMainUtil::~HctMainUtil()
{
}

CString HctMainUtil::getSerialNumber(void)
{
	return _T("test serial number");
}

CString HctMainUtil::getEyeSideString(OctGlobal::EyeSide side)
{
	CString ret = _T("");

	switch (side)
	{
	case OctGlobal::EyeSide::OD:	ret = _T("OD");		break;
	case OctGlobal::EyeSide::OS:	ret = _T("OS");		break;
	case OctGlobal::EyeSide::BOTH:	ret = _T("OU");		break;
	default:		break;
	}

	return ret;
}

CString HctMainUtil::getPatternNameString(OctGlobal::OctScanPattern pattern)
{
	CString ret = NULL;

	ret = (CString)pattern.getPatternNameString();

	return ret;
}
