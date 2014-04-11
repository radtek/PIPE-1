// DateFormat.h: interface for the CDateFormat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATEFORMAT_H__C8E045E3_4E95_4F22_B4FB_DE31445ABB8F__INCLUDED_)
#define AFX_DATEFORMAT_H__C8E045E3_4E95_4F22_B4FB_DE31445ABB8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDateFormat  
{
public:
	void GetOracleTime();
	void GetDateOnly();
	CString OracleTimeStamp;
	CDateFormat();
	virtual ~CDateFormat();

private:

  time_t SysTm;
  struct tm *CurrLocalTm;
};

#endif // !defined(AFX_DATEFORMAT_H__C8E045E3_4E95_4F22_B4FB_DE31445ABB8F__INCLUDED_)
