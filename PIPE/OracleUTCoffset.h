// OracleUTCoffset.h: interface for the COracleUTCoffset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORACLEUTCOFFSET_H__D5F024F7_B792_43DA_B43B_6AF9919B585F__INCLUDED_)
#define AFX_ORACLEUTCOFFSET_H__D5F024F7_B792_43DA_B43B_6AF9919B585F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SECONDS_IN_HOUR 3600 // number of seconds in hour

class COracleUTCoffset  
{
public:
	long GetUTCoffset();
  CString dbNam,
          dbUsr,
          dbPwd;

	COracleUTCoffset();
	virtual ~COracleUTCoffset();

private:
  CString strSQL;

};

#endif // !defined(AFX_ORACLEUTCOFFSET_H__D5F024F7_B792_43DA_B43B_6AF9919B585F__INCLUDED_)
