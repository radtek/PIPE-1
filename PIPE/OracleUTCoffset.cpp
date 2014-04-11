// OracleUTCoffset.cpp: implementation of the COracleUTCoffset class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pancim_oracle.h"
#include "OracleUTCoffset.h"
#include "obound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef ORACL_ORACLE
#include "oracl.h"
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COracleUTCoffset::COracleUTCoffset()
{
  strSQL = "select SUBSTR(TZ_OFFSET('CST'),1,3) from dual";
}

COracleUTCoffset::~COracleUTCoffset()
{

}

long COracleUTCoffset::GetUTCoffset()
{
  char OffsetBuffer[6];
  long UTCoffset_hour;
  long UTCoffset_seconds;
  int oResult;
  ODatabase odb;
  ODynaset oDyn;


  odb.ErrorReset();

  oResult = odb.Open((LPCTSTR) dbNam,(LPCTSTR) dbUsr,(LPCTSTR) dbPwd);
  
  
  // debug ------------------------------------------------------------
  // CString oErrTxt = odb.GetErrorText();
  // printf("dbname=%s  dbusr=%s   dbpwd=%s\n",(LPCTSTR) dbNam,(LPCTSTR) dbUsr,(LPCTSTR) dbPwd);
  // printf("oResult=%d  %s\n", oResult, (LPCTSTR) oErrTxt);
  // ------------------------------------------------------------------
  
  if (oResult == OSUCCESS)
  {
    oDyn.Open(odb, (LPCTSTR) strSQL,ODYNASET_READONLY);
    if (oDyn.IsOpen())
    {
      oDyn.GetFieldValue(0, OffsetBuffer, 4);
    }
    oDyn.Close();
  }
  odb.Close();
  
  // debug ------------------------------------------------------------
  // printf("OffsetBuffer=%s\n", OffsetBuffer);
  // ------------------------------------------------------------------
  
  UTCoffset_hour = atol(OffsetBuffer);
 
  // debug ------------------------------------------------------------
  // printf("UTCoffset_hour=%d\n", UTCoffset_hour);
  // ------------------------------------------------------------------

  // convert to seconds
  UTCoffset_seconds = UTCoffset_hour * SECONDS_IN_HOUR;
  
  // debug ------------------------------------------------------------
  // printf("UTCoffset_seconds=%d\n", UTCoffset_seconds);
  // ------------------------------------------------------------------

  
  return UTCoffset_seconds;
}
