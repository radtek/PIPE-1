// ConvertTimeZone.h: interface for the CConvertTimeZone class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONVERTTIMEZONE_H__F33E24F8_62BB_4B50_AD65_12C68C481013__INCLUDED_)
#define AFX_CONVERTTIMEZONE_H__F33E24F8_62BB_4B50_AD65_12C68C481013__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SECONDS_IN_HOUR 3600  // number of seconds in one hour

class CConvertTimeZone  
{
public:
	CTime ConvertTS(CTime OracleTS, long OracleOffset, long LocalOffset);
	CConvertTimeZone();
	virtual ~CConvertTimeZone();

};

#endif // !defined(AFX_CONVERTTIMEZONE_H__F33E24F8_62BB_4B50_AD65_12C68C481013__INCLUDED_)
