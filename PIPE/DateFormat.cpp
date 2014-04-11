// DateFormat.cpp: implementation of the CDateFormat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pancim_oracle.h"
#include "DateFormat.h"
#include <ostream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDateFormat::CDateFormat()
{

}

CDateFormat::~CDateFormat()
{

}

/* --------------------------------------
 date format: dd-mon-yyyy
----------------------------------------- */
void CDateFormat::GetDateOnly()
{
  
  char strMonth[12][4] = {"JAN", "FEB", "MAR",
                          "APR", "MAY", "JUN",
                          "JUL", "AUG", "SEP",
                          "OCT", "NOV", "DEC"};

  time(&SysTm);
  CurrLocalTm = localtime(&SysTm);
  OracleTimeStamp.Format("%02d-%s-%d",CurrLocalTm->tm_mday,strMonth[CurrLocalTm->tm_mon],(CurrLocalTm->tm_year+1900));
}

/* --------------------------------------
  Get system time and format for Oracle 
   (format: dd-mon-yyyy hh:mm:ss AM/PM)
----------------------------------------- */
void CDateFormat::GetOracleTime()
{
  CString strAMPM,
          tsBuf;
  char strMonth[12][4] = {"JAN", "FEB", "MAR",
                          "APR", "MAY", "JUN",
                          "JUL", "AUG", "SEP",
                          "OCT", "NOV", "DEC"};

  time(&SysTm);
  CurrLocalTm = localtime(&SysTm);
  if(CurrLocalTm->tm_hour > 12)
  {
    CurrLocalTm->tm_hour -= 12;
    strAMPM = "PM";
  }
  else
  {
    strAMPM = "AM";
  }
  if(CurrLocalTm->tm_hour == 0)
    CurrLocalTm->tm_hour = 12;
  
  tsBuf.Format("%02d-%s-%d %d:%02d:00 %s",CurrLocalTm->tm_mday,strMonth[CurrLocalTm->tm_mon],(CurrLocalTm->tm_year+1900),
                    CurrLocalTm->tm_hour, CurrLocalTm->tm_min, strAMPM);
  OracleTimeStamp.Format("TO_DATE('%s','dd-mon-yyyy hh:mi:ss am')", tsBuf);
}
